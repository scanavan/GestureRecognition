#ifndef LEAPDATA_H
#define LEAPDATA_H

#define PI				3.14159265

#include <iostream>
#include <fstream>
#include <vector>
#include "Point.h"
//#include "RealTimeLeapData.h"


class LeapData {
public:
	LeapData();
	LeapData(std::string path);
	//LeapData(RealTimeLeapData leapData);
	void RealTimeLeap(std::string path);
	void parse(int lineNum, std::string line);
	void parseRealTime(int lineNum, std::string line);
	void clearAll();

	std::vector<float> splitString(std::string line);
	void printAttributes();
	std::vector<Point> getProjection();
//	std::vector<float> getFingertipDist();
	void setNewScaleFactor();
	void setFingerAreas();
	void setRatio();
	void setNewFingertipDist();
	void setFingertipAngles();
	void setOldScaleFactor();
	int getNumFingers();
	std::vector<float> getFingertipDist();
	std::vector<float> getFingertipDistRefined();
	std::vector<float> getFingertipInterDist();
	std::vector<Point> getFingertipPosition();
	Point getHandDirection();
	Point getHandSphereCenter();
	float getHandSphereRadius();
	std::vector<Point> getFingerDirection();
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
	float getRatio();
	std::vector<float> getTranslation();
	float getTranslationProbability();

	//new attributes
	float getNewScaleFactor();
	float getFingerArea();
	std::vector<float> getNewFingertipDistRefined();
	std::vector<float> getFingertipAngles();
	std::vector<Point> getProjectionPoints();
	std::vector<float> getFingertipElevation();
	std::vector<float> getFingerAngles();
	std::string getGesture();
	std::vector<int> getExtendedFingers();
	void setMaximums();
	float getMax_X();
	float getMax_Y();
	void setFingertipDistance();
	void setFingerAngles();

private:
	std::vector<float>fingerAngles;

	int numFingers;
	std::vector<float> fingertipDist;
	std::vector<float> fingertipDistRefined;
	std::vector<float> fingertipInterDist;
	std::vector<Point> fingertipPosition;
	float ratio;
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
	float fingerArea;
	std::vector<float> newFingertipDistRefined;
	std::vector<float> fingertipAngles;
	std::vector<Point> projectionPoints;
	std::vector<float> fingertipElevation;
	std::string gesture;
	std::vector<int> extendedFingers;
	std::vector<Point> fingerDirections;
	std::vector<Point> extendedTipPosition;
	float max_X;
	float max_Y;
	std::vector<Point> fingerDirection;
};

#endif 