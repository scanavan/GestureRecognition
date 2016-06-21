#ifndef KINECTIMAGE_H
#define KINECTIMAGE_H
#include <string>
#include <opencv\cv.h>

class KinectImage {
public:
	KinectImage(std::string imageName);
	int getHeight();
	int getWidth();
	void displayImage();
	void deleteWindow();
	std::string getName();
	cv::Mat returnImage();
protected:
	std::string imageName;
	cv::Mat image;
};
#endif