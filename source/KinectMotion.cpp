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
	//leap = new LeapData(fleap);
	depth = cv::imread(fdepth, CV_LOAD_IMAGE_UNCHANGED);
	//rgb = cv::imread(frgb, CV_LOAD_IMAGE_UNCHANGED);

	int index = fdepth.find_last_of("/");
	char gestureNumber = fdepth.at(index - 1);
	gesture = "G";
	gesture.push_back(gestureNumber);

	depth = updateImage(depth);
	//depth = rotateImage(depth);
	getHand(depth);
	//createWindow(depth, "lol");
	initData();
	//otherDistContour(scaled_binary);
	sil = silhouette(scaled_depth);
	contour_dist = distContour(scaled_binary);
	hull = hullAreas(scaled_depth);
	Occ occ_data = cellOccupancy(scaled_depth);
	occ_avg = occ_data.avgD;
	occ_nonz = occ_data.nonZ;
	fingers();
}


void KinectMotion::initData()
{
	unscaled_contour = getContour(binarize(depth));
	scaled_depth = scaleHand(depth);
	scaled_binary = binarize(scaled_depth);

	palm_center = palmCenter(scaled_binary, 150);
	scaled_contour = getContour(scaled_binary);

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

	return rawContour;
}

std::vector<cv::Point> KinectMotion::getSortedContour(cv::Mat image)
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
	std::vector<cv::Point> edges = getSortedContour(image);
	std::vector<cv::Point> sampleContour;
	float * retVal = new float[SAMPLE_SIZE] {0};

	for (int i = 0; i < SAMPLE_SIZE; i++)
	{
		sampleContour.push_back(edges[(int)(i * (float)(edges.size()) / SAMPLE_SIZE)]);
	}

	float max = 0;
	for (int i = 0; i < sampleContour.size(); i++)
	{
		float temp = std::sqrt(std::pow(palm_center.x - sampleContour.at(i).x, 2) + std::pow(palm_center.y - sampleContour.at(i).y, 2));
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
	//createWindow(image, "Updated Image");
	//createWindow(dst, "Rotated Image");
	return dst;
}

/*
@def - gets average distance of contour from image center for 32 radial sections
@param - image - image matrix
*/
float * KinectMotion::silhouette(cv::Mat image)
{
	cv::Point center = cv::Point(image.cols / 2, image.rows / 2);
	std::vector<float> bins[32];

	float angle, dist;
	int x, y, bin;
	for (int i = 0; i < scaled_contour.size(); ++i)
	{
		x = scaled_contour[i].x - center.x; y = center.y - scaled_contour[i].y;
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

	double hand_area = contourArea(scaled_contour);

	std::vector<cv::Point> hull;
	convexHull(scaled_contour, hull);

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
			if (scaled_binary.at<uchar>(i, j) != 0) hull_image.at<uchar>(i, j) = 0;
		}
	}

	//cv::Mat new_image = cv::Mat::zeros(image.size(), CV_8UC3);
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
		//cv::drawContours(new_image, thresholded_hull_contours, i, cv::Scalar(i * 255 / thresholded_hull_contours.size(), 0, 255));
		cv::Moments m = moments(thresholded_hull_contours[i]);
		cv::Point centroid = cv::Point((int)(m.m10 / m.m00), (int)(m.m01 / m.m00));
		angles.push_back(atan2(palm_center.y - centroid.y, centroid.x - palm_center.x));
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
			if (uimage.at<uchar>(i, j) > min + 4 || uimage.at<uchar>(i,j) < min) uimage.at<uchar>(i, j) = 0;
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
cv::Mat KinectMotion::getHand(cv::Mat image)
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
		if (numPixels > 0 && !atWrist)
		{
			handToWrist++;
			if (numPixels > max)
			{
				max = numPixels;
			}
			if (i > top + 100 && tmp != 0 && numPixels <= tmp * thresholdRatio && tmp <= max * .78)
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

void KinectMotion::sortContourDist() {
	std::vector<float> tmp;
	for (int i = 0; i < SAMPLE_SIZE; i++) {
		tmp.push_back(contour_dist[i]);
	}

	std::sort(tmp.begin(), tmp.end());

	for (int i = 0; i < SAMPLE_SIZE; i++) {
		contour_dist[i] = tmp[i];
	}
}

void KinectMotion::otherDistContour(cv::Mat image) {
	std::vector<cv::Point> sampleContour;

	for (int i = 0; i < unscaled_contour.size() - (int)(unscaled_contour.size() / 4); i++)
	{
		sampleContour.push_back(unscaled_contour[(int)(i * (float)(unscaled_contour.size()) / (unscaled_contour.size() - (int)(unscaled_contour.size() / 4)))]);
	}

	for (int i = 0; i < sampleContour.size(); i++) {
		if (palm_center.y - sampleContour.at(i).y > 0) {
			float temp = std::sqrt(std::pow(palm_center.x - sampleContour.at(i).x, 2) + std::pow(palm_center.y - sampleContour.at(i).y, 2));
			reg_dist_contour.push_back(temp);
		}
	}
}

int KinectMotion::countFingers() {
	float last_contour_dist;
	bool goingUp = true;
	int fingers = 0;
	int numberOfIncreasing = 0;
	std::vector<float> localMaxima{ 0 };
	if (reg_dist_contour.size() > 0) {
		last_contour_dist = reg_dist_contour[0];
		for (int i = 0; i < reg_dist_contour.size(); i++) {
			if (reg_dist_contour[i] < last_contour_dist && goingUp && numberOfIncreasing > 12) {
				localMaxima.push_back(last_contour_dist);
				numberOfIncreasing = 0;
			}
			if (reg_dist_contour[i] > last_contour_dist) {
				goingUp = true;
				numberOfIncreasing++;
			}
			else if (reg_dist_contour[i] < last_contour_dist) {
				goingUp = false;
				numberOfIncreasing = 0;
			}
			last_contour_dist = reg_dist_contour[i];
		}
		for (int i = 0; i < localMaxima.size(); i++) {
			if (localMaxima[i] > 125 && fingers < 5) {
				fingers++;
			}
		}
	}

	return fingers;
}

int mod(int a, int b)
{
	if (a >= 0)
	{
		return a % b;
	}
	return (a + b) % b;
}
void KinectMotion::fingers()
{
	cv::Mat finger_image = cv::Mat::zeros(scaled_binary.size(),CV_8UC3); 
	for (int i = 0; i < scaled_binary.rows; ++i)
	{
		for (int j = 0; j < scaled_binary.cols; ++j)
		{
			if (scaled_binary.at<uchar>(i, j) > 5) finger_image.at<cv::Vec3b>(i, j) = cv::Vec3b(100, 100, 0);
		}
	}

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

	//std::cout << scaled_contour.size() << " " << sampled_contour.size() << std::endl;

	// Calculate distances
	std::vector<float> contour_distances;
	for (int i = 0; i < sampled_contour.size(); ++i)
	{
		contour_distances.push_back(std::sqrt(std::pow(palm_center.x - sampled_contour[i].x, 2) + std::pow(palm_center.y - sampled_contour[i].y, 2)));
	}

	// Find Fingers
	std::vector<int> finger_indicies;
	for (int i = 0; i < contour_distances.size(); ++i)
	{
		if ((contour_distances[i] - contour_distances[mod(i + 120, contour_distances.size())] > 70) && (contour_distances[i] - contour_distances[mod(i - 120, contour_distances.size())] > 70))
		{
			finger_image.at<cv::Vec3b>(sampled_contour[i].y, sampled_contour[i].x) = cv::Vec3b(0, 0, 255);
			finger_indicies.push_back(i);
		}
	}

	std::vector<std::vector<int>> finger_clusters;
	std::vector<int> cluster; 
	if (finger_indicies.size() > 0) cluster.push_back(finger_indicies[0]);
	for (int i = 1; i < finger_indicies.size(); ++i)
	{
		if (mod(finger_indicies[i] - finger_indicies[i - 1],sampled_contour.size()) > 100) {
			finger_clusters.push_back(cluster);
			cluster.clear();
		}
		cluster.push_back(finger_indicies[i]);
	}
	finger_clusters.push_back(cluster);

	// Find Tips
	//std::vector<int> finger_tips;
	//bool consec = false;
	//int num_consec = 0;
	//for (int i = 0; i < finger_indicies.size() - 1; ++i)
	//{
	//	if (finger_indicies[i + 1] - finger_indicies[i] > 1)
	//	{
	//		consec = false;
	//	}
	//	else
	//	{
	//		consec = true;
	//		num_consec++;
	//	}

	//	if (num_consec > 20 && !(consec))
	//	{
	//		float max_dist = 0;
	//		int max_dist_index;
	//		for (int j = finger_indicies[i]; j > (finger_indicies[i] - num_consec); --j)
	//		{
	//			if (contour_distances[j] > max_dist)
	//			{
	//				max_dist = contour_distances[j];
	//				max_dist_index = j;
	//			}
	//		}
	//		finger_tips.push_back(max_dist_index);
	//		//finger_image.at<cv::Vec3b>(sampled_contour[max_dist_index].y, sampled_contour[max_dist_index].x) = cv::Vec3b(0, 0, 255);
	//	}
	//	if (!consec) num_consec = 0;
	//}
	//for (int i = 0; i < finger_tips.size() - 1; ++i)
	//{
	//	if (mod(finger_tips[i + 1] - finger_tips[i] < 200,sampled_contour.size())) finger_tips.erase(finger_tips.begin()+i);
	//}

	// Display
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
			//finger_image.at<cv::Vec3b>(sampled_contour[finger_clusters[i][j]].y, sampled_contour[finger_clusters[i][j]].x) = cv::Vec3b(0, (i * 255 / finger_clusters.size()), 255);
		}
		finger_tips.push_back(max_dist_index);
	}

	for (int i = 0; i < finger_tips.size(); ++i)
	{
		//finger_image.at<cv::Vec3b>(sampled_contour[finger_tips[i]].y, sampled_contour[finger_tips[i]].x) = cv::Vec3b(0, (i * 255 / finger_clusters.size()), 255);
		//std::cout << finger_tips[i] << " ";
	}

	//createWindow(finger_image, "A");
	

	return;
}