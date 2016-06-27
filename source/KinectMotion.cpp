#include <iostream>
#include "KinectMotion.h"
#include <opencv\cv.h>
#include "LeapData.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "filenames.h" 
#include <vector>
#include <set>

KinectMotion::KinectMotion(const char * filepath)
{
	FileNames my_files;
	my_files.readDir(filepath);
	leap = new LeapData(my_files.leap[FILE_NUM]);
	depth = cv::imread(my_files.depth[FILE_NUM], CV_LOAD_IMAGE_UNCHANGED);
	rgb = cv::imread(my_files.rgb[FILE_NUM], CV_LOAD_IMAGE_UNCHANGED);
}

cv::Mat KinectMotion::getDepth() 
{
	return depth;
}

cv::Mat KinectMotion::getRgb() 
{
	return rgb;
}

/*
	@def - recognize difference between wrist, hand and arm
	cut's the image at the wrist
	@param - thresholdRatio pixel difference between arm and wrist (percentage)
*/
cv::Mat KinectMotion::getHand(cv::Mat image, double thresholdRatio) 
{
	int top = 0;
	bool foundHand = false;
	int handToWrist = 0;
	int numPixels = 0;
	int max = 0;
	bool atWrist = false;
	int tmp = 0;
	
	for (int i = 0; i < image.rows; i++) 
	{
		for (int j = 0; j < image.cols; j++) 
		{
			if (image.at<uchar>(i, j) != 0) 
			{
				numPixels++;
				if (!foundHand) 
				{
					top = i;
					foundHand = true;
				}
			}
		}
		if (numPixels > 0 && !atWrist) 
		{
			handToWrist++;
			if (numPixels > max) 
			{
				max = numPixels;
			}
			if (i > top + 50 && tmp != 0 && numPixels <= tmp * thresholdRatio && tmp <= max*.75) 
			{
				atWrist = true;
			}
			tmp = numPixels;
			numPixels = 0;
		}
	}

	for (int i = (top + handToWrist + 10); i < image.rows; i++) 
	{
		for (int j = 0; j < image.cols; j++) 
		{
			image.at<uchar>(i, j) = 0;
		}
	}

	return image;
}

/*
	@def - changes the image file to remove the background
	@param - thresholdVals gets pixel intensity between thresholds and removes pixels not
			 in threshold
	make_binary - creates binary image, or creates greyscale image
*/
cv::Mat KinectMotion::updateImage(int upperThresholdVal, int lowerThresholdVal, bool make_binary) 
{
	// get depth image determine hight and width of image
	cv::Mat iDepthMat;
	depth.convertTo(iDepthMat,CV_8UC3);
	int h = depth.rows;
	int w = depth.cols;

	// threshold
	for (int i = 0; i < h; i++) 
	{
		for (int j = 0; j < w; j++) 
		{
			if(i < 50 || j < 50 || i > h-100 || j > w-50) iDepthMat.at<uchar>(i, j) = 0;
			else 
			{
				cv::Scalar intensity = iDepthMat.at<uchar>(i, j);
				if (intensity.val[0] < upperThresholdVal && intensity.val[0] > lowerThresholdVal) 
				{
					if (make_binary) iDepthMat.at<uchar>(i, j) = 255;
				}
				else 
				{
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
		if (row_sums.at<int>(i) != 0) 
		{
			if (num_contiguous_rows == 0) first_contiguous_row = i;
			else last_contiguous_row = i;
			num_contiguous_rows++;
		}
		else 
		{
			if (num_contiguous_rows != 0) 
			{
				if (num_contiguous_rows < 50) 
				{
					for (int i = first_contiguous_row; i <= last_contiguous_row; i++) 
					{
						for (int j = 0; j < iDepthMat.cols; j++) 
						{
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

/*
	@def - draws the contours
	@param - image - the matrix image
*/
cv::Mat KinectMotion::makeContourImage(cv::Mat image) 
{
	cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	
	std::vector <std::vector<cv::Point>> contours;
	contours.push_back(getContour(image));
	cv::drawContours(contourImage, contours, 0, cv::Scalar(255,255,255));

	cv::Moments m = moments(contours[0]);
	cv::Point centroid = cv::Point((int)(m.m10 / m.m00), (int)(m.m01 / m.m00));

	contourImage.at<cv::Vec3b>(centroid.y, centroid.x) = cv::Vec3b(0, 255, 0);

	return contourImage;
}

/*
	@def - find center point of palm using GaussianBlur
	@param - image - the matrix image
*/
cv::Point palmCenter(cv::Mat image, int thresh) {

	cv::Mat new_image = image.clone();
	cv::GaussianBlur(image, new_image, cv::Size(0, 0), thresh, thresh);

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

/*
	TODO - make more general
	@def - breaks image into equal squares and gets the area of each square
		   as well as their average depths
	@param - image - the matrix image
*/
std::vector<Occ> KinectMotion::cellOccupancy(cv::Mat image) 
{
	std::vector<Occ> retVal;
	int nonZ = 0;
	float avgD = 0;
	float maxD = 0;
	for (int i = 0; i < image.rows; i = i + 16) 
	{
		for (int j = 0; j < image.cols; j = j + 16) 
		{
			cv::Mat sub = image(cv::Range(i, i + 16), cv::Range(j, j + 16));
			for (int a = 0; a < sub.rows; a++) 
			{
				for (int b = 0; b < sub.cols; b++) 
				{
					if (sub.at<uchar>(a, b) != 0) 
					{
						nonZ++;
						avgD += sub.at<uchar>(a, b);
					}
				}
			}
			if (nonZ != 0) 
			{
				avgD = avgD / 256;
				if (avgD > maxD) 
				{
					maxD = avgD;
				}
			}
			Occ temp = Occ(nonZ, avgD);
			retVal.push_back(temp);
			nonZ = 0;
			avgD = 0;
		}
	}
	if (maxD != 0) 
	{
		for (int x = 0; x < retVal.size(); x++) 
		{
			retVal.at(x).avgD = retVal.at(x).avgD / maxD;
		}
	}

	return retVal;
} 

/*
	@def - getbacktolater
*/
void KinectMotion::findDirection(cv::Mat image) 
{

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

/*
	@def - gets contours and sorts them starts at contour point directly above palm center
	@param - image - the matrix image
*/
std::vector<cv::Point> getContour(cv::Mat image) 
{
	std::vector<cv::Point> rawContour;
	std::vector<cv::Point> sortedContour;
	cv::Mat image_clone = binarize(image);
	cv::Point pc = palmCenter(image_clone,23);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image_clone, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	if (contours.size() == 1) rawContour = contours[0];
	else 
	{
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
		rawContour = contours[max_index];
	}

	int start = 0;
	for (int i = 0; i < rawContour.size(); i++) 
	{
		if (rawContour[i].x == pc.x && rawContour[i].y <= pc.y) 
		{
			start = i;
			break;
		}
	}
	for (int i = 0; i < rawContour.size(); i++) 
	{
		sortedContour.push_back(rawContour[(i + start) % rawContour.size()]);
	}

	return sortedContour;
}

/*
	@def - scales hand to a fixed square image
	@param - image - image matrix
*/
cv::Mat KinectMotion::scaleHand(cv::Mat image) 
{

	cv::Mat dst = cv::Mat::zeros(SCALE,SCALE,CV_8U);

	cv::Rect rect = getRect(image);

	cv::Mat croppedImage = image(rect);

	int width  = croppedImage.cols,
		height = croppedImage.rows;
	int max_dim = (width >= height) ? width : height;

	float scale = ((float) SCALE) / max_dim;
	cv::Rect roi;
	if (width >= height)
	{
		roi.width = SCALE;
		roi.x = 0;
		roi.height = height * scale;
		roi.y = (SCALE - roi.height) / 2;
	}
	else
	{
		roi.y = 0;
		roi.height = SCALE;
		roi.width = width * scale;
		roi.x = (SCALE - roi.width) / 2;
	}
	cv::resize(croppedImage, dst(roi), roi.size());

	return dst;
}

void createWindow(cv::Mat image, std::string imageName) 
{
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, image);
	cv::waitKey(0);
}

/*
	@def - takes the sorted contours and gets a sample of 180 points
		   calculates the distance from the palm center to each sampled point
	@param - image - image matrix
*/
std::vector<float> KinectMotion::distContour(cv::Mat image) 
{
	std::vector<cv::Point> edges = getContour(image);
	cv::Point center = palmCenter(image,23);
	std::vector<cv::Point> sampleContour;
	std::vector<float> retVal;

	for (int i = 0; i < SAMPLE_SIZE; i++) 
	{
		sampleContour.push_back(edges[(int)(i * (float)(edges.size()) / SAMPLE_SIZE)]);
	}

	float max = 0;
	for (int i = 0; i < sampleContour.size(); i++) 
	{
		float temp = std::sqrt(std::pow(center.x - sampleContour.at(i).x, 2) + std::pow(center.y - sampleContour.at(i).y, 2));
		retVal.push_back(temp);
		if (temp > max) 
		{
			max = temp;
		}
	}
	for (int j = 0; j < retVal.size(); j++) 
	{
		if (max != 0) 
		{
			retVal.at(j) = retVal.at(j) / max;
		}
	}

	return retVal;
}

cv::Rect KinectMotion::getRect(cv::Mat image)
{
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

	return cv::Rect(maxPoint, minPoint);
}

/*
	@def - rotates image according to how leap data interprets upward direction
	@param - image - image matrix
*/
cv::Mat KinectMotion::rotateImage(cv::Mat image) 
{
	float x = leap->getHandDirection().getX();
	float y = leap->getHandDirection().getY();
	float angle = atan(x / y) * (180 / PI);

	cv::Point2f src_center(image.cols / 2.0F, image.rows / 2.0F);
	cv::Mat rot_mat = getRotationMatrix2D(src_center, angle , 1.0);
	cv::Mat dst;
	cv::warpAffine(image, dst, rot_mat, image.size());
	return dst;
}

/*
	@def - gets average distance of contour from image center for 32 radial sections
	@param - image - image matrix
*/
float * silhouette(cv::Mat image)
{
	cv::Mat uimage = binarize(image, 11);
	
	cv::Point center = cv::Point(image.cols / 2, image.rows / 2);
	std::vector<cv::Point> contour = getContour(uimage);
	std::vector<float> bins[32];

	float angle, dist;
	int x,y,bin;
	for (int i = 0; i < contour.size(); ++i)
	{
		x = contour[i].x - center.x; y = center.y - contour[i].y;
		angle = atan2(y,x);
		if (angle < 0) angle = (2 * PI) + angle;
		dist = sqrt((x*x) + (y*y));
		bin = (int)(angle * 16 / PI);
		bins[bin].push_back(dist);
	}

	float * avgs = new float[32];
	float sum;
	for (int i = 0; i < 32; ++i)
	{
		sum = 0;
		for (int j = 0; j < bins[i].size(); ++j)
		{
			sum += bins[i][j];
		}
		avgs[i] = sum / bins[i].size();
	}

	return avgs;
}

float * hullAreas(cv::Mat image)
{
	float * ret_array = new float[6] { 0,0,0,0,0,0 };

	cv::Mat uimage = binarize(image, 11);

	std::vector<cv::Point> og_contour = getContour(uimage);
	double hand_area = contourArea(og_contour);

	std::vector<cv::Point> hull;
	convexHull(og_contour, hull);

	cv::Mat hull_image = cv::Mat::zeros(image.size(), CV_8U);
	std::vector <std::vector<cv::Point>> contours;
	contours.push_back(hull);
	cv::drawContours(hull_image, contours, 0, cv::Scalar(255, 255, 255));

	cv::Point seed = cv::Point(hull_image.rows/2,hull_image.cols/2);
	floodFill(hull_image, seed, cv::Scalar(255, 255, 255));

	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (uimage.at<uchar>(i, j) != 0) hull_image.at<uchar>(i, j) = 0;
		}
	}
	cv::Point pc = palmCenter(uimage,40);

	cv::Mat image_clone = hull_image.clone();
	cv::Mat new_image = cv::Mat::zeros(image.size(), CV_8UC3);
	cv::findContours(image_clone, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	std::vector<std::vector<cv::Point>> thresholded_hull_contours;
	for (int i = 0; i < contours.size(); ++i)
	{
		if (contourArea(contours[i]) > 800)
		{
			thresholded_hull_contours.push_back(contours[i]);
		}
	}
	
	if (thresholded_hull_contours.size() > 6) 
	{
		std::sort(thresholded_hull_contours.begin(), thresholded_hull_contours.end(),compareContourAreas);
		while (thresholded_hull_contours.size() > 6)
		{
			thresholded_hull_contours.pop_back();
		}
	}

	std::vector<float> angles;
	for (int i = 0; i < thresholded_hull_contours.size(); ++i)
	{
		cv::drawContours(new_image, thresholded_hull_contours, i, cv::Scalar(i * 255 / thresholded_hull_contours.size(), 0, 255));
		cv::Moments m = moments(thresholded_hull_contours[i]);
		cv::Point centroid = cv::Point((int)(m.m10 / m.m00), (int)(m.m01 / m.m00));
		angles.push_back(atan2(pc.y - centroid.y, centroid.x - pc.x));
		if (angles[i] < 0) angles[i] = (2 * PI) + angles[i];
		ret_array[i] = contourArea(thresholded_hull_contours[i]) / hand_area;
	}

	for (int i = 1; i < angles.size(); ++i)
	{
		for (int j = 1; j < angles.size(); ++j)
		{
			float temp_angle,temp_area;
			if (angles[j - 1] > angles[j])
			{
				temp_angle = angles[j - 1]; angles[j - 1] = angles[j]; angles[j] = temp_angle;
				temp_area = ret_array[j - 1]; ret_array[j - 1] = ret_array[j]; ret_array[j] = temp_area;
			}
		}
	}

	return ret_array;
}

cv::Mat binarize(cv::Mat image, int threshold)
{
	cv::Mat binary_image;
	image.convertTo(binary_image, CV_8U);
	for (int i = 0; i < binary_image.rows; ++i)
	{
		for (int j = 0; j < binary_image.cols; ++j)
		{
			if (binary_image.at<uchar>(i, j) >= threshold) binary_image.at<uchar>(i, j) = 255;
			else binary_image.at<uchar>(i, j) = 0;
		}
	}

	return binary_image;
}

bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) 
{
	double i = fabs(contourArea(cv::Mat(contour1)));
	double j = fabs(contourArea(cv::Mat(contour2)));
	return (i > j);
}

cv::Mat newThreshold(cv::Mat image) 
{
	cv::Mat uimage; image.convertTo(uimage, CV_8U);

	// find closest pixel and remove noise
	int min = 255;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if(j < 50 || i > image.rows-50 || j > image.cols-50) image.at<uchar>(i, j) = 0;
			else {
				if (uimage.at<uchar>(i, j) < 16) uimage.at<uchar>(i, j) = 0;
				else if (uimage.at<uchar>(i, j) < min) min = uimage.at<uchar>(i, j);
			}
		}
	}
	
	// threshold behind closest object
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (uimage.at<uchar>(i, j) > min + 4) uimage.at<uchar>(i, j) = 0;
		}
	}

	return uimage;
}

void cellOccupancy2(cv::Mat image)
{
	int i_size = image.rows / CELL_DIVS; int j_size = image.cols / CELL_DIVS;
	int box_size = i_size * j_size;
	double avgs[NUM_CELLS]; int nonZs[NUM_CELLS]; double sums[NUM_CELLS] = { 0 };
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (image.at<uchar>(i, j) != 0)
			{
				sums[(j / j_size)*CELL_DIVS + (i / i_size)] += image.at<uchar>(i, j);
				nonZs[(j / j_size)*CELL_DIVS + (i / i_size)]++;
			}
		}
	}
	for (int i = 0; i < NUM_CELLS; ++i)
	{
		avgs[i] = sums[i] / box_size;
	}
}