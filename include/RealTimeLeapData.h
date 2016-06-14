#ifndef REAL_TIME_LEAP_DATA
#define REAL_TIME_LEAP_DATA
#include <vector>
#include "Leap.h"

class RealTimeLeapData
{
public:
	RealTimeLeapData();
	void Clear();
	//extended fingers for current frame
	std::vector<Leap::Finger::Type>extendedFingers;
	std::vector<Leap::Vector>fingerDirections;
	std::vector<Leap::Vector>tipPositions;
};
#endif
