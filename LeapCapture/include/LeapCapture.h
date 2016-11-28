#ifndef LEAP_CAPTURE_H
#define LEAP_CAPTURE_H
#include "Leap.h"
#include <fstream>

class LeapCapture
{
public:
	LeapCapture();
	~LeapCapture();
	bool Capture();
	void writeArffFile(char button);
	void clearVectors();
	void WriteArffFileHeader(std::string outName);
	void GetGestureVector(std::vector<float>& data);
	void AppendArffFile(std::string outName);

private:
	Leap::Vector palmPosition;
	Leap::Vector stablePalmPosition;
	Leap::Vector normal;
	Leap::Vector direction;
	float pinchStrength;
	float grabStrength;
	float width;
	std::vector<Leap::Vector> fingerDirections;
	std::vector<Leap::Vector> fingertips;
	std::vector<Leap::Vector> stableTipPositions;
	std::vector<int> extendedFingers;
	Leap::Frame referenceFrame;
	int ctr = 1;
	Leap::Finger frontMostFinger;
	Leap::FingerList fingers;
	Leap::FingerList extended;
	std::vector<Leap::Finger::Type> types;
	float averageFingerWidth;
	float fingerWidths[5];
	float scaleFactor;
	bool handFound = false;

	std::ofstream outArffFile;
	Leap::Controller controller;
};
#endif