/*
 *
 */

#ifndef FEATURE_H
#define FEATURE_H

#include <iostream>
#include <vector>
#include "IFeatureVector.h"
#include "../../include/LeapData.h"
#include "../../include/KinectMotion.h"

class GestureVector : public IFeatureVector
{
public:	
	GestureVector(std::vector<float> data, unsigned int lab);
	GestureVector(const GestureVector *copy);
	std::vector<float> getFeatures();
	unsigned int getLabel();
	IFeatureVector* clone() const;
	double operator[](unsigned int k) const;
	unsigned int size() const;
	
private:
	std::vector<float> features;
	unsigned int label;
};

#endif