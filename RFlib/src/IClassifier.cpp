/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/src/IClassifier.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#include <ctime>
#include "../inc/IFeatureVector.h"
#include "../inc/IClassifier.h"

IClassifier::IClassifier(unsigned int nbLabels, bool occurence_normalization):
m_nb_labels(nbLabels),m_occurences(nbLabels),m_occurence_normalization(occurence_normalization)
{
	m_occurences.fill(1);
	srand( static_cast<unsigned int>( time(0) ));
}

IClassifier::IClassifier(const IClassifier& other):
m_nb_labels(other.m_nb_labels),
m_occurences(other.m_occurences),
m_occurence_normalization(other.m_occurence_normalization)
{
}

IClassifier::~IClassifier()
{
}

unsigned int 
IClassifier::classify(const IFeatureVector& vector) const
{
	Histogram probas = estimatePosterior(vector);
	return probas.argMax();
}

unsigned int 
IClassifier::getNbLabels() const
{
	return m_nb_labels;
}

void 
IClassifier::setOccurenceNormalization(bool on)
{
	m_occurence_normalization = on;
}
