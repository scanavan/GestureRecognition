#ifndef LEAPDATA_H
#define LEAPDATA_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

class Point {
public:
	Point() : x(0.f), y(0.f), z(0.f) {}
	Point(float _x, float _y, float _z);
	void printPoint();
private:
	float x;
	float y;
	float z;
};

class LeapData {
public:
	LeapData(std::string path);
	void parse(int lineNum, std::string line);
	std::vector<float> splitString(std::string line);
	void printAttributes();
private:
	int numFingers;
	std::vector<float> fingerTipDist;
	std::vector<float> fingerTipDistRefined;
	std::vector<float> fingerTipInterDist;
	std::vector<Point> fingerTipPosition;
	std::vector<float> handDirection;
	Point handSphereCenter;
	float handSphereRadius;
	std::vector<float> palmNormal;
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
};

#endif 

