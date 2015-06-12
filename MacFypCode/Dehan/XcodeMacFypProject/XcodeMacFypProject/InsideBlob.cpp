//
//  InsideBlob.cpp
//  OpencvTut
//
//  Created by Dehan on 4/16/15.
//  Copyright (c) 2015 nephelium. All rights reserved.
//
#include "FeatureExtractor.h"
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <opencv2/video/background_segm.hpp>

#include <stdio.h>

using namespace cv;
using namespace std;
int mainInsideBlob(){
    
    char fileName[100] = "/Users/dehan/projects/loot.3gp";
    VideoCapture stream1(fileName);
    
    Mat src;
    Mat dst;
    Mat resizedFrame;
    Mat processedFrame;
    Mat edgeDetected;
    Mat gFrame;
    Mat gProcessedFrame;
    
    //global variables
    Mat frame; //current frame
    
    int frameCounter = 0;
    
    while (true) {
        int readStatus = stream1.read(frame);//get one frame form video
        if(!readStatus)
            break;
        resize(frame, frame, Size(frame.size().width*3, frame.size().height*3));
        bilateralFilter(frame, gFrame, 15, 80, 80);
        //GaussianBlur( frame, gFrame, Size(3,3), 0, 0, BORDER_DEFAULT );

        cvtColor(frame, processedFrame, CV_BGR2GRAY);
        
        imshow( "Output", frame);
        imshow( "Processed Output", processedFrame );
        
        //Edge Detection
        int kernel_size = 3;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;
        Laplacian( frame, processedFrame, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( processedFrame, processedFrame );
        imshow( "Laplacian Edge Detection", processedFrame);
        
        Laplacian( gFrame, gProcessedFrame, ddepth, kernel_size, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( gProcessedFrame, gProcessedFrame );
        imshow( "Laplacian Edge Detection w/Bilatteral Filter", processedFrame);
        //Sobel Operator
        Mat grad;
        scale = 1;
        delta = 0;
        ddepth = CV_16S;
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        /// Gradient X
        Sobel( frame, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
        /// Gradient Y
        Sobel( frame, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );
        convertScaleAbs( grad_y, abs_grad_y );
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
        imshow( "Sobel Edge Detection", grad);
        
        /// Gradient X
        Sobel( gFrame, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
        /// Gradient Y
        Sobel( gFrame, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );
        convertScaleAbs( grad_y, abs_grad_y );
        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
        imshow( "Sobel Edge Detection w/ Bilatteral Filter", grad);
        waitKey(500);
        cvWaitKey(0);
        
        frameCounter++;
        cout << frameCounter << "\n";
    }
    
    
    return 0;
}
