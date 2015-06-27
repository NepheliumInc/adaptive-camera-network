//#include <iostream>
//#include <Windows.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/video/tracking.hpp>
//#include <opencv2/video/background_segm.hpp>
//
//#include "MultipleMomentOnePredit.h"
//
//using namespace std;
//using namespace cv;
//
//#define drawCross( img, center, color, d )\
//line(img, Point(center.x - d, center.y - d), Point(center.x + d, center.y + d), color, 2, CV_AA, 0);\
//line(img, Point(center.x + d, center.y - d), Point(center.x - d, center.y + d), color, 2, CV_AA, 0 )
//
//MultipleMomentOnePredit::MultipleMomentOnePredit(){}
//MultipleMomentOnePredit::~MultipleMomentOnePredit(){}
//
//int MultipleMomentOnePredit::_function()
//{
//	Mat frame, blur_frame, back_subtracted_frame, morpho_frame, thresh_frame;  //for current frame and thrsholed frame
//	vector<Mat> channels;  //
//	VideoCapture capture("E:/Final Year/project/human tracking/coding/VS_OpenCV/video/cctv.mp4");  //capture video file from local file
//	vector<Vec4i> hierarchy;  //
//	vector<vector<Point> > contours, filtered_contours;  //
//
//	Ptr<BackgroundSubtractor> backSubPtr = new BackgroundSubtractorMOG2(300, 32, true);  // ---
//	Mat morpho_ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));  // ---
//	Mat drawing = Mat::zeros(frame.size(), CV_8UC1);
//
//	if (!(capture.read(frame)))  // error msg when video cannot read
//		cerr << "Problem opening video source" << endl;
//
//	// kalman filter init
//	KalmanFilter KF(4, 2, 0);
//	Mat_<float> state(4, 1);
//	Mat_<float> processNoise(4, 1, CV_32F);
//	Mat_<float> measurement(2, 1);   measurement.setTo(Scalar(0));
//
//	KF.statePre.at<float>(0) = 0;
//	KF.statePre.at<float>(1) = 0;
//	KF.statePre.at<float>(2) = 0;
//	KF.statePre.at<float>(3) = 0;
//
//	KF.transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); // Including velocity
//	KF.processNoiseCov = *(cv::Mat_<float>(4, 4) << 0.2, 0, 0.2, 0, 0, 0.2, 0, 0.2, 0, 0, 0.3, 0, 0, 0, 0, 0.3);
//
//	setIdentity(KF.measurementMatrix);
//	setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
//	setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
//	setIdentity(KF.errorCovPost, Scalar::all(.1));
//	// - end - kalman init
//
//	while (waitKey(10) != 27 && capture.grab()) // terminate when ESC pressed
//	{
//		capture.read(frame);  // read video frame and put it to frame mat
//
//		filtered_contours.clear();  // clear fitered contours vector
//
//		// edge detect on frame and save in thresh_frame
//		//cvtColor(frame, thresh_frame, COLOR_BGR2GRAY);  // convert color to grayscale and save to thresh_frame mat
//		//Canny(thresh_frame, thresh_frame, 100, 200, 3);  // detect Edges.
//		//imshow("Edge Detection", thresh_frame);
//
//		//medianBlur(thresh_frame, thresh_frame, 5);  // median blur
//
//		blur(frame, blur_frame, Size(4, 4));   // blur ---
//		backSubPtr->operator()(blur_frame, back_subtracted_frame, -1);  // background substraction ---
//
//		morphologyEx(back_subtracted_frame, morpho_frame, CV_MOP_CLOSE, morpho_ele);  // morphology pre processing ---
//
//		threshold(morpho_frame, thresh_frame, 128, 255, CV_THRESH_BINARY);  // shadow removing, binary ---
//
//		//findContours(thresh_frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//		//imshow("thresh frame", thresh_frame);  // ---
//		findContours(thresh_frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  // inputFrame, output vector of contours, retrieve external contours, all px of each contour ---
//
//		if (contours.size() == 0)
//		{
//			cout << "XX - no contours for the frame" << endl;
//			imshow("Video", frame);
//			continue;
//		}
//
//		for (size_t i = 0; i < contours.size(); i++)
//		{
//			if (contourArea(contours[i]) > 500)
//			{
//				filtered_contours.push_back(contours[i]);
//				drawContours(thresh_frame, contours, i, Scalar::all(255), CV_FILLED, 8, vector<Vec4i>(), 0, Point());
//			}
//		}
//
//		for (size_t i = 0; i < filtered_contours.size(); i++)
//		{
//			cout << "CONTOUR_AREA to " << i << " th contour: " << contourArea(filtered_contours[i]) << endl;
//		}
//
//		if (filtered_contours.size() == 0)
//		{
//			cout << "XXXX - no fitered contours" << endl;
//			imshow("Video", frame);
//			continue;
//		}
//
//		// get the moments
//		vector<Moments> mu(filtered_contours.size());
//		for (size_t i = 0; i < filtered_contours.size(); i++)
//		{
//			mu[i] = moments(filtered_contours[i], false);
//		}
//
//		// get the mass centers:
//		vector<Point2f> mc(filtered_contours.size());
//		for (size_t i = 0; i < filtered_contours.size(); i++)
//		{
//			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
//		}
//
//		Mat prediction = KF.predict();
//		Point predictPt(prediction.at<float>(0), prediction.at<float>(1));
//
//		for (size_t i = 0; i < mc.size(); i++)
//		{
//			drawCross(frame, mc[i], Scalar(255, 0, 0), 5);// scalar is color for predicted cross mark on identified object.
//			measurement(0) = mc[i].x;
//			measurement(1) = mc[i].y;
//		}
//
//		Point measPt(measurement(0), measurement(1));
//
//		Mat estimated = KF.correct(measurement);
//		Point statePt(estimated.at<float>(0), estimated.at<float>(1));
//
//		drawCross(frame, statePt, Scalar(255, 255, 255), 5);// scalar is color for another correct cross mark on identified object.
//
//		vector<vector<Point> > contours_poly(filtered_contours.size());
//		vector<Rect> boundRect(filtered_contours.size());
//		for (size_t i = 0; i < filtered_contours.size(); i++)
//		{
//			approxPolyDP(Mat(filtered_contours[i]), contours_poly[i], 3, true);
//			boundRect[i] = boundingRect(Mat(contours_poly[i]));
//		}
//
//		for (size_t i = 0; i < filtered_contours.size(); i++)
//		{
//			rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2, 8, 0);// scalar is for Bounding Box
//		}
//
//		//imshow("thresh frame", thresh_frame);
//		imshow("Video", frame);
//	}
//	return 0;
//}