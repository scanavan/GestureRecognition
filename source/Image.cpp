#include <iostream>
#include "Image.h"
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv\cv.h> 

Image::Image(std::string imageName){
	this->imageName = imageName;
	this->image = cv::imread(imageName, CV_LOAD_IMAGE_UNCHANGED);
}

int Image::getHeight() {
	return image.rows;
}

int Image::getWidth() {
	return image.cols;
}

void Image::displayImage() {
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, image);
	cv::waitKey(0);
}

void Image::deleteWindow() {
	cv::destroyWindow(imageName);
}

std::string Image::getName() {
	return imageName;
}

cv::Mat Image::returnImage() {
	return image;
}