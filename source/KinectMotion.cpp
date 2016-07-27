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

KinectMotion::KinectMotion(std::string fdepth)
{
	depth = cv::imread(fdepth, CV_LOAD_IMAGE_UNCHANGED);

	auto index = fdepth.find_last_of("/");
	char gestureNumber = fdepth.at(index - 1);
	gesture = "G";
	gesture.push_back(gestureNumber);

	depth = updateImage(depth);
	getHand(depth);
	initData();
	sil = silhouette();
	contour_dist = distContour();
	hull = hullAreas();
	Occ occ_data = cellOccupancy();
	occ_avg = occ_data.avgD;
	occ_nonz = occ_data.nonZ;
	fingers();

	//createWindow(scaled_binary, "Scaled Binary");
}

/*
	@def initialize data for object
	create scaled binary image
	find contour of scaled hand
	find palm center of scaled hand
*/
void KinectMotion::initData()
{
	scaled_depth = scaleHand(depth);
	scaled_binary = binarize(scaled_depth);

	scaled_contour = getContour(scaled_binary);
	palm_center = palmCenter(scaled_binary, 185);

}

// Getters
cv::Mat KinectMotion::getDepth()
{
	return depth;
}
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
float * KinectMotion::getFingerAngle()
{
	return finger_angles;
}
float * KinectMotion::getFingerDist()
{
	return finger_distances;
}
std::string KinectMotion::getGesture() {
	return gesture;
}

/*
	@def gets contour of largest object in image
	@param image (cv::Mat)
	@return contour as a vector of points
*/
std::vector<cv::Point> KinectMotion::getContour(cv::Mat image)
{
	std::vector<cv::Point> rawContour;
	std::vector<cv::Point> sortedContour;
	cv::Mat image_clone = binarize(image);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(image_clone, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	if (contours.size() == 1) rawContour = contours[0];
	else
	{
		size_t max_size = 0;
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

	return rawContour;
}

/*
	@def gets sorted copy of scaled_contour, beginning with point above the palm center
	@return sorted contour as vector of points
*/
std::vector<cv::Point> KinectMotion::getSortedContour()
{
	std::vector<cv::Point> sortedContour;

	int start = 0;
	for (int i = 0; i < scaled_contour.size(); i++)
	{
		if (scaled_contour[i].x == palm_center.x && scaled_contour[i].y <= palm_center.y)
		{
			start = i;
			break;
		}
	}
	for (int i = 0; i < scaled_contour.size(); i++)
	{
		sortedContour.push_back(scaled_contour[(i + start) % scaled_contour.size()]);
	}

	return sortedContour;
}

/*
	@def scales hand to a fixed square image
	@param image (cv::Mat)
	@return scaled image (cv::Mat)
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
		roi.height = static_cast<int>(height * scale);
		roi.y = (SCALE - roi.height) / 2;
	}
	else
	{
		roi.y = 20;
		roi.height = 440;
		roi.width = static_cast<int>(width * scale);
		roi.x = (SCALE - roi.width) / 2;
	}
	cv::resize(croppedImage, dst(roi), roi.size());

	return dst;
}

/*
	@def displays image at runtime
	@param image (cv::Mat) and title (std::string)
*/
void createWindow(cv::Mat image, std::string imageName)
{
	cv::namedWindow(imageName, cv::WINDOW_AUTOSIZE);
	cv::imshow(imageName, image);
	cv::waitKey(0);
}

/*
	@def gets a sample of 150 contour points and calculates the distance from the palm center to each sampled point
	@param image (cv::Mat)
	@return array of 150 distances (floats) from palm center
*/
float * KinectMotion::distContour()
{
	std::vector<cv::Point> edges = getSortedContour();
	std::vector<cv::Point> sampleContour;
	float * retVal = new float[SAMPLE_SIZE] {0};

	for (int i = 0; i < SAMPLE_SIZE; i++)
	{
		sampleContour.push_back(edges[(int)(i * (float)(edges.size()) / SAMPLE_SIZE)]);
	}

	float max = 0;
	for (int i = 0; i < sampleContour.size(); i++)
	{
		auto temp = static_cast<float>(std::sqrt(std::pow(palm_center.x - sampleContour.at(i).x, 2) + std::pow(palm_center.y - sampleContour.at(i).y, 2)));
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

/*
	@def finds rectangle around hand
	@param thresholded depth image (cv::Mat)
	@return rectangle (cv::Rect)
*/
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
	@def gets average distance of contour from image center for 32 radial sections
	@param image (cv::Mat)
	@return array of 32 distances (floats) of contour from image center
*/
float * KinectMotion::silhouette()
{
	cv::Point center = cv::Point(scaled_binary.cols / 2, scaled_binary.rows / 2);
	std::vector<float> bins[32];

	float angle, dist;
	int x, y, bin;
	for (int i = 0; i < scaled_contour.size(); ++i)
	{
		x = scaled_contour[i].x - center.x; y = center.y - scaled_contour[i].y;
		angle = static_cast<float>(atan2(y, x));
		if (angle < 0.f) angle = static_cast<float>((2.f * PI) + angle);
		dist = static_cast<float>(sqrt((x*x) + (y*y)));
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

/*
	@def finds areas between hand contour and it's convex hull
	@return array of 6 areas (floats)
*/
float * KinectMotion::hullAreas()
{
	float * ret_array = new float[6]{ 0,0,0,0,0,0 };

	double hand_area = contourArea(scaled_contour);

	std::vector<cv::Point> hull;
	convexHull(scaled_contour, hull);

	cv::Mat hull_image = cv::Mat::zeros(scaled_binary.size(), CV_8U);
	std::vector <std::vector<cv::Point>> contours;
	contours.push_back(hull);
	cv::drawContours(hull_image, contours, 0, cv::Scalar(255, 255, 255));

	cv::Point seed = cv::Point(hull_image.rows / 2, hull_image.cols / 2);
	floodFill(hull_image, seed, cv::Scalar(255, 255, 255));

	for (int i = 0; i < scaled_binary.rows; ++i)
	{
		for (int j = 0; j < scaled_binary.cols; ++j)
		{
			if (scaled_binary.at<uchar>(i, j) != 0) hull_image.at<uchar>(i, j) = 0;
		}
	}

	cv::findContours(hull_image, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
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
		cv::Moments m = moments(thresholded_hull_contours[i]);
		cv::Point centroid = cv::Point((int)(m.m10 / m.m00), (int)(m.m01 / m.m00));
		angles.push_back(static_cast<float>(atan2(centroid.x - palm_center.x, centroid.y - palm_center.y)));
		if (angles[i] < 0.f) angles[i] = static_cast<float>((2.f * PI) + angles[i]);
		ret_array[i] = static_cast<float>(contourArea(thresholded_hull_contours[i]) / hand_area);
	}

	return ret_array;
}

/*
	@def binarizes a grayscale image
	@param grayscale image (cv::Mat), optional threshold (int)
	@return binary image (cv::Mat)
*/
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

/*
	@def utility function for std::sort
	compares areas of two contours
	@param two contours (vector of points)
	@return boolean value
*/
bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2)
{
	double i = fabs(contourArea(cv::Mat(contour1)));
	double j = fabs(contourArea(cv::Mat(contour2)));
	return (i > j);
}

/*
	@def thresholds original depth image to filter out anything behind the hand
	@param original depth image (cv::Mat)
	@return updated image (cv::Mat)
*/
cv::Mat KinectMotion::updateImage(cv::Mat image)
{
	cv::Mat uimage; image.convertTo(uimage, CV_8U);

	// find closest pixel and remove noise
	int min = 255;
	for (int i = 50; i < image.rows - 50; i++)
	{
		for (int j = 50; j < image.cols - 50; j++)
		{
			if (uimage.at<uchar>(i, j) < min && uimage.at<uchar>(i,j) != 0) min = uimage.at<uchar>(i, j);
		}
	}

	// threshold behind closest object
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (uimage.at<uchar>(i, j) > min + 2 || uimage.at<uchar>(i,j) < min) uimage.at<uchar>(i, j) = 0;
		}
	}
	
	return uimage;
}

/*
	@def finds number of non-zero pixels and average depth inside each box of an 8x8 grid of hand image
	@return Occ struct - array of 64 numbers of non-zero pixels (ints), array of 64 averages (floats)
*/
Occ KinectMotion::cellOccupancy()
{
	int i_size = scaled_binary.rows / CELL_DIVS; int j_size = scaled_binary.cols / CELL_DIVS;
	int box_size = i_size * j_size;
	float * avgs = new float[NUM_CELLS] {0}; int * nonZs = new int[NUM_CELLS] {0}; float * sums = new float[NUM_CELLS] { 0 };
	for (int i = 0; i < scaled_binary.rows; ++i)
	{
		for (int j = 0; j < scaled_binary.cols; ++j)
		{
			if (scaled_binary.at<uchar>(i, j) != 0)
			{
				sums[(j / j_size)*CELL_DIVS + (i / i_size)] += scaled_binary.at<uchar>(i, j);
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

/*
	@def finds points where arm meets hand and removes arm from image
	@param thresholded depth image (cv::Mat)
	@return updated image (cv::Mat)
*/
cv::Mat KinectMotion::getHand(cv::Mat image)
{
	int top = 0;
	bool foundHand = false;
	bool foundWrist = false;
	int handToWrist = 0;
	int numPixels = 0;
	int max = 0;
	int tmp = 0;
	double thresholdRatio = 0;
	double maxThresholdRatio = 0;
	double previousNumPixels = 0;

	for (int i = 25; i < image.rows; i++)
	{
		for (int j = 30; j < image.cols - 30; j++)
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
		if (numPixels > 0)
		{
			handToWrist++;
			if (numPixels > max)
			{
				max = numPixels;
			}
			if (i > top + 115 && tmp != 0 && numPixels <= tmp * thresholdRatio && tmp <= max * .95)
			{
				foundWrist = true;
				break;
			}
			tmp = numPixels;
			numPixels = 0;
		}
		if (foundWrist) {
			break;
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
	@def true modulous function which accounts for negative numbers
	@param int a, int b
	@return a mod b
*/
int mod(int a, int b)
{
	if (a >= 0)
	{
		return a % b;
	}
	return (a + b) % b;
}

/*
	@def finds fingertips and calculates their distances and angles from the palm center
*/
void KinectMotion::fingers()
{
	// Sample scaled contour
	std::vector<cv::Point> sampled_contour = scaled_contour;
	int contour_start = 0;
	for (int i = 0; i < scaled_contour.size(); ++i) 
	{
		if (scaled_contour[i].x == palm_center.x && scaled_contour[i].y > palm_center.y)
		{
			sampled_contour.clear();
			for (int j = 0; j < scaled_contour.size(); ++j)
			{
				sampled_contour.push_back(scaled_contour[(i + j) % scaled_contour.size()]);
			}
			break;
		}
	}

	// Calculate distances
	std::vector<float> contour_distances;
	for (int i = 0; i < sampled_contour.size(); ++i)
	{
		contour_distances.push_back(static_cast<float>(std::sqrt(std::pow(palm_center.x - sampled_contour[i].x, 2) + std::pow(palm_center.y - sampled_contour[i].y, 2))));
	}

	// Find Fingers
	std::vector<int> finger_indicies;
	for (auto i = 0; i < contour_distances.size(); ++i)
	{
		if ((contour_distances[i] - contour_distances[mod(static_cast<int>(i + (sampled_contour.size() / 10)), static_cast<int>(contour_distances.size()))] > 70) && (contour_distances[i] - contour_distances[mod(static_cast<int>(i - (sampled_contour.size() / 10)), static_cast<int>(contour_distances.size()))] > 70))
		{
			finger_indicies.push_back(i);
		}
	}

	// Clusters 
	std::vector<std::vector<int>> finger_clusters;
	std::vector<int> cluster; 
	if (finger_indicies.size() > 0) cluster.push_back(finger_indicies[0]);
	for (auto i = 1; i < finger_indicies.size(); ++i)
	{
		if (mod(finger_indicies[i] - finger_indicies[i - 1],static_cast<int>(sampled_contour.size())) > 100) {
			finger_clusters.push_back(cluster);
			cluster.clear();
		}
		cluster.push_back(finger_indicies[i]);
	}
	finger_clusters.push_back(cluster);

	// Find Tips
	std::vector<int> finger_tips;
	for (int i = 0; i < finger_clusters.size(); ++i)
	{
		float max_dist_for_cluster = 0; int max_dist_index;
		for (int j = 0; j < finger_clusters[i].size(); ++j)
		{
			if (contour_distances[finger_clusters[i][j]] > max_dist_for_cluster)
			{
				max_dist_for_cluster = contour_distances[finger_clusters[i][j]];
				max_dist_index = finger_clusters[i][j];
			}
		}
		finger_tips.push_back(max_dist_index);
	}

	// Delete tips below palm center
	for (int i = 0; i < finger_tips.size(); ++i)
	{
		if (sampled_contour[finger_tips[i]].y > palm_center.y + 80)
		{
			finger_tips.erase(finger_tips.begin() + i);
		}
	}

	if (finger_tips.size() > 5)
	{
		std::cout << "AYYY" << std::endl;
		for (int i = 0; i < finger_tips.size(); ++i)
		{
			if (sampled_contour[finger_tips[i]].y > palm_center.y)
			{
				finger_tips.erase(finger_tips.begin() + i);
			}
		}
	}

	if (finger_tips.size() > 5) std::cout << "AYYY" << std::endl;

	float * finger_tip_distances = new float[5]{ 0 };
	float * finger_tip_angles = new float[5]{ 0 };
	for (int i = 0; i < finger_tips.size(); ++i)
	{
		finger_tip_angles[i] = static_cast<float>(atan2(sampled_contour[finger_tips[i]].x - palm_center.x , sampled_contour[finger_tips[i]].y - palm_center.y));
		finger_tip_distances[i] = static_cast<float>(std::sqrt(std::pow(palm_center.x - sampled_contour[finger_tips[i]].x, 2) + std::pow(palm_center.y - sampled_contour[finger_tips[i]].y, 2)));
	}
	for (int i = 1; i < finger_tips.size(); ++i)
	{
		for (int j = 1; j < finger_tips.size(); ++j)
		{
			float temp_angle, temp_distance;
			if (finger_tip_angles[j - 1] > finger_tip_angles[j])
			{
				temp_angle = finger_tip_angles[j - 1]; finger_tip_angles[j - 1] = finger_tip_angles[j]; finger_tip_angles[j] = temp_angle;
				temp_distance = finger_tip_distances[j - 1]; finger_tip_distances[j - 1] = finger_tip_distances[j]; finger_tip_distances[j] = temp_distance;
			}
		}
	}

	finger_angles = finger_tip_angles; finger_distances = finger_tip_distances;

	return;
}

/*
	@def finds palm center
	@param image (cv::Mat), optional sigma for Gaussian blur (int)
	@return palm center (cv::Point)
*/
cv::Point KinectMotion::palmCenter(cv::Mat image, int sigma) 
{
	cv::Mat new_image = image.clone();
	cv::GaussianBlur(image, new_image, cv::Size(0, 0), sigma, sigma);
	

	int max = 0;
	for (int i = 0; i < new_image.rows; ++i)
	{
		for (int j = 0; j < new_image.cols; ++j)
		{
			if (static_cast<int>(new_image.at<uchar>(i, j)) > max)
			{
				max = static_cast<int>(new_image.at<uchar>(i, j));
				possible_palm_centers.clear();
				possible_palm_centers.push_back(cv::Point(j, i));
			}
			else if (static_cast<int>(new_image.at<uchar>(i, j)) == max)
			{
				possible_palm_centers.push_back(cv::Point(j, i));
			}
		}
	}

	double max_min = 0;
	int center_index;
	for (int i = 0; i < possible_palm_centers.size(); ++i)
	{
		double current_min = 8000000;
		for (int j = 0; j < scaled_contour.size(); j += 16)
		{
			auto temp = std::sqrt(std::pow(possible_palm_centers[i].x - scaled_contour[j].x, 2) + std::pow(possible_palm_centers[i].y - scaled_contour[j].y, 2));
			if (temp < current_min)
			{
				current_min = temp;
				if (current_min < max_min) break;
			}
		}
		if (current_min > max_min)
		{
			max_min = current_min;
			center_index = i;
		}
	}

	return possible_palm_centers[center_index];
}