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

cv::Point palmCenter(cv::Mat image, int thresh);
std::vector<cv::Point> getContour(cv::Mat image);
void createWindow(cv::Mat image, std::string imageName);
float * silhouette(cv::Mat image);
float * hullAreas(cv::Mat image);

/*
 * FEATURES
 * 
 * Silhouette			float[32]
 * Contour Distances	float[180] 
 * Cell Occupancy		int[?], float[?] - depends on scaled image size
 * Convex Hull			float[6]
 * 
 */

#endif