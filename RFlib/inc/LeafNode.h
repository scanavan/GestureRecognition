/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/inc/LeafNode.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#ifndef LEAFNODE_H
#define LEAFNODE_H

#include "RandomizedTree.h"

class Histogram;

class LeafNode : public RandomizedTree
{
public:
	//! Constructor
	LeafNode(unsigned int nbLabels);

	//! Copy Constructor
	LeafNode(const LeafNode& other);

	//!Copy a leaf node into another one (cast)
	void copy(RandomizedTree* other);

	//! Destructor
	~LeafNode();

	//! A leaf is... a leaf of course, what do you think O_o ?
	bool isLeaf() const;

	//! Get the histogram
	const Histogram& getHistogram() const;

	//! Accessor directly to the ith bin of the histogram
	const double& operator[](unsigned int i) const;

	//! the same version, but that allows to modify the ith bin
	double& operator[](unsigned int i);

private:
	// overload method from parent class
	void train(const IFeatureVector& vector, int label, int d);

private:
	Histogram* m_histogram;
};

#endif //LEAFNODE_H
