#include "simulation.hpp"
#include "individu.hpp"
#include "champ_de_vitesse.hpp"
#include <cstdlib>

extern const unsigned int TAILLE_GRILLE;
extern std::vector<Vect> Distances_sorties;

Simulation::Simulation(unsigned int taille_grille) : m_taille_grille(taille_grille), m_fenetre(sf::VideoMode(10*taille_grille, 10*taille_grille), "Simulation evacuation", sf::Style::Close | sf::Style::Titlebar), m_sorties(0), m_murs(0)
{
	// m_fenetre.setFramerateLimit(25);

	m_fenetre.clear(sf::Color::White);
	m_fenetre.display();
	Individu::init();
}

void Simulation::addSortie(int x, int y)
{
	m_sorties.push_back({x,y});
}

void Simulation::addPylone(double x, double y)
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
			sous_liste = Individu::getVecteursCase(i,j);
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
		new Individu(x,y,r,0,0, true);
	}
}

void Simulation::addIndiv(double x, double y) const
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
			sous_liste = Individu::getVecteursCase(i,j);
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
		new Individu(x, y, r, 7*r, 14*r, false);
}

void Simulation::addNIndiv(unsigned int n) const
{
	for(unsigned int i=0; i<n; i++)
	{
		addIndiv(2.+(m_taille_grille-4)*(rand()/(double)RAND_MAX), 2.+(m_taille_grille-4)*(rand()/(double)RAND_MAX));
	}
}

void Simulation::run()
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

	while (m_fenetre.isOpen() && ((Individu::nbIndiv() != 0) || !(active)))
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
						calculs_champs(m_taille_grille, m_sorties, m_murs);
					}
					break;

				case(sf::Event::MouseButtonPressed) :
					if(event.mouseButton.button == sf::Mouse::Left)
						addNIndiv(100);
						// addIndiv(event.mouseButton.x/10.0, event.mouseButton.y/10.0);
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
				sous_liste = Individu::getVecteursCase(Distances_sorties[i].getX(),Distances_sorties[i].getY());
				iter = sous_liste->size()-1;
				for(int k=iter; k>-1; k--)
				{
					ptr_indiv= sous_liste->at(k);
					ptr_indiv->calculVitesse();
					escape = ptr_indiv->move();
					if(escape)
					{
						delete sous_liste->at(k);
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
					sous_liste = Individu::getVecteursCase(i,j);
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
			Individu::getVecteursCase(m_murs.at(i).getX(), m_murs.at(i).getY())->at(0)->display(m_fenetre);
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
