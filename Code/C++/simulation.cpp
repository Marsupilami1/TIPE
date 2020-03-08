#include "simulation.h"
#include "individu.h"
#include "Carte_des_distances.h"
#include <cstdlib>

Simulation::Simulation(unsigned int taille_grille) : m_taille_grille(taille_grille), m_fenetre(sf::VideoMode(10*taille_grille, 10*taille_grille), "Simulation evacuation", sf::Style::Close | sf::Style::Titlebar), m_sorties(0)
{
	m_fenetre.setFramerateLimit(45);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 5;
	
	m_fenetre.clear(sf::Color::White);
	m_fenetre.display();
}

void Simulation::add_sortie(int x, int y)
{
	m_sorties.push_back({x,y});
}

void Simulation::add_indiv(double x, double y) const
{
	double r = 0.5;
	bool ok = true;
	
	int run = individu::nb_indiv();
	for(int i = 0; i<run; i++)
	{
		individu* ind_lambda = individu::getElementListe(i);
		if((ind_lambda->get_pos()-vect{x,y}).norme()<ind_lambda->get_R()+r)
		{
			ok = false;
			break;
		}
	}
	if(ok)
		new individu(x,y,r,1,4);
}

void Simulation::add_n_indiv(unsigned int n) const
{
	for(unsigned int i=0; i<n; i++)
	{
		add_indiv(m_taille_grille*(rand()/(double)RAND_MAX), m_taille_grille*(rand()/(double)RAND_MAX));
	}
}

void Simulation::run()
{
	bool escape = false;
	bool active = false;
	
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
						calculs_champs(m_taille_grille, m_sorties);
					}
					break;
					
				case(sf::Event::MouseButtonPressed) :
					if(event.mouseButton.button == sf::Mouse::Left)
						add_indiv(event.mouseButton.x/10.0, event.mouseButton.y/10.0);
					else if(event.mouseButton.button == sf::Mouse::Right)
						add_sortie(event.mouseButton.x/10, event.mouseButton.y/10);
					break;
				
				default :
					break;
			}
		}
		
		if(active)
		{
			m_fenetre.clear(sf::Color::White);
			
			// Individus
			int run = individu::nb_indiv();
			for(int i = 0; i<run; i++)
			{
				individu::getElementListe(i)->calcul_vitesse();
				escape = individu::getElementListe(i)->move();
				if(escape)
				{
					i--;
					run--;
				} else {
					individu::getElementListe(i)->Display(m_fenetre);
				}
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
		} else {
			m_fenetre.clear(sf::Color::White);
			int run = individu::nb_indiv();
			for(int i = 0; i<run; i++)
			{
				individu::getElementListe(i)->Display(m_fenetre);
			}
			for(unsigned int i=0; i<m_sorties.size(); i++)
			{
				sf::RectangleShape rectangle(sf::Vector2f(10, 10));
				rectangle.setPosition(sf::Vector2f(10*m_sorties.at(i).get_X(), 10*m_sorties.at(i).get_Y()));
				rectangle.setFillColor(sf::Color(0,170,30));
				m_fenetre.draw(rectangle);
			}
			m_fenetre.display();
		}
    }
    
	if(m_fenetre.isOpen())
	{
		std::cout << "Simulation Terminée" << std::endl;
		m_fenetre.close();
	}
}
