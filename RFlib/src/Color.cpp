/**
 * @file     $URL: svn+ssh://teboul@athena.mas.ecp.fr/home/svnrepositories/BuildingModeling/branches/Olivier/Core/Materials/src/Color.cpp $
 * @author   $Author: teboul $
 * @date     $Date: 2010-02-26 19:39:20 +0100 (ven., 26 févr. 2010) $
 * @brief
 */

#include <cstdlib>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "../inc/Color.h"
using std::istringstream;
using std::cout;
using std::endl;

Color::Color()
{
	setR(255);
	setG(255);
	setB(255);
	setAlpha(1.);
}

Color::Color( int r, int g, int b, double alpha)
{
	setR(r);
	setG(g);
	setB(b);
	setAlpha(alpha);
}

Color::Color(double d, double alpha)
{
	rainbow(d);
	setAlpha(alpha);

}

Color::Color(std::string name, double alpha)
{
	setAlpha(alpha);

	if(name.compare("white") == 0)
	{
		setR(255);
		setG(255);
		setB(255);
	}
	else if(name.compare("black") == 0)
	{
		setR(0);
		setG(0);
		setB(0);
	}
	else if(name.compare("red") == 0)
	{
		setR(255);
		setG(0);
		setB(0);
	}
	else if(name.compare("blue") == 0)
	{
		setR(0);
		setG(0);
		setB(255);
	}
	else if(name.compare("light blue") == 0)
	{
		setR(128);
		setG(255);
		setB(255);
	}
	else if(name.compare("dark blue") == 0)
	{
		setR(0);
		setG(0);
		setB(128);
	}
	else if(name.compare("green") == 0)
	{
		setR(0);
		setG(255);
		setB(0);
	}
	else if(name.compare("dark green") == 0)
	{
		setR(0);
		setG(128);
		setB(0);
	}
	else if(name.compare("light green") == 0)
	{
		setR(128);
		setG(255);
		setB(0);
	}
	else if(name.compare("yellow") == 0)
	{
		setR(255);
		setG(255);
		setB(0);
	}
	else if(name.compare("gold") == 0)
	{
		setR(192);
		setG(192);
		setB(0);
	}
	else if(name.compare("light yellow") == 0)
	{
		setR(255);
		setG(255);
		setB(128);
	}
	else if(name.compare("magenta") == 0)
	{
		setR(255);
		setG(0);
		setB(255);
	}
	else if(name.compare("cyan") == 0)
	{
		setR(0);
		setG(255);
		setB(255);
	}
	else if(name.compare("orange") == 0)
	{
		setR(255);
		setG(128);
		setB(0);
	}
	else if(name.compare("pink") == 0)
	{
		setR(255);
		setG(128);
		setB(192);
	}
	else if(name.compare("fushia") == 0)
	{
		setR(255);
		setG(0);
		setB(128);
	}
	else if(name.compare("violet") == 0)
	{
		setR(128);
		setG(0);
		setB(255);
	}
	else if(name.compare("orchid") == 0)
	{
		setR(128);
		setG(0);
		setB(128);
	}
	else if(name.compare("bordeaux") == 0)
	{
		setR(64);
		setG(0);
		setB(64);
	}
	else if(name.compare("brown") == 0)
	{
		setR(128);
		setG(64);
		setB(0);
	}
	else if(name.compare("dark brown") == 0)
	{
		setR(64);
		setG(0);
		setB(0);
	}
	else if(name.compare("light gray") == 0)
	{
		setR(192);
		setG(192);
		setB(192);
	}
	else if(name.compare("dark gray") == 0)
	{
		setR(64);
		setG(64);
		setB(64);
	}
	else
	{
		setR(128);
		setG(128);
		setB(128);
	}
}

Color::Color(const Color& other)
{
	*this = other;
}

Color::~Color()
{
}

bool
Color::operator==(const Color &other) const
{
	return ( m_R == other.getR() &&
			 m_G == other.getG() &&
			 m_B == other.getB() &&
			 m_alpha == other.getAlpha() );
}

bool
Color::operator!=(const Color &other) const
{
	return !((*this)==other);
}

const unsigned char
Color::getR() const
{
	return m_R;
}

const unsigned char
Color::getG() const
{
	return m_G;
}

const unsigned char
Color::getB() const
{
	return m_B;
}

const double
Color::getAlpha() const
{
	return m_alpha;
}

Color& 
Color::operator=(const Color& other)
{
	m_R = other.getR();
	m_G = other.getG();
	m_B = other.getB();
	m_alpha = other.getAlpha();

	return *this;
}

void
Color::setR(int r)
{
	m_R = static_cast<unsigned char>(r);
}

void 
Color::setG(int g)
{
	m_G = static_cast<unsigned char>(g);
}

void
Color::setB(int b)
{
	m_B = static_cast<unsigned char>(b);
}

void
Color::setAlpha(double alpha)
{
	m_alpha = alpha;
}

Color&
Color::rainbow_cube(double x)
{
	if(x<0)
	{
		setR(0);
		setG(0);
		setB(0);
	}

	else if(x < 0.2)
	{
		setR(255);
		setG(static_cast<int>(5*255*x));
		setB(0);
	}

	else if(x<0.4)
	{
		setR(static_cast<int>(255*(2-5*x)));
		setG(255);
		setB(0);
	}

	else if(x<0.6)
	{
		setR(0);
		setG(255);
		setB(static_cast<int>(255*(5*x-2)));
	}

	else if(x<0.8)
	{
		setR(0);
		setG(static_cast<int>(255*(4-5*x)));
		setB(255);
	}

	else if(x<=1)
	{
		setR(static_cast<int>(255*(5*x-4)));
		setG(0);
		setB(255);
	}

	else
	{
		setR(255);
		setG(255);
		setB(255);
	}

	return *this;
}

Color&
Color::rainbow(double x)
{
	if(x<0)
	{
		setR(0);
		setG(0);
		setB(0);
	}
	else if(x>1)
	{
		setR(255);
		setG(255);
		setB(255);
	}
	else
	{	
		int r,g,b;
		int h = static_cast<int>(x*270+0.5);
		hsv2rgb(h,240./255.,1.0,r,g,b);
		setR(r);
		setG(g);
		setB(b);
	}

	return *this;
}

Color& 
Color::random()
{
	double x = static_cast<double>(rand())/static_cast<double>(RAND_MAX);
	rainbow(x);
	return *this;
}



std::string
Color::toString() const
{
    std::stringstream strm;

    strm << "#" << std::hex
         << std::setw(2) << std::setfill('0') << (int) m_R
         << std::setw(2) << std::setfill('0') << (int) m_G
         << std::setw(2) << std::setfill('0') << (int) m_B
         << std::dec << std::setprecision(3) << m_alpha;

    return strm.str();
}

bool
Color::isDefault() const
{
    return *this == Color();
}

void
Color::rgb2hsv(int r, int g, int b, int& h, double& s, double& v)
{
	// r,g,b between 0 and 1
	double red = r/255.;
	double green = g/255.;
	double blue = b/255.;

	// min
	double inf = red;
	if(green<inf)
	{
		inf = green;
	}
	if(blue<inf)
	{
		inf = blue;
	}

	// max
	double sup = red;
	if(sup<green)
	{
		sup = green;
	}
	if(sup<blue)
	{
		sup = blue;
	}

	// Value
	v = sup;

	// saturation
	double c = sup-inf;
	if( sup > 0 )
	{
		s = c/sup;
	}
	else 
	{
		s = 0;
	}

	// hue
	double hue = 0;
	if( red == sup ) hue = (green-blue)/c;
	else if( green == sup ) hue = 2 + (blue-red)/c;
	else hue = 4 + (red-green)/c;
	
	hue *= 60.;
	if( hue < 0 ) hue += 360;

	h = static_cast<int>(hue+0.5);
}

void
Color::hsv2rgb(int h, double s, double v, int& r, int& g, int& b)
{
	double red, green, blue;
	if( s == 0 )
	{
		red		= v;
		green	= v;
		blue	= v;
	}
	else
	{
		double h2 = static_cast<double>(h)/60.;
		int i = static_cast<int>( h2 );
		double f = h2 - i;
		double p = v * ( 1 - s );
		double q = v * ( 1 - s * f );
		double t = v * ( 1 - s * ( 1 - f ) );
		
		switch( i ) 
		{
			case 0:
				red		= v;
				green	= t;
				blue	= p;
				break;
			case 1:
				red		= q;
				green	= v;
				blue	= p;
				break;
			case 2:
				red		= p;
				green	= v;
				blue	= t;
				break;
			case 3:
				red		= p;
				green	= q;
				blue	= v;
				break;
			case 4:
				red		= t;
				green	= p;
				blue	= v;
				break;
			default:
				red		= v;
				green	= p;
				blue	= q;
				break;
		}
	}

	r = static_cast<int>(255*red + 0.5);
	g = static_cast<int>(255*green + 0.5);
	b = static_cast<int>(255*blue + 0.5);
}
