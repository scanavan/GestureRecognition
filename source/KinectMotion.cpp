
#include <iostream>
#include "KinectMotion.h"
#include <opencv\cv.h>
#include "Image.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "filenames.h" 
#include <vector>

KinectMotion::KinectMotion(std::string iDepth, std::string iRgb) :depth(Image(iDepth)), rgb(Image(iRgb)) {

}

Image KinectMotion::getDepth() {
	return depth;
}

Image KinectMotion::getRgb() {
	return rgb;
}

void KinectMotion::blob(cv::Mat imMat) {
	cv::SimpleBlobDetector::Params params;
	params.minThreshold = 1;
	params.maxThreshold = 25;
	params.filterByArea = false;
	params.filterByCircularity = false;
	params.filterByConvexity = false;
	params.filterByInertia = false;
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);
	std::vector<cv::KeyPoint> keypoints;
	detector->detect(imMat, keypoints);
	cv::Mat iDepthMat_with_keypoints;
	drawKeypoints(imMat, keypoints, iDepthMat_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	imshow("keypoints", iDepthMat_with_keypoints);
	cv::waitKey(0);
}

cv::Mat KinectMotion::getHand(cv::Mat image, double thresholdRatio) {
	int top = 0;
	bool foundHand = false;
	int handToWrist = 0;
	int numWhitePixels = 0;
	int max = 0;
	bool atWrist = false;
	int tmp = 0;
	
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) == 255) {
				numWhitePixels++;
				if (!foundHand) {
					top = i;
					foundHand = true;
				}
			}
		}
		if (numWhitePixels > 0 && !atWrist) {
			handToWrist++;
			if (numWhitePixels > max) {
				max = numWhitePixels;
			}
			if (i > top + 50 && tmp != 0 && numWhitePixels <= tmp * thresholdRatio && tmp <= max*.75) {
				atWrist = true;
			}
			tmp = numWhitePixels;
			numWhitePixels = 0;
		}
	}

	for (int i = (top + handToWrist); i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			image.at<uchar>(i, j) = 0;
		}
	}

	return image;

}

cv::Mat KinectMotion::updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary) {

	// get depth image determine hight and width of image
	cv::Mat iDepthMat = depth.returnImage();
	int h = depth.getHeight();
	int w = depth.getWidth();

	// threshold
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if(i < 50 || j < 50 || i > h-150 || j > w-50) iDepthMat.at<uchar>(i, j) = 0;
			else {
				cv::Scalar intensity = iDepthMat.at<uchar>(i, j);
				if (intensity.val[0] < upperThresholdVal && intensity.val[0] > lowerThresholdVal) {
					if (make_binary) iDepthMat.at<uchar>(i, j) = 255;
				}
				else {
					iDepthMat.at<uchar>(i, j) = 0;
				}
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
	//std::vector <unsigned char> range_vector;
	//int found_first = 0;
	//int first_col = 0;
	//int last_col = 0;
	//for (int i = 0; i < iDepthMat.rows; i++) {
	//	range_vector.push_back(0);
	//	for (int j = 0; j < iDepthMat.cols; j++) {
	//		if (iDepthMat.at<uchar>(i, j) != 0) {
	//			if (found_first == 0) {
	//				found_first = 1;
	//				first_col = j;
	//			}
	//			last_col = j;
	//		}
	//	}
	//	found_first = 0;
	//	for (int j = first_col; j < last_col; j++) {
	//		range_vector.back()++;
	//	}
	//}

	// display row ranges
	//for (int i = 0; i < h; i++) {
	//	for (int j = 0; j < 20; j++) {
	//		iDepthMat.at<uchar>(i, j) = range_vector[i];
	//	}
	//}

	return iDepthMat;

}
float KinectMotion::blobMax(cv::Mat depth) {
	// set up the parameters (check the defaults in opencv's code in blobdetector.cpp)
	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = 50.0f;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = false;
	params.minArea = 750.0f;
	params.maxArea = 2000.0f;

	// set up and create the detector using the parameters
	cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(params);

	// detect!
	std::vector<cv::KeyPoint> keypoints;
	detector->detect(depth, keypoints);

	// extract the index of the largest blob
	int maxPoint;
	float max = 0.0;
	for (int i = 0; i < keypoints.size(); i++) {
		if (keypoints[i].size > max) {
			max = keypoints[i].size;
			maxPoint = i;
		}
	}
	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	cv::Mat im_with_keypoints;
	drawKeypoints(depth, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs
	imshow("keypoints", im_with_keypoints);
	cv::waitKey(0);
	std::cout << maxPoint << std::endl;

	return max;
}

Point::Point(int i, int j) {
	this->i = i;
	this->j = j;
}

std::vector <Point> KinectMotion::findEdges(cv::Mat image) {
	
	int last_val = static_cast<int>(image.at<uchar>(0, 0));
	std::vector <Point> edge_vector;

	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (last_val != static_cast<int>(image.at<uchar>(i, j))) {
				last_val = static_cast<int>(image.at<uchar>(i, j));
				edge_vector.push_back(Point(i, j));
			}
		}
	}

	return edge_vector;

}

cv::Mat KinectMotion::makeEdgeImage(cv::Mat image) {

	cv::Mat edge_image(image.rows, image.cols, CV_8UC3);
	int last_val = static_cast<int>(image.at<uchar>(0, 0));

	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (last_val != static_cast<int>(image.at<uchar>(i, j))) {
				last_val = static_cast<int>(image.at<uchar>(i, j));
				edge_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0,0,255);
			}
			else {
				//edge_image.at<cv::Vec3b>(i, j)[0] = image.at<uchar>(i, j);
				//edge_image.at<cv::Vec3b>(i, j)[1] = image.at<uchar>(i, j);
				//edge_image.at<cv::Vec3b>(i, j)[2] = image.at<uchar>(i, j);
				edge_image.at<cv::Vec3b>(i,j) = cv::Vec3b(0, 0, 0);
			}
		}
	}

	return edge_image;

}

Point KinectMotion::palmCenter(cv::Mat image) {
	std::vector<Point> edges = findEdges(image);
	int xSum = 0;
	int ySum = 0;
	for (int a = 0; a < edges.size(); a++) {
		xSum += edges.at(a).i;
		ySum += edges.at(a).j;
	}
	Point retVal(xSum / edges.size(), ySum / edges.size());
	image = makeEdgeImage(image);
	image.at<cv::Vec3b>(retVal.i, retVal.j) = cv::Vec3b(255,255,255);
	cv::namedWindow("center", cv::WINDOW_AUTOSIZE);
	cv::imshow("center", image);
	cv::waitKey(0);
	return retVal;
}