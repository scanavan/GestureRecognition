/**
 * @file     $URL: svn+ssh://teboul@athena/home/svnrepositories/BuildingModeling/branches/Olivier/Vision/inc/Patch.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-28 12:04:56 +0100 (mer., 28 oct. 2009) $
 * @brief
 */

#ifndef PATCH_H
#define PATCH_H

#include <iostream>
//#include <highgui.h>
//#include <cv.h>
#include <opencv2/opencv.hpp>
#include "Histogram.h"

#include "IFeatureVector.h"

class Patch : public IFeatureVector
{
public:
	//! Constructor for rectangular patch
    Patch(IplImage* image, int x, int y, int w, int h);

	//! Constructor for a square patch
	Patch(IplImage* image, int x, int y, int d);

	//! Copy constructor
	Patch(const Patch& other);

	//! Destructor
	~Patch();

	// inherited method from IFeatureVector
	IFeatureVector* clone() const;
	double operator[](unsigned int k) const;
	unsigned int size() const;

	//! Get value of the patch at location i,j and for channel c
	double getValue(int i, int j, int c) const;

	//! Get the geometry of the patch
	std::pair<int, int> getSize() const;
    
	//! Get the x position of the patch
	int getX() const;
    
	//! Get the y position of the patch
	int getY() const;
    
	//! Get the image of the patch
	IplImage* getImage() const;

	//! Set the image
	void setImage(IplImage* image);

	//! Check if a patch is totally included into its image
	bool isValid() const;

	//! convert the patch to an image (with given size)
	void toImage(IplImage* dst) const;

	//! Save a patch with big scale into an image
	void saveAsImage(const char* filename, int scale=20) const;

	//! compute the color histogram of given channel (0=R)
	Histogram histogram(int channel=0) const;

private:
	//! Geometry of the patch
	int m_x;
	int m_y;
	int m_w;
	int m_h;

	// patch is 3 channels... should be specified somewhere

	//! Image the patch belongs to (not owned by the class patch, must be deleted somewhere else
	IplImage* m_image;
};

#endif //PATCH_H
