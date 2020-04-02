#ifndef DEF_SIMULATION
#define DEF_SIMULATION

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector> //Listes dynamiques
#include "vect.h" //Classe de vect

class Simulation
{
	public :
	Simulation(unsigned int taille_grille); //Constructeur
	void add_sortie(int x, int y); //Sorties
	void add_indiv(double x, double y) const; //Rajoute un individus
	void add_n_indiv(unsigned int n) const;
	void run();
	
	private :
	unsigned int m_taille_grille; //m_ -> privé
	sf::RenderWindow m_fenetre; //bib SFML
	std::vector<vect> m_sorties; //Liste dynamique de sortie -> contient des vecteurs
};

#endif
