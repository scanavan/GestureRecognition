#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include <vector>
#include "KinectImage.h"
#include "LeapData.h"
#include <set>

#define _FILE_NUM_ 0
#define PI 3.14159265
#define _SCALE_ 480

struct Occ {
	int nonZ;
	float avgD;
	Occ(int nonZ, float avgD);
};

class KinectMotion {
public:
	KinectMotion(const char * filepath);
	KinectImage * getDepth();
	KinectImage * getRgb();
	cv::Mat updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary = true);
	float blobMax(cv::Mat depth);
	cv::Mat getHand(cv::Mat image, double ratio);
	cv::Mat makeContourImage(cv::Mat image);
	cv::Point handCenter(cv::Mat image);
	std::vector <Occ> cellOccupancy(cv::Mat image);
	void findDirection(cv::Mat image);
	cv::Mat scaleHand(cv::Mat image);
	std::vector<float>distContour(cv::Mat image);
	cv::Rect getRect(cv::Mat image);
	cv::Mat rotateImage(cv::Mat image);
	//void blob(cv::Mat imMat);
	//int * palmCenter2(cv::Mat image);
private:
	KinectImage * depth;
	KinectImage * rgb;
	LeapData * leap;
}; 

cv::Point palmCenter(cv::Mat image);
std::vector<cv::Point> getContour(cv::Mat image);
void createWindow(cv::Mat image, std::string imageName);
float * silhouette(cv::Mat image);

/*
 * FEATURES
 * 
 * Silhouette			float[32]
 * Contour Distances	float[180] 
 * Cell Occupancy		float[?] - depends on scaled image size
 * Convex Hull			float[6]
 * 
 */

#endif