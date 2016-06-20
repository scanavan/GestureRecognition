<<<<<<< HEAD
#ifndef REAL_TIME_LEAP_DATA
#define REAL_TIME_LEAP_DATA
#include <vector>
#include "Leap.h"
#include "Point.h"

using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);
	void Clear();
	std::vector<int> getExtendedFingers();
	std::vector<Point> getFingerDirections();
	std::vector<Point> getTipPositions();
	Point getHandDirection();
	Point getPalmNormal();
	Point getPalmPosition();
	int getNumFingers();

  private:
	  int numFingers;
	  std::vector<int> extendedFingers;
	  std::vector<Point> fingerDirections;
	  std::vector<Point> tipPositions;
	  Point handDirection;
	  Point palmNormal;
	  Point palmPosition;
};

class RealTimeLeapData
{
public:
	RealTimeLeapData();
	void Clear();

	//extended fingers for current frame
	std::vector<int>extendedFingers;
	std::vector<Point>fingerDirections;
	std::vector<Point>tipPositions;
	Point handDirection;
	Point palmNormal;
	Point palmPosition;
	int numFingers;
};
#endif
=======
#ifndef REAL_TIME_LEAP_DATA
#define REAL_TIME_LEAP_DATA
#include <vector>
#include "Leap.h"

class RealTimeLeapData
{
public:
	//clear all of the data
	void Clear();
	//NOTE: These are public for ease of use, can make them private with getters/setters
	//extended fingers for current frame
	std::vector<Leap::Finger::Type>extendedFingers;
	//direction of fingers
	std::vector<Leap::Vector>fingerDirections;
	//fingertip positions
	std::vector<Leap::Vector>tipPositions;
};
#endif
>>>>>>> 5edf4a8a857148df591ecef302d0cb64643a7cc9
