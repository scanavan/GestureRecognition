#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include <vector>
#include "Image.h"
#include "LeapData.h"
#include <set>

#define _FILE_NUM_ 0
#define PI 3.14159265

struct Occ {
	int nonZ;
	float avgD;
	Occ(int nonZ, float avgD);
};

class KinectMotion {
public:
	KinectMotion(const char * filepath);
	Image * getDepth();
	Image * getRgb();
	cv::Mat updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary = true);
	float blobMax(cv::Mat depth);
	cv::Mat getHand(cv::Mat image, double ratio);
	cv::Mat makeContourImage(cv::Mat image);
	cv::Point handCenter(cv::Mat image);
	cv::Point palmCenter(cv::Mat image);
	std::vector <Occ> cellOccupancy(cv::Mat image);
	void findDirection(cv::Mat image);
	cv::Mat scaleHand(cv::Mat image);
	std::vector<float>distContour(cv::Mat image);
	cv::Rect getRect(cv::Mat image);
	cv::Mat rotateImage(cv::Mat image);
	//void blob(cv::Mat imMat);
	//int * palmCenter2(cv::Mat image);
private:
	Image * depth;
	Image * rgb;
	LeapData * leap;
}; 

std::vector<cv::Point> getContour(cv::Mat image);
void createWindow(cv::Mat image, std::string imageName);

#endif