#ifndef DEF_INDIVIDU
#define DEF_INDIVIDU

#include <vector>
#include "vect.hpp"
#include "champ_de_vitesse.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

const unsigned int TAILLE_GRILLE = 40;

class Individu
{
	public :
		Individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV, bool is_pylone);
		~Individu();
		
		void afficher();
		static std::vector<Individu*>* getVecteursCase(int x, int y);
		void calculVitesse();
		bool move();
		void display(sf::RenderWindow &window);
		static int nbIndiv();
		bool touch(Individu* indiv) const;
		bool repulsion(Individu* indiv) const;
		bool attraction(Individu* indiv) const;
		Vect getPos();
		Vect getVit();
		double getX();
		double getY();
		double getR();
		bool isPylone();
		std::vector<Individu*> alentours(int l);
		static void init();
	
	
	private :
		Vect m_position;
		Vect m_vitesse;
		double m_rayon;
		double m_rayon_repulsion;
		double m_rayon_suivi;
		bool m_pylone;
		static std::vector<Individu*>* m_liste[TAILLE_GRILLE][TAILLE_GRILLE]; // Liste des indivs par case
		static int m_compteur;
};

int recherche(std::vector<Individu*>* L, Individu* element);

#endif
