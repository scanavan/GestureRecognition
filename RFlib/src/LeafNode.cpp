/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/src/LeafNode.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#include "../inc/Histogram.h"
#include "../inc/LeafNode.h"

LeafNode::LeafNode(unsigned int nbLabels):RandomizedTree(0,nbLabels,1)
{
	m_histogram = new Histogram(m_nb_classes);
}

LeafNode::LeafNode(const LeafNode& other):RandomizedTree(other)
{
	m_histogram = new Histogram(*other.m_histogram);
}

LeafNode::~LeafNode()
{
    delete m_histogram;
}

void 
LeafNode::train(const IFeatureVector& vector, int label, int d)
{
	(*m_histogram)[label]++;
}

const Histogram& 
LeafNode::getHistogram() const
{
	return *m_histogram;
}

bool 
LeafNode::isLeaf() const
{
    return true;
}

void 
LeafNode::copy(RandomizedTree* other)
{
    m_depth = other->getDepth();
    m_nb_classes = other->getNbClasses();
    m_vector_size= other->getVectorSize();
    m_id = other->getID();

    LeafNode* otherLeaf = dynamic_cast<LeafNode*>(other);
    if(otherLeaf)
    {
        *m_histogram = *otherLeaf->m_histogram;
    }

    else
    {
        //throw std::exception("bad dynamic_cast");
    }
}
const double& 
LeafNode::operator[](unsigned int i) const
{
	return (*m_histogram)[i];
}

double& 
LeafNode::operator[](unsigned int i)
{
	return (*m_histogram)[i];
}
