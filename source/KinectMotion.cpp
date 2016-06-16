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

	for (int i = (top + handToWrist + 10); i < image.rows; i++) {
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

	// calculate sum of each row as one column 
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

	return iDepthMat;

}
float KinectMotion::blobMax(cv::Mat image) {
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
	detector->detect(image, keypoints);

	// extract the index of the largest blob
	int maxKeyIndex;
	float max = 0.0;
	for (int i = 0; i < keypoints.size(); i++) {
		if (keypoints[i].size > max) {
			max = keypoints[i].size;
			maxKeyIndex = i;
		}
	}
	// Draw detected blobs as red circles.
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures the size of the circle corresponds to the size of blob
	cv::Mat im_with_keypoints;
	drawKeypoints(image, keypoints, im_with_keypoints, cv::Scalar(0, 0, 255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

	// Show blobs

	std::cout << maxKeyIndex << std::endl;

	return max;
}

cv::Mat KinectMotion::makeContourImage(cv::Mat image) {

	cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	
	std::vector <std::vector<cv::Point>> contours;
	contours.push_back(getContour(image));
	cv::drawContours(contourImage, contours, 0, cv::Scalar(255,255,255));

	return contourImage;

}

cv::Point KinectMotion::handCenter(cv::Mat image) {
	std::vector<cv::Point> edges = getContour(image);
	int xSum = 0;
	int ySum = 0;
	for (int a = 0; a < edges.size(); a++) {
		xSum += edges.at(a).x;
		ySum += edges.at(a).y;
	}
	cv::Point retVal(xSum / edges.size(), ySum / edges.size());
	image = makeContourImage(image);
	image.at<cv::Vec3b>(retVal.x, retVal.y) = cv::Vec3b(255,255,255);

	return retVal;
}

cv::Point KinectMotion::palmCenter(cv::Mat image) {

	cv::Mat new_image = image.clone();
	cv::GaussianBlur(image, new_image, cv::Size(0, 0), 23, 23);

	int max = 0;
	cv::Point center;
	for (int i = 0; i < new_image.rows; ++i)
	{
		for (int j = 0; j < new_image.cols; ++j)
		{
			if (static_cast<int>(new_image.at<uchar>(i, j)) >= max)
			{
				max = static_cast<int>(new_image.at<uchar>(i, j));
				center = cv::Point(j, i);
			}
		}
	}

	return center;

}

Occ::Occ(int nonZ, float avgD) {
	this->nonZ = nonZ;
	this->avgD = avgD;
}

std::vector<Occ> KinectMotion::cellOccupancy(cv::Mat image) {
	std::vector<Occ> retVal;
	int nonZ = 0;
	float avgD = 0;
	float maxD = 0;
	for (int i = 0; i < image.rows; i = i + 16) {
		for (int j = 0; j < image.cols; j = j + 16) {
			cv::Mat sub = image(cv::Range(i, i + 16), cv::Range(j, j + 16));
			for (int a = 0; a < sub.rows; a++) {
				for (int b = 0; b < sub.cols; b++) {
					if (sub.at<uchar>(a, b) != 0) {
						nonZ++;
						avgD += sub.at<uchar>(a, b);
					}
				}
			}
			if (nonZ != 0) {
				avgD = avgD / 256;
				if (avgD > maxD) {
					maxD = avgD;
				}
			}
			Occ temp = Occ(nonZ, avgD);
			retVal.push_back(temp);
			nonZ = 0;
			avgD = 0;
		}
	}
	if (maxD != 0) {
		for (int x = 0; x < retVal.size(); x++) {
			retVal.at(x).avgD = retVal.at(x).avgD / maxD;
		}
	}
	return retVal;
} 

void KinectMotion::findDirection(cv::Mat image) {

	std::vector<cv::Point> edges = getContour(image);
	double max_distance = 0;
	double current_distance;
	cv::Point ends[2];
	for (int i = 0; i < edges.size() - 1; ++i)
	{
		for (int j = i + 1; j < edges.size(); ++j)
		{
			int x = (edges.at(i).x - edges.at(j).x);
			int y = (edges.at(i).y - edges.at(j).y);
			current_distance = sqrt((x*x) + (y*y));
			if (current_distance > max_distance)
			{
				max_distance = current_distance;
				ends[0] = edges.at(i);
				ends[1] = edges.at(j);
			}
		}
	}

	cv::Mat edge_image = makeContourImage(image);
	edge_image.at<cv::Vec3b>(ends[0].x,ends[0].y) = cv::Vec3b(255, 255, 0);
	edge_image.at<cv::Vec3b>(ends[1].x,ends[1].y) = cv::Vec3b(255, 255, 0);

	return;
}

std::vector<cv::Point> getContour(cv::Mat image) {
	
	cv::Mat image_clone;
	image.convertTo(image_clone,CV_8U);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image_clone, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	if (contours.size() == 1) return contours[0];

	int max_size = 0;
	int max_index;
	for (int i = 0; i < contours.size(); ++i)
	{
		if (contours[i].size() > max_size)
		{
			max_size = contours[i].size();
			max_index = i;
		}
	}

	return contours[max_index];
}

cv::Mat KinectMotion::scaleHand(cv::Mat image) {

	cv::Mat dst = cv::Mat::zeros(320,320,CV_8U);

	std::vector<cv::Point> edges = getContour(image);
	int xMin = 2000;
	int xMax = 0;
	int yMin = 2000;
	int yMax = 0;
	cv::Point maxPoint;
	cv::Point minPoint;
	int currentX;
	int currentY;

	for (int a = 0; a < edges.size(); a++)
	{
		currentX = edges.at(a).x;
		currentY = edges.at(a).y;

		if (currentX < xMin)
		{
			xMin = currentX;
		}
		if (currentY < yMin)
		{
			yMin = currentY;
		}
		if (currentX > xMax)
		{
			xMax = currentX;
		}
		if (currentY > yMax)
		{
			yMax = currentY;
		}
	}

	maxPoint.x = xMax + 10;
	maxPoint.y = yMax + 10;
	minPoint.x = xMin - 10;
	minPoint.y = yMin - 10;

	cv::Mat croppedImage = image(cv::Rect(maxPoint, minPoint));

	int width  = croppedImage.cols,
		height = croppedImage.rows;
	int max_dim = (width >= height) ? width : height;

	float scale = ((float) 320) / max_dim;
	cv::Rect roi;
	if (width >= height)
	{
		roi.width = 320;
		roi.x = 0;
		roi.height = height * scale;
		roi.y = (320 - roi.height) / 2;
	}
	else
	{
		roi.y = 0;
		roi.height = 320;
		roi.width = width * scale;
		roi.x = (320 - roi.width) / 2;
	}
	cv::resize(croppedImage, dst(roi), roi.size());

	return dst;

}

void createWindow(cv::Mat image, std::string imageName) {
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, image);
	cv::waitKey(0);
}