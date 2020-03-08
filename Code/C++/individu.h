#ifndef DEF_INDIVIDU
#define DEF_INDIVIDU

#include <vector>
#include "vect.h"
#include "Carte_des_distances.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class individu
{
	public :
	individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV);
	
	void afficher();
	static individu* getElementListe(int k);
	void calcul_vitesse();
	bool move();
	void Display(sf::RenderWindow &window);
	static int nb_indiv();
	vect get_pos();
	double get_X();
	double get_Y();
	double get_R();
	
	
	private :
	vect m_position;
	vect m_vitesse;
	double m_rayon;
	double m_rayon_repulsion;
	double m_rayon_suivi;
	static std::vector<individu*> m_Liste; // Liste des indivs...
};

int recherche(std::vector<individu*> const& L, individu* element);

#endif
