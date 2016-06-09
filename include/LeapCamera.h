#ifndef LEAP_CAMERA_H
#define LEAP_CAMERA_H

#include <string>
#include <vector>
#include "Leap.h"
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include "RealTimeLeapData.h"

class LeapCamera : public Camera
{
public:
	//Constructor to allow showing of hand info(text out), and whether we want to use a window.
	LeapCamera(bool showHandInfo = false, bool useOpenCVWindow = true);
	//main loop for Leap camera
	void Run() override;
	//fingers
	Leap::Finger::Type GetFingerType();
private:
	//human readable version of finger name
	std::string GetLeapFingerName(Leap::Finger::Type type);

	//names of finger types
	std::vector<std::string> fingerNames;
	//names of bone types
	std::vector<std::string> boneNames;
	//states that the leap can be in
	std::vector<std::string> stateNames;
	//current real Leap Frame
	Leap::Frame leapFrame;
	//list of hands from leap
	Leap::HandList hands;
	//actual Leap camera - defined by a controller
	Leap::Controller controller;

	//get an image from the leap
	void LeapGetImage();
	//use opencv to get an image from the leap from a stream
	void OpenCVGetImage();
	//Gets hand information from the leap and outputs to console
	void PrintHandInformation();
	//read data from Leap and populate class data
	void PopulateHandData();
	//initialize the Leap - set policies, enable properties, etc.
	void Initialize() override;

	//video capture from opencv
	cv::VideoCapture capture;
	//current frame we are on
	cv::Mat frame;

	//do we want to show hand information?
	bool showHandInfo;

	RealTimeLeapData leapData;
};
#endif
