#include "simulation.hpp"
#include "individu.hpp"
#include <ctime>

// extern const unsigned int T; // Défini dans individu.h

int main(int argc, char* argv[])
{
	srand(time(NULL));

	const unsigned int T = 50;
	Simulation<T> simul;

	for(unsigned int i=0; i<T-1; i++)
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

	simul.run();

	return 0;
}
