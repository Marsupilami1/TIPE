#include "individu.h"
#include "simulation.h"
#include <ctime>

unsigned int TAILLE_GRILLE = 40;

int main(int argc, char* argv[])
{	
	srand(time(NULL));
	Simulation simul(TAILLE_GRILLE);
	simul.add_n_indiv(100);
	
	simul.run();
	
	return 0;
}
