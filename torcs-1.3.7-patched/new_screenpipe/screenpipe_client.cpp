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
#include <etcd/Client.hpp>

using namespace std;
using namespace cv;

#define image_width 640
#define image_height 480
#define resize_width 640
#define resize_height 480

static auto start = std::chrono::system_clock::now();
static double total_time = 0;
static int count_time = 0;

//ETCD setup
etcd::Client etcd_client("http://etcd:2379");
unsigned char image[resize_width*resize_height * 3];
IplImage* resizeRGB=cvCreateImage(cvSize(resize_width,resize_height),IPL_DEPTH_8U,3);
int key;
IplImage* out_red = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
IplImage* out_green = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
IplImage* out_blue = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);

int indes = 0;

void watch_for_changes()
{
    etcd_client.watch("/test/shared/red", indes + 1, true).then([](pplx::task<etcd::Response> resp_task)
    {
        //IplImage* resizeRGB=cvCreateImage(cvSize(resize_width,resize_height),IPL_DEPTH_8U,3);
        //IplImage* out_red = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
        //IplImage* out_green = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
        //IplImage* out_blue = cvCreateImage(cvSize(resize_width,resize_height), IPL_DEPTH_8U, 1);
        strcpy(out_red->imageData, etcd_client.get("/test/shared/red").get().value().as_string().c_str());  
        strcpy(out_green->imageData, etcd_client.get("/test/shared/green").get().value().as_string().c_str());
        strcpy(out_blue->imageData, etcd_client.get("/test/shared/blue").get().value().as_string().c_str());

        try
        {
            cvMerge(out_blue, out_green, out_red, NULL, resizeRGB);
            cvShowImage("Image from TORCS", resizeRGB);
            key = cvWaitKey(1);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end-start;
            total_time += elapsed_seconds.count();
            count_time += 1;
            std::cout << "average fps: " << count_time/total_time << "s" << std::endl;
            start = end;
        }
        catch (Exception e)
        {
            std::cout << e.msg << std::endl;
        }
        watch_for_changes();
    });
}

int main(int argc, char const *argv[])
{
    cvNamedWindow("Image from TORCS",1);
    watch_for_changes();
    getchar();

    return 0;
}
