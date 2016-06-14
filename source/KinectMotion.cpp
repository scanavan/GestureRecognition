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
#include <set>

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
	int numPixels = 0;
	int max = 0;
	bool atWrist = false;
	int tmp = 0;
	
	for (int i = 0; i < image.rows; i++) {
		for (int j = 0; j < image.cols; j++) {
			if (image.at<uchar>(i, j) != 0) {
				numPixels++;
				if (!foundHand) {
					top = i;
					foundHand = true;
				}
			}
		}
		if (numPixels > 0 && !atWrist) {
			handToWrist++;
			if (numPixels > max) {
				max = numPixels;
			}
			if (i > top + 50 && tmp != 0 && numPixels <= tmp * thresholdRatio && tmp <= max*.75) {
				atWrist = true;
			}
			tmp = numPixels;
			numPixels = 0;
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
	
	std::vector <Point> edge_vector;

	/*int last_val = static_cast<int>(image.at<uchar>(0, 0));
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (last_val != static_cast<int>(image.at<uchar>(i, j))) {
				last_val = static_cast<int>(image.at<uchar>(i, j));
				edge_vector.push_back(Point(i, j));
			}
		}
	}*/
	
	for (int i = 1; i < image.rows; ++i)
	{
		for (int j = 1; j < image.cols; ++j)
		{
			if (image.at<uchar>(i - 1, j) != image.at<uchar>(i, j) || image.at<uchar>(i, j - 1) != image.at<uchar>(i, j)) {
				edge_vector.push_back(Point(i, j));
			}
		}
	}
	return edge_vector;

}

cv::Mat KinectMotion::makeEdgeImage(cv::Mat image) {

	cv::Mat edge_image(image.rows, image.cols, CV_8UC3);

	//int last_val = static_cast<int>(image.at<uchar>(0, 0));
	//for (int i = 0; i < image.rows; ++i)
	//{
	//	for (int j = 0; j < image.cols; ++j)
	//	{
	//		if (last_val != static_cast<int>(image.at<uchar>(i, j))) {
	//			last_val = static_cast<int>(image.at<uchar>(i, j));
	//			edge_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0,0,255);
	//		}
	//		else {
	//			//edge_image.at<cv::Vec3b>(i, j)[0] = image.at<uchar>(i, j);
	//			//edge_image.at<cv::Vec3b>(i, j)[1] = image.at<uchar>(i, j);
	//			//edge_image.at<cv::Vec3b>(i, j)[2] = image.at<uchar>(i, j);
	//			edge_image.at<cv::Vec3b>(i,j) = cv::Vec3b(0, 0, 0);
	//		}
	//	}
	//}

	for (int i = 1; i < image.rows; ++i)
	{
		for (int j = 1; j < image.cols; ++j)
		{
			if (image.at<uchar>(i-1, j) != image.at<uchar>(i, j) || image.at<uchar>(i, j-1) != image.at<uchar>(i, j)) {
				edge_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 255);
			}
			else {
				//edge_image.at<cv::Vec3b>(i, j)[0] = image.at<uchar>(i, j);
				//edge_image.at<cv::Vec3b>(i, j)[1] = image.at<uchar>(i, j);
				//edge_image.at<cv::Vec3b>(i, j)[2] = image.at<uchar>(i, j);
				edge_image.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
			}
		}
	}

	return edge_image;

}

Point KinectMotion::handCenter(cv::Mat image) {
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

int * KinectMotion::palmCenter(cv::Mat image) {

	std::vector<Point> edges = findEdges(image);
	int xMin = 2000;
	int xMax = 0;
	int yMin = 2000;
	int yMax = 0;
	Point xMin_p;
	Point xMax_p;
	Point yMin_p;
	Point yMax_p;
	int currentX;
	int currentY;
	for (int a = 0; a < edges.size(); a++)
	{
		currentX = edges.at(a).i;
		currentY = edges.at(a).j;

		if (currentX < xMin)
		{
			xMin = currentX;
			xMin_p = edges.at(a);
		}
		if (currentY < yMin)
		{
			yMin = currentY;
			yMin_p = edges.at(a);
		}
		if (currentX > xMax)
		{
			xMax = currentX;
			xMax_p = edges.at(a);
		}
		if (currentY > yMax)
		{
			yMax = currentY;
			yMax_p = edges.at(a);
		}
	}
	int * retVal = new int[4]{ xMin,yMin,xMax,yMax };

	//double current_distance;
	//double current_min;
	//double maxMin = 0;
	//Point maxMin_p;
	//Point edge_p;
	//for (int i = xMin; i < xMax; ++i)
	//{
	//	for (int j = yMin; j < yMax; ++j)
	//	{
	//		current_min = 10000;
	//		for (int k = 0; k < edges.size(); ++k)
	//		{
	//			int x = (edges.at(k).i - i);
	//			int y = (edges.at(k).j - j);
	//			current_distance = sqrt((x*x) - (y*y));
	//			if (current_distance < current_min)
	//			{
	//				current_min = current_distance;
	//				edge_p.i = edges.at(k).i;
	//				edge_p.j = edges.at(k).j;
	//			}
	//		}
	//		if (current_min > maxMin)
	//		{
	//			maxMin = current_min;
	//			maxMin_p.i = i;
	//			maxMin_p.j = j;
	//		}
	//	}
	//}

	//image = makeEdgeImage(image);
	//std::cout << maxMin << std::endl;
	//image.at<cv::Vec3b>(retVal.i, retVal.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(xMin_p.i, xMin_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(yMin_p.i, yMin_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(xMax_p.i, xMax_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(yMax_p.i, yMax_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(maxMin_p.i, maxMin_p.j) = cv::Vec3b(255, 255, 255);
	//image.at<cv::Vec3b>(edge_p.i, edge_p.j) = cv::Vec3b(255, 255, 255);

	cv::Mat new_image = image.clone();
	cv::GaussianBlur(image, new_image, cv::Size(0, 0), 23, 23);

	int max = 0;
	for (int i = 0; i < new_image.rows; ++i)
	{
		for (int j = 0; j < new_image.cols; ++j)
		{
			if (static_cast<int>(new_image.at<uchar>(i, j)) > max)
			{
				max = static_cast<int>(new_image.at<uchar>(i, j));
			}
		}
	}
	for (int i = 0; i < new_image.rows; ++i)
	{
		for (int j = 0; j < new_image.cols; ++j)
		{
			if (static_cast<int>(new_image.at<uchar>(i, j)) == max)
			{
				image.at<uchar>(i, j) = 0;
			}
		}
	}

	cv::namedWindow("center", cv::WINDOW_AUTOSIZE);
	cv::imshow("center", image);
	cv::waitKey(0);
	return retVal;
}

std::vector<std::set<float>> KinectMotion::cellOccupancy(cv::Mat image) {
	std::vector<std::set<float>> retVal;
	int nonZ = 0;
	float avgD = 0;
	//cv::Mat zero = cv::Mat::zeros(16, 16, CV_8U);
	for (int i = 0; i < image.rows; i = i + 16) {
		for (int j = 0; j < image.cols; j = j + 16) {
			cv::Mat sub = image(cv::Range(i, i + 15), cv::Range(j, j + 15));
			for (int a = 0; a < sub.rows; a++) {
				for (int b = 0; b < sub.cols; b++) {
					if (sub.at<uchar>(a, b) != 0) {
						nonZ++;
						avgD += sub.at<uchar>(a, b);
					}
					if (nonZ != 0) {
						avgD = avgD / nonZ;
					}
					std::set<float> temp;
					temp.insert(nonZ);
					temp.insert(avgD);
					retVal.push_back(temp);
				}
			}
		}
	}
	return retVal;
}