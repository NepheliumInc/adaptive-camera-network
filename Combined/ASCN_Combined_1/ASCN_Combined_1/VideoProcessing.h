#include <opencv2\highgui\highgui.hpp>	// open cv
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\video\tracking.hpp>
#include <opencv2\video\background_segm.hpp>

// blob detection
// in	: *frame
// out	: contours

// human detection
// in	: *contours, *frame
// out	: filtered_contours

// kalman filtering

// comparison algo (get relevent contours)

using namespace std;
using namespace cv;


class VideoProcessing
{
public:
	VideoProcessing();
	int BlobDetection(Mat *frame, vector<vector<Point>> *outBlobs);
	int HumanDetection(vector<vector<Point>> *blobs, Mat *frame, vector<vector<Point>> *outHumanBlobs);
	~VideoProcessing();

private:
	Mat thresh_frame;
	Ptr<BackgroundSubtractor> backSubPtr = new BackgroundSubtractorMOG2(300, 32, true);
	Mat morpho_ele = getStructuringElement(MORPH_RECT, Size(7, 7), Point(3, 3));

protected:

};