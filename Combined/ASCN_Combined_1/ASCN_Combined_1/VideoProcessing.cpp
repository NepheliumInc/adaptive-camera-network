#include "VideoProcessing.h"

VideoProcessing::VideoProcessing(){}

int VideoProcessing::BlobDetection(Mat *frame, vector<vector<Point>> *outBlobs){
	blur(*frame, thresh_frame, Size(4, 4));   // blur ---
	backSubPtr->operator()(thresh_frame, thresh_frame, -1);  // background substraction
	morphologyEx(thresh_frame, thresh_frame, CV_MOP_CLOSE, morpho_ele);  // morphology pre processing
	threshold(thresh_frame, thresh_frame, 128, 255, CV_THRESH_BINARY);  // shadow removing, binary
	findContours(thresh_frame, *outBlobs, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  // inputFrame, output vector of blobs, retrieve external blobs, all px of each contour
	return (*outBlobs).size();
}

int VideoProcessing::HumanDetection(vector<vector<Point>> *blobs, Mat *frame, vector<vector<Point>> *outHumanBlobs){
	for (size_t i = 0; i < (*blobs).size(); i++)
	{
		if (contourArea((*blobs)[i]) > 100)
		{
			(*outHumanBlobs).push_back((*blobs)[i]);
		}
	}
	return (*outHumanBlobs).size();
}

VideoProcessing::~VideoProcessing(){}