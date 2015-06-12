//
//  TableTaken.cpp
//  OpencvTut
//
//  Created by Dehan on 5/8/15.
//  Copyright (c) 2015 nephelium. All rights reserved.
//

#include "TableTaken.h"
#include <iostream>
//#include "opencv2\highgui\highgui.hpp"
//#include <opencv2\highgui\highgui.hpp>
//#include <opencv2\imgproc\imgproc.hpp>
//#include <opencv\cvaux.h>
//#include <opencv\cv.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/opencv.hpp>
//#include <opencv2\core\core.hpp>


using namespace std;
using namespace cv;


int main()
{
    Mat originalImage = imread("/Users/dehan/projects/Table1.png");
    cvtColor(originalImage, originalImage,CV_BGR2GRAY);
    Mat outputMat(originalImage.rows, originalImage.cols, CV_8UC1);
    Mat outputMat2(originalImage.rows, originalImage.cols, CV_8UC1);
    Mat outputMat3(originalImage.rows, originalImage.cols, CV_8UC1);
    GaussianBlur(originalImage, outputMat, Size(3,3), 2);
    bilateralFilter(outputMat, outputMat2, 3, 6,10);
    Canny(outputMat, outputMat3, 3, 3);
    imshow("TEST", outputMat2);
    imshow("TEST3", outputMat3);
    printf("done");
    return 0;
}