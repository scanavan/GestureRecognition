#ifndef LEAPDATA_H
#define LEAPDATA_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "Point.h"


class LeapData {
public:
	LeapData(std::string path);
	void parse(int lineNum, std::string line);
	std::vector<float> splitString(std::string line);
	void printAttributes();
	std::vector<Point> getProjection();
	void setNewScaleFactor();
	void setNewFingerTipDist();
private:
	int numFingers;
	std::vector<float> fingerTipDist;
	std::vector<float> fingerTipDistRefined;
	std::vector<float> fingerTipInterDist;
	std::vector<Point> fingerTipPosition;
	Point handDirection;
	Point handSphereCenter;
	float handSphereRadius;
	Point palmNormal;
	Point palmPosition;
	Point palmPositionRefined;
	std::vector<float> palmVelocity;
	float rotationAngle;
	std::vector<float> rotationAxis;
	std::vector<float> rotationMatrix;
	float rotationProbability;
	float scaleFactor;
	float scaleProbability;
	std::vector<float> translation;
	float translationProbability;

	//new attributes
	float newScaleFactor;
	std::vector<float> newFingerTipDistRefined;
};

#endif 

