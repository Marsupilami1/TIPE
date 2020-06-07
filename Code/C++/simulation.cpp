#include "simulation.h"
#include "individu.h"
#include "Carte_des_distances.h"
#include <cstdlib>

extern const unsigned int TAILLE_GRILLE;
extern std::vector<vect> Distances_sorties;

Simulation::Simulation(unsigned int taille_grille) : m_taille_grille(taille_grille), m_fenetre(sf::VideoMode(10*taille_grille, 10*taille_grille), "Simulation evacuation", sf::Style::Close | sf::Style::Titlebar), m_sorties(0), m_murs(0)
{
	m_fenetre.setFramerateLimit(60);
	
	m_fenetre.clear(sf::Color::White);
	m_fenetre.display();
	individu::init();
}

void Simulation::add_sortie(int x, int y)
{
	m_sorties.push_back({x,y});
}

void Simulation::add_pylone(double x, double y)
{
	double r = 0.5;
	bool ok = true;
	std::vector<individu*>* sousListe;
	individu* ind_lambda;
	int iter;
	
	for(unsigned int i=(x-1<0?0:x-1); i<=x+1 && i<TAILLE_GRILLE && ok; i++)
	{
		for(unsigned int j=(y-1<0?0:y-1); j<TAILLE_GRILLE && j<=y+1 && ok; j++)
		{
			sousListe = individu::getVecteursCase(i,j);
			iter = sousListe->size();
			for(int k=0; k<iter; k++)
			{
				ind_lambda = sousListe->at(k);
				if((ind_lambda->get_pos()-vect{x,y}).norme()<ind_lambda->get_R()+r) // Si plus proches que leur rayon...
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
		new individu(x,y,r,0,0, true);
	}
}

void Simulation::add_indiv(double x, double y) const
{
	double r = 0.2;
	bool ok = true;
	std::vector<individu*>* sousListe;
	individu* ind_lambda;
	int iter;
	
	for(unsigned int i=(x-1<0?0:x-1); i<=x+1 && i<TAILLE_GRILLE && ok; i++)
	{
		for(unsigned int j=(y-1<0?0:y-1); j<TAILLE_GRILLE && j<=y+1 && ok; j++)
		{
			sousListe = individu::getVecteursCase(i,j);
			iter = sousListe->size();
			for(int k=0; k<iter; k++)
			{
				ind_lambda = sousListe->at(k);
				if((ind_lambda->get_pos()-vect{x,y}).norme()<ind_lambda->get_R()+r) // Si plus proches que leur rayon...
				{
					ok = false;
					break;
				}
			}
		}
	}
	
	if(ok)
		new individu(x, y, r, 4*r, 10*r, false);
}

void Simulation::add_n_indiv(unsigned int n) const
{
	for(unsigned int i=0; i<n; i++)
	{
		add_indiv(2.+(m_taille_grille-4)*(rand()/(double)RAND_MAX), 2.+(m_taille_grille-4)*(rand()/(double)RAND_MAX));
	}
}

void Simulation::run()
{
	bool escape = false;
	bool active = false;
	std::vector<individu*>* sousListe;
	int iter;
	individu* ptr_indiv = NULL;
	
	for(unsigned int i=0; i<m_sorties.size(); i++)
	{
		sf::RectangleShape rectangle(sf::Vector2f(10, 10));
		rectangle.setPosition(sf::Vector2f(10*m_sorties.at(i).get_X(), 10*m_sorties.at(i).get_Y()));
		rectangle.setFillColor(sf::Color(0,170,30));
		m_fenetre.draw(rectangle);
	}
	m_fenetre.display();
	
	while (m_fenetre.isOpen() && ((individu::nb_indiv() != 0) || !(active)))
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
						//~ add_n_indiv(100);
						add_indiv(event.mouseButton.x/10.0, event.mouseButton.y/10.0);
					else if(event.mouseButton.button == sf::Mouse::Right)
						add_sortie(event.mouseButton.x/10, event.mouseButton.y/10);
					else if(event.mouseButton.button == sf::Mouse::Middle)
						add_pylone(event.mouseButton.x/10 +.5, event.mouseButton.y/10 +.5);
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
				sousListe = individu::getVecteursCase(Distances_sorties[i].get_X(),Distances_sorties[i].get_Y());
				iter = sousListe->size()-1;
				for(int k=iter; k>-1; k--)
				{
					ptr_indiv= sousListe->at(k);
					ptr_indiv->calcul_vitesse();
					escape = ptr_indiv->move();
					if(escape)
					{
						delete sousListe->at(k);
					} else {
						ptr_indiv->Display(m_fenetre);
					}
					
				}
			}
			
		} else {
			for(unsigned int i=0; i<TAILLE_GRILLE; i++)
			{
				for(unsigned int j=0; j<TAILLE_GRILLE; j++)
				{
					sousListe = individu::getVecteursCase(i,j);
					iter = sousListe->size();
					for(int k=0; k<iter; k++)
					{
						sousListe->at(k)->Display(m_fenetre);
					}
				}
			}
		}
		//Murs
		for(unsigned int i=0; i<m_murs.size(); i++)
		{
		    individu::getVecteursCase(m_murs.at(i).get_X(), m_murs.at(i).get_Y())->at(0)->Display(m_fenetre);
		}
		// Sorties
		for(unsigned int i=0; i<m_sorties.size(); i++)
		{
			sf::RectangleShape rectangle(sf::Vector2f(10, 10));
			rectangle.setPosition(sf::Vector2f(10*m_sorties.at(i).get_X(), 10*m_sorties.at(i).get_Y()));
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




