// Used to create shared memory for data and command sharing.
//
// Required files:
//      1. IPC_command.cpp
//      2. torcs_data.pb.h 
//      3. torcs_data.pb.cc
//
// $ g++ IPC_command.cpp torcs_data.pb.cc -o IPC_command`pkg-config --cflags --libs opencv protobuf libzmq`
// 

#include <iostream>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>  
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core_c.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/highgui/highgui_c.h>
#include "torcs_data.pb.h"
#include <etcd/Client.hpp>

using namespace std;
using namespace cv;

#define image_width 640
#define image_height 480
#define resize_width 640
#define resize_height 480

struct shared_use_st  
{  
    int written;
    unsigned char data[image_width*image_height*3];
    int pause;
    int save_flag; 
};

int main(int argc, char const *argv[])
{    
    void *shm = NULL;
    struct shared_use_st *shared;
    int shmid;
    
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
    printf("\n********** Memory sharing started, attached at blablabal %X **********\n", shmid);

    shared = (struct shared_use_st*)shm; 
    shared->written = 0;
    shared->pause = 1; 
    shared->save_flag = 0;

    //ETCD setup
    etcd::Client etcd("http://127.0.0.1:2379");
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
            pplx::task<etcd::Response> response_task = etcd.set("/test/shared/width", std::to_string(torcs_data.width(0)));
            try
            {
                etcd::Response response = response_task.get();
                if (response.is_ok())
                std::cout << "The new value for /test/shared/width is successfully set" << std::endl;
                else
                std::cout << "operation failed, details: " << response.error_message();
            }
            catch (std::exception const & ex)
            {
                std::cerr << "communication problem, details: " << ex.what();
            }

            response_task = etcd.set("/test/shared/height", std::to_string(torcs_data.height(0)));
            try
            {
                etcd::Response response = response_task.get();
                if (response.is_ok())
                std::cout << "The new value for /test/shared/height is successfully set" << std::endl;
                else
                std::cout << "operation failed, details: " << response.error_message();
            }
            catch (std::exception const & ex)
            {
                std::cerr << "communication problem, details: " << ex.what();
            }

            response_task = etcd.set("/test/shared/red", torcs_data.red(0));
            try
            {
                etcd::Response response = response_task.get();
                if (response.is_ok())
                std::cout << "The new value for /test/shared/red is successfully set" << std::endl;
                else
                std::cout << "operation failed, details: " << response.error_message();
            }
            catch (std::exception const & ex)
            {
                std::cerr << "communication problem, details: " << ex.what();
            }

            response_task = etcd.set("/test/shared/green", torcs_data.green(0));
            try
            {
                etcd::Response response = response_task.get();
                if (response.is_ok())
                std::cout << "The new value for /test/shared/green is successfully set" << std::endl;
                else
                std::cout << "operation failed, details: " << response.error_message();
            }
            catch (std::exception const & ex)
            {
                std::cerr << "communication problem, details: " << ex.what();
            }

            response_task = etcd.set("/test/shared/blue", torcs_data.blue(0));
            try
            {
                etcd::Response response = response_task.get();
                if (response.is_ok())
                std::cout << "The new value for /test/shared/blue is successfully set" << std::endl;
                else
                std::cout << "operation failed, details: " << response.error_message();
            }
            catch (std::exception const & ex)
            {
                std::cerr << "communication problem, details: " << ex.what();
            }

            response_task = etcd.set("/test/shared/save_flag", std::to_string(torcs_data.save_flag(0)));
            try
            {
                etcd::Response response = response_task.get();
                if (response.is_ok())
                std::cout << "The new value for /test/shared/save_flag is successfully set" << std::endl;
                else
                std::cout << "operation failed, details: " << response.error_message();
            }
            catch (std::exception const & ex)
            {
                std::cerr << "communication problem, details: " << ex.what();
            }
            shared->written=0;
        }
    }

    if(shmdt(shm) == -1)  
    {  
        fprintf(stderr, "shmdt failed\n");  
        exit(EXIT_FAILURE);  
    }  

    if(shmctl(shmid, IPC_RMID, 0) == -1)  
    {  
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }
    printf("\n********** Memory sharing stopped. Good Bye! **********\n");  

    return 0;
}
