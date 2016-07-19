#include "SampleListener.h"

void SampleListener::Clear()
{
	extendedFingers.clear();
	fingerDirections.clear();
	tipPositions.clear();
}

void SampleListener::onInit(const Controller& controller) {
	std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
	std::cout << "Connected" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
	// Note: not dispatched when running in a debugger.
	std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
	std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
	// Get the most recent frame and report some basic information
	const Frame frame = controller.frame();
	Clear();
	extendedFingers = { 0,0,0,0,0 };

	HandList hands = frame.hands();
	for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
		//Get the first hand
		const Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		std::cout << std::string(2, ' ') << handType << std::endl;
		// Get the hand's normal vector and direction
		const Vector normal = hand.palmNormal();
		const Vector direction = hand.direction();
		const Vector position = hand.palmPosition();

		palmNormal = Point(normal.x, normal.y, normal.z);
		palmNormal.printPoint();
		handDirection = Point(direction.x, direction.y, direction.z);
		handDirection.printPoint();
		palmPosition = Point(position.x, position.y, position.z);
		palmPosition.printPoint();

		// Get fingers
		const FingerList fingers = hand.fingers();
		const FingerList extended = fingers.extended();

		std::vector<Finger::Type> types;
		for (FingerList::const_iterator fl = extended.begin(); fl != extended.end(); ++fl) {
			const Finger finger = *fl;
			types.push_back(finger.type());
		}
		numFingers = static_cast<int>(types.size());
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
		for (int i = 0; i < extendedFingers.size(); i++) {
			if (i == extendedFingers.size() - 1) {
				std::cout << extendedFingers[i];
			}
			else {
				std::cout << extendedFingers[i] << ",";
			}
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

}

void SampleListener::onFocusGained(const Controller& controller) {
	std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
	std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
	std::cout << "Device Changed" << std::endl;
	const DeviceList devices = controller.devices();

	for (int i = 0; i < devices.count(); ++i) {
		std::cout << "id: " << devices[i].toString() << std::endl;
		std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
	}
}

void SampleListener::onServiceConnect(const Controller& controller) {
	std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
	std::cout << "Service Disconnected" << std::endl;
}

std::vector<int> SampleListener::getExtendedFingers() {
	return extendedFingers;
}
std::vector<Point> SampleListener::getFingerDirections() {
	return fingerDirections;
}
std::vector<Point> SampleListener::getTipPositions() {
	return tipPositions;
}
Point SampleListener::getHandDirection() {
	return handDirection;
}
Point SampleListener::getPalmNormal() {
	return palmNormal;
}
Point SampleListener::getPalmPosition() {
	return palmPosition;
}

int SampleListener::getNumFingers() {
	return numFingers;
}