#include "individu.h"
#include "simulation.h"
#include <ctime>

extern const unsigned int TAILLE_GRILLE; // Défini dans individu.h

int main(int argc, char* argv[])
{
	srand(time(NULL));
	Simulation simul(TAILLE_GRILLE);
	//~ simul.add_n_indiv(250);
	
	simul.run();
	
	return 0;
}
