#ifndef SAMPLE_LISTENER
#define SAMPLE_LISTENER
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
#endif