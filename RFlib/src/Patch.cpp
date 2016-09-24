/**
 * @file     $URL: svn+ssh://teboul@athena/home/svnrepositories/BuildingModeling/branches/Olivier/Vision/src/Patch.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:38 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#include <sstream>
#include "../inc/Patch.h"

Patch::Patch(IplImage *image, int x, int y, int w, int h):
m_x(x),m_y(y),m_w(w),m_h(h)
{
	m_image = image;
}

Patch::Patch(IplImage* image, int x, int y, int d)
:m_x(x),m_y(y),m_w(d),m_h(d)
{
	m_image = image;
}

Patch::Patch(const Patch &other):m_x(other.m_x),m_y(other.m_y),m_w(other.m_w),m_h(other.m_h)
{
	m_image = other.m_image;
}

Patch::~Patch()
{
	m_image = 0;
}

IFeatureVector* 
Patch::clone() const
{
	return new Patch(*this);
}

double 
Patch::operator[](unsigned int k) const
{
	int i = k%m_w;
	int j = ( (k-i)/m_w )%m_h;
	int c = ((k-i)/m_w -j)/m_h;

	return getValue(i,j,c);
}

unsigned int 
Patch::size() const
{
	return 3*m_w*m_h;
}

std::pair<int, int>
Patch::getSize() const
{
    return std::pair<int, int>(m_w,m_h);
}

double 
Patch::getValue(int i, int j, int c) const
{
	int w = m_w/2;
    int h = m_h/2;
	int x = m_x + i - w;
	int y = m_y + j - h;

	double val = cvGet2D(m_image,y,x).val[c];
	return val;
}

int
Patch::getX() const
{
    return m_x;
}
    
int
Patch::getY() const
{
    return m_y;
}
    
IplImage*
Patch::getImage() const
{
    return m_image;
}

void 
Patch::setImage(IplImage* image)
{
	m_image = image;
}

bool 
Patch::isValid() const
{
	return (m_x-m_w/2>0) && (m_y-m_h/2>0) && (m_x+m_w/2<m_image->width) && (m_y+m_h/2<m_image->height);
}

void 
Patch::toImage(IplImage* dst) const
{
	cvSetImageROI(m_image,cvRect(m_x-m_w/2,m_y-m_h/2,m_w,m_h));
	cvResize(m_image,dst,CV_INTER_NN);
	cvResetImageROI(m_image);
}

void 
Patch::saveAsImage(const char* filename, int scale) const
{
	IplImage* bigPatch = cvCreateImage(cvSize(m_w*scale,m_h*scale),m_image->depth,m_image->nChannels);
	toImage(bigPatch);
	cvSaveImage(filename,bigPatch);
	cvReleaseImage(&bigPatch);
}

Histogram
Patch::histogram(int channel) const
{
	Histogram h(256);
	for(int i=-m_w/2; i<=m_w/2; i++)
	{
		for(int j=-m_h/2; j<=m_h/2; j++)
		{
			int val = static_cast<int>(cvGet2D(m_image,m_y+j,m_x+i).val[0]+0.5);
			h[val]++;
		}
	}
	return h;
}
