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
	Point(int i = 0, int j = 0);
}; 

struct Occ {
	int nonZ;
	float avgD;
	Occ(int nonZ, float avgD);
};

class KinectMotion {
public:
	KinectMotion(std::string iDepth, std::string iRgb);
	Image getDepth();
	Image getRgb();
	cv::Mat updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary = true);
	float blobMax(cv::Mat depth);
	cv::Mat getHand(cv::Mat image, double ratio);
	std::vector <Point> findEdges(cv::Mat image);
	cv::Mat makeEdgeImage(cv::Mat image);
	cv::Point handCenter(cv::Mat image);
	cv::Point palmCenter(cv::Mat image);
	std::vector <Occ> cellOccupancy(cv::Mat image);
	void findDirection(cv::Mat image);
	void normalizeHand(cv::Mat image);
	//void blob(cv::Mat imMat);
	//int * palmCenter2(cv::Mat image);
private:
	Image depth;
	Image rgb;
}; 

std::vector<cv::Point> getContour(cv::Mat image);

#endif