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
	int color();
	int depth();
	void leapInfo();
	void setKeyPressed(bool val);
	void endProgram();
	void setPath(std::string path);
	void changeGesture();
	std::atomic<int> imageNum = 0;
private:
	std::atomic<bool> keyPressedDepth = false;
	std::atomic<bool> keyPressedColor = false;
	std::atomic<bool> keyPressedLeap = false;
	std::atomic<bool> counter = true;
	std::string path;
	std::atomic<int> gestureNum = 1;
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

