#ifndef DEF_VECT
#define DEF_VECT


#include <iostream>
#include "math.h"


class vect
{
	public :
	vect(double x = 0.0, double y = 0.0); // Constructeur
	vect(int x, int y); //2fct diff
	vect(vect const& v); // Constructeur
	
	double get_X() const;
	double get_Y() const;

	//Opérations sur le vect
	void afficher() const;
	void rotate(double theta);
	double norme();
	vect entier();
	
	bool egal(vect const& v) const;
	vect oppose() const;
	double scalaire(vect const& v) const;
	vect& operator+=(vect const& v); //Definiton de lois de compositions
	vect& operator-=(vect const& v);
	vect& operator*=(double const& k);
	vect& operator/=(double const& k);
	
	vect normalise() const; //Normalisation d'un vect
	
	private :
	double m_x;
	double m_y;
};

bool operator==(vect const& u, vect const& v);
bool operator!=(vect const& u, vect const& v);

vect operator+(vect const& u, vect const& v);
vect operator-(vect const& u);
vect operator-(vect const& u, vect const& v);
vect operator*(double const& k, vect const& u);
vect operator*(vect const& u, double const& k);
double operator*(vect const& u, vect const& v);
vect operator/(vect const&u, double const& k);

std::ostream& operator<<(std::ostream &flux, vect const& u); //Affichage


#endif
