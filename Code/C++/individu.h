#ifndef DEF_INDIVIDU
#define DEF_INDIVIDU

#include <vector>
#include "vect.h"
#include "Carte_des_distances.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const unsigned int TAILLE_GRILLE = 40;

class individu
{
	public :
	individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV);
	~individu();
	
	void afficher();
	static std::vector<individu*>* getVecteursCase(int x, int y);
	void calcul_vitesse();
	bool move();
	void Display(sf::RenderWindow &window);
	static int nb_indiv();
	bool touch(individu* indiv) const;
	bool repulsion(individu* indiv) const;
	bool attraction(individu* indiv) const;
	vect get_pos();
	vect get_vit();
	double get_X();
	double get_Y();
	double get_R();
	std::vector<individu*> alentours(int l);
	static void init();
	
	
	private :
	vect m_position;
	vect m_vitesse;
	double m_rayon;
	double m_rayon_repulsion;
	double m_rayon_suivi;
	static std::vector<individu*>* m_Liste[TAILLE_GRILLE][TAILLE_GRILLE]; // Liste des indivs par case
	static int compteur;
};

int recherche(std::vector<individu*>* L, individu* element);

#endif
