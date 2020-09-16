#ifndef DEF_CHAMP
#define DEF_CHAMP

#include "vect.hpp"
#include <vector>

void voisins(std::vector<std::vector<int>> &carte, std::vector<Vect>* vois, int d);
std::vector<std::vector<int>> distances(const unsigned int n,		\
									  const std::vector<Vect> sorties,	\
									  const std::vector<Vect> murs,		\
									  std::vector<Vect>* distances_sorties);

int direction(int octet);
std::vector<std::vector<int>> vitesses(std::vector<std::vector<int>> distances);

void calculs_champs(const unsigned int n, const std::vector<Vect> sorties, \
					const std::vector<Vect> murs, std::vector<Vect>* distances_sorties, \
					std::vector<std::vector<int>>* champ_vitesses);

#endif
