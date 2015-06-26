#include <iostream>
#include <Windows.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/background_segm.hpp>

#include "Models.h"

using namespace std;
using namespace cv;

#define drawCross( img, center, color, d )\
line(img, Point(center.x - d, center.y - d), Point(center.x + d, center.y + d), color, 2, CV_AA, 0);\
line(img, Point(center.x + d, center.y - d), Point(center.x - d, center.y + d), color, 2, CV_AA, 0 );

int getReleventContour(Point p);

int measurmentMapIndex = 0;
map<int, models::TrackingObject> trackingObjectMap;

int main()
{
	Mat frame, thresh_frame;
	VideoCapture capture("E:/Final Year/project/human tracking/coding/VS_OpenCV/video/cctv.mp4");  //capture video file from local file
	VideoWriter outputVideo;
	vector<vector<Point> > contours, filtered_contours;

	Ptr<BackgroundSubtractor> backSubPtr = new BackgroundSubtractorMOG2(300, 32, true);  // ---
	Mat morpho_ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3,3));  // ---
	Mat drawing = Mat::zeros(frame.size(), CV_8UC1);  //

	if (!(capture.read(frame)))  // error msg when video cannot read
	{
		cerr << "Problem opening video source" << endl;
		return -1;
	}

	// Setup output video
	string NAME = "E:/output-tracking000.avi";   // Form the new name with container
	Size S = Size((int)capture.get(CV_CAP_PROP_FRAME_WIDTH),(int)capture.get(CV_CAP_PROP_FRAME_HEIGHT));
	outputVideo = VideoWriter(NAME, CV_FOURCC('x','v','i','d'), 3/*capture.get(CV_CAP_PROP_FPS)*/, S, true);
	
	if (!(outputVideo.isOpened()))
	{
		cout << "Couldn't open the output file" << endl;
		return -1;
	}

	//int imgindex = 0;

	while (waitKey(10) != 27 && capture.grab()) // terminate when ESC pressed
	{
		capture.read(frame);  // read video frame and put it to frame mat
		//resize(frame, frame, Size(frame.size().width / 2, frame.size().height / 2));
		filtered_contours.clear();  // clear fitered contours vector
		blur(frame, thresh_frame, Size(4, 4));   // blur ---
		backSubPtr->operator()(thresh_frame, thresh_frame, -1);  // background substraction ---
		morphologyEx(thresh_frame, thresh_frame, CV_MOP_CLOSE, morpho_ele);  // morphology pre processing ---
		threshold(thresh_frame, thresh_frame, 128, 255, CV_THRESH_BINARY);  // shadow removing, binary ---
		findContours(thresh_frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  // inputFrame, output vector of contours, retrieve external contours, all px of each contour ---

		if (contours.size() == 0)
		{
			cout << "XX - no contours for the frame" << endl;
			imshow("Video", frame);
			continue;
		}

		for (size_t i = 0; i < contours.size(); i++)
		{
			if (contourArea(contours[i]) > 100)
			{
				filtered_contours.push_back(contours[i]);
				drawContours(thresh_frame, contours, i, Scalar::all(255), CV_FILLED, 8, vector<Vec4i>(), 0, Point());
			}
		}

		if (filtered_contours.size() == 0)
		{
			cout << "XX - no fitered contours" << endl;
			imshow("Video", frame);
			continue;
		}

		// get the moments
		vector<Moments> mu(filtered_contours.size());
		for (size_t i = 0; i < filtered_contours.size(); i++)
		{
			mu[i] = moments(filtered_contours[i], false);
		}

		// get the mass centers:
		vector<Point2f> mc(filtered_contours.size());
		for (size_t i = 0; i < filtered_contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		for (size_t i = 0; i < mc.size(); i++)
		{
			int selectedContourId = getReleventContour(mc[i]);
			
			Point measurePre(trackingObjectMap[selectedContourId].getLastPoint());
			vector<Point> pointsVector = trackingObjectMap[selectedContourId].addCenterPoint(mc[i]);
			Point measureCurrent(trackingObjectMap[selectedContourId].getLastPoint());
/*
			cout << selectedContourId << " :: POINT-PRE (" << measurePre.x << " , " << measurePre.y << ")" << endl;
			cout << selectedContourId << " :: POINT-POST (" << measureCurrent.x << " , " << measureCurrent.y << ")" << endl;
*/
			//drawCross(frame, measurePre, Scalar(255, 0, 0), 5);
			drawCross(frame, measureCurrent, Scalar(0, 255, 0), 5);

			Mat prediction = trackingObjectMap[selectedContourId].kalmanPredict();
			Point kalmanPredict(prediction.at<float>(0), prediction.at<float>(1));

			Mat corrected = trackingObjectMap[selectedContourId].kalmanCorrect(mc[i]);
			Point kalmanCorrected(corrected.at<float>(0), corrected.at<float>(1));
/*
			cout << "measurmentMat :: " << kalmanFilterMap[selectedContourId].measurementMatrix.at<float>(0) << " - " << kalmanFilterMap[selectedContourId].measurementMatrix.at<float>(1) << endl;
			cout << "predictiedMat :: " << kalmanFilterMap[selectedContourId].statePre.at<float>(0) << " - " << kalmanFilterMap[selectedContourId].statePre.at<float>(1) << " - " << kalmanFilterMap[selectedContourId].statePre.at<float>(2) << " - " << kalmanFilterMap[selectedContourId].statePre.at<float>(3) << endl;
			cout << "_correctedMat :: " << kalmanFilterMap[selectedContourId].statePost.at<float>(0) << " - " << kalmanFilterMap[selectedContourId].statePost.at<float>(1) << " - " << kalmanFilterMap[selectedContourId].statePost.at<float>(2) << " - " << kalmanFilterMap[selectedContourId].statePost.at<float>(3) << endl;
*/
			drawCross(frame, kalmanPredict, Scalar(0, 0, 255), 5);
			//drawCross(frame, kalmanCorrected, Scalar(0, 255, 255), 5);

			stringstream lbl;
			lbl << "Blob_" << selectedContourId;
			putText(frame, lbl.str(), measureCurrent, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 2);

			for (int i = pointsVector.size()-1; i !=0; i--)
			{
				line(frame, pointsVector[i], pointsVector[i-1], Scalar(200, 220, 80), 3);
			}
		}

		vector<vector<Point> > contours_poly(filtered_contours.size());
		vector<Rect> boundRect(filtered_contours.size());
		for (size_t i = 0; i < filtered_contours.size(); i++)
		{
			approxPolyDP(Mat(filtered_contours[i]), contours_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(contours_poly[i]));
		}

		for (size_t i = 0; i < filtered_contours.size(); i++)
		{
			rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 0), 2, 8, 0);// scalar is for Bounding Box
		}

		imshow("Video", frame);
		/*stringstream imgname;
		imgname << "E:/" << imgindex << ".jpg";
		imwrite(imgname.str(), frame);
		imgindex++;*/
		outputVideo.write(frame);
	}

	return 0;
}



int getReleventContour(Point p)
{
	map<int, models::TrackingObject> possibleObjectMap;

	for (map<int, models::TrackingObject>::iterator i = trackingObjectMap.begin(); i != trackingObjectMap.end(); i++)
	{
		if ((abs(p.x - i->second.getLastPoint().x) < 90) && (abs(p.y - i->second.getLastPoint().y) < 90))
			possibleObjectMap[i->first] = i->second;
	}

	if (possibleObjectMap.size() == 0)
	{
		int newIndex = measurmentMapIndex;
		models::TrackingObject tObj(p);
		trackingObjectMap[newIndex] = tObj;
		measurmentMapIndex++;
		return newIndex;
	}
	else if (possibleObjectMap.size() == 1)
		return possibleObjectMap.begin()->first;
	else
		//get predicted map and select one of them
		return possibleObjectMap.begin()->first;
}
