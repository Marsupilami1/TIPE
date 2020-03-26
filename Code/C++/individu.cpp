#include "individu.h"
#include <iostream>
#include <chrono>
#include <random>

const double PI = 3.14159265358979323846264338427950;
extern unsigned int TAILLE_GRILLE; //extern -> chope ds un autre fichier

// Génération d'une loi de répartition normale
std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> distribution(0.,PI/12.);


std::vector<individu*> individu::m_Liste(0);
extern std::vector<std::vector<int>> Champs_de_vitesses; //Chmps de vit : tableau bi-dim d'entiers

individu::individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV)
{
	m_position = {pos_x, pos_y};
	m_vitesse = {0.,0.};
	m_rayon = rayon;
	m_rayon_repulsion = rayon_repulsion;
	m_rayon_suivi = rayon_CdV;
	
	m_Liste.push_back(this);
}

individu::~individu() //Def le destructeur pour l'enlever de la liste
{
	m_Liste.erase(m_Liste.begin()+recherche(m_Liste, this)); //"""Efface l'add de l'element de l'individu"""
}


void individu::afficher()
{
	std::cout << "Position : " << m_position;
}

individu* individu::getElementListe(int k)
{
	return m_Liste.at(k); // <-> [k] sans le out of range
}

//!! Règles de déplacements
void individu::calcul_vitesse()
{
	double alpha = 0.5;
	// Vitesse du champs de vitesses
	int x = m_position.get_X();
	int y = m_position.get_Y();
	int v_chemin = Champs_de_vitesses[x][y];
	vect vit = {0,0};
	if(v_chemin > -1)
	{
		vit = std::vector<vect> {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}}[v_chemin];
	}
	m_vitesse = alpha*vit;
	/* A revoir */
	m_vitesse.rotate(distribution(generator));
	
	
	// Si Approche d'un autre
	/* A revoir*/
	if(Champs_de_vitesses[(m_position+m_vitesse).entier().get_X()][(m_position+m_vitesse).entier().get_Y()] != -1) //Si il sort:
	{
		int run=nb_indiv();
		for(int i=0; i<run; i++)
		{
			if(repulsion(*(individu::getElementListe(i))))
			{
				m_vitesse = (individu::getElementListe(i)->get_vit())*0.3;
				break;
			}
		}
	}
	
	// Si sortie de zone
	/* A revoir pour les coins */
	m_position += m_vitesse;
	//Application du pdf
	if((m_position).get_Y()+m_rayon>=TAILLE_GRILLE)
	{
		m_position = {m_position.get_X()+(TAILLE_GRILLE-m_position.get_Y()-m_rayon)*m_vitesse.get_X()/(m_vitesse.get_Y()),TAILLE_GRILLE-m_rayon};
		m_vitesse = {m_vitesse.get_X(),-m_vitesse.get_Y()};
	} else if(m_position.get_Y()<m_rayon)
	{
		m_position = {m_position.get_X()+(m_rayon-m_position.get_Y())*m_vitesse.get_X()/(m_vitesse.get_Y()),m_rayon};
		m_vitesse = {m_vitesse.get_X(),-m_vitesse.get_Y()};
	} else if((m_position).get_X()+m_rayon>=TAILLE_GRILLE)
	{
		m_position = {TAILLE_GRILLE-m_rayon,m_position.get_Y()+(TAILLE_GRILLE-m_position.get_X()-m_rayon)*m_vitesse.get_Y()/(m_vitesse.get_X())};
		m_vitesse = {-m_vitesse.get_X(),m_vitesse.get_Y()};
	} else if(m_position.get_X()<m_rayon)
	{
		m_position = {m_rayon,m_position.get_Y()+(m_rayon-m_position.get_X())*m_vitesse.get_Y()/(m_vitesse.get_X())};
		m_vitesse = {-m_vitesse.get_X(),m_vitesse.get_Y()};
	} else
	{
		m_position -= m_vitesse;
	}
	
	// Si collision (!) -> possibilité de rester bloquer, même sans collision.
	if(Champs_de_vitesses[(m_position+m_vitesse).entier().get_X()][(m_position+m_vitesse).entier().get_Y()] != -1)
	{
		int run=nb_indiv();
		for(int i=0; i<run; i++)
		{
			if(touch(*(individu::getElementListe(i))))
			{
				m_vitesse = {0,0};
				break;
			}
		}
	}
}

//Actualise la position après le mouvement
bool individu::move()
{
	m_position += m_vitesse;
	
	int x = m_position.entier().get_X();
	int y = m_position.entier().get_Y();
	std::cout << "this : " << this << std::endl << "x : " << x << std::endl << "y : " << y;
	std:: cout << std::endl;
	if(Champs_de_vitesses.at(x).at(y) == -1) //Si il sort...
	{
		return true;
	}
	return false;
}

//Affichage
void individu::Display(sf::RenderWindow &window)
{
	sf::CircleShape cercle(10*m_rayon);
	cercle.setFillColor(sf::Color(250,10,20)); //Petit bonhomme en rouge
	cercle.setPosition(10*(m_position.get_X()-m_rayon), 10*(m_position.get_Y()-m_rayon));
	cercle.setOutlineThickness(1);
	cercle.setOutlineColor(sf::Color::Black);
	window.draw(cercle);
}


int individu::nb_indiv()
{
	return m_Liste.size();
}

bool individu::touch(individu const& indiv) const
{
	return (&indiv != this && (m_position-indiv.m_position).norme() < m_rayon+indiv.m_rayon);
}

bool individu::repulsion(individu const& indiv) const
{
	return (&indiv != this && m_vitesse*(indiv.m_position-m_position)>0 &&(m_position-indiv.m_position).norme() < m_rayon_repulsion);
}

vect individu::get_pos()
{
	return m_position;
}

vect individu::get_vit()
{
	return m_vitesse;
}

double individu::get_X()
{
	return m_position.get_X();
}
double individu::get_Y()
{
	return m_position.get_Y();
}
double individu::get_R()
{
	return m_rayon;
}

/* A revoir (dicho ?) */
int recherche(std::vector<individu*> const& L, individu* element)
{
	unsigned int i = 0;
	while (L.at(i) != element)
	{
		i++;
	}
	return i;
}

