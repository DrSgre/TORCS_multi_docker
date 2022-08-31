/***************************************************************************

    file                 : main.cpp
    created              : Sat Mar 18 23:54:30 CET 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: main.cpp,v 1.14.2.3 2012/06/01 01:59:42 berniw Exp $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>

#include <GL/glut.h>

#include <tgfclient.h>
#include <client.h>

#include "linuxspec.h"
#include <raceinit.h>


#include <sys/shm.h> 
#define image_width 640
#define image_height 480
#include <iostream> 
#include <unistd.h> 

#include <stdlib.h>  
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui/highgui_c.h>
#include "torcs_data.pb.h"
#include <etcd/Client.hpp>
#include <thread>

using namespace std;
using namespace cv;

extern bool bKeepModules;

static void
init_args(int argc, char **argv, const char **raceconfig)
{
	int i;
	char *buf;
    
    setNoisy(false);
    setVersion("2013");

	i = 1;

	while(i < argc) {
		if(strncmp(argv[i], "-l", 2) == 0) {
			i++;

			if(i < argc) {
				buf = (char *)malloc(strlen(argv[i]) + 2);
				sprintf(buf, "%s/", argv[i]);
				SetLocalDir(buf);
				free(buf);
				i++;
			}
		} else if(strncmp(argv[i], "-L", 2) == 0) {
			i++;

			if(i < argc) {
				buf = (char *)malloc(strlen(argv[i]) + 2);
				sprintf(buf, "%s/", argv[i]);
				SetLibDir(buf);
				free(buf);
				i++;
			}
		} else if(strncmp(argv[i], "-D", 2) == 0) {
			i++;

			if(i < argc) {
				buf = (char *)malloc(strlen(argv[i]) + 2);
				sprintf(buf, "%s/", argv[i]);
				SetDataDir(buf);
				free(buf);
				i++;
			}
		} else if(strncmp(argv[i], "-s", 2) == 0) {
			i++;
			SetSingleTextureMode();
		} else if (strncmp(argv[i], "-t", 2) == 0) {
		    i++;
		    if (i < argc) {
			long int t;
			sscanf(argv[i],"%ld",&t);
			setTimeout(t);
			printf("UDP Timeout set to %ld 10E-6 seconds.\n",t);
			i++;
		    }
		} else if (strncmp(argv[i], "-nodamage", 9) == 0) {
		    i++;
		    setDamageLimit(false);
		    printf("Car damages disabled!\n");
		} else if (strncmp(argv[i], "-nofuel", 7) == 0) {
		    i++;
		    setFuelConsumption(false);
		    printf("Fuel consumption disabled!\n");
		} else if (strncmp(argv[i], "-noisy", 6) == 0) {
		    i++;
		    setNoisy(true);
		    printf("Noisy Sensors!\n");
		} else if (strncmp(argv[i], "-ver", 4) == 0) {
		    i++;
		    if (i < argc) {
					setVersion(argv[i]);
		    		printf("Set version: \"%s\"\n",getVersion());
		    		i++;
		    }
		} else if (strncmp(argv[i], "-nolaptime", 10) == 0) {
		    i++;
		    setLaptimeLimit(false);
		    printf("Laptime limit disabled!\n");   
		} else if(strncmp(argv[i], "-k", 2) == 0) {
			i++;
			// Keep modules in memory (for valgrind)
			printf("Unloading modules disabled, just intended for valgrind runs.\n");
			bKeepModules = true;
#ifndef FREEGLUT
		} else if(strncmp(argv[i], "-m", 2) == 0) {
			i++;
			GfuiMouseSetHWPresent(); /* allow the hardware cursor */
#endif
		} else if(strncmp(argv[i], "-r", 2) == 0) {
			i++;
			*raceconfig = "";

			if(i < argc) {
				*raceconfig = argv[i];
				i++;
			}

			if((strlen(*raceconfig) == 0) || (strstr(*raceconfig, ".xml") == 0)) {
				printf("Please specify a race configuration xml when using -r\n");
				exit(1);
			}
		} else {
			i++;		/* ignore bad args */
		}
	}

#ifdef FREEGLUT
	GfuiMouseSetHWPresent(); /* allow the hardware cursor (freeglut pb ?) */
#endif
}

struct shared_use_st  
{  
    int written;
    uint8_t data[image_width*image_height*3];
    int pause;
    int zmq_flag;   
    int save_flag;  
};

int* pwritten = NULL;
uint8_t* pdata = NULL;
int* ppause = NULL;
int* pzmq_flag = NULL;
int* psave_flag = NULL;

void *shm = NULL;
static struct shared_use_st *shared = NULL;

void EtcdShare() {
	try {
			#define image_width 640
		#define image_height 480
		#define resize_width 640
		#define resize_height 480


		//ETCD setup
		etcd::Client etcd("http://etcd:2379");
		std::cout << "Connected to etcd...\n";

		TorcsData torcs_data;
		unsigned char image[resize_width*resize_height * 3];

		// Setup opencv
		IplImage* screenRGB=cvCreateImage(cvSize(image_width,image_height),IPL_DEPTH_8U,3);
		IplImage* resizeRGB=cvCreateImage(cvSize(resize_width,resize_height),IPL_DEPTH_8U,3);
		int key;
		IplImage* out_red = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
		IplImage* out_green = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
		IplImage* out_blue = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);

		while (true) {
			std::cout << shared->written << std::endl;
			if (shared->written == 1) {

				for (int h = 0; h < image_height; h++) {
					for (int w = 0; w < image_width; w++) {
					screenRGB->imageData[(h*image_width+w)*3+2]=shared->data[((image_height-h-1)*image_width+w)*3+0];
					screenRGB->imageData[(h*image_width+w)*3+1]=shared->data[((image_height-h-1)*image_width+w)*3+1];
					screenRGB->imageData[(h*image_width+w)*3+0]=shared->data[((image_height-h-1)*image_width+w)*3+2];
					}
				}
				cvResize(screenRGB, resizeRGB);
				cvSplit(resizeRGB, out_blue, out_green, out_red, NULL);

				torcs_data.clear_width();
				torcs_data.clear_height();
				torcs_data.clear_red();
				torcs_data.clear_green();
				torcs_data.clear_blue();
				torcs_data.clear_save_flag();
				torcs_data.add_red((const void*)out_red->imageData, (size_t) resize_width * resize_height);
				torcs_data.add_green((const void*)out_green->imageData, (size_t) resize_width * resize_height);
				torcs_data.add_blue((const void*)out_blue->imageData, (size_t) resize_width * resize_height);
				torcs_data.add_width(resize_width);
				torcs_data.add_height(resize_height);
				torcs_data.add_save_flag(shared->save_flag);

				string serialized_data;
				torcs_data.SerializeToString(&serialized_data);
				pplx::task<etcd::Response> response_task;
				response_task = etcd.rm("/test/shared/width");
				response_task = etcd.set("/test/shared/width", std::to_string(torcs_data.width(0)));
				response_task = etcd.rm("/test/shared/height");
				response_task = etcd.set("/test/shared/height", std::to_string(torcs_data.height(0)));
				response_task = etcd.rm("/test/shared/red");
				response_task = etcd.set("/test/shared/red", torcs_data.red(0));
				response_task = etcd.rm("/test/shared/green");
				response_task = etcd.set("/test/shared/green", torcs_data.green(0));
				response_task = etcd.rm("/test/shared/blue");
				response_task = etcd.set("/test/shared/blue", torcs_data.blue(0));
				response_task = etcd.rm("/test/shared/save_flag");
				response_task = etcd.set("/test/shared/save_flag", std::to_string(torcs_data.save_flag(0)));
				shared->written=0;
			}
		}
	}
	catch (Exception ex)
	{
		std::cout << ex.msg << std::endl;
	}
}

/*
 * Function
 *	main
 *
 * Description
 *	LINUX entry point of TORCS
 *
 * Parameters
 *
 *
 * Return
 *
 *
 * Remarks
 *
 */
int
main(int argc, char *argv[])
{
    int shmid; 
    // establish memory sharing 
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666|IPC_CREAT);  
    if(shmid == -1)  
    {  
        fprintf(stderr, "shmget failed\n");  
        exit(EXIT_FAILURE);  
    }  
  
    shm = shmat(shmid, 0, 0);  
    if(shm == (void*)-1)  
    {  
        fprintf(stderr, "shmat failed\n");  
        exit(EXIT_FAILURE);  
    }  
    printf("\n********** Memory sharing started, attached at %X **********\n \n", shm);  
    // set up shared memory 
    shared = (struct shared_use_st*)shm;  
    shared->written = 0;
    shared->pause = 1;
    shared->zmq_flag = 0;  
    shared->save_flag = 0;

 
    pwritten=&shared->written;
    pdata=shared->data;
    ppause=&shared->pause;
    pzmq_flag = &shared->zmq_flag;
	psave_flag = &shared->save_flag;

	const char *raceconfig = "";

	init_args(argc, argv, &raceconfig);
	LinuxSpecInit();			/* init specific linux functions */

	if(strlen(raceconfig) == 0) {
		std::thread sharing(EtcdShare);
		GfScrInit(argc, argv);	/* init screen */
		TorcsEntry();			/* launch TORCS */
		glutMainLoop();			/* event loop of glut */
	} else {
		// Run race from console, no Window, no OpenGL/OpenAL etc.
		// Thought for blind scripted AI training
		ReRunRaceOnConsole(raceconfig);
	}

	return 0;					/* just for the compiler, never reached */
}


