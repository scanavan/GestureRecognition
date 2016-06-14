#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include <vector>
#include "Image.h"
#include <set>

struct Point {
	int i;
	int j;
	Point(int i, int j);
};

class KinectMotion {
public:
	KinectMotion(std::string iDepth, std::string iRgb);
	Image getDepth();
	Image getRgb();
	cv::Mat updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary = true);
	void blob(cv::Mat imMat);
	float blobMax(cv::Mat depth);
	cv::Mat getHand(cv::Mat image, double ratio);
	std::vector <Point> findEdges(cv::Mat image);
	cv::Mat makeEdgeImage(cv::Mat image);
	Point handCenter(cv::Mat image);
	int * palmCenter(cv::Mat image);
	std::vector <std::set <float>> cellOccupancy(cv::Mat image);
private:
	Image depth;
	Image rgb;
}; 
#endif