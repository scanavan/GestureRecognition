#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include "Image.h"

class KinectMotion {
public:
	KinectMotion(std::string iDepth, std::string iRgb);
	Image getDepth();
	Image getRgb();
	cv::Mat displayUpdatedImage(int upperThresholdVal, int lowerThresholdVal);
	void findBiggestBlob(cv::Mat imMat);
	void blob(cv::Mat imMat);
private:
	Image depth;
	Image rgb;
}; 
#endif