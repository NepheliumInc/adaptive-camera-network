
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"

using namespace cv;

int mainr(){
    VideoCapture cap(0);
    while (true) {
        Mat WebCam;
        cap.read(WebCam);
        imshow("WebCam",WebCam);
    }
}