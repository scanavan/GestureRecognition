#include <iostream>
#include "KinectImage.h"
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv\cv.h>  

KinectImage::KinectImage(std::string imageName){
	this->imageName = imageName;
	this->image = cv::imread(imageName, CV_LOAD_IMAGE_UNCHANGED);
}

int KinectImage::getHeight() {
	return image.rows;
}

int KinectImage::getWidth() {
	return image.cols;
}

void KinectImage::displayImage() {
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, image);
	cv::waitKey(0);
}

void KinectImage::deleteWindow() {
	cv::destroyWindow(imageName);
}

std::string KinectImage::getName() {
	return imageName;
}

cv::Mat KinectImage::returnImage() {
	return image;
}