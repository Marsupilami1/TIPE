#include "individu.h"
#include "simulation.h"
#include <ctime>

int main(int argc, char* argv[])
{	
	srand(time(NULL));
	Simulation simul(40);
	
	//~ simul.add_n_indiv(50);
	
	simul.run();
	
	return 0;
}
