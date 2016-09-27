/**
 * @file     $URL: svn+ssh://svn_athena/home/svnrepositories/BuildingModeling/branches/Olivier/ML/inc/RandomizedTest.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-10-16 19:24:19 +0200 (ven., 16 oct. 2009) $
 * @brief
 */

#ifndef RANDOMIZEDTEST_H
#define RANDOMIZEDTEST_H

class IFeatureVector;

class RandomizedTest
{
public:
	//! default constructor
	RandomizedTest();

    //! Constructor of the test, the geometry is given by a patch
    RandomizedTest(unsigned int size, double minV=0., double maxV=255.);

    //! Assignement operator
    RandomizedTest& operator=(const RandomizedTest& other);

    //! Copy constructor
    RandomizedTest(const RandomizedTest& other);

    //! Destructor of the Randomized test
    ~RandomizedTest();

    //! Process the test on a patch
    bool process(const IFeatureVector& vector) const;

    //! Get the geometry of the tested patchs
    unsigned int getSize() const; 

	//! get the range of the data (between 0 and range)
	double getMinValue() const;
	double getMaxValue() const;

    //! I/O
    friend std::ostream& operator<<(std::ostream& out, const RandomizedTest& t);
    friend std::istream& operator>>(std::istream& in, RandomizedTest& t);

private:
    //! Randomly generate a test
    void generate();

private:
    //! geometry
	unsigned int m_size;

	//! range
	double m_min_value;
	double m_max_value;

    //! (random) value of the vector to compare
    unsigned int m_i;
    unsigned int m_j;
	double m_th;
	unsigned int m_type;
};

#endif //RANDOMIZEDTEST_H
