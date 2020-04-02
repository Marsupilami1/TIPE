#include "simulation.h"
#include "individu.h"
#include "Carte_des_distances.h"
#include <cstdlib>

Simulation::Simulation(unsigned int taille_grille) : m_taille_grille(taille_grille), m_fenetre(sf::VideoMode(10*taille_grille, 10*taille_grille),
"Simulation evacuation", sf::Style::Close | sf::Style::Titlebar), m_sorties(0)
{
	m_fenetre.setFramerateLimit(45); //Limite framerate
	/*sf::ContextSettings settings;
	settings.antialiasingLevel = 5;*/
	
	m_fenetre.clear(sf::Color::White); //! Fenêtre en blanc 
	m_fenetre.display();
}

void Simulation::add_sortie(int x, int y)
{
	m_sorties.push_back({x,y});
}

void Simulation::add_indiv(double x, double y) const
{
	double r = 0.2; //Rayon de l'individus
	bool ok = true; //Gestion du chevauchement
	
	int run = individu::nb_indiv();
	for(int i = 0; i<run; i++)
	{
		individu* ind_lambda = individu::getElementListe(i); //ind_lambda parcours les individus ds la liste
		//(*ind_lambda).position <-> ind_lambda->position
		if((ind_lambda->get_pos()-vect{x,y}).norme()<ind_lambda->get_R()+r) //Si la norme de la différence des vects est plus petits que la somme des rayons
		{
			ok = false;
			break;
		}
	}
	if(ok)
		new individu(x,y,r,0.6,4); //0.6 et 4 : rayons de répulsions, d'attraction cf individu
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
	
	//Dessine les sorties
	for(unsigned int i=0; i<m_sorties.size(); i++)
	{
		sf::RectangleShape rectangle(sf::Vector2f(10, 10));
		rectangle.setPosition(sf::Vector2f(10*m_sorties.at(i).get_X(), 10*m_sorties.at(i).get_Y()));
		rectangle.setFillColor(sf::Color(0,170,30));
		m_fenetre.draw(rectangle);
	}
	m_fenetre.display();
	
	while (m_fenetre.isOpen() && ((individu::nb_indiv() != 0) || !(active))) //Gestion de la croix rouge et du lancement avec active
    {
		sf::Event event; // Boucle d'événements
		//Gestion des évenements
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
						calculs_champs(m_taille_grille, m_sorties); //cf Cartes des distances
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
				individu::getElementListe(i)->calcul_vitesse();//Calcul de vitesse
				escape = individu::getElementListe(i)->move();//Déplacements
				if(escape)
				{
					delete individu::getElementListe(i);//Ils s'échappent
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
		} else { //Quand c'est pas lancé, on affiche sans rien bouger
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
