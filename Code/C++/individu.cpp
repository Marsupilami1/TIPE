#include "individu.hpp"
#include "simulation.hpp"
#include <iostream>
#include <chrono>
#include <random>

const double PI = 3.14159265358979323846264338427950;

// Génération d'une loi de répartition normale
std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> distribution(0.,PI/12.);

Individu::Individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV, bool is_pylone)
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

	// Constantes
	double alpha = 0.3;

	// Vitesse du champs de vitesses
	int x = m_position.getX();
	int y = m_position.getY();
	Vect v_chemin = champ_vitesses->at(x).at(y);
	m_vitesse = alpha*v_chemin.normalise();
	m_vitesse.rotate(distribution(generator));
	if(champ_vitesses->at((m_position+m_vitesse).getX()).at((m_position+m_vitesse).getY()) == Vect(0,0))
		return ;

	// Si repulsion
	// indiv_alentours = alentours(m_rayon_repulsion+1);
	indiv_alentours = alentours(TAILLE_GRILLE-1, liste_indiv, m_rayon_repulsion+1, x, y);
	run = indiv_alentours.size();
	unsigned int right = 0;
	unsigned int left = 0;
	for (unsigned int i=0; i<run; i++)
	{
		if(repulsion(indiv_alentours[i]) && indiv_alentours[i]->m_vitesse == Vect(0.,0.))
		{
			if(m_vitesse%(indiv_alentours[i]->m_position-m_position) > 0)
				left++;
			else
				right++;
		}
	}
	if(left+right != 0 && left != right)
	{
		m_vitesse.rotate(left<right ? PI/3.5 : -PI/3.5);
	}

	// Si sortie de zone
	/*
	m_position += m_vitesse;
	if(getY()+m_rayon >= TAILLE_GRILLE)
	{
		m_position = {getX()+(TAILLE_GRILLE-getY()-m_rayon)*m_vitesse.getX()/(m_vitesse.getY()),TAILLE_GRILLE-m_rayon};
		m_vitesse = alpha*(champ_vitesses->at(getX()).at(getY()).normalise());
	} else if(getY() < m_rayon)
	{
		m_position = {getX()+(m_rayon-getY())*m_vitesse.getX()/(m_vitesse.getY()),m_rayon};
		m_vitesse = alpha*(champ_vitesses->at(getX()).at(getY()).normalise());

	} else if(getX()+m_rayon >= TAILLE_GRILLE)
	{
		m_position = {TAILLE_GRILLE-m_rayon,getY()+(TAILLE_GRILLE-getX()-m_rayon)*m_vitesse.getY()/(m_vitesse.getX())};
		m_vitesse = alpha*(champ_vitesses->at(getX()).at(getY()).normalise());
	} else if(getX() < m_rayon)
	{
		m_position = {m_rayon,getY()+(m_rayon-getX())*m_vitesse.getY()/(m_vitesse.getX())};
		m_vitesse = alpha*(champ_vitesses->at(getX()).at(getY()).normalise());
	} else
	{
		m_position -= m_vitesse;
	}
	*/
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
	l= l<1 ? 1 : l;
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
