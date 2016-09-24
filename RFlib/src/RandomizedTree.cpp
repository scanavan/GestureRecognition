/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/src/RandomizedTree.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#include <string>
#include <fstream>
#include "../inc/Histogram.h"
#include "../inc/RandomizedTest.h"
#include "../inc/LeafNode.h"
#include "../inc/RandomizedTree.h"

RandomizedTree::RandomizedTree(int depth, int nbLabels, unsigned int vector_size, double minV, double maxV)
:m_depth(depth),m_nb_classes(nbLabels),m_vector_size(vector_size),m_min_value(minV),m_max_value(maxV)
{
	m_test = new RandomizedTest(m_vector_size,m_min_value,m_max_value);
    m_true_child = 0;
    m_false_child = 0;
}

RandomizedTree::RandomizedTree(int depth, int nbLabels, const RandomizedTest& test)
:m_depth(depth),m_nb_classes(nbLabels),m_vector_size(test.getSize()),
m_min_value(test.getMinValue()),m_max_value(test.getMaxValue())
{
    m_test = new RandomizedTest(test);
    m_true_child = 0;
    m_false_child = 0;
}

RandomizedTree::RandomizedTree(const RandomizedTree& other)
:m_depth(other.m_depth),m_nb_classes(other.m_nb_classes),
m_vector_size(other.m_vector_size),m_min_value(other.m_min_value),m_max_value(other.m_max_value),
m_id(other.m_id)
{
    // copy the test
    m_test = new RandomizedTest(*other.m_test);

    // copy the subtrees
	m_true_child = 0;
    if(other.m_true_child)
    {
        if(! other.m_true_child->isLeaf()) m_true_child = new RandomizedTree(*other.m_true_child);
        else m_true_child = new LeafNode(*dynamic_cast<LeafNode*>(other.m_true_child));
    }

	m_false_child=0;
    if(other.m_false_child)
    {
        if(! other.m_false_child->isLeaf()) m_false_child = new RandomizedTree(*other.m_false_child);
        else m_false_child = new LeafNode(*dynamic_cast<LeafNode*>(other.m_false_child));
    }
}

void
RandomizedTree::copy(RandomizedTree* other)
{
    m_depth			= other->m_depth;
    m_nb_classes	= other->m_nb_classes;
    m_vector_size	= other->m_vector_size;
	m_min_value		= other->m_min_value;
	m_max_value		= other->m_max_value;
    *m_test			= *other->m_test;
    m_id			= other->m_id;

    // first delete the subtrees if they exists
    if(m_true_child) 
    {
        delete m_true_child;
        m_true_child = 0;
    }
    if(m_false_child) 
    {
        delete m_false_child;
        m_false_child = 0;
    }

    // rebuild the children according to the other tree
    if(other->m_true_child) 
    {
        if(! other->m_true_child->isLeaf()) 
		{
			m_true_child = new RandomizedTree(m_depth-1,m_nb_classes,m_vector_size,m_min_value,m_max_value);
		}
        else 
		{
			m_true_child = new LeafNode(m_nb_classes);
		}
        m_true_child->copy(other->m_true_child);
    }

    if(other->m_false_child)
    {
        if(! other->m_false_child->isLeaf())
		{
			m_false_child = new RandomizedTree(m_depth-1,m_nb_classes,m_vector_size,m_min_value,m_max_value);
		}
        else 
		{
			m_false_child = new LeafNode(m_nb_classes);
		}
        m_false_child->copy(other->m_false_child);
    }
}

RandomizedTree::~RandomizedTree()
{
    delete m_test;
    if(m_true_child) 
    {
        delete m_true_child;
        m_true_child = 0;
    }
    if(m_false_child) 
    {
        delete m_false_child;
        m_false_child = 0;
    }
}

bool 
RandomizedTree::isLeaf() const
{
    return false;
}

void 
RandomizedTree::train(const IFeatureVector& vector, int label)
{
	train(vector,label,m_depth);
}

const LeafNode*
RandomizedTree::patchToLeaf(const IFeatureVector& vector) const
{
    if(isLeaf()) return dynamic_cast<const LeafNode*>(this);
	bool test = m_test->process(vector);

    // vector goes to left child (True)
    if(test)
    {
        if(!m_true_child)
        {
            //! return an outlier leaf
            return NULL;
        }
        else
        {
            return m_true_child->patchToLeaf(vector);
        }
    }

    // vector goes to right child (False)
    else
    {
        if(!m_false_child)
        {
            //! return an outlier leaf
            return NULL;
        }
        return m_false_child->patchToLeaf(vector);
    }

	return NULL;
}



void 
RandomizedTree::train(const IFeatureVector& vector, int label, int d)
{
    // apply the randomized test
    bool test = m_test->process(vector);

    // Patch goes to left child (True)
	RandomizedTree* childtree;
    if(test)
    {
        if(!m_true_child)
        {
            if(d>1)	m_true_child = new RandomizedTree(d-1,m_nb_classes,m_vector_size,m_min_value,m_max_value);
            else m_true_child = new LeafNode(m_nb_classes);
        }
		childtree = m_true_child;
    }

    // patch goes to right child (False)
    else
    {
        if(!m_false_child)
        {
            if(d>1)	m_false_child = new RandomizedTree(d-1,m_nb_classes,m_vector_size,m_min_value,m_max_value);
            else m_false_child = new LeafNode(m_nb_classes);
        }
		childtree = m_false_child;
    }

	// recursive call
	childtree->train(vector,label,d-1);
}

std::ostream& 
operator<<(std::ostream& out, const RandomizedTree& v)
{
    // turn the tree into a set of nodes and edges
    std::vector<RandomizedTree*> nodes;
    std::vector<t_Edge> edges;
    v.compact(nodes,edges);

    // dump the global caracteristics of the tree
    out<<"Tree : depth "<<v.m_depth<<", "<<v.m_nb_classes<<" labels, patch size "
		<<v.m_vector_size<<" "<<v.m_min_value<<" "<<v.m_max_value<<std::endl;

    // dump the nodes : id, nature and features
    out<<v.getNumberNodes()<<" Nodes :"<<std::endl;
    for(unsigned int i = 0; i<nodes.size();i++)
    {
        RandomizedTree& node = *nodes[i];
        if(!node.isLeaf())
        {
            out << node.m_id<<" S "<<*node.m_test<<std::endl;
        }
        else
        {
			out << node.m_id<<" L ";
			for(int k=0; k<v.m_nb_classes; k++) out<<dynamic_cast<LeafNode&>(node)[k]<<" ";
            out<<std::endl;
        }
    }

    // dump the edges : topology and left/right
    out<<"Edges :"<<std::endl;
    for(unsigned int i=0; i<edges.size(); i++)
    {
        out<<edges[i].first.first<<" "<<edges[i].first.second<<" "<<edges[i].second<<std::endl;
    }
    
    return out;
}

std::istream&
operator>>(std::istream& in, RandomizedTree& tree)
{
    std::map<int,RandomizedTree*> nodes;

    std::string st;
    for(int i=0; i<3; i++) 
	{
		in>>st;
	}

    // get the global caracteristics of the tree
    int size, depth, nbclasses;
	double minV, maxV;
    in>>depth>>st>>nbclasses>>st>>st>>st>>size>>minV>>maxV;

    // get the number of nodes in the tree
    int nbNodes;
    in>>nbNodes>>st>>st;

    // create the nodes
    int id;
	double b;
    std::string s;
    RandomizedTest test;
    RandomizedTree* root=0;
    for(int i=0; i<nbNodes; i++)
    {
        RandomizedTree* node;

        in>>id>>s;
        if(!s.compare("S"))
        {
            in>>test;
            node = new RandomizedTree(depth,nbclasses,test);
        }
        else
        {
            node = new LeafNode(nbclasses);
            for(int c = 0; c<nbclasses; c++)
            {
                in>>b;
                (*dynamic_cast<LeafNode*>(node))[c] = b;
            }
        }
	
        node->m_id = id;
        nodes.insert(std::pair<int,RandomizedTree*>(id,node));

        if(!root) root = node; // the root has the smallest id, it's in fact the first node to be dumped, so the first to be met here
    }

    // create the edges
    in>>st>>st; // read "Edge :"
    for(int i=0; i<nbNodes-1; i++)
    {
        int id1, id2, nature;
        in>>id1>>id2>>nature;
        RandomizedTree* node = nodes[id1];
        if(nature == 1) 
        {
            node->m_true_child = nodes[id2];
        }
        else
        {
            node->m_false_child = nodes[id2];
        }
    }

    tree.copy(root);

    delete root;
    nodes.clear();

    return in;
}

void
RandomizedTree::annotate(int& current_id)
{
    m_id = current_id;
    current_id++;
    if(m_true_child) m_true_child->annotate(current_id);
    if(m_false_child) m_false_child->annotate(current_id);
}

int
RandomizedTree::getNumberNodes() const
{
    int a = 0;
    int b = 0;
    if(m_true_child) a = m_true_child->getNumberNodes();
    if(m_false_child) b = m_false_child->getNumberNodes();
	
    return 1+a+b;
}

void
RandomizedTree::compact(std::vector<RandomizedTree*>& nodes, std::vector<t_Edge>& edges) const
{
    nodes.push_back(const_cast<RandomizedTree*>(this));// bad cast
	
    if(m_true_child)
    {
        edges.push_back(std::pair<std::pair<int,int>,int>(std::pair<int,int>(m_id,m_true_child->m_id),1));
        m_true_child->compact(nodes,edges);
    }

    if(m_false_child)
    {
        edges.push_back(std::pair<std::pair<int,int>,int>(std::pair<int,int>(m_id,m_false_child->m_id),0));
        m_false_child->compact(nodes,edges);		
    }
}

void
RandomizedTree::expand(std::map<int,RandomizedTree*>& nodes, std::vector<t_Edge>& edges)
{
    // get the id of the children, by traversing the edge list
    int count=0;
    for(unsigned int k=0; k<edges.size(); k++)
    {
        t_Edge& edge = edges[k];
        if(edge.first.first == m_id)
        {
            if(edge.second == 1) 
            {
                count++;
                m_true_child = nodes[edge.first.second];
                //remove edge from edges
                edges.erase(edges.begin()+k);
                k--;
            }

            else 
            {
                count++;
                m_false_child = nodes[edge.first.second];
                //remove edge from edges
                edges.erase(edges.begin()+k);
                k--;
            }
            if(count==2)
            {
                // remove node from node
                break; //should be faster like that
            }
        }
    }
    if(m_true_child)
    {
        m_true_child->expand(nodes,edges);
    }
    if(m_false_child)
    {
        m_false_child->expand(nodes,edges);
    }
}

void 
RandomizedTree::toDot(const char* filename)
{
	// annotate the tree
	int current_id = 0;
	annotate(current_id);

    std::vector<RandomizedTree*> nodes;
    std::vector<t_Edge> edges;
    compact(nodes,edges);

    std::ofstream outFile(filename);
    outFile<<"digraph RandomizedTree{"<<std::endl;
	
    for(unsigned int i=0; i<nodes.size(); i++)
    {
        outFile<<"\tNode"<<nodes[i]->m_id<<"[label = "<<nodes[i]->m_id<<"]"<<std::endl;
    }

    for(unsigned int i=0; i<edges.size(); i++)
    {
        outFile<<"\tNode"<<edges[i].first.first<<" -> Node"<<edges[i].first.second<<"[color = ";
        if(edges[i].second) outFile<<"\"#0000ff\"]"<<std::endl;
        else outFile<<"\"#ff0000\"]"<<std::endl;
    }

    outFile<<"}"<<std::endl;
    outFile.close();
}

int 
RandomizedTree::getID() const
{
    return m_id;
}

int 
RandomizedTree::getDepth() const
{
    return m_depth;
}

int 
RandomizedTree::getNbClasses() const
{
    return m_nb_classes;
}

unsigned int 
RandomizedTree::getVectorSize() const
{
    return m_vector_size;
}

double
RandomizedTree::getMinValue() const
{
    return m_min_value;
}

double
RandomizedTree::getMaxValue() const
{
    return m_max_value;
}
