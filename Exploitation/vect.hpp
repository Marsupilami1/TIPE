#ifndef DEF_VECT
#define DEF_VECT

#include <iostream>
#include <cmath>

class Vect
{
	public :
		Vect(double x = 0.0, double y = 0.0); // Constructeur
		Vect(int x, int y);
		Vect(Vect const& v); // Constructeur
		
		double getX() const;
		double getY() const;
		
		void afficher() const;
		void rotate(double theta);
		double norme();
		Vect entier();
		
		bool egal(Vect const& v) const;
		Vect oppose() const;
		double scalaire(Vect const& v) const;
		double vectoriel(Vect const& v) const;
		Vect& operator+=(Vect const& v);
		Vect& operator-=(Vect const& v);
		Vect& operator*=(double const& k);
		Vect& operator/=(double const& k);
		
		Vect normalise() const;
	
	private :
		double m_x;
		double m_y;
};

bool operator==(Vect const& u, Vect const& v);
bool operator!=(Vect const& u, Vect const& v);

Vect operator+(Vect const& u, Vect const& v);
Vect operator-(Vect const& u);
Vect operator-(Vect const& u, Vect const& v);
Vect operator*(double const& k, Vect const& u);
Vect operator*(Vect const& u, double const& k);
double operator*(Vect const& u, Vect const& v);
double operator%(Vect const& u, Vect const& v);
Vect operator/(Vect const&u, double const& k);

std::ostream& operator<<(std::ostream &flux, Vect const& u);


#endif
