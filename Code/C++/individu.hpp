#ifndef DEF_INDIVIDU
#define DEF_INDIVIDU

#include <vector>
#include "vect.hpp"
#include "champ_de_vitesse.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const unsigned int TAILLE_GRILLE = 40;
template <unsigned int TAILLE_GRILLE>
class Simulation;
class Individu
{
	public :
		Individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV, bool is_pylone);
	
		void afficher();
		void calculVitesse(Simulation<40>* simul);
		bool move();
		void display(sf::RenderWindow &window);
		bool touch(Individu* indiv) const;
		bool repulsion(Individu* indiv) const;
		bool attraction(Individu* indiv) const;
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
		double m_rayon_suivi;
		bool m_pylone;
};

int recherche(std::vector<Individu*>* L, Individu* element);

#endif
