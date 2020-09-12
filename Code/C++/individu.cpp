#include "individu.hpp"
#include "simulation.hpp"
#include <iostream>
#include <chrono>
#include <random>

const double PI = 3.14159265358979323846264338427950;

// Génération d'une loi de répartition normale
std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> distribution(0.,PI/12.);

extern std::vector<std::vector<int>> Champ_de_vitesses;

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

void Individu::calculVitesse(Simulation<40>* simul)
{
	unsigned int run = 0;
	std::vector<Vect> liste_vitesse = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
	std::vector<Individu*> indiv_alentours(0);

	// Constantes
	double alpha = 0.2;

	// Vitesse du champs de vitesses
	int x = m_position.getX();
	int y = m_position.getY();
	int v_chemin = Champ_de_vitesses[x][y];
	Vect vit = {0,0};
	if(v_chemin > -1)
		vit = liste_vitesse[v_chemin];
	m_vitesse = alpha*vit.normalise();
	m_vitesse.rotate(distribution(generator));
	if(Champ_de_vitesses[(m_position+m_vitesse).entier().getX()][(m_position+m_vitesse).entier().getY()] == -1)
		return ;

	// Si repulsion
	// indiv_alentours = alentours(m_rayon_repulsion+1);
	indiv_alentours = simul->alentours(m_rayon_repulsion+1, x, y);
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
	if(left+right != 0)
		m_vitesse.rotate(left<right ? PI/3.5 : -PI/3.5);

	// Si sortie de zone
	m_position += m_vitesse;
	if((m_position).getY()+m_rayon>=TAILLE_GRILLE)
	{
		m_position = {m_position.getX()+(TAILLE_GRILLE-m_position.getY()-m_rayon)*m_vitesse.getX()/(m_vitesse.getY()),TAILLE_GRILLE-m_rayon};
		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
	} else if(m_position.getY()<m_rayon)
	{
		m_position = {m_position.getX()+(m_rayon-m_position.getY())*m_vitesse.getX()/(m_vitesse.getY()),m_rayon};
		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
	} else if((m_position).getX()+m_rayon>=TAILLE_GRILLE)
	{
		m_position = {TAILLE_GRILLE-m_rayon,m_position.getY()+(TAILLE_GRILLE-m_position.getX()-m_rayon)*m_vitesse.getY()/(m_vitesse.getX())};
		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
	} else if(m_position.getX()<m_rayon)
	{
		m_position = {m_rayon,m_position.getY()+(m_rayon-m_position.getX())*m_vitesse.getY()/(m_vitesse.getX())};
		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
	} else
	{
		m_position -= m_vitesse;
	}

	if(Champ_de_vitesses[(m_position+m_vitesse).entier().getX()][(m_position+m_vitesse).entier().getY()] == -1)
		return ;

	indiv_alentours = simul->alentours(1, m_position.getX(), m_position.getY());
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

bool Individu::move()
{
	if(m_pylone)
		return false;

	int xp = m_position.entier().getX();
	int yp = m_position.entier().getY();
	if(Champ_de_vitesses.at(xp).at(yp) == -1)
		return true;

	m_position += m_vitesse;
	xp = m_position.entier().getX();
	yp = m_position.entier().getY();

	if(Champ_de_vitesses.at(xp).at(yp) == -1)
	{
		m_position -= m_vitesse;
		return true;
	}

	return false;
}

void Individu::display(sf::RenderWindow &window)
{

	sf::CircleShape cercle(10*m_rayon);
	if(m_pylone)
	{
		cercle.setFillColor(sf::Color(0,0,0));
	} else {
		cercle.setFillColor(sf::Color(250,10,20));
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
	{
		i++;
	}
	return i;
}
