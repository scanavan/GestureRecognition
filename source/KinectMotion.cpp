#include <iostream>
#include "KinectMotion.h"
#include <opencv\cv.h>
#include "Image.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "filenames.h" 

KinectMotion::KinectMotion(std::string iDepth, std::string iRgb):depth(Image(iDepth)),rgb(Image(iRgb)) {

}

Image KinectMotion::getDepth() {
	return depth;
}

Image KinectMotion::getRgb() {
	return rgb;
}

void KinectMotion::displayUpdatedImage(Image iDepth, int upperThresholdVal, int lowerThresholdVal) {
	cv::Mat iDepthMat = iDepth.returnImage();
	int h = iDepth.getHeight();
	int w = iDepth.getWidth();
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			cv::Scalar intensity = iDepthMat.at<uchar>(i, j);
			if (intensity.val[0] < upperThresholdVal && intensity.val[0] > lowerThresholdVal) {
				iDepthMat.at<uchar>(i, j) = 255;
			}
			else {
				iDepthMat.at<uchar>(i, j) = 0;
			}
		}
	}

	std::string imageName = iDepth.getName();
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, iDepthMat);
	cv::waitKey(0);
}