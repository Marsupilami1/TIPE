#ifndef DEF_SIMULATION
#define DEF_SIMULATION

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "vect.hpp"

class Simulation
{
	public :
		Simulation(unsigned int taille_grille);
		void addSortie(int x, int y);
		void addPylone(double x, double y);
		void addIndiv(double x, double y) const;
		void addNIndiv(unsigned int n) const;
		void run();
	
	private :
		unsigned int m_taille_grille;
		sf::RenderWindow m_fenetre;
		std::vector<Vect> m_sorties;
		std::vector<Vect> m_murs;
};

#endif
