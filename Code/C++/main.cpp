#include "individu.hpp"
#include "simulation.hpp"
#include <ctime>

extern const unsigned int TAILLE_GRILLE; // Défini dans individu.h

int main(int argc, char* argv[])
{
	srand(time(NULL));
	Simulation simul(TAILLE_GRILLE);
	for(double i=0.5; i<TAILLE_GRILLE; i+=1)
	{
		simul.addPylone(0.5,i);
		simul.addPylone(TAILLE_GRILLE-0.5,i);
		simul.addPylone(i,TAILLE_GRILLE-0.5);
		simul.addPylone(i,0.5);
	}
	simul.addNIndiv(800);
	simul.addSortie(0, 20);

	simul.run();

	return 0;
}
