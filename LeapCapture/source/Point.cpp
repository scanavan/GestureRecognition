#include "Point.h"

// constructor
Point::Point(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

// Prints the Point x,y,z coordinates
void Point::printPoint() {
	std::cout << '(' << x << ", " << y << ", " << z << ')' << " ";
}

// returns float distance between this Point and param Point
float Point::getMagnitude(Point p) {
	return sqrtf(powf((x - p.x), 2) + powf((y - p.y), 2) + powf((z - p.z), 2));
}

// returns float DotProduct of this Point and param Point
float Point::getDotProduct(Point p) {
	return (x * p.x + y * p.y + z * p.z);
}

// getter for X coordinate
float Point::getX() {
	return x;
}

// getter for Y coordinate
float Point::getY() {
	return y;
}

// getter for Z coordinate
float Point::getZ() {
	return z;
}

// returns Point as a string separated by commas
std::string Point::writePoint() {
	return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z);
}

