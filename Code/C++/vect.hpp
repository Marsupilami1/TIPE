#ifndef VECT_HPP
#define VECT_HPP

#include <iostream>
#include <cmath>

class Vect
{
	public :
		Vect(double x = 0.0, double y = 0.0);
		Vect(int x, int y);
		Vect(unsigned int x, unsigned int y);
		Vect(Vect const& v);
		
		double getX() const;
		double getY() const;

		void afficher() const;
		void rotate(double theta);
		double norme() const;
		bool egal(Vect const& v) const;
		Vect oppose() const;
		Vect normalise() const;
		
		double scalaire(Vect const& v) const;
		double vectoriel(Vect const& v) const;
		Vect& operator+=(Vect const& v);
		Vect& operator-=(Vect const& v);
		Vect& operator*=(double const& k);
		Vect& operator/=(double const& k);

	private :
		double m_x;
		double m_y;
};

bool operator==(Vect const& u, Vect const& v);
bool operator!=(Vect const& u, Vect const& v);

Vect operator+(Vect const& u, Vect const& v);
Vect operator-(Vect const& u); // opposé
Vect operator-(Vect const& u, Vect const& v); // soustraction
Vect operator*(double const& k, Vect const& u); // mult par un scalaire
Vect operator*(Vect const& u, double const& k);
double operator*(Vect const& u, Vect const& v); // produit scalaire
double operator%(Vect const& u, Vect const& v); // déterminant
Vect operator/(Vect const&u, double const& k);

std::ostream& operator<<(std::ostream &flux, Vect const& u);


#endif /* VECT_HPP */
