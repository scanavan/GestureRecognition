#ifndef REAL_TIME_LEAP_DATA
#define REAL_TIME_LEAP_DATA
#include <vector>
#include "Leap.h"

class RealTimeLeapData
{
public:
	RealTimeLeapData();
	//extended fingers for current frame
	std::vector<Leap::Finger::Type>extendedFingers;
};
#endif
