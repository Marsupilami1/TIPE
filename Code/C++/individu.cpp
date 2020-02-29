#include "individu.h"
#include <iostream>

std::vector<individu*> individu::m_Liste(0);
extern std::vector<std::vector<int>> Champs_de_vitesses;

individu::individu(double pos_x, double pos_y, double rayon, double rayon_CdV)
{
	m_position = {pos_x, pos_y};
	m_vitesse = {0.,0.};
	m_rayon = rayon;
	m_CdV = rayon_CdV;
	
	//~ static std::vector<individu> Liste(0);
	
	m_Liste.push_back(this);
}


void individu::afficher()
{
	std::cout << "Position : " << m_position;
}

individu* individu::getElementListe(int k)
{
	return m_Liste.at(k);
}

void individu::calcul_vitesse()
{
	int x = m_position.get_X();
	int y = m_position.get_Y();
	int v_chemin = Champs_de_vitesses[x][y];
	vect vit;
	switch(v_chemin)
	{
		case 0 :
			vit = {1, 0};
			break;
		case 1 :
			vit = {1, -1};
			break;
		case 2 :
			vit = {0, -1};
			break;
		case 3:
			vit = {-1, -1};
			break;
		case 4 :
			vit = {-1, 0};
			break;
		case 5:
			vit = {-1, 1};
			break;
		case 6:
			vit = {0, 1};
			break;
		case 7 :
			vit = {1, 1};
			break;
		case -1 :
			vit = {0,0};
			break;
	}
	m_vitesse = 0.5*vit;
}

bool individu::move()
{
	m_position += m_vitesse;
	
	int x = m_position.get_X();
	int y = m_position.get_Y();
	if(Champs_de_vitesses[x][y] == -1)
	{
		m_Liste.erase(m_Liste.begin()+recherche(m_Liste, this));
		delete this;
		return true;
	}
	return false;
}

void individu::Display(sf::RenderWindow &window)
{
	sf::CircleShape cercle(10*m_rayon);
	cercle.setFillColor(sf::Color(250,10,20));
	cercle.setPosition(10*m_position.get_X(), 10*m_position.get_Y());
	cercle.setOutlineThickness(1);
	cercle.setOutlineColor(sf::Color::Black);
	window.draw(cercle);
}


int individu::nb_indiv()
{
	return m_Liste.size();
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

int recherche(std::vector<individu*> const& L, individu* element)
{
	unsigned int i = 0;
	while (L.at(i) != element)
	{
		i++;
	}
	return i;
}
