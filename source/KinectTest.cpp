#include "KinectMotion.h"
#include <opencv\cv.h>
#include <vector>

cv::Mat updateDepthImage(KinectMotion kinect_motion, int upperThresholdVal, int lowerThresholdVal) {
	
	// get depth image determine hight and width of image
	cv::Mat iDepthMat = kinect_motion.getDepth().returnImage();
	int h = kinect_motion.getDepth().getHeight();
	int w = kinect_motion.getDepth().getWidth();

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
					for (int i = first_contiguous_row; i <= last_contiguous_row; i++) {
						for (int j = 0; j < iDepthMat.cols; j++) {
							iDepthMat.at<uchar>(i, j) = 0;
						}
					}
				}
			}
			num_contiguous_rows = 0;
		}
	}

	// fill in area between fingers and such
	std::vector <unsigned char> range_vector;
	int found_first = 0;
	int first_col = 0;
	int last_col = 0;
	for (int i = 0; i < iDepthMat.rows; i++) {
		range_vector.push_back(0);
		for (int j = 0; j < iDepthMat.cols; j++) {
			if (iDepthMat.at<uchar>(i, j) != 0) {
				if (found_first == 0) {
					found_first = 1;
					first_col = j;
				}
				last_col = j;
			}
		}
		found_first = 0;
		for (int j = first_col; j < last_col; j++) {
			range_vector.back()++;
		}
	}

	// display row ranges
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < 20; j++) {
			iDepthMat.at<uchar>(i, j) = range_vector[i]; 
		}
	}

	return iDepthMat;

}