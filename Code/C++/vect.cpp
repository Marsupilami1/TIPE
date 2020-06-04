#include "vect.h"

vect::vect(double x, double y) : m_x(x), m_y(y)
{	
}

vect::vect(int x, int y) :m_x((double) x), m_y((double) y)
{
}

vect::vect(vect const& v) : m_x(v.m_x), m_y(v.m_y)
{
}


double vect::get_X() const
{
	return m_x;
}
double vect::get_Y() const
{
	return m_y;
}


void vect::afficher() const
{
	std::cout << "Vecteur : ( " << m_x << " , " << m_y << " )\n";
}

void vect::rotate(double theta)
{
	double mem = m_x;
	double theta2 = theta*theta;
	m_x = m_x*(1-theta2/2) - m_y*theta;
	m_y = m_y*(1-theta2/2) + mem*theta;
}

double vect::norme()
{
	return sqrt(m_x*m_x + m_y*m_y);
}

vect vect::entier()
{
	vect res = {floor(m_x), floor(m_y)};
	return res;
}


bool vect::egal(vect const& v) const
{
	return (m_x == v.m_x && m_y == v.m_y);
}

vect vect::oppose() const
{
	vect res(-m_x, -m_y);
	return res;
}

double vect::scalaire(vect const& v) const
{
	return m_x * v.m_x + m_y * v.m_y;
}

double vect::vectoriel(vect const& v) const
{
	return m_x * v.m_y - m_y * v.m_x;
}

vect& vect::operator+=(vect const& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	
	return *this;
}

vect& vect::operator-=(vect const& v)
{
	return *this += -v;
}

vect& vect::operator*=(double const& k)
{
	m_x *= k;
	m_y *= k;
	
	return *this;
}

vect& vect::operator/=(double const& k)
{
	return *this *= 1/k;
}


bool operator==(vect const& u, vect const& v)
{
	return u.egal(v);	
}

bool operator!=(vect const& u, vect const& v)
{
	return !(u==v);
}

vect operator+(vect const& u, vect const& v)
{
	vect copie(u);
	copie += v;
	return copie;
}

vect operator-(vect const& u)
{
	return u.oppose();
}

vect operator-(vect const& u, vect const& v)
{
	return u + (-v);
}

vect operator*(double const& k, vect const& u)
{
	vect copie(u);
	copie *= k;
	return copie;
}

vect operator*(vect const& u, double const& k)
{
	return k*u;
}

double operator*(vect const& u, vect const& v)
{
	return u.scalaire(v);
}

double operator%(vect const& u, vect const& v)
{
	return u.vectoriel(v);
}

vect operator/(vect const& u, double const& k)
{
	return u*(1/k);
}

vect vect::normalise() const
{
	vect id(*this);
	return (id/(id.norme()));
}


std::ostream& operator<<(std::ostream &flux, vect const& u)
{
    u.afficher();
    return flux;
}
