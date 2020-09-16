#include "simulation.hpp"
#include "individu.hpp"
#include <ctime>

// extern const unsigned int TAILLE_GRILLE; // Défini dans individu.h

int main(int argc, char* argv[])
{
	srand(time(NULL));
	const unsigned int TAILLE_GRILLE = 40;
	Simulation<TAILLE_GRILLE > simul;

	for(unsigned int i=0; i<TAILLE_GRILLE-1; i++)
	{
		simul.addPylone(0,i);
		simul.addPylone(TAILLE_GRILLE-1,TAILLE_GRILLE-1-i);
		simul.addPylone(i,TAILLE_GRILLE-1);
		simul.addPylone(TAILLE_GRILLE-1-i,0);
	}
	// simul.addNIndiv(800);
	// simul.addSortie(20, 39);
	// simul.addSortie(19, 39);

	simul.run();

	return 0;
}
