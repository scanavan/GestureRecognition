#ifndef KINECT_CAMERA_H
#define KINECT_CAMERA_H
#include "Camera.h"
#include <Kinect.h>
#include <opencv2/opencv.hpp>

class KinectCamera : public Camera
{
public:
	KinectCamera();
	void Run() override;

private:
	struct Image
	{
		Image() : width(0), height(0), bufferSize(0), description(nullptr) {}
		int width;
		int height;
		unsigned bufferSize;
		IFrameDescription* description;
		cv::Mat bufferMat;
		cv::Mat colorMat;
	};
	void Initialize() override;
	void InitializeColor();
	void InitializeDepth();
	void GetColorDescription();
	void GetDepthDescription();
	void ProcessColorData();
	void ProcessDepthData();

	IKinectSensor* sensor;

	IColorFrameReader* reader;
	IDepthFrameReader* depthReader;

	Image* color;
	Image* depth;

	std::string colorWindowTitle;
	std::string depthWindowTitle;
};
#endif
