#ifndef IMAGE_H
#define IMAGE_H
#include <string>
#include <opencv\cv.h>

class Image {
public:
	Image(std::string imageName);
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