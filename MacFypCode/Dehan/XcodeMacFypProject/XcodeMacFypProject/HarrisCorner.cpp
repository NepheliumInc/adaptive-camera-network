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
#include <stdio.h>

using namespace cv;
using namespace std;





Mat src_gray;
const char* source_window = "Source image";
const char* corners_window = "Corners detected";
int thresh = 200;
int max_thresh = 255;

int mainWQW(){
    Mat src;
    Mat dst;
    Mat resizedFrame;
    Mat processedFrame;
    Mat edgeDetected;
    
    char fileName[100] = "/Users/dehan/projects/loot.3gp";
    VideoCapture stream1(fileName);

    CvCapture* capture;

    //-- 2. Read the video stream
    capture = cvCaptureFromCAM( -1 );


    
    //global variables
    Mat frame; //current frame
    void cornerHarris_demo( int, void* );
    
    int frameCounter = 0;
    
    if( capture )
    {
        while( true )
        {
            frame = cvQueryFrame( capture );
            
            //-- 3. Apply the classifier to the frame
            if( !frame.empty() )
            {
                resize(frame, src, Size(frame.size().width/3, frame.size().height/3));
                
                //GaussianBlur( frame, src, Size(3,3), 0, 0, BORDER_DEFAULT );
                
                cvtColor(src, src_gray, CV_BGR2GRAY);
                equalizeHist( src_gray, src_gray);
                
                /// Create a window and a trackbar
                namedWindow( source_window, WINDOW_AUTOSIZE );
                createTrackbar( "Threshold: ", source_window, &thresh, max_thresh, cornerHarris_demo );
                imshow( source_window, src );
                
                cornerHarris_demo( 0, 0 );
                
                frameCounter++;
                cout << frameCounter << "\n";
                

            }
            else
            { printf(" --(!) No captured frame -- Break!"); break; }
            
            int c = waitKey(10);
            if( (char)c == 'c' ) { break; }
        }
    }
    return 0;


}

void cornerHarris_demo( int, void* )
{
    
    Mat dst, dst_norm, dst_norm_scaled,src;
    dst = Mat::zeros( src.size(), CV_32FC1 );
    
    /// Detector parameters
    int blockSize = 3;
    int apertureSize = 3;
    double k = 0.04;
    
    /// Detecting corners
    cornerHarris( src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT );
    
    /// Normalizing
    normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
    convertScaleAbs( dst_norm, dst_norm_scaled );
    
    /// Drawing a circle around corners
    for( int j = 0; j < dst_norm.rows ; j++ )
    {
        for( int i = 0; i < dst_norm.cols; i++ )
        {
            if( (int) dst_norm.at<float>(j,i) > thresh )
            {
                circle( dst_norm_scaled, Point( i, j ), 5,  Scalar(0), 2, 8, 0 );
            }
        }
    }
    /// Showing the result
    namedWindow( corners_window, WINDOW_AUTOSIZE );
    imshow( corners_window, dst_norm_scaled );
}