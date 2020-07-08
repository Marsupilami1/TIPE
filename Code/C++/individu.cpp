#include "individu.hpp"
#include <iostream>
#include <chrono>
#include <random>

const double PI = 3.14159265358979323846264338427950;

// Génération d'une loi de répartition normale
std::default_random_engine generator (std::chrono::system_clock::now().time_since_epoch().count());
std::normal_distribution<double> distribution(0.,PI/12.);

std::vector<Individu*>* Individu::m_liste[TAILLE_GRILLE][TAILLE_GRILLE];
int Individu::m_compteur = 0;

extern std::vector<std::vector<int>> Champ_de_vitesses;

Individu::Individu(double pos_x, double pos_y, double rayon, double rayon_repulsion, double rayon_CdV, bool is_pylone)
{
	m_position = {pos_x, pos_y};
	m_vitesse = {0.,0.};
	m_rayon = rayon;
	m_rayon_repulsion = rayon_repulsion;
	m_rayon_suivi = rayon_CdV;
	m_pylone = is_pylone;
    if(!is_pylone)
		m_compteur++;
	
	m_liste[(int)pos_x][(int)pos_y]->push_back(this);
}

Individu::~Individu()
{
	m_compteur--;
	int x = m_position.getX();
	int y = m_position.getY();
	m_liste[x][y]->erase(m_liste[x][y]->begin()+recherche(m_liste[x][y],this));
}

void Individu::afficher()
{
	std::cout << "Position : " << m_position;
}

std::vector<Individu*>* Individu::getVecteursCase(int x, int y)
{
	return m_liste[x][y];
}

void Individu::calculVitesse()
{
	if(m_pylone)
	{
	    m_vitesse = {0,0};
	} else {
    	double alpha = 0.1; // Vitesse du champ de vitesse
    	double beta = 0.3; // Influence des autres individus
    	//~ double gamma = 0.2; // Inertie
    	//~ double delta = 0.1; // Transfert de vitesse à un autre individu
    	int run = 0;
    	std::vector<Vect> liste_vitesse = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    	std::vector<Individu*> indiv_alentours(0);
    	
    	// Vitesse du champs de vitesses
    	int x = m_position.getX();
    	int y = m_position.getY();
    	int v_chemin = Champ_de_vitesses[x][y];
    	Vect vit = {0,0};
    	if(v_chemin > -1)
    		vit = liste_vitesse[v_chemin];
    	m_vitesse = alpha*vit.normalise();
    	m_vitesse.rotate(distribution(generator));
    	
    	// Influence des autres
    	indiv_alentours = alentours(m_rayon_suivi);
    	run=indiv_alentours.size();
    	Vect influence = {0,0};
    	for(int i=0; i<run; i++)
    	{
    		if(attraction(indiv_alentours[i]))
    		{
    			influence += (indiv_alentours[i]->getVit())/((indiv_alentours[i]->getPos()-m_position).norme());
    		}
    	}
    	m_vitesse += beta*influence;
    	m_vitesse = m_vitesse.normalise()*alpha;
    	
    	// Si Approche d'un autre
    	if(Champ_de_vitesses[(m_position+m_vitesse).entier().getX()][(m_position+m_vitesse).entier().getY()] == -1)
    		return ;
    	
    	indiv_alentours = alentours(m_rayon_repulsion);
    	run=indiv_alentours.size();
    	for(int i=0; i<run; i++)
    	{
    		if(repulsion(indiv_alentours[i]) && indiv_alentours[i]->isPylone())
    		{
    			if((indiv_alentours[i]->getVit()-m_position)%m_vitesse < 0)
					m_vitesse.rotate(PI/4.);
				else
					m_vitesse.rotate(-PI/4.);
				break;
    		}
    	}
    	for(int i=0; i<run; i++)
    	{
    		if(repulsion(indiv_alentours[i]))
    		{
    			m_vitesse /= 2;
    			break;
    		}
    	}
    	
    	
    	// Si sortie de zone
    	m_position += m_vitesse;
    	if((m_position).getY()+m_rayon>=TAILLE_GRILLE)
    	{
    		m_position = {m_position.getX()+(TAILLE_GRILLE-m_position.getY()-m_rayon)*m_vitesse.getX()/(m_vitesse.getY()),TAILLE_GRILLE-m_rayon};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
    	} else if(m_position.getY()<m_rayon)
    	{
    		m_position = {m_position.getX()+(m_rayon-m_position.getY())*m_vitesse.getX()/(m_vitesse.getY()),m_rayon};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
    	} else if((m_position).getX()+m_rayon>=TAILLE_GRILLE)
    	{
    		m_position = {TAILLE_GRILLE-m_rayon,m_position.getY()+(TAILLE_GRILLE-m_position.getX()-m_rayon)*m_vitesse.getY()/(m_vitesse.getX())};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
    	} else if(m_position.getX()<m_rayon)
    	{
    		m_position = {m_rayon,m_position.getY()+(m_rayon-m_position.getX())*m_vitesse.getY()/(m_vitesse.getX())};
    		m_vitesse = alpha*(liste_vitesse[Champ_de_vitesses[m_position.entier().getX()][m_position.entier().getY()]].normalise());
    	} else
    	{
    		m_position -= m_vitesse;
    	}
    	
    	if(Champ_de_vitesses[(m_position+m_vitesse).entier().getX()][(m_position+m_vitesse).entier().getY()] == -1)
    		return ;
    	
    	
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


bool Individu::move()
{
	if(m_pylone)
	    return false;
	int x = m_position.getX();
	int y = m_position.getY();
	
	m_position += m_vitesse;
	
	int xp = m_position.entier().getX();
	int yp = m_position.entier().getY();
	
	if(Champ_de_vitesses.at(xp).at(yp) == -1)
	{
		m_position -= m_vitesse;
		return true;
	}
	
	if(x!=xp || y!=yp)
	{
		m_liste[x][y]->erase(m_liste[x][y]->begin()+recherche(m_liste[x][y],this));
		m_liste[xp][yp]->push_back(this);
	}
	
	return false;
}

void Individu::display(sf::RenderWindow &window)
{
	
	sf::CircleShape cercle(10*m_rayon);
	if(m_pylone)
	{
	    cercle.setFillColor(sf::Color(0,0,0));
	} else {
    	cercle.setFillColor(sf::Color(250,10,20));
	}
	cercle.setPosition(10*(m_position.getX()-m_rayon), 10*(m_position.getY()-m_rayon));
	cercle.setOutlineThickness(1);
	cercle.setOutlineColor(sf::Color::Black);
	window.draw(cercle);
	
}


int Individu::nbIndiv()
{
	return m_compteur;
}

bool Individu::touch(Individu* indiv) const
{
	return (indiv != this && ((m_position+m_vitesse)-(indiv->m_position)).norme() < m_rayon+indiv->m_rayon);
}

bool Individu::repulsion(Individu* indiv) const
{
	return (indiv != this && m_vitesse*(indiv->m_position-m_position)>0 && (m_position-indiv->m_position).norme() < m_rayon_repulsion);
}

bool Individu::attraction(Individu* indiv) const
{
	return (indiv != this && m_vitesse*(indiv->m_position-m_position)>0 && (m_position-indiv->m_position).norme() < m_rayon_suivi);
}

Vect Individu::getPos()
{
	return m_position;
}

Vect Individu::getVit()
{
	return m_vitesse;
}

double Individu::getX()
{
	return m_position.getX();
}
double Individu::getY()
{
	return m_position.getY();
}
double Individu::getR()
{
	return m_rayon;
}

bool Individu::isPylone()
{
    return m_pylone;
}

std::vector<Individu*> Individu::alentours(int l)
{
	l=(l<1?1:l);
	unsigned int gauche = m_position.getX()-l;
	unsigned int droite = gauche+2*l+1;
	unsigned int bas = m_position.getY()-l;
	unsigned int haut = bas+2*l+1;
	unsigned int t = TAILLE_GRILLE-1;
	std::vector<Individu*> lindiv(0);
	for(unsigned int i=(gauche<0?0:gauche); i<(droite>t?t:droite); i++)
	{
		for(unsigned int j=(bas<0?0:bas); j<(haut>t?t:haut); j++)
		{
			lindiv.insert(lindiv.end(),m_liste[i][j]->begin(),m_liste[i][j]->end());
		}
	}
	return lindiv;
}

int recherche(std::vector<Individu*>* L, Individu* element)
{
	unsigned int i = 0;
	while (L->at(i) != element)
	{
		i++;
	}
	return i;
}

void Individu::init()
{
	for(unsigned int i=0; i<TAILLE_GRILLE; i++)
	{
		for(unsigned int j=0; j<TAILLE_GRILLE; j++)
		{
			m_liste[i][j] = new std::vector<Individu*>;
		}
	}
}
