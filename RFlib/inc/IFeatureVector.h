/**
 * @file     $URL: svn+ssh://teboul@athena/home/svnrepositories/BuildingModeling/branches/Olivier/Vision/inc/Patch.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-28 12:04:56 +0100 (mer., 28 oct. 2009) $
 * @brief
 */

#ifndef IFEATURE_VECTOR_H
#define IFEATURE_VECTOR_H

#include <istream>
#include <string>

class IFeatureVector
{
public:
	IFeatureVector();
	IFeatureVector(const IFeatureVector& other);
	virtual ~IFeatureVector();
	
	virtual IFeatureVector* clone() const = 0;
	virtual double operator[](unsigned int k) const = 0;
	virtual unsigned int size() const = 0;
};

#endif //PATCH_H
