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

KinectMotion::KinectMotion(std::string fleap, std::string fdepth)
{
	leap = new LeapData(fleap);
	depth = cv::imread(fdepth, CV_LOAD_IMAGE_UNCHANGED);
	//rgb = cv::imread(frgb, CV_LOAD_IMAGE_UNCHANGED);

	int index = fdepth.find_last_of("/");
	char gestureNumber = fdepth.at(index - 1);
	gesture = "G";
	gesture.push_back(gestureNumber);

	depth = updateImage(depth);
	depth = rotateImage(depth);
	getHand2(depth);
	depth = scaleHand(depth);
	sil = silhouette(depth);
	contour_dist = distContour(binarize(depth));
	hull = hullAreas(depth);
	Occ occ_data = cellOccupancy(depth);
	occ_avg = occ_data.avgD;
	occ_nonz = occ_data.nonZ;

}

cv::Mat KinectMotion::getDepth()
{
	return depth;
}

//cv::Mat KinectMotion::getRgb()
//{
//	return rgb;
//}

float * KinectMotion::getSil()
{
	return sil;
}
float * KinectMotion::getContourDist()
{
	return contour_dist;
}
int * KinectMotion::getOccNonz()
{
	return occ_nonz;
}
float * KinectMotion::getOccAvg()
{
	return occ_avg;
}
float * KinectMotion::getHull()
{
	return hull;
}
std::string KinectMotion::getGesture() {
	return gesture;
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
@def - draws the contours
@param - image - the matrix image
*/
cv::Mat KinectMotion::makeContourImage(cv::Mat image)
{
	cv::Mat contourImage(image.size(), CV_8UC3, cv::Scalar(0, 0, 0));

	std::vector <std::vector<cv::Point>> contours;
	contours.push_back(getContour(image));
	cv::drawContours(contourImage, contours, 0, cv::Scalar(255, 255, 255));

	cv::Moments m = moments(contours[0]);
	cv::Point centroid = cv::Point((int)(m.m10 / m.m00), (int)(m.m01 / m.m00));

	contourImage.at<cv::Vec3b>(centroid.y, centroid.x) = cv::Vec3b(0, 255, 0);

	return contourImage;
}

/*
@def - find center point of palm using GaussianBlur
@param - image - the matrix image
*/
cv::Point KinectMotion::palmCenter(cv::Mat image, int thresh) {

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

/*
@def - gets contours and sorts them starts at contour point directly above palm center
@param - image - the matrix image
*/
std::vector<cv::Point> KinectMotion::getContour(cv::Mat image)
{
	std::vector<cv::Point> rawContour;
	std::vector<cv::Point> sortedContour;
	cv::Mat image_clone = binarize(image);
	cv::Point pc = palmCenter(image_clone, 23);

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

	cv::Mat dst = cv::Mat::zeros(SCALE, SCALE, CV_8U);

	cv::Rect rect = getRect(image);

	cv::Mat croppedImage = image(rect);

	int width = croppedImage.cols,
		height = croppedImage.rows;
	int max_dim = (width >= height) ? width : height;

	float scale = ((float)440) / max_dim;
	cv::Rect roi;
	if (width >= height)
	{
		roi.width = 440;
		roi.x = 20;
		roi.height = height * scale;
		roi.y = (SCALE - roi.height) / 2;
	}
	else
	{
		roi.y = 20;
		roi.height = 440;
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
float * KinectMotion::distContour(cv::Mat image)
{
	std::vector<cv::Point> edges = getContour(image);
	cv::Point center = palmCenter(image, 150);
	std::vector<cv::Point> sampleContour;
	float * retVal = new float[SAMPLE_SIZE] {0};

	for (int i = 0; i < SAMPLE_SIZE; i++)
	{
		sampleContour.push_back(edges[(int)(i * (float)(edges.size()) / SAMPLE_SIZE)]);
	}

	float max = 0;
	for (int i = 0; i < sampleContour.size(); i++)
	{
		float temp = std::sqrt(std::pow(center.x - sampleContour.at(i).x, 2) + std::pow(center.y - sampleContour.at(i).y, 2));
		retVal[i] = temp;
		if (temp > max)
		{
			max = temp;
		}
	}
	for (int j = 0; j < SAMPLE_SIZE; j++)
	{
		if (max != 0)
		{
			retVal[j] = retVal[j] / max;
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

	maxPoint.x = xMax;
	maxPoint.y = yMax;
	minPoint.x = xMin;
	minPoint.y = yMin;

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
	cv::Mat rot_mat = getRotationMatrix2D(src_center, angle, 1.0);
	cv::Mat dst;
	cv::warpAffine(image, dst, rot_mat, image.size());
	return dst;
}

/*
@def - gets average distance of contour from image center for 32 radial sections
@param - image - image matrix
*/
float * KinectMotion::silhouette(cv::Mat image)
{
	cv::Mat uimage = binarize(image, 11);

	cv::Point center = cv::Point(image.cols / 2, image.rows / 2);
	std::vector<cv::Point> contour = getContour(uimage);
	std::vector<float> bins[32];

	float angle, dist;
	int x, y, bin;
	for (int i = 0; i < contour.size(); ++i)
	{
		x = contour[i].x - center.x; y = center.y - contour[i].y;
		angle = atan2(y, x);
		if (angle < 0) angle = (2 * PI) + angle;
		dist = sqrt((x*x) + (y*y));
		bin = (int)(angle * 16 / PI);
		bins[bin].push_back(dist);
	}

	float * avgs = new float[32]{ 0 };
	float sum;
	for (int i = 0; i < 32; ++i)
	{
		sum = 0;
		for (int j = 0; j < bins[i].size(); ++j)
		{
			sum += bins[i][j];
		}
		if (bins[i].size() == 0) avgs[i] = 0;
		else avgs[i] = sum / bins[i].size();
	}

	return avgs;
}

float * KinectMotion::hullAreas(cv::Mat image)
{
	float * ret_array = new float[6]{ 0,0,0,0,0,0 };

	cv::Mat uimage = binarize(image, 11);

	std::vector<cv::Point> og_contour = getContour(uimage);
	double hand_area = contourArea(og_contour);

	std::vector<cv::Point> hull;
	convexHull(og_contour, hull);

	cv::Mat hull_image = cv::Mat::zeros(image.size(), CV_8U);
	std::vector <std::vector<cv::Point>> contours;
	contours.push_back(hull);
	cv::drawContours(hull_image, contours, 0, cv::Scalar(255, 255, 255));

	cv::Point seed = cv::Point(hull_image.rows / 2, hull_image.cols / 2);
	floodFill(hull_image, seed, cv::Scalar(255, 255, 255));

	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			if (uimage.at<uchar>(i, j) != 0) hull_image.at<uchar>(i, j) = 0;
		}
	}
	cv::Point pc = palmCenter(uimage, 150);

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
		std::sort(thresholded_hull_contours.begin(), thresholded_hull_contours.end(), compareContourAreas);
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
			float temp_angle, temp_area;
			if (angles[j - 1] > angles[j])
			{
				temp_angle = angles[j - 1]; angles[j - 1] = angles[j]; angles[j] = temp_angle;
				temp_area = ret_array[j - 1]; ret_array[j - 1] = ret_array[j]; ret_array[j] = temp_area;
			}
		}
	}

	return ret_array;
}

cv::Mat KinectMotion::binarize(cv::Mat image, int threshold)
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

cv::Mat KinectMotion::updateImage(cv::Mat image)
{
	cv::Mat uimage; image.convertTo(uimage, CV_8U);

	// find closest pixel and remove noise
	int min = 255;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			//if(j < 50 || i > image.rows-50 || j > image.cols-50) image.at<uchar>(i, j) = 0;
			//else {
			if (uimage.at<uchar>(i, j) < 16) uimage.at<uchar>(i, j) = 0;
			else if (uimage.at<uchar>(i, j) < min) min = uimage.at<uchar>(i, j);
			//}
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

Occ KinectMotion::cellOccupancy(cv::Mat image)
{
	int i_size = image.rows / CELL_DIVS; int j_size = image.cols / CELL_DIVS;
	int box_size = i_size * j_size;
	float * avgs = new float[NUM_CELLS] {0}; int * nonZs = new int[NUM_CELLS] {0}; float * sums = new float[NUM_CELLS] { 0 };
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

	Occ ret_val;
	ret_val.nonZ = nonZs; ret_val.avgD = avgs;

	return ret_val;
}
cv::Mat KinectMotion::getHand2(cv::Mat image)
{
	int top = 0;
	bool foundHand = false;
	int handToWrist = 0;
	double numPixels = 0;
	int max = 0;
	bool atWrist = false;
	int tmp = 0;
	double thresholdRatio = 0;
	double maxThresholdRatio = 0;
	double previousNumPixels = 0;

	for (int i = 25; i < image.rows; i++)
	{
		for (int j = 30; j < image.cols-30; j++)
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
		if (foundHand && numPixels != 0) {
			thresholdRatio = previousNumPixels / numPixels;
			if (maxThresholdRatio < thresholdRatio) {
				maxThresholdRatio = thresholdRatio;
			}
		}
		previousNumPixels = numPixels;
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

