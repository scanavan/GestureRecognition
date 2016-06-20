#include "LeapData.h"
#include <sstream>

// constructor
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
		setNewScaleFactor();
		setOldScaleFactor();
		setNewFingerTipDist();
		projectionPoints = getProjection();
		setFingerTipAngles();
		//gets the gesture based on the path
		int index = path.find_last_of("/");
		char gestureNumber = path.at(index - 1);
		gesture = "G";
		gesture.push_back(gestureNumber);
		if (gesture == "Gd") {
			std::cout << path << std::endl;
		}
	}
	else {
		std::cout << "bad file" << std::endl;
	}
	ifs.close();
}

// sets the fingerTipAngles and also fingerTipElevation
// fingerTipElevation = ||fingerTipPosition - projectedPoint|| / newScaleFactor
// fingerTipAngles are determined using law of cosines
void LeapData::setFingerTipAngles() {
	float a, b, c;
	for (int i = 0; i < 5; i++) {
		if (i >= numFingers) {
			fingerTipAngles.push_back(0);
			fingerTipElevation.push_back(0);
		}
		else {
			a = fingerTipDist[i];
			b = projectionPoints[i].getMagnitude(palmPosition);
			c = projectionPoints[i].getMagnitude(fingerTipPosition[i]);
			fingerTipAngles.push_back(acos((-powf(c, 2) + powf(a, 2) + powf(b, 2)) / (2 * a * b)));
			fingerTipElevation.push_back(c / newScaleFactor);
			//
			//		scale factor 
			//
		}
	}
}

// sets the newFingerTipDistance based on newScaleFactor
// newFingerTipDist = dist(fingerTipPosition, palmPosition) / newScaleFactor
void LeapData::setNewFingerTipDist() {
	for (int i = 0; i < 5; i++) {
		if (i >= numFingers) {
			newFingerTipDistRefined.push_back(0.000000);
		}
		else {
			newFingerTipDistRefined.push_back(fingerTipPosition[i].getMagnitude(palmPosition) / newScaleFactor);
		}
	}
}

// calculates newScaleFactor
// newScaleFactor = dist((avg(fingerTipPositions) / numFingers), palmPosition)
void LeapData::setNewScaleFactor() {
	if (numFingers == 0) {
		newScaleFactor = 1;
	}
	else {
		float averageX = 0;
		float averageY = 0;
		float averageZ = 0;
		for (int i = 0; i < numFingers; i++) {
			averageX = averageX + fingerTipPosition[i].getX();
			averageY = averageY + fingerTipPosition[i].getY();
			averageZ = averageZ + fingerTipPosition[i].getZ();
		}
		Point p = Point(averageX / numFingers, averageY / numFingers, averageZ / numFingers);
		newScaleFactor = p.getMagnitude(palmPosition);

	}
}

void LeapData::setOldScaleFactor() {
	if (numFingers == 0) {
		oldScaleFactor = 1;
	}
	else {
		oldScaleFactor = 0;
		for (int i = 0; i < numFingers; ++i)
		{
			if (fingerTipDist[i] > oldScaleFactor)
			{
				oldScaleFactor = fingerTipDist[i];
			}
		}
	}
}

//std::vector<float> LeapData::getFingerTipDist() {
//	std::vector<float> returnVal;
//	for (int i = 0; i < numFingers; i++) {
//		returnVal.push_back(fingerTipPosition[i].getMagnitude(palmPosition));
//	}
//}

// returns vector of Points projected into normal plane
// projection = q(point to project) - dot(
std::vector<Point> LeapData::getProjection() {
	std::vector<Point> returnVal;
	// p = (palmNormal / |palmnormal|) * 1
	float k = 1 / palmPosition.getMagnitude(palmNormal);
	Point p = Point(palmNormal.getX() * k, palmNormal.getY() * k, palmNormal.getZ() * k);
	//d = fingerTipPosition - p
	// returnVal = fingerTipPosition - dot(handDirection, d) * handDirection
	for (int i = 0; i < 5; i++) {
		if (i >= numFingers) {
			returnVal.push_back(Point(0,0,0));
		}
		else {
			Point d = Point(fingerTipPosition[i].getX() - p.getX(), fingerTipPosition[i].getY() - p.getY(), fingerTipPosition[i].getZ() - p.getZ());
			returnVal.push_back(Point(fingerTipPosition[i].getX() - handDirection.getDotProduct(d) * handDirection.getX(),
				fingerTipPosition[i].getY() - handDirection.getDotProduct(d) * handDirection.getY(),
				fingerTipPosition[i].getZ() - handDirection.getDotProduct(d) * handDirection.getZ()));
		}
	}
	return returnVal;
}

// parse all the attributes from CSV file
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
		handDirection = Point(vect[0], vect[1], vect[2]);
		break;
	case 6:
		handSphereCenter = Point(vect[0], vect[1], vect[2]);
		break;
	case 7:
		handSphereRadius = vect[0];
		break;
	case 8:
		palmNormal = Point(vect[0], vect[1], vect[2]);
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

// returns vector of floats with CSV line content split at the commas
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

// prints all the attributes
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
	handDirection.printPoint();
	std::cout << std::endl;

	std::cout << "palmNormal:" << " ";
	palmNormal.printPoint();
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

	std::cout << "New scale factor: " << newScaleFactor << std::endl;
	std::cout << "Old scale factor: " << oldScaleFactor << std::endl;

	std::cout << "newFingerTipDistRefined:" << " ";
	for (int i = 0; i < newFingerTipDistRefined.size(); i++) {
		std::cout << newFingerTipDistRefined[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "fingerTipAngles:" << " ";
	for (int i = 0; i < fingerTipAngles.size(); i++) {
		std::cout << fingerTipAngles[i] << " ";
	}
	std::cout << std::endl;

	std::cout << "projectionPoints:" << " ";
	for (int i = 0; i < projectionPoints.size(); i++) {
		projectionPoints[i].printPoint();
	}
	std::cout << std::endl;

	std::cout << "fingerTipElevation:" << " ";
	for (int i = 0; i < fingerTipElevation.size(); i++) {
		std::cout << fingerTipElevation[i] << " ";
	}
	std::cout << std::endl;
}

void LeapData::writeToFile(std::string path) {
	std::ofstream file;
	file.open(path);
	std::cout << std::fixed;
	file << "numFingers: " << numFingers << "\n";

	file << "fingerTipDist: " ;
	for (int i = 0; i < fingerTipDist.size(); i++) {
		file << std::to_string(fingerTipDist[i]) << " ";
	}
	file << "\n";

	file << "newFingerTipDistRefined: ";
	for (int i = 0; i < newFingerTipDistRefined.size(); i++) {
		file << std::to_string(newFingerTipDistRefined[i]) << " ";
	}
	file << "\n";

	file << "fingerTipInterDist: ";
	for (int i = 0; i < fingerTipInterDist.size(); i++) {
		file << std::to_string(fingerTipInterDist[i]) << " ";
	}
	file << "\n";

	file << "fingerTipPosition: ";
	for (int i = 0; i < fingerTipPosition.size(); i++) {
		file << fingerTipPosition[i].writePoint();
	}
	file << "\n";

	file << "handDirection: ";
	file << handDirection.writePoint();
	file << "\n";

	file << "handSphereCenter: ";
	file << handSphereCenter.writePoint();
	file << "\n";

	file << "handSphereRadius: " << std::to_string(handSphereRadius) << std::endl;

	file << "palmNormal: ";
	file << palmNormal.writePoint();
	file << "\n";

	file << "palmPosition: ";
	file << palmPosition.writePoint();
	file << "\n";

	file << "palmPositionRefined: ";
	file << palmPositionRefined.writePoint();
	file << "\n";

	file << "palmVelocity: ";
	for (int i = 0; i < palmVelocity.size(); i++) {
		file << std::to_string(palmVelocity[i]) << " ";
	}
	file << "\n";

	file << "rotationAngle: " << std::to_string(rotationAngle) << std::endl;

	file << "rotationAxis: ";
	for (int i = 0; i < rotationAxis.size(); i++) {
		file << std::to_string(rotationAxis[i]) << " ";
	}
	file << "\n";

	file << "rotationMatrix: ";
	for (int i = 0; i < rotationMatrix.size(); i++) {
		file << std::to_string(rotationMatrix[i]) << " ";
	}
	file << "\n";

	file << "rotationProbability: " << std::to_string(rotationProbability) << std::endl;

	file << "newScaleFactor: " << newScaleFactor << std::endl;

	file << "translation: ";
	for (int i = 0; i < translation.size(); i++) {
		file << std::to_string(translation[i]) << " ";
	}
	file << "\n";

	file << "translationProbability: " << std::to_string(translationProbability) << std::endl;

	file << "fingerTipAngles: ";
	for (int i = 0; i < fingerTipAngles.size(); i++) {
		file << std::to_string(fingerTipAngles[i]) << " ";
	}
	file << "\n";

	file << "projectionPoints: ";
	for (int i = 0; i < projectionPoints.size(); i++) {
		file << projectionPoints[i].writePoint();
	}
	file << "\n";

	file << "fingerTipElevation: ";
	for (int i = 0; i < fingerTipElevation.size(); i++) {
		file << std::to_string(fingerTipElevation[i]) << " ";
	}
	file << "\n";

	file.close();
}

int LeapData::getNumFingers() {
	return numFingers;
}
std::vector<float> LeapData::getFingerTipDist() {
	return fingerTipDist;
}
std::vector<float> LeapData::getFingerTipDistRefined() {
	return fingerTipDistRefined;
}
std::vector<float> LeapData::getFingerTipInterDist() {
	return fingerTipInterDist;
}
std::vector<Point> LeapData::getFingerTipPosition() {
	return fingerTipPosition;
}
Point LeapData::getHandDirection() {
	return handDirection;
}
Point LeapData::getHandSphereCenter() {
	return handSphereCenter;
}
float LeapData::getHandSphereRadius() {
	return handSphereRadius;
}
Point LeapData::getPalmNormal() {
	return palmNormal;
}
Point LeapData::getPalmPosition() {
	return palmPosition;
}
Point LeapData::getPalmPositionRefined() {
	return palmPositionRefined;
}
std::vector<float> LeapData::getPalmVelocity() {
	return palmVelocity;
}
float LeapData::getRotationAngle() {
	return rotationAngle;
}
std::vector<float> LeapData::getRotationAxis() {
	return rotationAxis;
}
std::vector<float> LeapData::getRotationMatrix() {
	return rotationMatrix;
}
float LeapData::getRotationProbability() {
	return rotationProbability;
}
float LeapData::getScaleFactor() {
	return scaleFactor;
}

float LeapData::getScaleProbability() {
	return scaleProbability;
}
std::vector<float> LeapData::getTranslation() {
	return translation;
}
float LeapData::getTranslationProbability() {
	return translationProbability;
}
float LeapData::getNewScaleFactor() {
	return newScaleFactor;
}
std::vector<float> LeapData::getNewFingerTipDistRefined() {
	return newFingerTipDistRefined;
}
std::vector<float> LeapData::getFingerTipAngles() {
	return fingerTipAngles;
}
std::vector<Point> LeapData::getProjectionPoints() {
	return projectionPoints;
}
std::vector<float> LeapData::getFingerTipElevation() {
	return fingerTipElevation;
}

std::string LeapData::getGesture() {
	return gesture;
}
