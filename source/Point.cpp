#include "Point.h"

Point::Point(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void Point::printPoint() {
	std::cout << '(' << x << ", " << y << ", " << z << ')' << " ";
}

float Point::getMagnitude(Point p) {
	return sqrtf(powf((x - p.x), 2) + powf((y - p.y), 2) + powf((z - p.z), 2));
}

float Point::getDotProduct(Point p) {
	return (x * p.x + y * p.y + z * p.z);
}

float Point::getX() {
	return x;
}

float Point::getY() {
	return y;
}

float Point::getZ() {
	return z;
}