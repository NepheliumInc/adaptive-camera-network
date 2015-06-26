//#include <iostream>
//#include <Windows.h>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/video/tracking.hpp>
//#include <opencv2/video/background_segm.hpp>
//
//using namespace std;
//using namespace cv;
//
//#define drawCross( img, center, color, d )\
//line(img, Point(center.x - d, center.y - d), Point(center.x + d, center.y + d), color, 2, CV_AA, 0);\
//line(img, Point(center.x + d, center.y - d), Point(center.x - d, center.y + d), color, 2, CV_AA, 0 )
//
//#include "MultipleMomentOnePredit.h"
//
//string getReleventContour(Point p);
//
//map<string, Mat_<float>> measurementMap;  // <contourId, measureMap>
//int measurmentMapIndex = 0;
//map<string, KalmanFilter> kalmanFilterMap;  // <contourId, kalmanObj>
//
//int MultipleMomentMultiplePredit::_function()
//{
//	/*MultipleMomentOnePredit mmop;
//	mmop._function();*/
//
//	Mat frame, blur_frame, back_subtracted_frame, morpho_frame, thresh_frame;  //for current frame and thrsholed frame
//	vector<Mat> channels;  //
//	VideoCapture capture("E:/Final Year/project/human tracking/coding/VS_OpenCV/video/Axis P1355 Loading Dock Night(iphone).mp4");  //capture video file from local file
//	vector<Vec4i> hierarchy;  //
//	vector<vector<Point> > contours, filtered_contours;  //
//
//	Ptr<BackgroundSubtractor> backSubPtr = new BackgroundSubtractorMOG2(300, 32, true);  // ---
//	Mat morpho_ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));  // ---
//	Mat drawing = Mat::zeros(frame.size(), CV_8UC1);  //
//
//	if (!(capture.read(frame)))  // error msg when video cannot read
//		cerr << "Problem opening video source" << endl;
//
//	while (waitKey(1) != 27 && capture.grab()) // terminate when ESC pressed
//	{
//		capture.read(frame);  // read video frame and put it to frame mat
//
//		//resize(frame, frame, Size(frame.size().width / 2, frame.size().height / 2));
//
//		filtered_contours.clear();  // clear fitered contours vector
//
//		blur(frame, blur_frame, Size(4, 4));   // blur ---
//		backSubPtr->operator()(blur_frame, back_subtracted_frame, -1);  // background substraction ---
//
//		morphologyEx(back_subtracted_frame, morpho_frame, CV_MOP_CLOSE, morpho_ele);  // morphology pre processing ---
//
//		threshold(morpho_frame, thresh_frame, 128, 255, CV_THRESH_BINARY);  // shadow removing, binary ---
//
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
//			if (contourArea(contours[i]) > 50)
//			{
//				filtered_contours.push_back(contours[i]);
//				drawContours(thresh_frame, contours, i, Scalar::all(255), CV_FILLED, 8, vector<Vec4i>(), 0, Point());
//			}
//		}
//
//		if (filtered_contours.size() == 0)
//		{
//			cout << "XX - no fitered contours" << endl;
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
//		/*
//		cout << "----------------------------------------------" << endl;
//		*/
//		for (size_t i = 0; i < mc.size(); i++)
//		{
//			string selectedContourId = getReleventContour(mc[i]);
//
//			Mat_<float> measurement = measurementMap[selectedContourId];
//			Point measurePre(measurement(0), measurement(1));
//
//			measurement(0) = mc[i].x;
//			measurement(1) = mc[i].y;
//			Point measureCurrent(measurement(0), measurement(1));
//
//			measurementMap[selectedContourId] = measurement;
//			/*
//			cout << selectedContourId << " :: POINT-PRE (" << measurePre.x << " , " << measurePre.y << ")" << endl;
//			cout << selectedContourId << " :: POINT-POST (" << measureCurrent.x << " , " << measureCurrent.y << ")" << endl;
//			*/
//			drawCross(frame, measurePre, Scalar(255, 0, 0), 5);
//			drawCross(frame, measureCurrent, Scalar(0, 255, 0), 5);
//
//			Mat prediction = kalmanFilterMap[selectedContourId].predict();
//			Point kalmanPredict(prediction.at<float>(0), prediction.at<float>(1));
//
//			Mat corrected = kalmanFilterMap[selectedContourId].correct(measurement);
//			Point kalmanCorrected(corrected.at<float>(0), corrected.at<float>(1));
//			/*
//			cout << selectedContourId << " :: KALMAN-PREDICTED (" << kalmanPredict.x << " , " << kalmanPredict.y << ")" << endl;
//			cout << selectedContourId << " :: KALMAN-statePre (" << kalmanFilterMap[selectedContourId].statePre.at<float>(0) << " , " << kalmanFilterMap[selectedContourId].statePre.at<float>(1) << ")" << endl;
//			cout << selectedContourId << " :: KALMAN-ESTIMATED (" << kalmanCorrected.x << " , " << kalmanCorrected.y << ")" << endl;
//			cout << selectedContourId << " :: KALMAN-statePost (" << kalmanFilterMap[selectedContourId].statePost.at<float>(0) << " , " << kalmanFilterMap[selectedContourId].statePost.at<float>(1) << ")" << endl;
//			*/
//			/*
//			cout << "measurmentMat :: " << kalmanFilterMap[selectedContourId].measurementMatrix.at<float>(0) << " - " << kalmanFilterMap[selectedContourId].measurementMatrix.at<float>(1) << endl;
//			cout << "predictiedMat :: " << kalmanFilterMap[selectedContourId].statePre.at<float>(0) << " - " << kalmanFilterMap[selectedContourId].statePre.at<float>(1) << " - " << kalmanFilterMap[selectedContourId].statePre.at<float>(2) << " - " << kalmanFilterMap[selectedContourId].statePre.at<float>(3) << endl;
//			cout << "_correctedMat :: " << kalmanFilterMap[selectedContourId].statePost.at<float>(0) << " - " << kalmanFilterMap[selectedContourId].statePost.at<float>(1) << " - " << kalmanFilterMap[selectedContourId].statePost.at<float>(2) << " - " << kalmanFilterMap[selectedContourId].statePost.at<float>(3) << endl;
//			*/
//			drawCross(frame, kalmanPredict, Scalar(0, 0, 255), 5);
//			drawCross(frame, kalmanCorrected, Scalar(0, 255, 255), 5);
//
//		}
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
//			rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 0), 2, 8, 0);// scalar is for Bounding Box
//		}
//
//		//imshow("thresh frame", thresh_frame);
//		imshow("Video", frame);
//	}
//	return 0;
//}
//
//
//
//string getReleventContour(Point p)
//{
//	map<string, Mat_<float>> possibleMeasurmentMap;
//	for (map<string, Mat_<float>>::iterator i = measurementMap.begin(); i != measurementMap.end(); i++)
//	{
//		if ((abs(p.x - i->second(0)) < 40) && (abs(p.y - i->second(1)) < 40))
//		{
//			possibleMeasurmentMap[i->first] = i->second;
//		}
//	}
//
//	if (possibleMeasurmentMap.size() == 0)
//	{
//		// initialize measurement map
//		Mat_<float> m(2, 1);   m.setTo(Scalar(0));
//		m(0) = p.x;
//		m(1) = p.y;
//		stringstream newIndex;
//		newIndex << "Blob_" << measurmentMapIndex;
//		measurementMap[newIndex.str()] = m;
//		// - end - measurment
//
//		// initialize kalman filter
//		KalmanFilter KF(4, 2, 0);
//		//Mat_<float> state(4, 1);
//		//Mat_<float> processNoise(4, 1, CV_32F);
//		KF.statePost.at<float>(0) = m(0);
//		KF.statePost.at<float>(1) = m(1);
//		KF.statePost.at<float>(2) = 0;
//		KF.statePost.at<float>(3) = 0;
//		KF.transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); // Including velocity
//		KF.processNoiseCov = *(cv::Mat_<float>(4, 4) << 0.2, 0, 0.2, 0, 0, 0.2, 0, 0.2, 0, 0, 0.3, 0, 0, 0, 0, 0.3);
//		setIdentity(KF.measurementMatrix);
//		setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
//		setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
//		setIdentity(KF.errorCovPost, Scalar::all(.1));
//		kalmanFilterMap[newIndex.str()] = KF;
//		// - end - kalman init
//
//		cout << "Init measurmentMat :: " << KF.measurementMatrix.at<float>(0) << " - " << KF.measurementMatrix.at<float>(1) << endl;
//		cout << "Init predictiedMat :: " << KF.statePre.at<float>(0) << " - " << KF.statePre.at<float>(1) << " - " << KF.statePre.at<float>(2) << " - " << KF.statePre.at<float>(3) << endl;
//		cout << "Init _correctedMat :: " << KF.statePost.at<float>(0) << " - " << KF.statePost.at<float>(1) << " - " << KF.statePost.at<float>(2) << " - " << KF.statePost.at<float>(3) << endl;
//
//		measurmentMapIndex++;
//
//		return newIndex.str();
//	}
//	else if (possibleMeasurmentMap.size() == 1)
//	{
//		return possibleMeasurmentMap.begin()->first;
//	}
//	else
//	{
//		//get predicted map and select one of them
//		return possibleMeasurmentMap.begin()->first;
//	}
//}
