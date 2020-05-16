#include "individu.h"
#include <iostream>
#include <chrono>
#include <random>

const double PI = 3.14159265358979323846264338427950;

// Génération d'une loi de répartition normale
std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> distribution(0.,PI/12.);

std::vector<individu*>* individu::m_Liste[TAILLE_GRILLE][TAILLE_GRILLE];
int individu::compteur = 0;

extern std::vector<std::vector<int>> Champ_de_vitesses;

individu::individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV, bool is_pylone)
{
	m_position = {pos_x, pos_y};
	m_vitesse = {0.,0.};
	m_rayon = rayon;
	m_rayon_repulsion = rayon_repulsion;
	m_rayon_suivi = rayon_CdV;
	m_pylone = is_pylone;
    
	compteur++;
	
	m_Liste[(int)pos_x][(int)pos_y]->push_back(this);
}

individu::~individu()
{
	compteur--;
	int x = m_position.get_X();
	int y = m_position.get_Y();
	m_Liste[x][y]->erase(m_Liste[x][y]->begin()+recherche(m_Liste[x][y],this));
}

void individu::afficher()
{
	std::cout << "Position : " << m_position;
}

std::vector<individu*>* individu::getVecteursCase(int x, int y)
{
	return m_Liste[x][y];
}

void individu::calcul_vitesse()
{
	if(m_pylone)
	{
	    m_vitesse = {0,0};
	} else {
    	double alpha = 0.1; // Vitesse du champ de vitesse
    	double beta = 0.4; // Influence des autres individus
    	//~ double gamma = 0.1; // Transfert de vitesse à un autre individu
    	int run = 0;
    	std::vector<vect> liste_vitesse = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    	std::vector<individu*> indiv_alentours(0);
    	
    	// Vitesse du champs de vitesses
    	int x = m_position.get_X();
    	int y = m_position.get_Y();
    	int v_chemin = Champ_de_vitesses[x][y];
    	vect vit = {0,0};
    	if(v_chemin > -1)
    		vit = liste_vitesse[v_chemin];
    	m_vitesse = alpha*(vit.normalise());
    	m_vitesse.rotate(distribution(generator));
    	
    	// Influence des autres
    	indiv_alentours = alentours(m_rayon_suivi);
    	run=indiv_alentours.size();
    	vect influence = {0,0};
    	for(int i=0; i<run; i++)
    	{
    		if(attraction(indiv_alentours[i]))
    		{
    			influence += (indiv_alentours[i]->get_vit())/((indiv_alentours[i]->get_pos()-m_position).norme());
    		}
    	}
    	m_vitesse += beta*influence;
    	m_vitesse = m_vitesse.normalise()*alpha;
    	
    	// Si Approche d'un autre
    	if(Champ_de_vitesses[(m_position+m_vitesse).entier().get_X()][(m_position+m_vitesse).entier().get_Y()] == -1)
    		return ;
    	
    	int nb_gauche = 0;
    	int nb_droite = 0;
    	indiv_alentours = alentours(m_rayon_repulsion);
    	run=indiv_alentours.size();
    	for(int i=0; i<run; i++)
    	{
    		if(repulsion(indiv_alentours[i]))
    		{
    			if((indiv_alentours[i]->get_pos()-m_position)%m_vitesse>0)
					nb_gauche++;
    			    //nb_gauche += indiv_alentours[i]->get_R();
    			else
					nb_droite++;
    			    //nb_droite += indiv_alentours[i]->get_R();
    		}
    	}
    	if(nb_gauche+nb_droite>0)
    	{
    	    m_vitesse.rotate((nb_gauche>nb_droite? 1 : -1)*PI/4);
    	    m_vitesse /= 2;
    	}
    	
    	
    	// Si sortie de zone
    	m_position += m_vitesse;
    	if((m_position).get_Y()+m_rayon>=TAILLE_GRILLE)
    	{
    		m_position = {m_position.get_X()+(TAILLE_GRILLE-m_position.get_Y()-m_rayon)*m_vitesse.get_X()/(m_vitesse.get_Y()),TAILLE_GRILLE-m_rayon};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().get_X()][m_position.entier().get_Y()]].normalise());
    	} else if(m_position.get_Y()<m_rayon)
    	{
    		m_position = {m_position.get_X()+(m_rayon-m_position.get_Y())*m_vitesse.get_X()/(m_vitesse.get_Y()),m_rayon};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().get_X()][m_position.entier().get_Y()]].normalise());
    	} else if((m_position).get_X()+m_rayon>=TAILLE_GRILLE)
    	{
    		m_position = {TAILLE_GRILLE-m_rayon,m_position.get_Y()+(TAILLE_GRILLE-m_position.get_X()-m_rayon)*m_vitesse.get_Y()/(m_vitesse.get_X())};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().get_X()][m_position.entier().get_Y()]].normalise());
    	} else if(m_position.get_X()<m_rayon)
    	{
    		m_position = {m_rayon,m_position.get_Y()+(m_rayon-m_position.get_X())*m_vitesse.get_Y()/(m_vitesse.get_X())};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().get_X()][m_position.entier().get_Y()]].normalise());
    	} else
    	{
    		m_position -= m_vitesse;
    	}
    	
    	if(Champ_de_vitesses[(m_position+m_vitesse).entier().get_X()][(m_position+m_vitesse).entier().get_Y()] == -1)
    		return ;
    	
    	// Si collision (!) -> possibilité de rester bloqué, même sans collision.
    	indiv_alentours = alentours(1);
    	run=indiv_alentours.size();
    	for(int i=0; i<run; i++)
    	{
    		if(touch(indiv_alentours[i]))
    		{
    			m_vitesse = {0,0};
    			break;
    		}
    	}
	}
}


bool individu::move()
{
	if(m_pylone)
	    return false;
	int x = m_position.get_X();
	int y = m_position.get_Y();
	
	m_position += m_vitesse;
	
	int xp = m_position.entier().get_X();
	int yp = m_position.entier().get_Y();
	
	if(Champ_de_vitesses.at(xp).at(yp) == -1)
	{
		m_position -= m_vitesse;
		return true;
	}
	
	if(x!=xp || y!=yp)
	{
		m_Liste[x][y]->erase(m_Liste[x][y]->begin()+recherche(m_Liste[x][y],this));
		m_Liste[xp][yp]->push_back(this);
	}
	
	return false;
}

void individu::Display(sf::RenderWindow &window)
{
	
	sf::CircleShape cercle(10*m_rayon);
	if(m_pylone)
	{
	    cercle.setFillColor(sf::Color(0,0,0));
	} else {
    	cercle.setFillColor(sf::Color(250,10,20));
	}
	cercle.setPosition(10*(m_position.get_X()-m_rayon), 10*(m_position.get_Y()-m_rayon));
	cercle.setOutlineThickness(1);
	cercle.setOutlineColor(sf::Color::Black);
	window.draw(cercle);
	
}


int individu::nb_indiv()
{
	return compteur;
}

bool individu::touch(individu* indiv) const
{
	return (indiv != this && ((m_position+m_vitesse)-(indiv->m_position+indiv->m_vitesse)).norme() < m_rayon+indiv->m_rayon);
}

bool individu::repulsion(individu* indiv) const
{
	return (indiv != this && m_vitesse*(indiv->m_position-m_position)>0 && (m_position-indiv->m_position).norme() < m_rayon_repulsion);
}

bool individu::attraction(individu* indiv) const
{
	return (indiv != this && m_vitesse*(indiv->m_position-m_position)>0 && (m_position-indiv->m_position).norme() < m_rayon_suivi);
}

vect individu::get_pos()
{
	return m_position;
}

vect individu::get_vit()
{
	return m_vitesse;
}

double individu::get_X()
{
	return m_position.get_X();
}
double individu::get_Y()
{
	return m_position.get_Y();
}
double individu::get_R()
{
	return m_rayon;
}

std::vector<individu*> individu::alentours(int l)
{
	l=(l<1?1:l);
	unsigned int gauche = m_position.get_X()-l;
	unsigned int droite = gauche+2*l+1;
	unsigned int bas = m_position.get_Y()-l;
	unsigned int haut = bas+2*l+1;
	unsigned int t = TAILLE_GRILLE-1;
	std::vector<individu*> lindiv(0);
	for(unsigned int i=(gauche<0?0:gauche); i<(droite>t?t:droite); i++)
	{
		for(unsigned int j=(bas<0?0:bas); j<(haut>t?t:haut); j++)
		{
			lindiv.insert(lindiv.end(),m_Liste[i][j]->begin(),m_Liste[i][j]->end());
		}
	}
	return lindiv;
}

int recherche(std::vector<individu*>* L, individu* element)
{
	unsigned int i = 0;
	while (L->at(i) != element)
	{
		i++;
	}
	return i;
}

void individu::init()
{
	for(unsigned int i=0; i<TAILLE_GRILLE; i++)
	{
		for(unsigned int j=0; j<TAILLE_GRILLE; j++)
		{
			m_Liste[i][j] = new std::vector<individu*>;
		}
	}
}
