#ifndef DEF_SIMULATION
#define DEF_SIMULATION

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include "individu.hpp"
#include "vect.hpp"
#include "champ_de_vitesse.hpp"

extern std::vector<Vect> Distances_sorties;
class Individu;

template <unsigned int TAILLE_GRILLE>
class Simulation
{
	public :
		Simulation();
		void addSortie(int x, int y);
		void addPylone(double x, double y);
		void addIndiv(double x, double y);
		void addNIndiv(unsigned int n);
		void run();
		std::vector<Individu*>* getVecteursCase(int x, int y) const;
		std::vector<Individu*> alentours(int l, unsigned int x, unsigned int y);
		std::vector<Individu*>* m_liste[TAILLE_GRILLE][TAILLE_GRILLE];

	private :
		sf::RenderWindow m_fenetre;
		std::vector<Vect> m_sorties;
		std::vector<Vect> m_murs;
		int m_compteur;
};

/* Implementation */
template<unsigned int TAILLE_GRILLE>
Simulation<TAILLE_GRILLE>::Simulation() : m_fenetre(sf::VideoMode(10*TAILLE_GRILLE, 10*TAILLE_GRILLE), "Simulation evacuation", sf::Style::Close | sf::Style::Titlebar), m_sorties(0), m_murs(0)
{
	// m_fenetre.setFramerateLimit(45);

	m_fenetre.clear(sf::Color::White);
	m_fenetre.display();
	for (unsigned int i=0; i<TAILLE_GRILLE; i++) {
		for (unsigned int j=0; j<TAILLE_GRILLE; j++) {
			m_liste[i][j] = new std::vector<Individu*>;
		}
	}
	m_compteur = 0;
}

template<unsigned int TAILLE_GRILLE>
void Simulation<TAILLE_GRILLE>::addSortie(int x, int y)
{
	m_sorties.push_back({x,y});
}

template<unsigned int TAILLE_GRILLE>
void Simulation<TAILLE_GRILLE>::addPylone(double x, double y)
{
	double r = 0.35;
	bool ok = true;
	std::vector<Individu*>* sous_liste;
	Individu* ind_lambda;
	int iter;

	for(unsigned int i=(x-1<0?0:x-1); i<=x+1 && i<TAILLE_GRILLE && ok; i++)
	{
		for(unsigned int j=(y-1<0?0:y-1); j<TAILLE_GRILLE && j<=y+1 && ok; j++)
		{
			sous_liste = getVecteursCase(i,j);
			iter = sous_liste->size();
			for(int k=0; k<iter; k++)
			{
				ind_lambda = sous_liste->at(k);
				if((ind_lambda->getPos()-Vect{x,y}).norme()<ind_lambda->getR()+r) // Si plus proches que leur rayon
				{
					ok = false;
					break;
				}
			}
		}
	}

	if(ok)
	{
		m_murs.push_back({x,y});
		Individu* ptr_indiv = new Individu(x,y,r,0,0, true);
		m_liste[int(x)][int(y)]->push_back(ptr_indiv);
	}
}

template<unsigned int TAILLE_GRILLE>
void Simulation<TAILLE_GRILLE>::addIndiv(double x, double y)
{
	double r = 0.2;
	bool ok = true;
	std::vector<Individu*>* sous_liste;
	Individu* ind_lambda;
	int iter;

	for(unsigned int i=(x-1<0?0:x-1); i<=x+1 && i<TAILLE_GRILLE && ok; i++)
	{
		for(unsigned int j=(y-1<0?0:y-1); j<TAILLE_GRILLE && j<=y+1 && ok; j++)
		{
			sous_liste = getVecteursCase(i,j);
			iter = sous_liste->size();
			for(int k=0; k<iter; k++)
			{
				ind_lambda = sous_liste->at(k);
				if((ind_lambda->getPos()-Vect{x,y}).norme()<ind_lambda->getR()+r) // Si plus proches que leur rayon
				{
					ok = false;
					break;
				}
			}
		}
	}

	if(ok)
	{
		Individu* ptr_indiv = new Individu(x, y, r, 7*r, 14*r, false);
		m_liste[int(x)][int(y)]->push_back(ptr_indiv);
		m_compteur++;
	}
}

template<unsigned int TAILLE_GRILLE>
void Simulation<TAILLE_GRILLE>::addNIndiv(unsigned int n)
{
	for(unsigned int i=0; i<n; i++)
	{
		addIndiv(2.+(TAILLE_GRILLE-4)*(rand()/(double)RAND_MAX), 2.+(TAILLE_GRILLE-4)*(rand()/(double)RAND_MAX));
	}
}

template<unsigned int TAILLE_GRILLE>
void Simulation<TAILLE_GRILLE>::run()
{
	bool escape = false;
	bool active = false;
	std::vector<Individu*>* sous_liste;
	int iter;
	Individu* ptr_indiv = NULL;

	for(unsigned int i=0; i<m_sorties.size(); i++)
	{
		sf::RectangleShape rectangle(sf::Vector2f(10, 10));
		rectangle.setPosition(sf::Vector2f(10*m_sorties.at(i).getX(), 10*m_sorties.at(i).getY()));
		rectangle.setFillColor(sf::Color(0,170,30));
		m_fenetre.draw(rectangle);
	}
	m_fenetre.display();

	while (m_fenetre.isOpen() && ((m_compteur != 0) || !(active)))
	{
		sf::Event event; // Boucle d'événements
		while (m_fenetre.pollEvent(event))
		{
			switch(event.type)
			{
				case(sf::Event::Closed) :
					m_fenetre.close();
					break;

				case(sf::Event::KeyPressed) :
					if(event.key.code == sf::Keyboard::Return)
					{
						active = true;
						calculs_champs(TAILLE_GRILLE, m_sorties, m_murs);
						std::cout << "Calcul du champ terminé" << std::endl;
					}
					break;

				case(sf::Event::MouseButtonPressed) :
					if(event.mouseButton.button == sf::Mouse::Left)
						// addNIndiv(100);
						addIndiv(event.mouseButton.x/10.0, event.mouseButton.y/10.0);
					else if(event.mouseButton.button == sf::Mouse::Right)
						addSortie(event.mouseButton.x/10, event.mouseButton.y/10);
					else if(event.mouseButton.button == sf::Mouse::Middle)
						addPylone(event.mouseButton.x/10 +.5, event.mouseButton.y/10 +.5);
					break;

				default :
					break;
			}
		}

		m_fenetre.clear(sf::Color::White);
		if(active)
		{
			// Mouvement individus
			for(unsigned int i=0; i<TAILLE_GRILLE*TAILLE_GRILLE; i++)
			{
				sous_liste = getVecteursCase(Distances_sorties[i].getX(),Distances_sorties[i].getY());
				iter = sous_liste->size()-1;
				for(int k=iter; k>-1; k--)
				{
					ptr_indiv = sous_liste->at(k);
					ptr_indiv->calculVitesse(this);
					int x = ptr_indiv->getX();
					int y = ptr_indiv->getY();
					escape = ptr_indiv->move();
					int xp = ptr_indiv->getX();
					int yp = ptr_indiv->getY();
					if (x != xp || y != yp)
					{
						m_liste[x][y]->erase(m_liste[x][y]->begin()+recherche(m_liste[x][y],ptr_indiv));
						m_liste[xp][yp]->push_back(ptr_indiv);
					}

					if(escape)
					{
						m_liste[x][y]->erase(m_liste[x][y]->begin()+recherche(m_liste[x][y],ptr_indiv));
						m_compteur--;
						delete ptr_indiv;
					} else {
						ptr_indiv->display(m_fenetre);
					}
				}
			}
		} else {
			for(unsigned int i=0; i<TAILLE_GRILLE; i++)
			{
				for(unsigned int j=0; j<TAILLE_GRILLE; j++)
				{
					sous_liste = getVecteursCase(i,j);
					iter = sous_liste->size();
					for(int k=0; k<iter; k++)
					{
						sous_liste->at(k)->display(m_fenetre);
					}
				}
			}
		}
		//Murs
		for(unsigned int i=0; i<m_murs.size(); i++)
		{
			getVecteursCase(m_murs.at(i).getX(), m_murs.at(i).getY())->at(0)->display(m_fenetre);
		}
		// Sorties
		for(unsigned int i=0; i<m_sorties.size(); i++)
		{
			sf::RectangleShape rectangle(sf::Vector2f(10, 10));
			rectangle.setPosition(sf::Vector2f(10*m_sorties.at(i).getX(), 10*m_sorties.at(i).getY()));
			rectangle.setFillColor(sf::Color(0,170,30));
			m_fenetre.draw(rectangle);
		}
		m_fenetre.display();
	}

	if(m_fenetre.isOpen())
	{
		std::cout << "Simulation Terminée" << std::endl;
		m_fenetre.close();
	}
}

template<unsigned int TAILLE_GRILLE>
std::vector<Individu*>* Simulation<TAILLE_GRILLE>::getVecteursCase(int x, int y) const
{
	return m_liste[x][y];
}

template<unsigned int TAILLE_GRILLE>
std::vector<Individu*> Simulation<TAILLE_GRILLE>::alentours(int l, unsigned int x, unsigned int y)
{
	l= l<1 ? 1 : l;
	unsigned int gauche = x-l;
	unsigned int droite = x+l;
	unsigned int bas = y-l;
	unsigned int haut = y+l;
	unsigned int t = TAILLE_GRILLE-1;
	std::vector<Individu*> lindiv(0);
	for(unsigned int i=(gauche<0?0:gauche); i<=(droite>t?t:droite); i++)
	{
		for(unsigned int j=(bas<0?0:bas); j<=(haut>t?t:haut); j++)
		{
			lindiv.insert(lindiv.end(),m_liste[i][j]->begin(),m_liste[i][j]->end());
		}
	}
	return lindiv;
}

#endif
