//
//  main2.cpp
//  OpencvTut
//
//  Created by Dehan on 3/20/15.
//  Copyright (c) 2015 nephelium. All rights reserved.
//

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


int mainX()
{
    Mat originalImage = imread("/Users/dehan/projects/images2.jpg");
    Mat greyImage;
    Mat newImage(originalImage.rows,originalImage.cols,CV_8UC1,Scalar(255));
    cvtColor(originalImage, greyImage, CV_BGR2GRAY, 0);
    Mat meanFiltered(originalImage.rows,originalImage.cols,CV_8UC1,Scalar(255));
    Mat medianFiltered(originalImage.rows,originalImage.cols,CV_8UC1,Scalar(255));
    Mat modeFiltered(originalImage.rows,originalImage.cols,CV_8UC1,Scalar(255));
    
    
    
    /////Mean Filtered
    int r = 1;
    int meanKernalRows = 2*r +1;
    int meanKernalColumns = 2*r +1;
    for (int row = 0; row < greyImage.rows; row++) {
        for (int column = 0; column<greyImage.cols; column++) {
            //int level = greyImage.at<uchar>(row,column);
            int kernelStartRow  = row-(meanKernalRows-1)/2;
            int kernelEndRow    = row+(meanKernalRows-1)/2;
            int kernelStartCol  = column-(meanKernalColumns-1)/2;
            int kernelEndCol    = column-(meanKernalColumns-1)/2;
            if (kernelStartRow >0 && kernelEndRow<greyImage.rows && kernelStartCol >0 && kernelEndCol < greyImage.cols) {
                int totalIntensity = 0;
                for (int krow =0; krow < meanKernalRows; krow++,kernelStartRow++) {
                    for (int kcolumn=0; kcolumn<meanKernalColumns; kcolumn++, kernelStartCol++) {
                        totalIntensity += greyImage.at<uchar>(kernelStartRow,kernelStartCol);
                    }
                }
                int averageIntensity = totalIntensity/(meanKernalRows*meanKernalColumns);
                meanFiltered.at<uchar>(row,column) = averageIntensity;
            }
        }
    }
    /*
    int rows = originalImage.rows;
    int columns = originalImage.cols;
    
    int colourFrequency1[256] = {};
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            int pixel = greyImage.at<uchar>(i, j);
            colourFrequency1[pixel] += 1;
        }
    }
    cout << "freq of intensity at 150 = " << colourFrequency1[150] << endl;
    
    int max = 0;
    for (int i = 0; i < 256; i++){
        if (max < colourFrequency1[i]){
            max = colourFrequency1[i];
        }
    }
    cout << "max = " << max << endl;
    
    Mat histogram(310, 260, CV_8UC1, Scalar(255));
    
    int hist[256] = {};
    double maxd = max;
    for (int i = 0; i < 256; i++){
        hist[i] = cvRound(double(colourFrequency1[i] / maxd) * 300);
        Point pt1 = Point(i, 300-hist[i]);
        Point pt2 = Point(i, 300);
        line(histogram, pt1, pt2, Scalar(100, 0, 0), 1, 8, 0);
    }
    
    cout << "hist[150] =" << hist[150] << endl;
    
    
    
    double	lowerBound = 100;
    double	lowerNewBound = 50;
    
    double	upperBound = 190;
    double	upperNewBound = 230;
    
    for (int i = 0; i < rows ; i++){
        for (int j = 0; j < columns; j++){
            int pix = greyImage.at<uchar>(i, j);
            if (pix < lowerBound){
                double m1 = (lowerNewBound / lowerBound);
                greyImage.at<uchar>(i, j) = cvRound(m1* pix);
                newImage.at<uchar>(i,j) = cvRound(m1*pix);
                
            }
            else if (pix >= lowerBound && pix <= upperBound){
                double m2 = (upperNewBound - lowerNewBound) / (upperBound - lowerBound);
                greyImage.at<uchar>(i, j) = cvRound( m2 * (pix - lowerBound) + lowerNewBound);
                newImage.at<uchar>(i,j) = cvRound(m2*(pix - lowerBound) + lowerNewBound);
            }
            else{
                double m3 =(255 - upperNewBound) / (255 - upperBound);
                greyImage.at<uchar>(i, j) = cvRound(m3 * (pix - upperBound) + upperNewBound);
                newImage.at<uchar>(i,j) = cvRound(m3*(pix - lowerBound) + lowerNewBound );
            }
        }
    }
    
    for (int i = 0; i<256; i++)
    {
        cout << i << " = " << colourFrequency1[i] << endl;
    }
    //cout << max << endl;
    
    
    
    ////////////After
    
    int colourFrequency2[256] = {}; // u should give ={} bcos it will hav garbage values frm that array.
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            int pixel = greyImage.at<uchar>(i, j);
            colourFrequency2[pixel] += 1;
        }
    }
    cout << "freq of intensity at 150 = " << colourFrequency2[150] << endl;
    
    max = 0;
    for (int i = 0; i < 256; i++){
        if (max < colourFrequency2[i]){
            max = colourFrequency2[i];
        }
    }
    cout << "max = " << max << endl;
    
    Mat histogram2(310, 260, CV_8UC3, Scalar(255, 255, 255));
    
    int hist2[256] = {};
    maxd = max;
    for (int i = 0; i < 256; i++){
        hist2[i] = cvRound(double(colourFrequency2[i] / maxd) * 300);
        Point pt1 = Point(i, 300-hist2[i]);
        Point pt2 = Point(i, 300);
        line(histogram2, pt1, pt2, Scalar(0, 0, 0), 1, 8, 0);
    }
    
    
    
    
    
    //imshow("originalImage1: ", histogram);
    imshow("originalImage1: ", originalImage);
    imshow("Histo1: ", histogram);
    imshow("Histo2: ", histogram2);
    imshow("originalImage2: ", greyImage);
    
    cvWaitKey(0);
    //cvDestroyWindow("originalImage2: ");
    //cvReleaseoriginalImage(originalImage);
    */
    
    imshow("GreyImage: ", greyImage);
    imshow("Filtered: ", medianFiltered);
    
    cvWaitKey(0);
    //cvDestroyWindow("originalImage2: ");
    //cvReleaseoriginalImage(originalImage);
    return 0;
}