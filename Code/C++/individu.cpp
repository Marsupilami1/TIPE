#include "individu.hpp"
#include "simulation.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

const double PI = 3.14159265358979323846264338427950;

// Génération d'une loi de répartition normale
std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> distribution(0.,PI/12.);

Individu::Individu(double pos_x, double pos_y, double rayon,	\
				   double rayon_repulsion, double rayon_CdV, bool is_pylone)
{
	m_position = {pos_x, pos_y};
	m_vitesse = {0.,0.};
	m_rayon = rayon;
	m_rayon_repulsion = rayon_repulsion;
	m_rayon_suivi = rayon_CdV;
	m_pylone = is_pylone;
}

void Individu::afficher()
{
	std::cout << "Position : " << m_position;
}

void Individu::calculVitesse(std::vector<std::vector<Vect>>* champ_vitesses, \
							 std::vector<Individu*>** liste_indiv)
{
	unsigned int run = 0;
	const unsigned int TAILLE_GRILLE = champ_vitesses->size();
	std::vector<Individu*> indiv_alentours(0);
	Vect force(0.,0.);

	// Constantes
	double v_max = 1.3*1.34;
	double V0 = 2.1;
	double U0 = 10;
	double dt = 2.;
	double tau = 0.5;
	double sigma = 0.03;
	double R = 0.2;
	double c = 0.5;
	double cos_phi = -0.94;


	// Vitesse du champs de vitesses
	int x = m_position.getX();
	int y = m_position.getY();
	Vect v_champ = v_max * (champ_vitesses->at(x).at(y)).normalise();
	v_champ.rotate(distribution(generator));
	force = (v_champ - m_vitesse) / tau;
	if(champ_vitesses->at((m_position+v_champ).getX()).at((m_position+v_champ).getY()) == Vect(0,0))
		return ;

	// Repulsion
	indiv_alentours = alentours(TAILLE_GRILLE-1, liste_indiv,	\
								m_rayon_repulsion, m_position.getX(), m_position.getY());
	for (Individu*  ind_beta : indiv_alentours) {
		Vect r_ab = ind_beta->m_position - m_position;
		Vect next_pos = ind_beta->m_vitesse*dt;
		double b = 0.5 * sqrt(pow(r_ab.norme() + (r_ab - next_pos).norme(), 2) \
							  - pow(next_pos.norme(), 2));
		Vect f = -(ind_beta->m_pylone ? U0 : V0) * exp(-b / (ind_beta->m_pylone ? R : sigma)) * r_ab;
		if (m_vitesse * f < f.norme() * cos_phi)
			force += f;
		else {
			force += c * f;
		}
	}

	m_vitesse += force * dt;
	if (m_vitesse.norme() > v_max)
		m_vitesse = v_max * m_vitesse.normalise();

	/*
	if(champ_vitesses->at((m_position+m_vitesse).getX()).at((m_position+m_vitesse).getY()) == Vect(0,0))
		return ;
	indiv_alentours = alentours(TAILLE_GRILLE-1, liste_indiv, 1, m_position.getX(), m_position.getY());
	run = indiv_alentours.size();
	for(unsigned int i=0; i<run; i++)
	{
		if(touch(indiv_alentours[i]))
		{
			m_vitesse = {0,0};
			break;
		}
	}
	*/
}

bool Individu::move(std::vector<std::vector<Vect>>* champ_vitesses)
{
	if(m_pylone)
		return false;

	int xp = getX();
	int yp = getY();
	if(champ_vitesses->at(xp).at(yp) == Vect(0,0))
		return true;

	m_position += m_vitesse;
	xp = getX();
	yp = getY();

	if(champ_vitesses->at(xp).at(yp) == Vect(0,0))
	{
		m_position -= m_vitesse;
		return true;
	}

	return false;
}

void Individu::display(sf::RenderWindow &window, sf::Color color)
{

	sf::CircleShape cercle(10*m_rayon);
	if(m_pylone)
	{
		cercle.setFillColor(sf::Color(0,0,0));
	} else {
		// cercle.setFillColor(sf::Color(250,10,20));
		cercle.setFillColor(color);
	}
	cercle.setPosition(10*(m_position.getX()-m_rayon), 10*(m_position.getY()-m_rayon));
	cercle.setOutlineThickness(1);
	cercle.setOutlineColor(sf::Color::Black);
	window.draw(cercle);

}

bool Individu::touch(Individu* indiv) const
{
	return (indiv != this && ((m_position+m_vitesse)-(indiv->m_position)).norme() < m_rayon+indiv->m_rayon);
}

bool Individu::repulsion(Individu* indiv) const
{
	return (indiv != this && (m_vitesse*(indiv->m_position-m_position)) > 0. && (m_position-indiv->m_position).norme() < m_rayon_repulsion);
}

bool Individu::attraction(Individu* indiv) const
{
	return (indiv != this && m_vitesse*(indiv->m_position-m_position)>0 && (m_position-indiv->m_position).norme() < m_rayon_suivi);
}

Vect Individu::getPos()
{
	return m_position;
}

Vect Individu::getVit()
{
	return m_vitesse;
}

double Individu::getX()
{
	return m_position.getX();
}
double Individu::getY()
{
	return m_position.getY();
}
double Individu::getR()
{
	return m_rayon;
}

bool Individu::isPylone()
{
	return m_pylone;
}

int recherche(std::vector<Individu*>* L, Individu* element)
{
	unsigned int i = 0;
	// while (i < L->size() && L->at(i) != element)
	while (L->at(i) != element)
		i++;
	return i;
}

std::vector<Individu*> alentours(unsigned int t, std::vector<Individu*>** liste_indiv, \
				 int l, unsigned int x, unsigned int y)
{
	l = l<1 ? 1 : l;
	unsigned int gauche = x-l;
	unsigned int droite = x+l;
	unsigned int bas = y-l;
	unsigned int haut = y+l;
	std::vector<Individu*> lindiv(0);
	for(unsigned int i=(gauche<0?0:gauche); i<=(droite>t?t:droite); i++)
	{
		for(unsigned int j=(bas<0?0:bas); j<=(haut>t?t:haut); j++)
		{
			lindiv.insert(lindiv.end(),(*(liste_indiv+((t+1)*i+j)))->begin(),\
			(*(liste_indiv+((t+1)*i+j)))->end());
		}
	}
	return lindiv;
}
