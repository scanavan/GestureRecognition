#include <iostream>
#include "KinectMotion.h"
#include <opencv\cv.h>
#include "Image.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv\cv.h>
#include "filenames.h"

KinectMotion::KinectMotion(std::string iDepth, std::string iRgb):depth(Image(iDepth)),rgb(Image(iRgb)) {

}

Image KinectMotion::getDepth() {
	return depth;
}

Image KinectMotion::getRgb() {
	return rgb;
}

void KinectMotion::test() {
	FileNames myFiles;

	myFiles.readDir("C:/JB/Gesture_1_Dataset/p1/g1");

	int upperThresholdVal = 20;
	int lowerThresholdVal = 1;



	KinectMotion image = KinectMotion(myFiles.depth[0], myFiles.rgb[0]);
	Image iDepth = image.getDepth();

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

	std::string imageName = "Our Image";
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, iDepthMat);
	cv::waitKey(0);

}