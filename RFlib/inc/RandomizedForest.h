/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/inc/RandomizedForest.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief	 This class implement a classifier based on Randomized Forest
 */

#ifndef RANDOMIZEDFOREST_H
#define RANDOMIZEDFOREST_H

#include "IClassifier.h"

class RandomizedTree;

class RandomizedForest : public IClassifier
{
public:
	//! Default constructor
	RandomizedForest();

    //! Constructor
    RandomizedForest(unsigned int nbLabels, bool occurence_normalization, 
					 unsigned int depth, unsigned int nb_trees, 
					 unsigned int vector_size, double minValue, double maxValue);

    //! Copy constructor
    RandomizedForest(const RandomizedForest& other);

	//! assignement operator
	RandomizedForest& operator=(const RandomizedForest& other);

    //! Destructor
    ~RandomizedForest();

public:
	//! training
    void train(const IFeatureVector& vector, unsigned int label);

	//! testing
	Histogram estimatePosterior(const IFeatureVector& vector) const;

	// load from file
    void load(const std::string& filename);

    //! save in a file
    void save(const std::string& filename);

    //! Clone the classifier
    IClassifier* clone() const;

    //! Clean the classifier, so that it forgets whatever it has learnt before
    void reset();

public:
    //! Get the number of classes
    unsigned int getNbClasses() const;

    //! get the size of the considered square patches
    unsigned int getVectorSize() const;

    //! Get the number of tree in the forest
    unsigned int getNbTrees() const;

    //! Get the depth of the forest
    unsigned int getDepth() const;

	//! get min value expected in the feature vectors
	double getMinValue() const;

	//! get max value expected in the feature vectors
	double getMaxValue() const;

    //! I/O for randomized forest
    friend std::ostream& operator<<(std::ostream& out, RandomizedForest& forest);
    friend std::istream& operator>>(std::istream& in, RandomizedForest& forest);

private:
    //! clean the forest
    void clean();

    //! give ids to the nodes of the forest
    void annotate();

private:
    unsigned int m_depth;
    unsigned int m_nb_trees;
    unsigned int m_vector_size;
	double m_min_value;
	double m_max_value;

    std::vector<RandomizedTree*> m_trees;
};

#endif //RANDOMIZEDFOREST_H
