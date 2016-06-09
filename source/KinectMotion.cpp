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

void removeRows(cv::Mat * image, int first_row, int last_row) {
	
	for (int i = first_row; i <= last_row; i++) {
		for (int j = 0; j < image->cols; j++) {
			image->at<uchar>(i, j) = 0;
		}
	}

}

void getRange(cv::Mat * image) {

	int found_first = 0;
	int first_col = 0;
	int last_col = 0;

	for (int i = 0; i < image->rows; i++) {
		for (int j = 0; j < image->cols; j++) {
			if (image->at<uchar>(i, j) != 0) {
				if (found_first == 0) {
					found_first = 1;
					first_col = j;
				}
				last_col = j;
			}
		}
		found_first = 0;
		for (int j = first_col; j < last_col; j++) {
			image->at<uchar>(i, j) = 255;
		}
	}

}

void KinectMotion::displayUpdatedImage(int upperThresholdVal, int lowerThresholdVal) {
	
	// get depth image determine hight and width of image
	cv::Mat iDepthMat = depth.returnImage();
	int h = depth.getHeight();
	int w = depth.getWidth();

	// threshold
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
	
	// calculate sum of each row as one column (this isn't really that useful because it's 32 bit)
	cv::Mat row_sums;
	cv::reduce(iDepthMat, row_sums, 1, CV_REDUCE_SUM, CV_32S);
	
	// remove anything that isn't a hand (not tall enough)
	int num_contiguous_rows = 0;
	int first_contiguous_row = 0;
	int last_contiguous_row = 0;
	for (int i = 0; i < h; i++) {
		if (row_sums.at<int>(i) != 0) {
			if (num_contiguous_rows == 0) first_contiguous_row = i;
			else last_contiguous_row = i;
			num_contiguous_rows++;
		}
		else {
			if (num_contiguous_rows != 0) {
				if (num_contiguous_rows < 50) {
					removeRows(&iDepthMat, first_contiguous_row, last_contiguous_row);
				}
			}
			num_contiguous_rows = 0;
		}
	}

	// fill in area between fingers and such
	// getRange(&iDepthMat);

	// get new sums (without irrelevant regions)
	cv::reduce(iDepthMat, row_sums, 1, CV_REDUCE_SUM, CV_32S);

	// display row sums (doesn't actually work bc of 32 bit)
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < 20; j++) {
			iDepthMat.at<uchar>(i, j) = (unsigned char) row_sums.at<int>(i);
		}
	}

	// display image
	std::string imageName = depth.getName();
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, iDepthMat);
	cv::waitKey(0);
}