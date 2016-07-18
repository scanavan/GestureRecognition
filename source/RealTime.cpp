#include "RealTime.h"



int RealTime::depth(std::string path, int imageNum)
{
	cv::setUseOptimized(true);

	// Sensor
	IKinectSensor* pSensor;
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)) {
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return -1;
	}

	hResult = pSensor->Open();
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		return -1;
	}

	// Source
	IDepthFrameSource* pDepthSource;
	hResult = pSensor->get_DepthFrameSource(&pDepthSource);
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::get_DepthFrameSource()" << std::endl;
		return -1;
	}

	// Reader
	IDepthFrameReader* pDepthReader;
	hResult = pDepthSource->OpenReader(&pDepthReader);
	if (FAILED(hResult)) {
		std::cerr << "Error : IDepthFrameSource::OpenReader()" << std::endl;
		return -1;
	}

	// Description
	IFrameDescription* pDescription;
	hResult = pDepthSource->get_FrameDescription(&pDescription);
	if (FAILED(hResult)) {
		std::cerr << "Error : IDepthFrameSource::get_FrameDescription()" << std::endl;
		return -1;
	}

	int width = 0;
	int height = 0;
	pDescription->get_Width(&width); // 512
	pDescription->get_Height(&height); // 424
	unsigned int bufferSize = width * height * sizeof(unsigned short);

	// Range ( Range of Depth is 500-8000[mm], Range of Detection is 500-4500[mm] ) 
	unsigned short min = 0;
	unsigned short max = 0;
	pDepthSource->get_DepthMinReliableDistance(&min); // 500
	pDepthSource->get_DepthMaxReliableDistance(&max); // 4500
	std::cout << "Range : " << min << " - " << max << std::endl;

	cv::Mat bufferMat(height, width, CV_16UC1);
	cv::Mat depthMat(height, width, CV_8UC1);
	cv::Mat inverted(height, width, CV_8UC1);

	cv::namedWindow("Depth");

	while (counter) {
		// Frame
		IDepthFrame* pDepthFrame = nullptr;
		hResult = pDepthReader->AcquireLatestFrame(&pDepthFrame);
		if (SUCCEEDED(hResult)) {
			hResult = pDepthFrame->AccessUnderlyingBuffer(&bufferSize, reinterpret_cast<UINT16**>(&bufferMat.data));
			if (SUCCEEDED(hResult)) {
				bufferMat.convertTo(depthMat, CV_8U, -255.0f / 8000.0f, 255.0f);
			}
		}
		SafeRelease(pDepthFrame);
		cv::subtract(cv::Scalar::all(255), depthMat, inverted);
		cv::imshow("Depth", inverted);
		cv::waitKey(30);
		if (keyPressedDepth) {
			cv::imwrite(path + std::to_string(imageNum) + "_depth.png", inverted);
			imageNum++;
			keyPressedDepth = false;
		}
	}

	SafeRelease(pDepthSource);
	SafeRelease(pDepthReader);
	SafeRelease(pDescription);
	if (pSensor) {
		pSensor->Close();
	}
	SafeRelease(pSensor);
	cv::destroyAllWindows();


	return 0;
}

int RealTime::color(std::string path, int imageNum)
{

	

	//SafeRelease(pColorSource);
	//SafeRelease(pColorReader);
	//SafeRelease(pDescription);
	//if (pSensor) {
	//	pSensor->Close();
	//}
	//SafeRelease(pSensor);
	//cv::destroyAllWindows();

	//return 0;

	cv::setUseOptimized(true);

	// Sensor
	IKinectSensor* pSensor;
	HRESULT hResult = S_OK;
	hResult = GetDefaultKinectSensor(&pSensor);
	if (FAILED(hResult)) {
		std::cerr << "Error : GetDefaultKinectSensor" << std::endl;
		return -1;
	}

	hResult = pSensor->Open();
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::Open()" << std::endl;
		return -1;
	}

	// Source
	IColorFrameSource* pColorSource;
	hResult = pSensor->get_ColorFrameSource(&pColorSource);
	if (FAILED(hResult)) {
		std::cerr << "Error : IKinectSensor::get_ColorFrameSource()" << std::endl;
		return -1;
	}

	// Reader
	IColorFrameReader* pColorReader;
	hResult = pColorSource->OpenReader(&pColorReader);
	if (FAILED(hResult)) {
		std::cerr << "Error : IColorFrameSource::OpenReader()" << std::endl;
		return -1;
	}

	// Description
	IFrameDescription* pDescription;
	hResult = pColorSource->get_FrameDescription(&pDescription);
	if (FAILED(hResult)) {
		std::cerr << "Error : IColorFrameSource::get_FrameDescription()" << std::endl;
		return -1;
	}

	int width = 0;
	int height = 0;
	pDescription->get_Width(&width); // 1920
	pDescription->get_Height(&height); // 1080
	unsigned int bufferSize = width * height * 4 * sizeof(unsigned char);

	cv::Mat bufferMat(height, width, CV_8UC4);
	cv::Mat colorMat(height / 2, width / 2, CV_8UC4);
	cv::namedWindow("Color");

	while (counter) {
		// Frame
		IColorFrame* pColorFrame = nullptr;
		hResult = pColorReader->AcquireLatestFrame(&pColorFrame);
		if (SUCCEEDED(hResult)) {
			hResult = pColorFrame->CopyConvertedFrameDataToArray(bufferSize, reinterpret_cast<BYTE*>(bufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);
			if (SUCCEEDED(hResult)) {
				cv::resize(bufferMat, colorMat, cv::Size(), 0.5, 0.5);
			}
		}
		SafeRelease(pColorFrame);

		cv::imshow("Color", colorMat);
		cv::waitKey(30);
		if (keyPressedColor) {
			cv::imwrite(path + std::to_string(imageNum) + "_color.png", colorMat);
			imageNum++;
			keyPressedColor = false;
		}
	}


	SafeRelease(pColorSource);
	SafeRelease(pColorReader);
	SafeRelease(pDescription);
	if (pSensor) {
		pSensor->Close();
	}
	SafeRelease(pSensor);
	cv::destroyAllWindows();

	return 0;


}

int RealTime::leapInfo(std::string path, int imageNum) {
	//SampleListener listener;
	Controller controller;
	//controller.addListener(listener);
	std::vector<int>extendedFingers;
	std::vector<Point>fingerDirections;
	std::vector<Point>tipPositions;
	std::vector<Point>ExtendedTipPositions;
	Point handDirection;
	Point palmNormal;
	Point palmPosition;
	int numFingers;

	// Keep this process running until Enter is pressed
	
	extendedFingers = { 0,0,0,0,0 };

	
	while (counter) {
		const Frame frame = controller.frame();
		HandList hands = frame.hands();
		for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
			// Get the first hand
			const Hand hand = *hl;
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
				const Bone distal = finger.bone(Bone::TYPE_DISTAL);
				const Vector tip = distal.nextJoint();
				ExtendedTipPositions.push_back(Point(tip.x, tip.y, tip.z));
			}
			for (int i = ExtendedTipPositions.size(); i < 5; ++i)
			{
				ExtendedTipPositions.push_back(Point(0.000000, 0.000000, 0.000000));
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
		}

		cv::waitKey(30);
		if (keyPressedLeap) {
			//write the file
			std::ofstream file;
			file.open(path + std::to_string(imageNum) + "_leap_motion.csv");
			file << "extendedFingers";
			for (int i = 0; i < extendedFingers.size(); i++) {
				file << ',' + std::to_string(extendedFingers[i]);
			}
			file << "\nfingerDirections";
			for (int i = 0; i < fingerDirections.size(); i++) {
				file << ',' + fingerDirections[i].writePoint();
			}
			file << "\ntipPositions";
			for (int i = 0; i < tipPositions.size(); i++) {
				file << ',' + tipPositions[i].writePoint();
			}
			file << "\nExtendedTipPositions";
			for (int i = 0; i < ExtendedTipPositions.size(); i++) {
				file << ',' + ExtendedTipPositions[i].writePoint();
			}
			file << "\nHandDirection," + handDirection.writePoint();
			file << "\npalmNormal," + palmNormal.writePoint();
			file << "\npalmPosition," + palmPosition.writePoint();
			file << "\nnumFingers," + std::to_string(numFingers);
			file.close();
			keyPressedLeap = false;
			imageNum++;
		}
		fingerDirections.clear();
		tipPositions.clear();
		ExtendedTipPositions.clear();
	}
	

}

void RealTime::setKeyPressed(bool val) {
	keyPressedDepth = val;
	keyPressedColor = val;
	keyPressedLeap = val;
}

void RealTime::endProgram() {
	counter = false;
}