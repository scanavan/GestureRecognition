#ifndef REAL_TIME_LEAP_DATA
#define REAL_TIME_LEAP_DATA
#include <vector>
#include "Leap.h"

class RealTimeLeapData
{
public:
	//clear all of the data
	void Clear();
	//NOTE: These are public for use of use, can make them private with getters/setters
	//extended fingers for current frame
	std::vector<Leap::Finger::Type>extendedFingers;
	//direction of fingers
	std::vector<Leap::Vector>fingerDirections;
	//fingertip positions
	std::vector<Leap::Vector>tipPositions;
};
#endif
