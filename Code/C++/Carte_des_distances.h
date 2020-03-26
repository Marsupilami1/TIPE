#ifndef DEF_DISTANCES
#define DEF_DISTANCES

#include "vect.h"
#include <vector>

void voisins(std::vector<std::vector<int>> &Carte, std::vector<vect> &vois, unsigned int d);
std::vector<std::vector<int>> distances(const unsigned int n, const std::vector<vect> sorties); //Renvoi le tableau des distances / aux sorties

int direction(int octet);
std::vector<std::vector<int>> vitesses(std::vector<std::vector<int>> distances); //Renvoi le tableau des vitesses



void calculs_champs(const unsigned int n, const std::vector<vect> sorties); //cf cpp


#endif
