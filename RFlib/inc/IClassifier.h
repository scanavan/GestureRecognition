/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/inc/IClassifier.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#ifndef ICLASSIFIERS_H
#define ICLASSIFIERS_H

#include "Histogram.h"
class IFeatureVector;

class IClassifier
{
public:
    //! Constructor
    IClassifier(unsigned int nbLabels = 2, bool occurence_normalization = false);

    //! Copy constructor
    IClassifier(const IClassifier& other);

    //! Destructor
    virtual ~IClassifier();

public:
    //! virtual method to train a classifier from a patch 
    virtual void train(const IFeatureVector& vector, unsigned int label)=0;
    
    //! Estimate posterior probability of a patch
	virtual Histogram estimatePosterior(const IFeatureVector& vector) const = 0;

    //! virtual method to load a classifier from a file
    virtual void load(const std::string& filename) = 0;

    //! virtual method to save a classifier in a file
    virtual void save(const std::string& filename) = 0;

    //! Clone the classifier
    virtual IClassifier* clone() const =0;

    //! Clean the classifier, so that it forgets whatever it has learnt before
    virtual void reset() = 0;

public:
	// hard classification of a patch : argmax
    unsigned int classify(const IFeatureVector& vector) const;

    //! Get the number of label the classifier handles
    unsigned int getNbLabels() const;

    //! set the occurence normalization
    void setOccurenceNormalization(bool on);

protected:
    //! Number of considered classes
    unsigned int m_nb_labels;

    //! Keep track of the number of encountered label during training. Can be used to balance the training data
    Histogram m_occurences;

    //! Use occurence normalization while testing ?
    bool m_occurence_normalization;
};

#endif //ICLASSIFIERS_H
