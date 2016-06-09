#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <math.h>
#include <string>

class Point {
public:
	Point() : x(0.f), y(0.f), z(0.f) {}
	Point(float _x, float _y, float _z);
	void printPoint();
	std::string writePoint();
	float getMagnitude(Point p);
	float getDotProduct(Point p);
	float getX();
	float getY();
	float getZ();
private:
	float x;
	float y;
	float z;
};

#endif // !POINT_H

