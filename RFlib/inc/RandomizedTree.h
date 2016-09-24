/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/inc/RandomizedTree.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#ifndef RANDOMIZEDTREE_H
#define RANDOMIZEDTREE_H

#include <ostream>
#include <istream>
#include <vector>
#include <map>
#include <utility>

class LeafNode;
class RandomizedTest;
class IFeatureVector;

typedef std::pair<std::pair<int,int>,int> t_Edge;

class RandomizedTree
{
public:
    //! Constructor
    RandomizedTree(int depth=1, int nbLabels=2, unsigned int vector_size=1, double minV=0., double maxV=255.);

    //! constructor given a test
    RandomizedTree(int depth, int nbLabels, const RandomizedTest& test);

    //! Copy Constructor
    RandomizedTree(const RandomizedTree& other);

    //! copy a randomized tree
    virtual void copy(RandomizedTree* other);

    //! Destructor
    virtual ~RandomizedTree();

    //! Process patch
    void train(const IFeatureVector& vector, int label);

    //! Tell if the node is a leaf
    virtual bool isLeaf() const;

    //! Put Id to the nodes of the tree
    void annotate(int& current_id);

    //! Compute the number of nodes of the tree
    int getNumberNodes() const;

    //! sum up the tree into two lists : nodes and edges
    void compact(std::vector<RandomizedTree*>& nodes, std::vector<t_Edge>& edges) const;

    //! create a tree from the node list and the edges list
    void expand(std::map<int,RandomizedTree*>& nodes, std::vector<t_Edge>& edges);

    //! I/O for trees
    friend std::ostream& operator<<(std::ostream& out, const RandomizedTree& tree);
    friend std::istream& operator>>(std::istream& in, RandomizedTree& tree);

    //! plot the tree as a graphViz .dot file
    void toDot(const char* filename);

    //! Getters
    int getID() const;
    int getDepth() const;
    int getNbClasses() const;
	unsigned int getVectorSize() const;
	double getMinValue() const;
	double getMaxValue() const;

    //! Turn a patch into a LeafNode for testing purposes
    const LeafNode* patchToLeaf(const IFeatureVector& vector) const; 
    
protected:
    // process a patch down to depth d
    virtual void train(const IFeatureVector& patch, int label, int d);

protected:
    //! The depth of the tree
    int m_depth;

    //! the number of classes considered in the classification
    int m_nb_classes;

	//! size of the feature vector
	unsigned int m_vector_size;

	//! min value of the data
	double m_min_value;

	//! max value of the data
	double m_max_value;

    //! a test, build randomly at the construction of the tree
    RandomizedTest* m_test;

    // id of the node in the tree (DFS)
    int m_id;

private:
    //! the True Child and the False child --> remember it's a decision tree...
    RandomizedTree* m_true_child;
    RandomizedTree* m_false_child;
};


#endif //RANDOMIZEDTREE_H
