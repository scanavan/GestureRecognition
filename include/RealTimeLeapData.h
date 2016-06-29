#ifndef REAL_TIME_LEAP_DATA
#define REAL_TIME_LEAP_DATA
#include <vector>
#include "Leap.h"
#include "Point.h"
#include "SampleListener.h"

using namespace Leap;

class RealTimeLeapData
{
public:
	RealTimeLeapData();
	void getData();
	void Clear();
	std::vector<int> getExtendedFingers();
	std::vector<Point> getFingerDirections();
	std::vector<Point> getTipPositions();
	std::vector<Point> getExtendedTipPositions();
	Point getHandDirection();
	Point getPalmNormal();
	Point getPalmPosition();
	int getNumFingers();
	std::string getGesture();

	//extended fingers for current frame
	std::vector<int>extendedFingers;
	std::vector<Point>fingerDirections;
	std::vector<Point>tipPositions;
	std::vector<Point>ExtendedTipPositions;
	Point handDirection;
	Point palmNormal;
	Point palmPosition;
	int numFingers;
	std::string gesture;
};
#endif
