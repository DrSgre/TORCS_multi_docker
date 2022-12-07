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
#include <sw/redis++/redis++.h>

using namespace std;
using namespace cv;
using namespace sw::redis;

#define image_width 640
#define image_height 480
#define resize_width 640
#define resize_height 480

auto redis = Redis("tcp://172.20.0.2:6379");
auto sub = redis.subscriber();
unsigned char image[resize_width*resize_height * 3];
int key;


int indes = 0;

void watch_for_changes()
{
    sub.on_message([](std::string channel, std::string msg) {
        try {
            string output_string = redis.get(msg).value();

            std::vector<uchar> image_vector(output_string.begin(), output_string.end());

            auto p_img = cv::Mat(480, 640, CV_8UC3, image_vector.data());
            cv::imshow("Image from TORCS", p_img);
            key = cvWaitKey(1);
        }
        catch (std::exception const & ex)
        {
            std::cerr << "communication problem, details: " << ex.what();
        }
    });
    sub.subscribe("video_streaming");
}

int main(int argc, char const *argv[])
{
    cvNamedWindow("Image from TORCS",1);
    watch_for_changes();
    while (true) {
        try {
            sub.consume();
        } catch (std::exception const & ex)
        {
            std::cerr << "communication problem, details: " << ex.what();
        }
    }  
    return 0;
}
