#ifndef LEAPDATA_H
#define LEAPDATA_H


#include <iostream>
#include <fstream>
#include <vector>
#include "Point.h"
#include "RealTimeLeapData.h"


class LeapData {
public:
	LeapData();
	LeapData(std::string path);
	LeapData(RealTimeLeapData leapData);
	void parse(int lineNum, std::string line);
	std::vector<float> splitString(std::string line);
	void printAttributes();
	std::vector<Point> getProjection();
//	std::vector<float> getFingerTipDist();
	void setNewScaleFactor();
	void setFingerAreas();
	void setNewFingerTipDist();
	void setFingerTipAngles();
	void setOldScaleFactor();
	void writeToFile(std::string path);
	int getNumFingers();
	std::vector<float> getFingerTipDist();
	std::vector<float> getFingerTipDistRefined();
	std::vector<float> getFingerTipInterDist();
	std::vector<Point> getFingerTipPosition();
	Point getHandDirection();
	Point getHandSphereCenter();
	float getHandSphereRadius();
	Point getPalmNormal();
	Point getPalmPosition();
	Point getPalmPositionRefined();
	std::vector<float> getPalmVelocity();
	float getRotationAngle();
	std::vector<float> getRotationAxis();
	std::vector<float> getRotationMatrix();
	float getRotationProbability();
	float getScaleFactor();
	float getScaleProbability();
	std::vector<float> getTranslation();
	float getTranslationProbability();

	//new attributes
	float getNewScaleFactor();
	std::vector<float> getFingerAreas();
	std::vector<float> getNewFingerTipDistRefined();
	std::vector<float> getFingerTipAngles();
	std::vector<Point> getProjectionPoints();
	std::vector<float> getFingerTipElevation();
	std::string getGesture();
	std::vector<int> getExtendedFingers();
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
	float oldScaleFactor;
	std::vector<float> FingerAreas;
	std::vector<float> newFingerTipDistRefined;
	std::vector<float> fingerTipAngles;
	std::vector<Point> projectionPoints;
	std::vector<float> fingerTipElevation;
	std::string gesture;
	std::vector<int> extendedFingers;
	std::vector<Point> fingerDirections;
};

#endif 

