#include <fstream>
#include <sstream>
#include "../inc/GestureVector.h"

GestureVector::GestureVector(std::vector<float> data, unsigned int lab)
{
	features = data;
	label = lab;
}

GestureVector::GestureVector(const GestureVector *copy)
{
	label = copy->label;
	features = copy->features;
}
std::vector<float> GestureVector::getFeatures()
{
	return features;
}

unsigned int GestureVector::getLabel()
{
	return label;
}

IFeatureVector*
GestureVector::clone() const
{
	return &GestureVector(*this);
}

double GestureVector::operator[](unsigned int k) const
{
	return features[k];
}

unsigned int GestureVector::size() const
{
	return static_cast<unsigned>(features.size());
}