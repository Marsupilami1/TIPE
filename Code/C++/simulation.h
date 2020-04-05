#ifndef DEF_SIMULATION
#define DEF_SIMULATION

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "vect.h"

class Simulation
{
	public :
	Simulation(unsigned int taille_grille);
	void add_sortie(int x, int y);
	void add_indiv(double x, double y) const;
	void add_n_indiv(unsigned int n) const;
	void run();
	
	private :
	unsigned int m_taille_grille;
	sf::RenderWindow m_fenetre;
	std::vector<vect> m_sorties;
};

#endif
