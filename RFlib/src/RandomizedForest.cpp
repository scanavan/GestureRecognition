/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/src/RandomizedForest.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "../inc/Histogram.h"
#include "../inc/RandomizedTree.h"
#include "../inc/LeafNode.h"

#include "../inc/RandomizedForest.h"


//============================================================================================================================//
RandomizedForest::RandomizedForest() {}

RandomizedForest::RandomizedForest(unsigned int nbLabels, 
								   bool occurence_normalization, 
								   unsigned int depth, 
								   unsigned int nb_trees, 
								   unsigned int vector_size,
								   double minV, double maxV)
:IClassifier(nbLabels,occurence_normalization),
m_depth(depth),m_nb_trees(nb_trees),m_vector_size(vector_size),m_min_value(minV),m_max_value(maxV)
{
    for(unsigned int i=0;i<nb_trees;i++)
    {
        m_trees.push_back(new RandomizedTree(m_depth,m_nb_labels,m_vector_size,m_min_value,m_max_value));
    }
}

RandomizedForest::RandomizedForest(const RandomizedForest& other)
:IClassifier(other),
m_depth(other.m_depth),m_nb_trees(other.m_nb_trees),
m_vector_size(other.m_vector_size),m_min_value(other.m_min_value),m_max_value(other.m_max_value)
{
    for(unsigned int t=0; t<other.m_nb_trees; t++)
    {
		m_trees.push_back(new RandomizedTree(*other.m_trees[t]));
    }
}

RandomizedForest& 
RandomizedForest::operator=(const RandomizedForest& other)
{
	m_depth						= other.m_depth;
	m_nb_trees					= other.m_nb_trees;
	m_vector_size				= other.m_vector_size;
    m_nb_labels					= other.m_nb_labels;
	m_min_value					= other.m_min_value;
	m_max_value					= other.m_max_value;
	m_occurences				= other.m_occurences;
	m_occurence_normalization	= other.m_occurence_normalization;
	
	for(unsigned int t=0; t<other.m_nb_trees; t++)
    {
		m_trees.push_back(new RandomizedTree(*other.m_trees[t]));
    }

	return *this;
}

RandomizedForest::~RandomizedForest()
{
    clean();
}

void
RandomizedForest::reset()
{
	clean();
    for(unsigned int i=0;i<m_nb_trees;i++)
    {
		m_trees.push_back(new RandomizedTree(m_depth,m_nb_labels,m_vector_size,m_min_value,m_max_value));
    }
}

//============================================================================================================================//
void
RandomizedForest::train(const IFeatureVector& vector, unsigned int label)
{
	for(unsigned int t=0; t<m_nb_trees; t++) 
	{
		m_trees[t]->train(vector,label);
	}
	m_occurences[label]++;
}


Histogram
RandomizedForest::estimatePosterior(const IFeatureVector& vector) const
{
	// the histograms
	Histogram histo_leaf(m_nb_labels);
	
	Histogram equiproba(m_nb_labels);
	equiproba.fill(1).normalize();

	Histogram occurence_normalization = m_occurences;
	occurence_normalization.normalize();

	Histogram probas(m_nb_labels);

	// get the proba for each tree
	for(unsigned int t=0; t<m_nb_trees; t++)
	{
		const LeafNode* leaf = m_trees[t]->patchToLeaf(vector);
		if(leaf)
		{
			histo_leaf = leaf->getHistogram();
			if(m_occurence_normalization) histo_leaf /= occurence_normalization;
			probas+=histo_leaf.normalize();
		}
		else
		{
			probas+=equiproba;
		}
	}

	// normalize the proba, dividing by the number of trees in the forest
	probas.scale(1./static_cast<double>(m_nb_trees));

	return probas;
}

void
RandomizedForest::load(const std::string& filename)
{
	// open the file to be loaded
    std::ifstream file;
	file.open(filename.c_str());

	// Reset forest 
	clean();
    std::string name;
   
    // depth and nb_trees
    std::getline (file,name);
	file>>m_depth>>m_nb_trees>>m_vector_size>>m_min_value>>m_max_value>>m_nb_labels;

	// occurence histogram
	std::getline (file,name);
	std::getline (file,name);
	std::getline (file,name);
	Histogram(m_nb_labels);
	file>>m_occurences;

    // allocate and load the trees    
    for(unsigned int i=0;i<m_nb_trees;i++)
    {
		RandomizedTree* t= new RandomizedTree;
		std::getline(file,name);
        file>>*t;
        m_trees.push_back(t);
    }    
    
    file.close();
}

void
RandomizedForest::save(const std::string& filename)
{
	// create a forest file
    std::ofstream file;
	std::string forest_name = filename + ".rf"; // "rf" will be the extension for randomized forest
    file.open(forest_name.c_str());

	// dump it
	file<<*this;
    
	// close the file
    file.close();
}

IClassifier* 
RandomizedForest::clone() const
{
	return new RandomizedForest(*this);
}

//============================================================================================================================//
unsigned int 
RandomizedForest::getNbClasses() const
{
	return m_nb_labels;
}

unsigned int 
RandomizedForest::getVectorSize() const
{
	return m_vector_size;
}

unsigned int 
RandomizedForest::getNbTrees() const
{
	return m_nb_trees;
}

unsigned int
RandomizedForest::getDepth() const
{
	return m_depth;
}

double
RandomizedForest::getMinValue() const
{
	return m_min_value;
}

double
RandomizedForest::getMaxValue() const
{
	return m_max_value;
}

//============================================================================================================================//

void
RandomizedForest::annotate()
{
	int current_id;
	for(unsigned int t=0; t<m_nb_trees; t++)
	{
		current_id = 0;
		m_trees[t]->annotate(current_id);
	}
}

void
RandomizedForest::clean()
{
    for(unsigned int i=0;i<m_trees.size();i++)
    {
        delete m_trees[i];
		m_trees[i]=0;
    }
    if(!m_trees.empty())
    {
        m_trees.clear();
    }
}


std::ostream& 
operator<<(std::ostream& out, RandomizedForest& forest)
{
	// annotate the trees
	forest.annotate();

    // depth and nb_trees
    out<< "depth, nb of trees, size of the patches and nb of classes:"<<std::endl
       << forest.m_depth<<" "<<forest.m_nb_trees<<" "
	   <<forest.m_vector_size<<" "<<forest.m_min_value<<" "<<forest.m_max_value<<" "
	   <<forest.m_nb_labels<<""<<std::endl<< std::endl;

	// write the occurence histogram
	out<<"Occurence of each label during training"<<std::endl;
	out<<forest.m_occurences<<std::endl<<std::endl;

    // dump the trees    
    for(unsigned int i=0;i<forest.m_nb_trees;i++)
    {
        out<<*(forest.m_trees[i]);
    }
    
    out<<std::endl<< std::endl;

	return out;
}

std::istream& 
operator>>(std::istream& in, RandomizedForest& forest)
{
	// Reset forest 
	forest.clean();
    std::string name;
   
    // depth and nb_trees
    std::getline (in,name);
	in>>forest.m_depth>>forest.m_nb_trees>>forest.m_vector_size>>forest.m_min_value>>forest.m_max_value>>forest.m_nb_labels;

	// occurence histogram
	std::getline (in,name);
	std::getline (in,name);
	std::getline (in,name);
	forest.m_occurences = Histogram(forest.m_nb_labels);
	in>>forest.m_occurences;

    // allocate and load the trees    
    for(unsigned int i=0;i<forest.m_nb_trees;i++)
    {
		RandomizedTree* t= new RandomizedTree;
		std::getline(in,name);
        in>>*t;
        forest.m_trees.push_back(t);
    }
	std::getline(in,name);
	return in;
}
