#include "KinectCamera.h"
#include <iostream>
#include <opencv2/opencv.hpp>

KinectCamera::KinectCamera() :
	  color(nullptr)
	, depth(nullptr)
	, sensor(nullptr)
	, reader(nullptr)
	, colorWindowTitle("Kinect Color")
	, depthWindowTitle("Kinect Depth")
{
	color = new Image;
	depth = new Image;
	Initialize();
}
void KinectCamera::InitializeColor()
{
	IColorFrameSource* pColorSource = nullptr;
	HRESULT hr = sensor->get_ColorFrameSource(&pColorSource);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to get Kienct color source.");
	}
	hr = pColorSource->OpenReader(&reader);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to open Kinect reader.");
	}
	hr = pColorSource->get_FrameDescription(&color->description);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to get color frame description.");
	}
	if (pColorSource != nullptr)
	{
		pColorSource->Release();
		pColorSource = nullptr;
	}
}

void KinectCamera::InitializeDepth()
{
	IDepthFrameSource* depthSource = nullptr;
	HRESULT hr = sensor->get_DepthFrameSource(&depthSource);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to get depth source.");
	}
	hr = depthSource->OpenReader(&depthReader);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to open Kinect reader.");
	}
	hr = depthSource->get_FrameDescription(&depth->description);
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to get depth frame description.");
	}
	if (depthSource != nullptr)
	{
		depthSource->Release();
		depthSource = nullptr;
	}
}
void KinectCamera::Initialize()
{
	HRESULT hr = GetDefaultKinectSensor(&sensor);
	if (FAILED(hr) || sensor == nullptr)
	{
		throw std::runtime_error("Failed to connect to Kinect sensor.");
	}
	hr = sensor->Open();
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to open Kinect.");
	}
	InitializeColor();
	InitializeDepth();
}

void KinectCamera::GetColorDescription()
{
	color->description->get_Width(&color->width);
	color->description->get_Height(&color->height);

	color->bufferSize = color->width * color->height * 4 * sizeof(unsigned char);

	color->bufferMat = cv::Mat(color->height, color->width, CV_8UC4);
	color->colorMat = cv::Mat(color->height / 2, color->width / 2, CV_8UC4);
}

void KinectCamera::GetDepthDescription()
{
	depth->description->get_Width(&depth->width);
	depth->description->get_Height(&depth->height);

	depth->bufferSize = depth->width * depth->height * sizeof(unsigned short);

	depth->bufferMat = cv::Mat(depth->height, depth->width, CV_16UC1);
	depth->colorMat = cv::Mat(depth->height, depth->width, CV_8UC1);
}

void KinectCamera::ProcessColorData()
{
	// Frame
	IColorFrame* pColorFrame = nullptr;
	HRESULT hr = reader->AcquireLatestFrame(&pColorFrame);
	if (SUCCEEDED(hr))
	{
		hr = pColorFrame->CopyConvertedFrameDataToArray(color->bufferSize, reinterpret_cast<BYTE*>(color->bufferMat.data), ColorImageFormat::ColorImageFormat_Bgra);
		if (SUCCEEDED(hr))
		{
			cv::resize(color->bufferMat, color->colorMat, cv::Size(), 0.5, 0.5);
		}
	}
	if (pColorFrame != nullptr)
	{
		pColorFrame->Release();
		pColorFrame = nullptr;
	}
}

void KinectCamera::ProcessDepthData()
{
	// Frame
	IDepthFrame* pDepthFrame = nullptr;
	HRESULT hr = depthReader->AcquireLatestFrame(&pDepthFrame);
	if (SUCCEEDED(hr))
	{
		pDepthFrame->AccessUnderlyingBuffer(&depth->bufferSize, reinterpret_cast<UINT16**>(&depth->bufferMat.data));
		if (SUCCEEDED(hr))
		{
			depth->bufferMat.convertTo(depth->colorMat, CV_8U, -255.f / 8000.f, 255.f);
		}
	}
	if (pDepthFrame != nullptr)
	{
		pDepthFrame->Release();
		pDepthFrame = nullptr;
	}
}

void KinectCamera::Run()
{
	GetColorDescription();
	GetDepthDescription();
	cv::namedWindow(colorWindowTitle);
	cv::namedWindow(depthWindowTitle);

	while (!quit)
	{
		ProcessColorData();
		ProcessDepthData();
		cv::imshow(colorWindowTitle, color->colorMat);
		cv::imshow(depthWindowTitle, depth->colorMat);
		int key = cv::waitKey(33);
		if (key == 101)
		{
			Exit();
		}
	}
}