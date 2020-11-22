#ifndef INDIVIDU_HPP
#define INDIVIDU_HPP

#include "vect.hpp"
#include "champ_de_vitesse.hpp"
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

template <unsigned int TAILLE_GRILLE>
class Simulation;
class Individu
{
	public :
		Individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, bool is_pylone);
	
		void calculVitesse(std::vector<std::vector<Vect>>* champ_vitesses, std::vector<Individu*>** indiv_liste);
		bool move(std::vector<std::vector<Vect>>* champ_vitesses);
		void display(sf::RenderWindow &window, sf::Color color);

		bool touch(Individu* indiv) const;
		bool repulsion(Individu* indiv) const;

		Vect getPos();
		Vect getVit();
		double getX();
		double getY();
		double getR();
		bool isPylone();

	private :
		Vect m_position;
		Vect m_vitesse;
		double m_rayon;
		double m_rayon_repulsion;
		bool m_pylone;
};

int recherche(std::vector<Individu*>* L, Individu* element);
std::vector<Individu*> alentours(unsigned int t,	\
				 std::vector<Individu*>** liste_indiv,	\
				 int l, unsigned int x, unsigned int y);

#endif /* INDIVIDU_HPP */
