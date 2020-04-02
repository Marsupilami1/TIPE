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
	individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV); //Def individu
	~individu(); //Destructeur 
	
	void afficher();
	static individu* getElementListe(int k); //On applique direct sur la classe
	void calcul_vitesse();
	bool move(); //Est-il sortit ?
	void Display(sf::RenderWindow &window);
	static int nb_indiv();
	bool touch(individu const& indiv) const; //2 mecs se touchent ?
	bool repulsion(individu const& indiv) const; //Zone de répulsion vide ?
	vect get_pos();
	vect get_vit();
	double get_X(); // <-> get_pos().get_X()
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
 /* Algo à refaire */
int recherche(std::vector<individu*> const& L, individu* element); //Recherche un individus dans la liste

#endif
