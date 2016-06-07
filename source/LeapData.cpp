#include "LeapData.h"
#include <sstream>


Point::Point(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}

void Point::printPoint() {
	std::cout << '(' << x << ", " << y << ", " << z << ')' << " ";
}
LeapData::LeapData(std::string path) {
	std::ifstream ifs;
	std::string line;

	ifs.open(path, std::ifstream::in);

	if (ifs.is_open()) {
		int counter = 0;
		while (!ifs.eof()) {
			std::getline(ifs, line);
			parse(counter, line);
			counter++;
		}
	}
	ifs.close();
}

void LeapData::parse(int lineNum, std::string line) {
	std::vector<float> vect = splitString(line);
	std::vector<Point> pointVect;
	switch (lineNum) {
	case 0:
		numFingers = vect[0];
		break;
	case 1:
		fingerTipDist = vect;
		break;
	case 2:
		fingerTipDistRefined = vect;
		break;
	case 3:
		fingerTipInterDist = vect;
		break;
	case 4:
		for (int i = 0; i < 5; i++) {
			pointVect.push_back(Point(vect[i], vect[i+5], vect[i+10]));
		}
		fingerTipPosition = pointVect;
		break;
	case 5:
		handDirection = vect;
		break;
	case 6:
		handSphereCenter = Point(vect[0], vect[1], vect[2]);
		break;
	case 7:
		handSphereRadius = vect[0];
		break;
	case 8:
		palmNormal = vect;
		break;
	case 9:
		palmPosition = Point(vect[0], vect[1], vect[2]);
		break;
	case 10:
		palmPositionRefined = Point(vect[0], vect[1], vect[2]);
		break;
	case 11:
		palmVelocity = vect;
		break;
	case 12:
		rotationAngle = vect[0];
		break;
	case 13:
		rotationAxis = vect;
		break;
	case 14:
		rotationMatrix = vect;
		break;
	case 15:
		rotationProbability = vect[0];
		break;
	case 16:
		scaleFactor = vect[0];
		break;
	case 17:
		scaleProbability = vect[0];
		break;
	case 18:
		translation = vect;
		break;
	case 19:
		translationProbability = vect[0];
		break;
	default:
		break;
	}

}

std::vector<float> LeapData::splitString(std::string line) {
	std::istringstream ss(line);
	std::string token;
	int counter = 0;
	std::vector<float> returnVal;
	std::string::size_type sz;

	while (std::getline(ss, token, ',')) {
		if (counter != 0) {
			returnVal.push_back(std::stof(token, &sz));
		}
		counter++;
	}

	return returnVal;
}

void LeapData::printAttributes() {
	std::cout << "number of fingers: " << numFingers << std::endl;

	std::cout << "fingerTipDist:" << " ";
	for (int i = 0; i < fingerTipDist.size(); i++) {
		std::cout << fingerTipDist[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "fingerTipDistRefined:" << " ";
	for (int i = 0; i < fingerTipDistRefined.size(); i++) {
		std::cout << fingerTipDistRefined[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "fingerTipInterDist:" << " ";
	for (int i = 0; i < fingerTipInterDist.size(); i++) {
		std::cout << fingerTipInterDist[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "handDirection:" << " ";
	for (int i = 0; i < handDirection.size(); i++) {
		std::cout << handDirection[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "palmNormal:" << " ";
	for (int i = 0; i < palmNormal.size(); i++) {
		std::cout << palmNormal[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "palmVelocity:" << " ";
	for (int i = 0; i < palmVelocity.size(); i++) {
		std::cout << palmVelocity[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "rotationAxis:" << " ";
	for (int i = 0; i < rotationAxis.size(); i++) {
		std::cout << rotationAxis[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "rotationMatrix:" << " ";
	for (int i = 0; i < rotationMatrix.size(); i++) {
		std::cout << rotationMatrix[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "translation:" << " ";
	for (int i = 0; i < translation.size(); i++) {
		std::cout << translation[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "handSphereRadius: " << handSphereRadius << std::endl;

	std::cout << "rotationAngle: " << rotationAngle << std::endl;

	std::cout << "rotationProbability: " << rotationProbability << std::endl;

	std::cout << "scaleFactor: " << scaleFactor << std::endl;

	std::cout << "scaleProbability: " << scaleProbability << std::endl;

	std::cout << "translationProbability: " << translationProbability << std::endl;

	std::cout << "fingerTipPosition:" << " ";
	for (int i = 0; i < fingerTipPosition.size(); i++) {
		fingerTipPosition[i].printPoint();
	}
	std::cout << std::endl;

	std::cout << "handSphereCenter:" << " ";
	handSphereCenter.printPoint();
	std::cout << std::endl;

	std::cout << "palmPosition:" << " ";
	palmPosition.printPoint();
	std::cout << std::endl;

	std::cout << "palmPositionRefined:" << " ";
	palmPositionRefined.printPoint();
	std::cout << std::endl;

}
