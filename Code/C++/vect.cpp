#include "vect.hpp"

Vect::Vect(double x, double y) : m_x(x), m_y(y)
{	
}

Vect::Vect(int x, int y) : m_x(double(x)), m_y(double(y))
{
}

Vect::Vect(unsigned int x, unsigned int y) : m_x(double(x)), m_y(double(y))
{
}

Vect::Vect(Vect const& v) : m_x(v.m_x), m_y(v.m_y)
{
}

double Vect::getX() const
{
	return m_x;
}
double Vect::getY() const
{
	return m_y;
}


void Vect::afficher() const
{
	std::cout << "Vecteur : ( " << m_x << " , " << m_y << " )\n";
}

void Vect::rotate(double theta)
{
	double mem = m_x;
	double theta2 = theta*theta;
	m_x = m_x*(1-theta2/2) - m_y*theta;
	m_y = m_y*(1-theta2/2) + mem*theta;
}

double Vect::norme() const
{
	return sqrt(m_x*m_x + m_y*m_y);
}

bool Vect::egal(Vect const& v) const
{
	return (m_x == v.m_x && m_y == v.m_y);
}

Vect Vect::oppose() const
{
	Vect res(-m_x, -m_y);
	return res;
}

Vect Vect::normalise() const
{
	Vect id(*this);
	if (id == Vect(0.,0.)) {
		return id;
	} else {
		return (id/(id.norme()));
	}
}

double Vect::scalaire(Vect const& v) const
{
	return m_x * v.m_x + m_y * v.m_y;
}

double Vect::vectoriel(Vect const& v) const
{
	return m_x * v.m_y - m_y * v.m_x;
}

Vect& Vect::operator+=(Vect const& v)
{
	m_x += v.m_x;
	m_y += v.m_y;
	
	return *this;
}

Vect& Vect::operator-=(Vect const& v)
{
	return *this += -v;
}

Vect& Vect::operator*=(double const& k)
{
	m_x *= k;
	m_y *= k;
	
	return *this;
}

Vect& Vect::operator/=(double const& k)
{
	return *this *= 1/k;
}


bool operator==(Vect const& u, Vect const& v)
{
	return u.egal(v);	
}

bool operator!=(Vect const& u, Vect const& v)
{
	return !(u==v);
}

Vect operator+(Vect const& u, Vect const& v)
{
	Vect copie(u);
	copie += v;
	return copie;
}

Vect operator-(Vect const& u)
{
	return u.oppose();
}

Vect operator-(Vect const& u, Vect const& v)
{
	return u + (-v);
}

Vect operator*(double const& k, Vect const& u)
{
	Vect copie(u);
	copie *= k;
	return copie;
}

Vect operator*(Vect const& u, double const& k)
{
	return k*u;
}

double operator*(Vect const& u, Vect const& v)
{
	return u.scalaire(v);
}

double operator%(Vect const& u, Vect const& v)
{
	return u.vectoriel(v);
}

Vect operator/(Vect const& u, double const& k)
{
	return u*(1/k);
}

std::ostream& operator<<(std::ostream &flux, Vect const& u)
{
	u.afficher();
	return flux;
}
