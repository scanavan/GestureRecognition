#include "LeapCamera.h"


LeapCamera::LeapCamera(bool showHandInformation, bool useOpenCVWindow) :
	  showHandInfo(showHandInformation)
{
	fingerNames.emplace_back("Thumb");
	fingerNames.emplace_back("Index");
	fingerNames.emplace_back("Middle");
	fingerNames.emplace_back("Ring");
	fingerNames.emplace_back("Pinky");

	boneNames.emplace_back("Metacarpal");
	boneNames.emplace_back("Proximal");
	boneNames.emplace_back("Middle");
	boneNames.emplace_back("Distal");

	stateNames.emplace_back("STATE_INVALID");
	stateNames.emplace_back("STATE_START");
	stateNames.emplace_back("STATE_UPDATE");
	stateNames.emplace_back("STATE_END");

	if (useOpenCVWindow)
	{
		capture = cv::VideoCapture(0);
		if (!capture.isOpened())
		{
			throw std::runtime_error("Failed to open camera.");
		}
	}
	Initialize();

}
void LeapCamera::Initialize()
{
	controller.setPolicy(Leap::Controller::POLICY_IMAGES);
	controller.enableGesture(Leap::Gesture::TYPE_CIRCLE);
	controller.enableGesture(Leap::Gesture::TYPE_KEY_TAP);
	controller.enableGesture(Leap::Gesture::TYPE_SCREEN_TAP);
	controller.enableGesture(Leap::Gesture::TYPE_SWIPE);
}
void LeapCamera::Run()
{
	while (!quit)
	{
		LeapGetImage();
		if (showHandInfo)
		{
			PrintHandInformation();
		}
		else
		{
			PopulateHandData();
		}
		int key = cv::waitKey(33);
		if (key == 101)
		{
			Exit();
		}
		std::cout << "There are " << leapData.extendedFingers.size() << " finger(s) extended in the current frame" << std::endl;
		for (auto &finger : leapData.extendedFingers)
		{
			std::cout << GetLeapFingerName(finger) << std::endl;
		}
		//remove current extended fingers
		//only saving per frame...
		leapData.extendedFingers.clear();
	}
}
std::string LeapCamera::GetLeapFingerName(Leap::Finger::Type type)
{
	std::string finger("Thumb");
	switch(type)
	{
	case 1:
		finger = "Index";
		break;
	case 2:
		finger = "Middle";
		break;
	case 3:
		finger = "Ring";
		break;
	case 4:
		finger = "Pinky";
		break;
	}
	return finger;
}
void LeapCamera::PopulateHandData()
{
	//get frame from leap
	leapFrame = controller.frame();
	//get hands
	hands = leapFrame.hands();
	for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
	{
		// Get the hand
		const Leap::Hand hand = *hl;
		const Leap::FingerList fingers = hand.fingers().extended();
		for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl)
		{
			const Leap::Finger finger = *fl;
			//std::cout << GetLeapFingerName(finger.type()) << std::endl;
			leapData.extendedFingers.emplace_back(finger.type());
		}
	}
}
//return the type of finger that leap is finding
Leap::Finger::Type LeapCamera::GetFingerType()
{
	return Leap::Finger::Type();
}
void LeapCamera::LeapGetImage()
{
	// Get the most recent frame and report some basic information
	const Leap::Frame frame = controller.frame();
	Leap::ImageList images = frame.images();
	Leap::Image image = images[0];
	if (!image.isValid())
	{
		return;
	}
	const unsigned char* buffer = image.data();
	cv::Mat result;
	result.create(image.height(), image.width(), CV_8UC1);
	memcpy(result.data, buffer, sizeof(uchar)*image.height()*image.width()*image.bytesPerPixel());
	cv::Mat flipped;
	cv::flip(result, flipped, 1);
	cv::namedWindow("Leap");
	cv::imshow("Leap", flipped);
}
void LeapCamera::OpenCVGetImage()
{
	capture >> frame;
	cv::namedWindow("OpenCV");
	cv::imshow("OpenCV", frame);
}
void LeapCamera::PrintHandInformation()
{
	leapFrame = controller.frame();

	std::cout << "leapFrame id: " << leapFrame.id()
		<< ", timestamp: " << leapFrame.timestamp()
		<< ", hands: " << leapFrame.hands().count()
		<< ", extended fingers: " << leapFrame.fingers().extended().count()
		<< ", tools: " << leapFrame.tools().count()
		<< ", gestures: " << leapFrame.gestures().count() << std::endl;
	
	hands = leapFrame.hands();
	for (Leap::HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl)
	{
		// Get the hand
		const Leap::Hand hand = *hl;
		std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
		std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
			<< ", palm position: " << hand.palmPosition() << std::endl;
		// Get the hand's normal vector and direction
		const Leap::Vector normal = hand.palmNormal();
		const Leap::Vector direction = hand.direction();

		// Calculate the hand's pitch, roll, and yaw angles
		std::cout << std::string(2, ' ') << "pitch: " << direction.pitch() * Leap::RAD_TO_DEG << " degrees, "
			<< "roll: " << normal.roll() * Leap::RAD_TO_DEG << " degrees, "
			<< "yaw: " << direction.yaw() * Leap::RAD_TO_DEG << " degrees" << std::endl;

		// Get the Arm bone
		Leap::Arm arm = hand.arm();
		std::cout << std::string(2, ' ') << "Arm direction: " << arm.direction()
			<< " wrist position: " << arm.wristPosition()
			<< " elbow position: " << arm.elbowPosition() << std::endl;

		// Get fingers
		const Leap::FingerList fingers = hand.fingers();
		for (Leap::FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
			const Leap::Finger finger = *fl;
			std::cout << std::string(4, ' ') << fingerNames[finger.type()]
				<< " finger, id: " << finger.id()
				<< ", length: " << finger.length()
				<< "mm, width: " << finger.width() << std::endl;

			// Get finger bones
			for (int b = 0; b < 4; ++b) {
				Leap::Bone::Type boneType = static_cast<Leap::Bone::Type>(b);
				Leap::Bone bone = finger.bone(boneType);
				std::cout << std::string(6, ' ') << boneNames[boneType]
					<< " bone, start: " << bone.prevJoint()
					<< ", end: " << bone.nextJoint()
					<< ", direction: " << bone.direction() << std::endl;
			}
		}
	}

	// Get tools
	const Leap::ToolList tools = leapFrame.tools();
	for (Leap::ToolList::const_iterator tl = tools.begin(); tl != tools.end(); ++tl) {
		const Leap::Tool tool = *tl;
		std::cout << std::string(2, ' ') << "Tool, id: " << tool.id()
			<< ", position: " << tool.tipPosition()
			<< ", direction: " << tool.direction() << std::endl;
	}

	// Get gestures
	const Leap::GestureList gestures = leapFrame.gestures();
	for (int g = 0; g < gestures.count(); ++g) {
		Leap::Gesture gesture = gestures[g];

		switch (gesture.type()) {
		case Leap::Gesture::TYPE_CIRCLE:
		{
			Leap::CircleGesture circle = gesture;
			std::string clockwiseness;

			if (circle.pointable().direction().angleTo(circle.normal()) <= Leap::PI / 2) {
				clockwiseness = "clockwise";
			}
			else {
				clockwiseness = "counterclockwise";
			}

			// Calculate angle swept since last leapFrame
			float sweptAngle = 0;
			if (circle.state() != Leap::Gesture::STATE_START) {
				Leap::CircleGesture previousUpdate = Leap::CircleGesture(controller.frame(1).gesture(circle.id()));
				sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * Leap::PI;
			}
			std::cout << std::string(2, ' ')
				<< "Circle id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", progress: " << circle.progress()
				<< ", radius: " << circle.radius()
				<< ", angle " << sweptAngle * Leap::RAD_TO_DEG
				<< ", " << clockwiseness << std::endl;
			break;
		}
		case Leap::Gesture::TYPE_SWIPE:
		{
			Leap::SwipeGesture swipe = gesture;
			std::cout << std::string(2, ' ')
				<< "Swipe id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", direction: " << swipe.direction()
				<< ", speed: " << swipe.speed() << std::endl;
			break;
		}
		case Leap::Gesture::TYPE_KEY_TAP:
		{
			Leap::KeyTapGesture tap = gesture;
			std::cout << std::string(2, ' ')
				<< "Key Tap id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", position: " << tap.position()
				<< ", direction: " << tap.direction() << std::endl;
			break;
		}
		case Leap::Gesture::TYPE_SCREEN_TAP:
		{
			Leap::ScreenTapGesture screentap = gesture;
			std::cout << std::string(2, ' ')
				<< "Screen Tap id: " << gesture.id()
				<< ", state: " << stateNames[gesture.state()]
				<< ", position: " << screentap.position()
				<< ", direction: " << screentap.direction() << std::endl;
			break;
		}
		default:
			std::cout << std::string(2, ' ') << "Unknown gesture type." << std::endl;
			break;
		}
	}

	if (!leapFrame.hands().isEmpty() || !gestures.isEmpty()) {
		std::cout << std::endl;
	}

}
