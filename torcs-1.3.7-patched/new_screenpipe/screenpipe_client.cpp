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
#include <fstream>
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

//ETCD setup
etcd::Client etcd_client("http://etcd:2379");
int key;


int indes = 0;

void watch_for_changes()
{
    etcd_client.watch("/test/shared/image", indes + 1, true).then([](pplx::task<etcd::Response> resp_task)
    {
        try {
            string output_string = etcd_client.get("/test/shared/image").get().value().as_string();

            std::vector<uchar> image_vector(output_string.begin(), output_string.end());

            cv::Mat p_img = cv::Mat(480, 640, CV_8UC3, image_vector.data());
            cv::imshow("Image from TORCS", p_img);
            key = cvWaitKey(1);
        }
        catch (std::exception const & ex)
        {
            std::cerr << "communication problem, details: " << ex.what();
        }
        watch_for_changes();
    });
}

int main()
{
    cvNamedWindow("Image from TORCS",1);
    watch_for_changes();
    while (true) { }

    return 0;
}
