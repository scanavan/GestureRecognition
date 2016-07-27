#ifndef KINECTMOTION_H
#define KINECTMOTION_H
#include <string>
#include <opencv\cv.h>
#include <vector>
#include "LeapData.h"
#include <set>

#define PI				3.14159265
#define SCALE			480
#define SAMPLE_SIZE		150
#define CELL_DIVS		8
#define NUM_CELLS		CELL_DIVS * CELL_DIVS

struct Occ {
	int * nonZ;
	float * avgD;
};

class KinectMotion {
public:
	KinectMotion(std::string depth);
	cv::Mat getHand(cv::Mat image);
	cv::Mat scaleHand(cv::Mat image);
	cv::Mat getDepth();
	float * getSil();
	float * getContourDist();
	int * getOccNonz();
	float * getOccAvg();
	float * getHull();
	float * getFingerAngle();
	float * getFingerDist();
	std::string getGesture();
private:
	cv::Mat depth;
	cv::Mat scaled_depth;
	cv::Mat scaled_binary;
	std::vector<float> reg_dist_contour;
	LeapData * leap;
	float * sil;
	float * contour_dist;
	int * occ_nonz;
	float * occ_avg;
	float * hull;
	float * finger_angles;
	float * finger_distances;
	std::string gesture;
	cv::Point palm_center;
	std::vector<cv::Point> scaled_contour;
	void initData();
	void fingers();
	std::vector<cv::Point> possible_palm_centers;

	cv::Rect getRect(cv::Mat image);
	float * silhouette();
	float * distContour();
	Occ cellOccupancy();
	float * hullAreas();
	cv::Mat binarize(cv::Mat image, int threshold = 5);
	std::vector<cv::Point> getContour(cv::Mat image);
	std::vector<cv::Point> getSortedContour();
	cv::Mat updateImage(cv::Mat image);
	cv::Point palmCenter(cv::Mat image, int sigma = 150);
};

void createWindow(cv::Mat image, std::string imageName);
bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);

/*
* FEATURES
*
* Silhouette			float[32]
* Contour Distances		float[150]
* Cell Occupancy		int[64], float[64] - size depends on scaled image size
* Convex Hull			float[6]
*
*/

#endif