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
	KinectMotion(std::string leap, std::string depth, std::string rgb);
	cv::Mat getHand(cv::Mat image, double ratio);
	cv::Mat makeContourImage(cv::Mat image);
	cv::Mat scaleHand(cv::Mat image);
	cv::Mat rotateImage(cv::Mat image);
	cv::Mat getDepth();
	float * getSil();
	float * getContourDist();
	int * getOccNonz();
	float * getOccAvg();
	float * getHull();
	std::string getGesture();
private:
	cv::Mat getRgb();
	cv::Mat depth;
	cv::Mat rgb;
	LeapData * leap;
	float * sil;
	float * contour_dist;
	int * occ_nonz;
	float * occ_avg;
	float * hull;
	std::string gesture;

	cv::Rect getRect(cv::Mat image);
	float * silhouette(cv::Mat image);
	float * distContour(cv::Mat image);
	Occ cellOccupancy(cv::Mat image);
	float * hullAreas(cv::Mat image);
	cv::Mat binarize(cv::Mat image, int threshold = 5);
	cv::Point palmCenter(cv::Mat image, int thresh = 23);
	std::vector<cv::Point> getContour(cv::Mat image);
	cv::Mat updateImage(cv::Mat image);
};

void createWindow(cv::Mat image, std::string imageName);
bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);

/*
* FEATURES
*
* Silhouette			float[32]
* Contour Distances	float[180]
* Cell Occupancy		int[64/256], float[64/256] - size depends on scaled image size
* Convex Hull			float[6]
*
*/

#endif