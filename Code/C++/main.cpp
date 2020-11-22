#include "simulation.hpp"
#include "individu.hpp"
#include <ctime>

int main(int argc, char* argv[])
{
	srand(time(NULL));

	const unsigned int T = 50; // Taille de la grille
	Simulation<T> simul;

	for(unsigned int i=0; i<T-1; i++) // Contour de pylones
	{
		simul.addPylone(0,i);
		simul.addPylone(T-1,T-1-i);
		simul.addPylone(i,T-1);
		simul.addPylone(T-1-i,0);

		simul.addPylone(1,i);
		simul.addPylone(T-2,T-2-i);
		simul.addPylone(i,T-2);
		simul.addPylone(T-2-i,1);
	}

	// simul.addNIndiv(800);
	// simul.addSortie(T/2, T-1);
	// simul.addSortie(T/2 -1, T-1);

	simul.run(); // Lancement de la simulation

	return 0;
}
