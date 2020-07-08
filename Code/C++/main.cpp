#include "individu.hpp"
#include "simulation.hpp"
#include <ctime>

extern const unsigned int TAILLE_GRILLE; // Défini dans individu.h

int main(int argc, char* argv[])
{
	srand(time(NULL));
	Simulation simul(TAILLE_GRILLE);
	simul.addNIndiv(250);
	
	simul.run();
	
	return 0;
}
