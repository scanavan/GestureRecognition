#ifndef REALTIME_H
#define REALTIME_H
#include <opencv2/opencv.hpp>
#include <string>
#include <Windows.h>
#include <Kinect.h>
#include <atomic>
#include <vector>
#include "Leap.h"
#include "Point.h"
#include "SampleListener.h"
#include <fstream>


class RealTime {
public:
	int color(std::string path, int imageNum);
	int depth(std::string path, int imageNum);
	int leapInfo(std::string path, int imageNum);
	void setKeyPressed(bool val);
	void endProgram();
private:
	std::atomic<bool> keyPressedDepth = false;
	std::atomic<bool> keyPressedColor = false;
	std::atomic<bool> keyPressedLeap = false;
	std::atomic<bool> counter = true;
	/*IKinectSensor* pSensor;
	IDepthFrameSource* pDepthSource;
	IColorFrameSource* pColorSource;
	IDepthFrameReader* pDepthReader;
	IColorFrameReader* pColorReader;
	IFrameDescription* pDescription;
	HRESULT chResult;
	HRESULT hResult;*/

	template<class Interface>
	inline void SafeRelease(Interface *& pInterfaceToRelease)
	{
		if (pInterfaceToRelease != NULL) {
			pInterfaceToRelease->Release();
			pInterfaceToRelease = NULL;
		}
	}
};



#endif // !REALTIME_H

