/**
 * @file     $URL: svn+ssh://teboul@athena.mas.ecp.fr/home/svnrepositories/BuildingModeling/branches/Olivier/Core/Materials/inc/Color.h $
 * @author   $Author: teboul $
 * @date     $Date: 2010-02-26 19:39:20 +0100 (ven., 26 févr. 2010) $
 * @brief    A color is defined by three value R,G and B, plus optionnally an alpha value
 */

#ifndef COLOR_H
#define COLOR_H

#include <string>

class Color
{
public:
	//! Default constructor : white
	Color();

	//! Constructor with  int R,G,B
	Color(int r, int g, int b, double alpha=1.);

	//! Constructor with a double : give a rainbow color
	//! negative is black, greater than one is white, in between is the rainbow.
	Color(double d, double alpha=1.);

	//! Constructor with a string as the name of the color
	Color(std::string name, double alpha = 1.);

	//! Copy Constructor
	Color(const Color& other);
	
	//! Destructor
	~Color();

	//! Overload of the equal operator==
	bool operator==(const Color& other) const;

	//! Overload of the equal operator!=
	bool operator!=(const Color& other) const;

	//! Accessor to the red component
	const unsigned char getR() const;

	//! Accessor to the green component
	const unsigned char getG() const;

	//! Accessor to the blue component
	const unsigned char getB() const;

	//! get the opacity value alpha
	const double getAlpha() const;

	//! Set the color : overload the assignation operator
	Color& operator=(const Color& other);

	//! Set the color as a rainbow color, defined by a float between 0 and 1 (based on hsv, from 0 to 270°)
	//! Lower than 0 is black, greater than one is white
	Color& rainbow(double x);
	
	//! Set the color as a rainbow color, defined by a float between 0 and 1
	//! based on moving along the edges of the rgb cube
	//! Lower than 0 is black, greater than one is white
	Color& rainbow_cube(double x);

	//! Make a random rainbow color
	Color& random();

	//! Set the alpha channel
	void setAlpha(double alpha);

    std::string toString() const;
    bool isDefault() const;

public:
	static void rgb2hsv(int r, int g, int b, int& h, double& s, double& v);
	static void hsv2rgb(int h, double s, double v, int& r, int& g, int& b);

private: // members
	unsigned char m_R;
	unsigned char m_G;
	unsigned char m_B;
	double m_alpha;

private: // methods
	void setR(int r);
	void setG(int g);
	void setB(int b);
};

#endif //COLOR_H
