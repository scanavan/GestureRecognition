/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/src/RandomizedTest.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#include <ostream>
#include <istream>
#include <iostream>
#include <string>
#include "../inc/IFeatureVector.h"
#include "../inc/RandomizedTest.h"

RandomizedTest::RandomizedTest():m_size(1),m_i(0),m_j(0),m_th(0),m_type(0),m_min_value(0.),m_max_value(255.)
{
}

RandomizedTest::RandomizedTest(unsigned int size, double minV, double maxV)
:m_size(size),m_min_value(minV),m_max_value(maxV)
{
	if(m_size>1)
	{
		generate();
	}
}

RandomizedTest::RandomizedTest(const RandomizedTest &other)
:m_i(other.m_i),m_j(other.m_j),m_size(other.m_size),
m_min_value(other.m_min_value),m_max_value(other.m_max_value),
m_th(other.m_th),m_type(other.m_type)
{
}

RandomizedTest& 
RandomizedTest::operator=(const RandomizedTest& other)
{
	m_i = other.m_i;
	m_j = other.m_j;
	m_size = other.m_size;
	m_min_value = other.m_min_value;
	m_max_value = other.m_max_value;
	m_th = other.m_th;
	m_type = other.m_type;
	return *this;
}

RandomizedTest::~RandomizedTest()
{
}

bool
RandomizedTest::process(const IFeatureVector& vector) const
{
	if(m_type%2==1) return vector[m_i]<vector[m_j];
	else return vector[m_i]<m_th;
}

void 
RandomizedTest::generate()
{
	m_i = rand()%m_size;
	m_j = rand()%m_size;

	double x = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	m_th = m_min_value + x*(m_max_value-m_min_value);

	// type = 1 --> compare v[i] with v[j]
	// type = 0 --> compare v[i] and th
	m_type = rand()%2;

	if( m_i == m_j) generate();
}

unsigned int
RandomizedTest::getSize() const
{
	return m_size;
}

double
RandomizedTest::getMinValue() const
{
	return m_min_value;
}

double
RandomizedTest::getMaxValue() const
{
	return m_max_value;
}

std::ostream&
operator<<(std::ostream& out, const RandomizedTest& t)
{
	out<<t.m_size<<" "<<t.m_min_value<<" "<<t.m_max_value<<" "<<t.m_i<<" "<<t.m_j<<" "<<t.m_th<<" "<<t.m_type<<" ";
	return out;
}

std::istream& 
operator>>(std::istream& in, RandomizedTest& t)
{
	in>>t.m_size>>t.m_min_value>>t.m_max_value>>t.m_i>>t.m_j>>t.m_th>>t.m_type;
	return in;
}
