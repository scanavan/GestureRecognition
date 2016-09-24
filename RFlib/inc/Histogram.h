/**
 * @file     $URL: svn+ssh://teboul@athena/home/svnrepositories/BuildingModeling/branches/Olivier/Core/Maths/inc/Histogram.h $
 * @author   $Author: teboul $
 * @date     $Date: 2009-11-10 17:43:05 +0100 (mar., 10 nov. 2009) $
 * @brief    This class implements histograms. It enables to store an histogram, and get information about it.
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <exception>
#include <ostream>
#include <istream>
#include <vector>

class Histogram
{
public:
	//! Constructor given a number of classes
	explicit Histogram(unsigned int nbClasses=2);

	//! constructor given the number of classes and the data itself
	Histogram(unsigned int nbClasses, double* data);

	//! Copy constructor
	Histogram(const Histogram& other);

	//! Destructor
	virtual ~Histogram();

	//! Assignement operator
	Histogram& operator= (const Histogram& other);

	//! Fill the histogram with a given value
	Histogram& zeros();
	Histogram& ones();
	Histogram& fill(double val);
	Histogram& basis(int i); // (0,0,...,0,1,0,...0)

	//! Get the sum of the value of the histogram
	double sum() const;

	//! Compute the max of the histogram
	double maxValue() const;

	//! Compute the argmax of the histogram
	unsigned int argMax() const;

	//! compute the min value
	double minValue() const;

	//! compute the index where the histogram is minimal
	unsigned int argMin() const;

	//! get the number of classes
	unsigned int getNbClasses() const;

	//! get a specific value of the histogram
	const double& operator[](int i) const;

	//! the same, but enable to modify the value
	double& operator[](int i);

	//! Get the L2 norm
	double norm() const;

	//! distance between histograms
	double distanceL1(const Histogram& other) const;
	double distanceL2(const Histogram& other) const;
	double distanceLP(const Histogram& other, int p) const;
	double kl(const Histogram& other) const;
	double battacharyya(const Histogram& other) const;
	double hellinger(const Histogram& other) const;
	
	//! Normalize the histogram
	Histogram& normalize();

	//! Normalize the histogram with L2 norm
	Histogram& normalize2();

	//! Add two histograms
	Histogram& operator+=(const Histogram& other);
	Histogram operator+(const Histogram other);

	//! Mutliplication element-wise of two histgrams
	Histogram& operator*=(const Histogram& other);
	Histogram operator*(const Histogram other);

	//! Division element-wise of two histograms (keep 0 if the divider is 0)
	Histogram& operator/=(const Histogram& other);
	Histogram operator/(const Histogram other);

	//! Scale an histogram
	Histogram& scale(double s); 

	//! set the histogram as a linear combination of histograms
	Histogram& linearCombination(const std::vector<Histogram>& vectors, const std::vector<double>& coeffs);

	//! Compute the dot product of two vectors
	double dot(const Histogram& other) const; 

	//! dump the histogram into a ofstream, to be save or plotted
	friend std::ostream& operator<<(std::ostream& out, const Histogram& hist);
	friend std::istream& operator>>(std::istream& in, Histogram& hist);

	//! Accessor to the weight
	double getWeight() const;

	//! Set the value of the weight
	void setWeight(double w);

	//! reset the value of the weight to one
	void resetWeight();

	//! Resize the histogram, by truncating or zero padding
	Histogram& resize(unsigned int k);

	//! interpret the values of the historgram as a pdf, and sample a index according to it
	unsigned int sample() const;

	//! set the histogram as a e-greedy distribution on a peak value k
	Histogram& egreedy(double epsilon, unsigned int k);

protected:
	//! stores the histogram itself	
	double* m_data;

	//! the given number of classes
	unsigned int m_nb_classes;

	//! Weight of the histogram : useful while doing sums of normalized histograms
	double m_weight;
};

class HistogramOperationError : public std::exception{};
class HistogramAssignementError : public std::exception{};

#endif //HISTOGRAM_H
