
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/video/tracking.hpp>

using namespace std;
using namespace cv;

namespace models
{
	class TrackingObject
	{
	public:
		TrackingObject();
		TrackingObject(Point initCenterPoint); // initialize center points vector and kalman object from initial center point
		~TrackingObject();

		vector<Point> addCenterPoint(Point p); // add new point to the end of Point vector and returns new vector<Point>
		Point getLastPoint(); // get the last point of the vector
		Mat kalmanPredict(); // call predict function for the internal kalman object
		Mat kalmanCorrect(Point p); // call correct function for the internal kalman object

	protected:
		vector<Point> centerPoint; // list of center points (5 of history and current)
		KalmanFilter kalmanFilter; // kalman filter object for the current human blob
		int paneltyIndex; // count of consecutive frames that blob doesn't appear

	};
}