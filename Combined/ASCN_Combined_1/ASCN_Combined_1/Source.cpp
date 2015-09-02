/*
	Authors: K D K Madusanka [kdk.madusanka@gmail.com]

	Description:
		This is the main source code for the "Adaptive Survellance Camera Networks" project.
		All other suppotive functionalities are referenced.
		The main program structure and multi-threading happening in here.

	Log:
*/


#include <stdio.h>
#include <iostream>

#include <Windows.h> // threading purpose
#include <process.h>

#include <opencv2\highgui\highgui.hpp>	// open cv
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <opencv2\video\background_segm.hpp>

#include "Graph.h"
#include "Models.h"
#include "VideoProcessing.h"

unsigned int __stdcall mythread(void* data);

graph::Graph _graph;


int main(int argc, char* argv[])
{
	cout << "Advanced Surveillance Camera Network (ASCN)\nInitialting ...\n";

	cout << "Network Graph checking ...\n";

	_graph = graph::Graph();
	

	_graph.initGraph();

	if (!(_graph.isConfigured()))
	{
		// cofigure graph.
		_graph.addNode(graph::Node("C001", "start", "1.1.1.1", "GR1"));
		_graph.addNode(graph::Node("C002", "normal", "1.1.1.2", "GR2"));
		_graph.addNode(graph::Node("C003", "normal", "1.1.1.3", "GR3"));
		_graph.addNeighbour("C001", "C002", 8, 10);
		_graph.addNeighbour("C002", "C001", 2, 10);
		_graph.addNeighbour("C001", "C003", 6, 1);
		_graph.addNeighbour("C003", "C001", 4, 1);
	}


	// config graph with process ids - initialte threds and assign thred id to nodes in graph
	vector<string> nodeKeys = _graph.getKeysOfAllNodes();
	
	for (int i = 0; i < nodeKeys.size(); i++)
	{
		const char *s = nodeKeys[i].c_str();
		_beginthreadex(NULL, 0, &mythread, (void*)&(*s), 0, NULL);
	}

	system("pause");
	return 0;

}









#define drawCross( img, center, color, d )\
line(img, Point(center.x - d, center.y - d), Point(center.x + d, center.y + d), color, 2, CV_AA, 0);\
line(img, Point(center.x + d, center.y - d), Point(center.x - d, center.y + d), color, 2, CV_AA, 0 );


int measurmentMapIndex = 0;
map<int, models::TrackingObject> trackingObjectMap;


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



unsigned int __stdcall mythread(void* data)
{
	char *nodeIdptr = static_cast<char*>(data);
	printf("THREAD [%d]:[%s]:: started.\n", GetCurrentThreadId(), &(*nodeIdptr));
	
	graph::Node *currentNodePtr = _graph.getNode(&(*nodeIdptr));
	//printf("THREAD [%d]:[%s]:: IP = %s\n", GetCurrentThreadId(), &(*nodeIdptr), (currentNodePtr->IP).c_str());

	currentNodePtr->ThreadId = GetCurrentThreadId();
	string videoLink = currentNodePtr->IP;

	printf("%s :: %d :: %s \n", currentNodePtr->Id.c_str(), currentNodePtr->ThreadId, videoLink.c_str());

	// ###################################################
	// opencv video tracking

	Mat frame;
	VideoCapture videoCapture(videoLink);
	vector<vector<Point> > blobs, filtered_blobs;
	
	//Ptr<BackgroundSubtractor> backSubPtr = new BackgroundSubtractorMOG2(300, 32, true);  // ---
	//Mat morpho_ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));  // ---
	Mat drawing = Mat::zeros(frame.size(), CV_8UC1);  //

	VideoProcessing _vProcessing = VideoProcessing();

	if (!(videoCapture.read(frame)))  // error msg when video cannot read
	{
		cerr << "Problem opening video source" << endl;
		return -1;
	}
	
	while (waitKey(10) != 27 && videoCapture.grab()) // terminate when ESC pressed
	{
		videoCapture.read(frame);  // read video frame and put it to frame mat
		if (frame.size().width > 1360 || frame.size().height > 760){
			resize(frame, frame, Size(frame.size().width / 2, frame.size().height / 2));
		}
		filtered_blobs.clear();  // clear fitered blobs vector
		
		if (_vProcessing.BlobDetection(&frame, &blobs) == 0)	// Bolb Detection
		{
			imshow(currentNodePtr->Id.c_str(), frame);
			continue;	// If no blobs detected continue while
		}

		if (_vProcessing.HumanDetection(&blobs, &frame, &filtered_blobs) == 0)	// Human Identification
		{
			imshow(currentNodePtr->Id.c_str(), frame);
			continue;	// If no human detected
		}

		// get the moments
		vector<Moments> mu(filtered_blobs.size());
		for (size_t i = 0; i < filtered_blobs.size(); i++)
		{
			mu[i] = moments(filtered_blobs[i], false);
		}

		// get the mass centers:
		vector<Point2f> mc(filtered_blobs.size());
		for (size_t i = 0; i < filtered_blobs.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		for (size_t i = 0; i < mc.size(); i++)
		{
			int selectedContourId = getReleventContour(mc[i]);

			Point measurePre(trackingObjectMap[selectedContourId].getLastPoint());
			vector<Point> pointsVector = trackingObjectMap[selectedContourId].addCenterPoint(mc[i]);
			Point measureCurrent(trackingObjectMap[selectedContourId].getLastPoint());

			drawCross(frame, measureCurrent, Scalar(0, 255, 0), 5);

			Mat prediction = trackingObjectMap[selectedContourId].kalmanPredict();
			Point kalmanPredict(prediction.at<float>(0), prediction.at<float>(1));

			Mat corrected = trackingObjectMap[selectedContourId].kalmanCorrect(mc[i]);
			Point kalmanCorrected(corrected.at<float>(0), corrected.at<float>(1));
			
			drawCross(frame, kalmanPredict, Scalar(0, 0, 255), 5);
			//drawCross(frame, kalmanCorrected, Scalar(0, 255, 255), 5);

			stringstream lbl;
			lbl << "Blob_" << selectedContourId;
			putText(frame, lbl.str(), measureCurrent, FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 255), 2);

			for (int i = pointsVector.size() - 1; i != 0; i--)
			{
				line(frame, pointsVector[i], pointsVector[i - 1], Scalar(200, 220, 80), 3);
			}
		}

		vector<vector<Point> > blobs_poly(filtered_blobs.size());
		vector<Rect> boundRect(filtered_blobs.size());
		for (size_t i = 0; i < filtered_blobs.size(); i++)
		{
			approxPolyDP(Mat(filtered_blobs[i]), blobs_poly[i], 3, true);
			boundRect[i] = boundingRect(Mat(blobs_poly[i]));
		}

		for (size_t i = 0; i < filtered_blobs.size(); i++)
		{
			rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(255, 255, 0), 2, 8, 0);// scalar is for Bounding Box
		}

		imshow(currentNodePtr->Id.c_str(), frame);
	}
	// end opencv video tracking


	printf("\nNode %s::%d FINISHED \n", currentNodePtr->Id.c_str(), currentNodePtr->ThreadId);
	return 0;
}
