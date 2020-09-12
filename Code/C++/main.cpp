#include "simulation.hpp"
#include "individu.hpp"
#include <ctime>

// extern const unsigned int TAILLE_GRILLE; // Défini dans individu.h

int main(int argc, char* argv[])
{
	srand(time(NULL));
	Simulation<40> simul;

	for(double i=0.5; i<TAILLE_GRILLE; i+=1)
	{
		simul.addPylone(0.5,i);
		simul.addPylone(TAILLE_GRILLE-0.5,i);
		simul.addPylone(i,TAILLE_GRILLE-0.5);
		simul.addPylone(i,0.5);
	}
	simul.addNIndiv(800);
	simul.addSortie(20, 39);
	simul.addSortie(19, 39);

	simul.run();

	return 0;
}
