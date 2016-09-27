/**
 * @file     $URL: svn+ssh://teboul@athena/home/svnrepositories/BuildingModeling/branches/Olivier/Core/Maths/src/Histogram.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2010-02-17 20:54:56 +0100 (mer., 17 févr. 2010) $
 * @brief
 */

#include <cstdlib>
#include <cfloat>
#include <cmath>
#include "../inc/Histogram.h"

Histogram::Histogram(unsigned int nbClasses):m_nb_classes(nbClasses),m_weight(1)
{
	m_data = new double[m_nb_classes];
	for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] = 0;
}

Histogram::Histogram(unsigned int nbClasses, double* data):m_nb_classes(nbClasses),m_weight(1)
{
	m_data = new double[m_nb_classes];
	for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] = data[i];
}

Histogram::Histogram(const Histogram& other):m_nb_classes(other.m_nb_classes),m_weight(other.m_weight)
{
	m_data = new double[m_nb_classes];
	for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] = other.m_data[i];
}

Histogram::~Histogram()
{
	delete[] m_data;
}

Histogram& 
Histogram::operator= (const Histogram& other)
{
	if(m_nb_classes != other.m_nb_classes)
	{
		delete[] m_data;
		m_data = new double[other.m_nb_classes];
		m_nb_classes = other.m_nb_classes;
	}
	for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] = other.m_data[i];
	m_weight = other.m_weight;
	return *this;
}

Histogram& 
Histogram::fill(double val)
{
	for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] = val;
	return *this;
}

Histogram& 
Histogram::zeros()
{
	return fill(0);
}

Histogram& 
Histogram::ones()
{
	return fill(1);
}

Histogram& 
Histogram::basis(int i)
{
	fill(0);
	m_data[i] = 1;
	return *this;
}

double
Histogram::sum() const
{
	double result = 0;
	for(unsigned int i=0; i<m_nb_classes; i++) result += m_data[i];
	return result;
}

Histogram& 
Histogram::normalize()
{
	m_weight = sum();
	if(m_weight>1e-9) for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] /= m_weight;
	return *this;
}

Histogram&
Histogram::normalize2()
{
	m_weight = norm();
	if(m_weight>1e-9) for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] /= m_weight;
	return *this;
}

double 
Histogram::norm() const
{
	return dot(*this);
}

double 
Histogram::maxValue() const
{
	double result = -1;
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		if(m_data[i]>result) result = m_data[i];
	}
	return result;
}

unsigned int 
Histogram::argMax() const
{
	double max = -1;
	int result = -1;

	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		if(m_data[i]>max)
		{
			result = i;
			max = m_data[i];
		}
	}
	return result;
}

double 
Histogram::minValue() const
{
	double result = DBL_MAX;
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		if(m_data[i]<result) result = m_data[i];
	}
	return result;
}

unsigned int 
Histogram::argMin() const
{
	double min = DBL_MAX;
	int result = -1;

	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		if(m_data[i]<min)
		{
			result = i;
			min = m_data[i];
		}
	}
	return result;
}

unsigned int 
Histogram::getNbClasses() const
{
	return m_nb_classes;
}

double 
Histogram::getWeight() const
{
	return m_weight;
}

void 
Histogram::setWeight(double w)
{
	m_weight = w;
}

void 
Histogram::resetWeight()
{
	m_weight = 1.;
}
const double& 
Histogram::operator[](int i) const
{
	return m_data[(i+m_nb_classes)%m_nb_classes];
}

double& 
Histogram::operator[](int i)
{
	return m_data[(i+m_nb_classes)%m_nb_classes];
}

std::ostream& 
operator<<(std::ostream& out, const Histogram& hist)
{
	for(unsigned int i=0; i<hist.m_nb_classes; i++) out<<hist[i]<<" ";
	return out;
}

std::istream& 
operator>>(std::istream& in, Histogram& hist)
{
	for(unsigned int i=0; i<hist.m_nb_classes; i++)
	{
		in>>(hist[i]);
	}
	return in;
}

Histogram& 
Histogram::operator+=(const Histogram& other)
{
	if(m_nb_classes != other.m_nb_classes) throw HistogramOperationError();
	
	else
	{
		for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] += other.m_data[i];
	}

	return *this;
}

Histogram  
Histogram::operator+(const Histogram other)
{
	Histogram result(*this);
	result += other;
	return result;
}

Histogram& 
Histogram::operator*=(const Histogram& other)
{
	if(m_nb_classes != other.m_nb_classes) throw HistogramOperationError();
	
	else
	{
		for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] *= other.m_data[i];
	}

	return *this;
}

Histogram
Histogram::operator*(const Histogram other)
{
	Histogram result(*this);
	result *= other;
	return result;
}

Histogram&
Histogram::operator/=(const Histogram& other)
{
	if(m_nb_classes != other.m_nb_classes) throw HistogramOperationError();
	
	else
	{
		for(unsigned int i=0; i<m_nb_classes; i++) 
		{
			if(other.m_data[i]>1e-10) m_data[i] /= other.m_data[i];
		}
	}

	return *this;
}

Histogram 
Histogram::operator/(const Histogram other)
{
	Histogram result(*this);
	result /= other;
	return result;
}

Histogram&  
Histogram::scale(double s)
{
	for(unsigned int i=0; i<m_nb_classes; i++) m_data[i] *= s;
	return *this;
}

double 
Histogram::dot(const Histogram& other) const
{
	if(m_nb_classes != other.m_nb_classes) throw HistogramOperationError();

	double result=0;
	for(unsigned int i=0; i<m_nb_classes; i++) result += m_data[i]*other.m_data[i];
	return result;
}

Histogram& 
Histogram::linearCombination(const std::vector<Histogram>& vectors, const std::vector<double>& coeffs)
{
	fill(0); 
	
	for(unsigned int i=0; i<vectors.size(); i++ )
	{
		Histogram h = vectors[i];
		h.scale(coeffs[i]);
		(*this) += h;
	}

	return *this;
}

Histogram&
Histogram::resize(unsigned int k)
{
	// copy the old data
	Histogram h(*this);
	delete[] m_data;
	
	// find the min number of classes between the old histogram and the new one
	int minNbClasses = k;
	if (k>m_nb_classes) minNbClasses = m_nb_classes;

	// reset the new data with the proper size
	m_nb_classes = k;
	m_data = new double[m_nb_classes];
	fill(0);
	
	// fill the data with the old one
	for(int i=0; i<minNbClasses; i++) m_data[i] = h[i];

	// return the new version of 'this'
	return *this;
}

unsigned int 
Histogram::sample() const
{
	// use a new histogram, not to modify this one
	Histogram copy(*this);

	// normalize it, so that it sums to 0
	copy.normalize();

	// uniformly sample a number in [0,1]
	double x = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	
	// check which indice goes x
	double partial_sum = 0.0;
	unsigned int k = -1;
	while(partial_sum<=x)
	{
		partial_sum += copy[k];
		k++;
	}

	// make sure k is ok
	if(k<0)
	{
		k=0;
	}
	if(k>copy.getNbClasses()-1)
	{
		k=copy.getNbClasses()-1;
	}

	return k;
}

Histogram& 
Histogram::egreedy(double epsilon, unsigned int k)
{
	double proba = epsilon/static_cast<double>(m_nb_classes);
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		m_data[i] = proba;
	}
	m_data[k] = 1-epsilon + proba;
	
	return *this;
}

double 
Histogram::distanceL1(const Histogram& other) const
{
	double d = 0.;
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		d += fabs(m_data[i] - other[i]);
	}
	return d;
}

double 
Histogram::distanceL2(const Histogram& other) const
{
	double d = 0.;
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		d += (m_data[i] - other[i])*(m_data[i] - other[i]);
	}
	return sqrt(d);
}

double 
Histogram::distanceLP(const Histogram& other, int p) const
{
	double d = 0.;
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		d += pow(m_data[i] - other[i],p);
	}
	return pow(d,1./static_cast<double>(p));
}

double 
Histogram::kl(const Histogram& other) const
{
	// normalize the two histograms
	Histogram h1 = (*this);
	h1.normalize();

	Histogram h2 = other;
	h2.normalize();

	// compute the KL divergence
	double d=0;
	double epsilon = 1e-15;
	for(unsigned int i = 0; i<m_nb_classes; i++)
	{
		d+= h1[i]*log(h1[i]/(h2[i]+epsilon));
	}
	return d;
}

double 
Histogram::battacharyya(const Histogram& other) const
{
	// normalize the two histograms
	Histogram h1 = (*this);
	h1.normalize();

	Histogram h2 = other;
	h2.normalize();

	// compute the battacharyya coefficient
	double bc=0;
	double epsilon = 1e-15;
	for(unsigned int i=0; i<m_nb_classes; i++)
	{
		bc += sqrt(h1[i]*h2[i]);
	}

	return -log(bc+epsilon);
}

double 
Histogram::hellinger(const Histogram& other) const
{
	return sqrt(1.0-battacharyya(other));
}
