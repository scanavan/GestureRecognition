#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include <vector>
#include "KinectImage.h"
#include "LeapData.h"
#include <set>

#define _FILE_NUM_		0
#define PI				3.14159265
#define _SCALE_			480
#define _SAMPLE_SIZE_	180.0
#define CELL_DIVS		8
#define NUM_CELLS		CELL_DIVS * CELL_DIVS

struct Occ {
	int nonZ;
	float avgD;
	Occ(int nonZ, float avgD);
};

class KinectMotion {
public:
	KinectMotion(const char * filepath);
	cv::Mat updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary = true);
	cv::Mat getHand(cv::Mat image, double ratio);
	cv::Mat makeContourImage(cv::Mat image);
	std::vector <Occ> cellOccupancy(cv::Mat image);
	void findDirection(cv::Mat image);
	cv::Mat scaleHand(cv::Mat image);
	std::vector<float>distContour(cv::Mat image);
	cv::Mat rotateImage(cv::Mat image);
	KinectImage * getDepth();
private:
	cv::Rect getRect(cv::Mat image);
	KinectImage * getRgb();
	KinectImage * depth;
	KinectImage * rgb;
	LeapData * leap;
}; 

void cellStuff(cv::Mat image);
cv::Mat binarize(cv::Mat image, int threshold = 5);
cv::Point palmCenter(cv::Mat image, int thresh = 23);
std::vector<cv::Point> getContour(cv::Mat image);
void createWindow(cv::Mat image, std::string imageName);
float * silhouette(cv::Mat image);
float * hullAreas(cv::Mat image);
bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);

cv::Mat newThreshold(cv::Mat image);

/*
 * FEATURES
 * 
 * Silhouette			float[32]
 * Contour Distances	float[180] 
 * Cell Occupancy		int[256], float[256] - size depends on scaled image size
 * Convex Hull			float[6]
 * 
 */

#endif