
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2\core\core.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <opencv2/video/background_segm.hpp>
#include <stdio.h>

#include <opencv2\video\tracking.hpp>
#include <opencv2\imgproc\imgproc.hpp>

using namespace cv;
using namespace std;

void getDiffInVideo();
void movementTracking();

int main(){
	
	getDiffInVideo();
	
	//movementTracking();
	
	return 0;
}



void movementTracking()
{
	Mat frame;
	char fileName[100] = "E:/Final Year/project/human tracking/coding/VS_OpenCV/video/waveBall.mp4"; //video/\mm2.avi"; //mm2.avi"; //cctv 2.mov"; //mm2.avi"; //";//_p1.avi";
	VideoCapture cap(fileName);   //0 is the id of video device.0 if you have only one camera

	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH = 170;
	int iHighH = 179;

	int iLowS = 150;
	int iHighS = 255;

	int iLowV = 60;
	int iHighV = 255;

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);

	int iLastX = -1;
	int iLastY = -1;

	//Capture a temporary image from the camera
	Mat imgTmp;
	cap.read(imgTmp);

	//Create a black image with the size as the camera output
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);;


	while (true)
	{
		if (!(cap.read(frame))) //get one frame form video
			break;

		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video



		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (removes small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (removes small holes from the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		imshow("Thresholded Image", imgThresholded);
		//Calculate the moments of the thresholded image
		Moments oMoments = moments(imgThresholded);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 10000)
		{
			//calculate the position of the ball
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				//Draw a red line from the previous point to the current point
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
			}

			iLastX = posX;
			iLastY = posY;
		}

		imshow("Thresholded Image", imgThresholded); //show the thresholded image
		imshow("line image", imgLines);
		imgOriginal = imgOriginal + imgLines;
		imshow("Original", imgOriginal); //show the original image

		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}


void getDiffInVideo(){
	//global variables/Users/dehan/Documents/xc/xcode/opncvStarter/OpencvTut/OpencvTut/ProBlobDetection.cpp
	Mat frame; //current frame
	Mat resize_blur_Img;
	Mat fgMaskMOG2; //fg mask fg mask generated by MOG2 method
	Mat binaryImg;
	//Mat TestImg;
	Mat ContourImg; //fg mask fg mask generated by MOG2 method
	Ptr< BackgroundSubtractor> pMOG2; //MOG2 Background subtractor


	//#######################################################################
	//KalmanFilter KF(4, 2, 0);
	//Point blobPos;

	//// intialization of KF...
	//KF.transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);
	//Mat_<float> measurement(2, 1); measurement.setTo(Scalar(0));

	//KF.statePre.at<float>(0) = blobPos.x;
	//KF.statePre.at<float>(1) = blobPos.y;
	//KF.statePre.at<float>(2) = 0;
	//KF.statePre.at<float>(3) = 0;
	//setIdentity(KF.measurementMatrix);
	//setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
	//setIdentity(KF.measurementNoiseCov, Scalar::all(10));
	//setIdentity(KF.errorCovPost, Scalar::all(.1));
	//// Image to show mouse tracking
	//vector<Point> mousev, kalmanv;
	//mousev.clear();
	//kalmanv.clear();
	//#######################################################################


	pMOG2 = new BackgroundSubtractorMOG2(300, 32, true);//300,0.0);

	char fileName[100] = "E:/Final Year/project/human tracking/coding/VS_OpenCV/video/cctv.mp4"; //video/\mm2.avi"; //mm2.avi"; //cctv 2.mov"; //mm2.avi"; //";//_p1.avi";
	VideoCapture stream1(fileName);   //0 is the id of video device.0 if you have only one camera

	//morphology element
	Mat element = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));






	Mat imgTmp;
	stream1.read(imgTmp);
	int iLastX = -1;
	int iLastY = -1;
	Mat imgLines = Mat::zeros(imgTmp.size(), CV_8UC3);
	//resize(imgLines, imgLines, Size(imgLines.size().width / 2, imgLines.size().height / 2));

	//unconditional loop
	while (true) {
		Mat cameraFrame;
		if (!(stream1.read(frame))) //get one frame form video
			break;

		//cvtColor(frame, frame, CV_BGR2HSV);
		resize_blur_Img = frame;
		//cvtColor(resize_blur_Img, resize_blur_Img, CV_BGR2HSV);
		//Resize
		//resize(frame, resize_blur_Img, Size(frame.size().width / 2, frame.size().height / 2));
		//Blur
		blur(resize_blur_Img, resize_blur_Img, Size(4, 4));
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


		//------------------------------------------------------------
		//Calculate the moments of the thresholded image
		Moments oMoments = moments(binaryImg);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;

		// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
		if (dArea > 10000)
		{
			//calculate the position
			int posX = dM10 / dArea;
			int posY = dM01 / dArea;

			if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
			{
				//Draw a red line from the previous point to the current point
				line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
			}

			iLastX = posX;
			iLastY = posY;
		}
		//------------------------------------------------------------



		//Find contour
		ContourImg = binaryImg.clone();
		//less blob delete
		vector< vector< Point> > contours;
		findContours(ContourImg,
			contours, // a vector of contours
			CV_RETR_EXTERNAL, // retrieve the external contours
			CV_CHAIN_APPROX_NONE); // all pixels of each contours

		vector< Rect > output;
		vector< vector< Point> >::iterator itc = contours.begin();

		//#######################################################################
		//vector<vector<int>> blobmap;
		//#######################################################################

		while (itc != contours.end()) {

			//Create bounding rect of object
			//rect draw on origin image
			Rect mr = boundingRect(Mat(*itc));
			//Mat window(mr.height,originalImage.width,CV_8UC3,Scalar(255));

			//#######################################################################
			/*vector<int> pos;
			pos.push_back(mr.x);
			pos.push_back(mr.y);
			blobmap.push_back(pos);*/
			//#######################################################################

			rectangle(resize_blur_Img, mr, CV_RGB(255, 0, 0));

			++itc;
		}


		//#######################################################################
		//if (blobmap.size() != 0)
		//{
		//	// First predict, to update the internal statePre variable
		//	Mat prediction = KF.predict();
		//	Point predictPt(prediction.at<float>(0), prediction.at<float>(1));

		//	// Get mouse point
		//	vector<int> firstPoint = blobmap.front();
		//	measurement(0) = firstPoint.front();
		//	measurement(1) = firstPoint.back();

		//	// The update phase 
		//	Mat estimated = KF.correct(measurement);

		//	Point statePt(estimated.at<float>(0), estimated.at<float>(1));
		//	Point measPt(measurement(0), measurement(1));
		//	// plot points
		//	//imshow("Blur_Resize", resize_blur_Img);
		//	//resize_blur_Img = Scalar::all(0);

		//	mousev.push_back(measPt);
		//	kalmanv.push_back(statePt);

		//	for (int i = 0; i < mousev.size() - 1; i++)
		//		line(resize_blur_Img, mousev[i], mousev[i + 1], Scalar(255, 255, 0), 1);

		//	for (int i = 0; i < kalmanv.size() - 1; i++)
		//		line(resize_blur_Img, kalmanv[i], kalmanv[i + 1], Scalar(0, 155, 255), 1);

		//}
		//#######################################################################


		///////////////////////////////////////////////////////////////////

		//Display
		resize_blur_Img = resize_blur_Img + imgLines;
		imshow("Shadow_Removed", binaryImg);
		imshow("Blur_Resize", resize_blur_Img);
		//imshow("MOG2", fgMaskMOG2);

		if (waitKey(100) == 27)
			break;
	}
}