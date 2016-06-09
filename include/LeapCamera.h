#ifndef LEAP_CAMERA_H
#define LEAP_CAMERA_H

#include <string>
#include <vector>
#include "Leap.h"
#include <opencv2/opencv.hpp>
#include "Camera.h"

class LeapCamera : public Camera
{
public:
	LeapCamera(bool showHandInfo = false, bool useOpenCVWindow = true);
	void Run() override;

private:
	std::vector<std::string> fingerNames;
	std::vector<std::string> boneNames;
	std::vector<std::string> stateNames;

	void LeapGetImage();
	void OpenCVGetImage();
	void GetHandInformation();

	cv::VideoCapture capture;
	cv::Mat frame;

	bool showHandInfo;

	Leap::Controller controller;

	void Initialize() override;
};
#endif
