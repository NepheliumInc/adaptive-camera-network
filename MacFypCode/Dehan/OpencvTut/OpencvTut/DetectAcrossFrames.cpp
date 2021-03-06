//
//  ProBlobDetection.cpp
//  OpencvTut
//
//  Created by Dehan on 4/20/15.
//  Copyright (c) 2015 nephelium. All rights reserved.
//
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <opencv2/video/background_segm.hpp>
#include <stdio.h>
using namespace cv;
using namespace std;
void getDiffInVideo();
void drawHist();

void getDiffInVideo2();
//Mat frame;
//Mat frame; //current frame
Mat resize_blur_Img;
Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
Mat binaryImg;
//Mat TestImg;
Mat ContourImg; //fg mask fg mask generated by MOG2 method
Ptr< BackgroundSubtractor> pMOG2; //MOG2 Background subtractor


//global variables

int mainAcross(){
    int frameCounter = 0;
    Mat src;
    Mat dst;
    Mat resizedFrame;
    Mat processedFrame;
    Mat edgeDetected;
    Mat frame;
    //getDiffInVideo();
    char fileName[100] = "/Users/dehan/projects/cctv.mp4";
    VideoCapture stream1(fileName);
    
    CvCapture* capture = cvCreateFileCapture("/Users/dehan/projects/cctv.mp4");
    
    /* iterate through first 10 frames */
    for (int i = 0; i < 43; i++)
    {
        /* set pointer to frame index i */
        cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, i);
        
        /* capture the frame and do sth with it */
        frame = cvQueryFrame(capture);
        //getDiffInVideo2();
        
        //cvtColor(frame, frame, CV_BGR2HSV);
        resize_blur_Img = frame;
        //cvtColor(resize_blur_Img, resize_blur_Img, CV_BGR2HSV);
        //Resize
        resize(frame, resize_blur_Img, Size(frame.size().width/2, frame.size().height/2) );
        //Blur
        blur(resize_blur_Img, resize_blur_Img, Size(4,4) );
        //Background subtraction
        pMOG2->operator()(resize_blur_Img, fgMaskMOG2, -1);//,-0.5);
        //morphology element
        Mat element = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3,3) );

        
        ///////////////////////////////////////////////////////////////////
        //pre procesing
        //1 point delete
        //morphologyEx(fgMaskMOG2, fgMaskMOG2, CV_MOP_ERODE, element);
        morphologyEx(fgMaskMOG2, binaryImg, CV_MOP_CLOSE, element);
        //morphologyEx(fgMaskMOG2, testImg, CV_MOP_OPEN, element);
        
        //Shadow delete
        //Binary
        threshold(binaryImg, binaryImg, 128, 255, CV_THRESH_BINARY);
        
        //Find contour
        ContourImg = binaryImg.clone();
        //less blob delete
        vector< vector< Point> > contours;
        findContours(ContourImg,
                     contours, // a vector of contours
                     CV_RETR_EXTERNAL, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // all pixels of each contours
        
        vector< Rect > output;
        vector< vector< Point> >::iterator itc= contours.begin();
        while (itc!=contours.end()) {
            
            //Create bounding rect of object
            //rect draw on origin image
            Rect mr= boundingRect(Mat(*itc));
            //Mat window(mr.height,originalImage.width,CV_8UC3,Scalar(255));
            
            rectangle(resize_blur_Img, mr, CV_RGB(255,0,0));
            for (int row=0; row < mr.width; row++) {
                for (int col=0; col < mr.height; col++) {
                    
                    
                    
                }
            }
            ++itc;
        }
        
        ///////////////////////////////////////////////////////////////////
        
        //Display
        imshow("Shadow_Removed", binaryImg);
        imshow("Blur_Resize", resize_blur_Img);
        imshow("MOG2", fgMaskMOG2);
        
        
        
        
        
        //imshow( "Output", frame );
        
        cout << "Frame: " << i <<endl;
        if (waitKey(1) >= 0)
            break;
    }
    waitKey(0);
    return 0;
}
void getDiffInVideo2(){
    Mat frame;
    Mat resize_blur_Img;
    Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
    Mat binaryImg;
    //Mat TestImg;
    Mat ContourImg; //fg mask fg mask generated by MOG2 method
    Ptr< BackgroundSubtractor> pMOG2; //MOG2 Background subtractor
    //morphology element
    Mat element = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3,3) );
    pMOG2 = new BackgroundSubtractorMOG2(300,32,true);//300,0.0);
        //cvtColor(frame, frame, CV_BGR2HSV);
        resize_blur_Img = frame;
        //cvtColor(resize_blur_Img, resize_blur_Img, CV_BGR2HSV);
        //Resize
        resize(frame, resize_blur_Img, Size(frame.size().width/2, frame.size().height/2) );
        //Blur
        blur(resize_blur_Img, resize_blur_Img, Size(4,4) );
        //Background subtraction
        pMOG2->operator()(resize_blur_Img, fgMaskMOG2, -1);//,-0.5);
        
        ///////////////////////////////////////////////////////////////////
        //pre procesing
        //1 point delete
        //morphologyEx(fgMaskMOG2, fgMaskMOG2, CV_MOP_ERODE, element);
        morphologyEx(fgMaskMOG2, binaryImg, CV_MOP_CLOSE, element);
        //morphologyEx(fgMaskMOG2, testImg, CV_MOP_OPEN, element);
        
        //Shadow delete
        //Binary
        threshold(binaryImg, binaryImg, 128, 255, CV_THRESH_BINARY);
        
        //Find contour
        ContourImg = binaryImg.clone();
        //less blob delete
        vector< vector< Point> > contours;
        findContours(ContourImg,
                     contours, // a vector of contours
                     CV_RETR_EXTERNAL, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // all pixels of each contours
        
        vector< Rect > output;
        vector< vector< Point> >::iterator itc= contours.begin();
        while (itc!=contours.end()) {
            
            //Create bounding rect of object
            //rect draw on origin image
            Rect mr= boundingRect(Mat(*itc));
            //Mat window(mr.height,originalImage.width,CV_8UC3,Scalar(255));
            
            rectangle(resize_blur_Img, mr, CV_RGB(255,0,0));
            for (int row=0; row < mr.width; row++) {
                for (int col=0; col < mr.height; col++) {
                    
                    
                    
                }
            }
            ++itc;
        }
        
        ///////////////////////////////////////////////////////////////////
        
        //Display
        imshow("Shadow_Removed", binaryImg);
        imshow("Blur_Resize", resize_blur_Img);
        imshow("MOG2", fgMaskMOG2);
        
    
    
}
