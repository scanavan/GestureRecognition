#include "RealTimeLeapData.h"

RealTimeLeapData::RealTimeLeapData() {
	//empty constructor
}

void RealTimeLeapData::getData() {

	//SampleListener listener;
	Controller controller;
	//controller.addListener(listener);

	// Keep this process running until Enter is pressed
	std::cout << "Enter Gesture..." << std::endl;
	std::string input;
	std::cin >> input;
	gesture = input;
	const Frame frame = controller.frame();
	Clear();
	extendedFingers = { 0,0,0,0,0 };

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		// Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		std::cout << std::string(2, ' ') << handType << std::endl;
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();
		const Vector position = hand.palmPosition();

		palmNormal = Point(normal.x, normal.y, normal.z);
		handDirection = Point(direction.x, direction.y, direction.z);
		palmPosition = Point(position.x, position.y, position.z);

		// Get fingers
		const FingerList fingers = hand.fingers();
		const FingerList extended = fingers.extended();

		std::vector<Finger::Type> types;
		for (FingerList::const_iterator fl = extended.begin(); fl != extended.end(); ++fl) {
			const Finger finger = *fl;
			types.push_back(finger.type());
		}
		numFingers = types.size();
		for (int i = 0; i < types.size(); ++i)
		{
			if (types[i] == Finger::TYPE_THUMB)
				extendedFingers[0] = 1;
			if (types[i] == Finger::TYPE_INDEX)
				extendedFingers[1] = 1;
			if (types[i] == Finger::TYPE_MIDDLE)
				extendedFingers[2] = 1;
			if (types[i] == Finger::TYPE_RING)
				extendedFingers[3] = 1;
			if (types[i] == Finger::TYPE_PINKY)
				extendedFingers[4] = 1;
		}

		for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Finger finger = *fl;
			const Vector direction = finger.direction();
			fingerDirections.push_back(Point(direction.x, direction.y, direction.z));
			const Bone distal = finger.bone(Bone::TYPE_DISTAL);
			const Vector tip = distal.nextJoint();
			tipPositions.push_back(Point(tip.x, tip.y, tip.z));
		}
		std::cout << "Number of Fingers: " << numFingers << std::endl;
		std::cout << "\nextendedFingers: ";
		for (int i = 0; i < extendedFingers.size(); i++) {
			if (i == extendedFingers.size() - 1) {
				std::cout << extendedFingers[i];
			}
			else {
				std::cout << extendedFingers[i] << ",";
			}

		}

	}
	//std::cin.get();
	//extendedFingers = listener.getExtendedFingers();
	//fingerDirections = listener.getFingerDirections();
	//tipPositions = listener.getTipPositions();
	//handDirection = listener.getHandDirection();
	//palmNormal = listener.getPalmNormal();
	//palmPosition = listener.getPalmPosition();
	//numFingers = listener.getNumFingers();
	//std::cout << "Number of Fingers: " << numFingers << std::endl;
	//std::cout << "handDirection: ";
	//handDirection.printPoint();
	//std::cout << "\npalmNormal: ";
	//palmNormal.printPoint();
	//std::cout << "\npalmPosition: ";
	//palmPosition.printPoint();
	//std::cout << "\nextendedFingers: ";
	//for (int i = 0; i < extendedFingers.size(); i++) {
	//	if (i == extendedFingers.size() - 1) {
	//		std::cout << extendedFingers[i];
	//	}
	//	else {
	//		std::cout << extendedFingers[i] << ",";
	//	}

	//}
	//std::cout << "\nfingerDirections: ";
	//for (int i = 0; i < fingerDirections.size(); i++) {
	//	fingerDirections[i].printPoint();
	//}
	//std::cout << "\ntipPositions: ";
	//for (int i = 0; i < tipPositions.size(); i++) {
	//	tipPositions[i].printPoint();
	//}

	//// Remove the sample listener when done
	//controller.removeListener(listener);

	//std::cin >> gesture;
}
void RealTimeLeapData::Clear()
{
	extendedFingers.clear();
	fingerDirections.clear();
	tipPositions.clear();
}

std::vector<int> RealTimeLeapData::getExtendedFingers() {
	return extendedFingers;
}
std::vector<Point> RealTimeLeapData::getFingerDirections() {
	return fingerDirections;
}
std::vector<Point> RealTimeLeapData::getTipPositions() {
	return tipPositions;
}
Point RealTimeLeapData::getHandDirection() {
	return handDirection;
}
Point RealTimeLeapData::getPalmNormal() {
	return palmNormal;
}
Point RealTimeLeapData::getPalmPosition() {
	return palmPosition;
}

int RealTimeLeapData::getNumFingers() {
	return numFingers;
}

std::string RealTimeLeapData::getGesture() {
	return gesture;
}
