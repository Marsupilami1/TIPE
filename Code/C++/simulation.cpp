#include <iostream>
#include <fstream>
#include <ctime>

#include "simulation.h"
#include "individu.h"
#include "Carte_des_distances.h"
#include <cstdlib>

extern const unsigned int TAILLE_GRILLE;
extern std::vector<vect> Distances_sorties;
int indiv;
std::ofstream resultats;

Simulation::Simulation(unsigned int taille_grille) : m_taille_grille(taille_grille), m_sorties(0)
{
    individu::init();
    indiv=500;
    resultats.open ("TIPE.txt");
}

void Simulation::add_sortie(int x, int y)
{
	m_sorties.push_back({x,y});
}

/* Pylones
void Simulation::add_pylone(double x, double y)
{
	double r = 1;
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
		new individu(x,y,r,0,0, true);
}*/

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
	    double indiv_x = 2.+(m_taille_grille-4)*(rand()/(double)RAND_MAX);
        double indiv_y = 2.+(m_taille_grille-4)*(rand()/(double)RAND_MAX);
        if (indiv_x!=10 || indiv_y!=10)
            add_indiv (indiv_x,indiv_y);
	}
}

void Simulation::run()
{
    time_t starting_time;
	bool escape = false;
	std::vector<individu*>* sousListe;
	int iter;
	individu* ptr_indiv = NULL;

	starting_time = time(NULL);

    add_sortie(10, 10);
    add_n_indiv(indiv);
    calculs_champs(m_taille_grille, m_sorties);

	while (individu::nb_indiv() != 0)
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
				}
			}
		}
    }

	if(individu::nb_indiv() == 0)
	{
		std::cout << "Simulation terminee en ";
        std::cout << time(NULL)-starting_time;
        std::cout << "s avec ";
        std::cout << indiv;
        std::cout << " individus" << std::endl;
        resultats << indiv;
        resultats << "//";
        resultats << time(NULL)-starting_time;
        resultats << "\n";
        if (indiv < 600)
        {
            indiv+=100;
            run();
        }
        resultats.close();
	}
}
