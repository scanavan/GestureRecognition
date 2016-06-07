#include <iostream>
#include "KinectMotion.h"
#include <opencv\cv.h>
#include "Image.h"

KinectMotion::KinectMotion(std::string iDepth, std::string iRgb):depth(Image(iDepth)),rgb(Image(iRgb)) {

}

Image KinectMotion::getDepth() {
	return depth;
}

Image KinectMotion::getRgb() {
	return rgb;
}