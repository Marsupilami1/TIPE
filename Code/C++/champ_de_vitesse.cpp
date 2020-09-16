#include "champ_de_vitesse.hpp"
#include <iostream>

void voisins(std::vector<std::vector<int>> &carte, std::vector<Vect>* vois, int d)
{
	unsigned int run = vois->size();
	for(unsigned int i=0; i<run; i++)
	{
		Vect obj = vois->at(i);
		int X = obj.getX();
		int Y = obj.getY();
		
		try{ // 1
			if(carte.at(X+1).at(Y) == 0){
				//~ Vect tt(X+1,Y);
				vois->push_back({X+1, Y});
				carte[X+1][Y] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 2
			if(carte.at(X-1).at(Y) == 0){
				//~ Vect tt(X+1,Y);
				vois->push_back({X-1, Y});
				carte[X-1][Y] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 3
			if(carte.at(X).at(Y+1) == 0){
				//~ Vect tt(X+1,Y);
				vois->push_back({X, Y+1});
				carte[X][Y+1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 4
			if(carte.at(X).at(Y-1) == 0){
				//~ Vect tt(X+1,Y);
				vois->push_back({X, Y-1});
				carte[X][Y-1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 5
			if(carte.at(X+1).at(Y+1) == 0 && carte.at(X+1).at(Y) <= d && carte.at(X).at(Y+1) <= d){
				//~ Vect tt(X+1,Y);
				vois->push_back({X+1, Y+1});
				carte[X+1][Y+1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 6
			if(carte.at(X-1).at(Y-1) == 0 && carte.at(X-1).at(Y) <= d && carte.at(X).at(Y-1) <= d){
				//~ Vect tt(X+1,Y);
				vois->push_back({X-1, Y-1});
				carte[X-1][Y-1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 7
			if(carte.at(X-1).at(Y+1) == 0 && carte.at(X-1).at(Y) <= d && carte.at(X).at(Y+1) <= d){
				//~ Vect tt(X+1,Y);
				vois->push_back({X-1, Y+1});
				carte[X-1][Y+1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 8
			if(carte.at(X+1).at(Y-1) == 0 && carte.at(X+1).at(Y) <= d && carte.at(X).at(Y-1) <= d){
				//~ Vect tt(X+1,Y);
				vois->push_back({X+1, Y-1});
				carte[X+1][Y-1] = d;}}
		catch(const std::out_of_range& e){}
	}
}

std::vector<std::vector<int>> distances(const unsigned int n,		\
									  const std::vector<Vect> sorties,	\
									  const std::vector<Vect> murs,		\
									  std::vector<Vect>* distances_sorties)
{
	std::vector<std::vector<int>> carte(n, std::vector<int>(n,0));
	*distances_sorties = std::vector<Vect>(0);
	for(unsigned int i=0; i<sorties.size(); i++)
	{
		carte.at(sorties.at(i).getX()).at(sorties.at(i).getY()) = -1;
		distances_sorties->push_back(sorties.at(i));
	}
	for(unsigned int i=0; i<murs.size(); i++)
		carte.at(murs.at(i).getX()).at(murs.at(i).getY()) = 3*n+10;
	
	for(unsigned int k=0; k<=3*n; k++)
	{
		voisins(carte, distances_sorties, k+1);
	}
	
	/*
	  for(unsigned int i=0; i<murs.size(); i++)
	  {
	  int X = murs.at(i).getX();
	  int Y = murs.at(i).getY();

	  if(carte.at(X+1).at(Y) < carte.at(X+2).at(Y)) //Sortie "vers" la gauche
	  carte.at(X+1).at(Y)++;
	  if(carte.at(X-1).at(Y) < carte.at(X-2).at(Y)) //Sortie "vers" la droite
	  carte.at(X-1).at(Y)++;
	  if(carte.at(X).at(Y+1) < carte.at(X).at(Y+2)) //Sortie "vers" le haut
	  carte.at(X).at(Y+1)++;
	  if(carte.at(X).at(Y-1) < carte.at(X).at(Y-2)) //Sortie "vers" le bas
	  carte.at(X).at(Y-1)++;
	  }
	*/
	
	for(unsigned int i=0; i<sorties.size(); i++)
		carte.at(sorties.at(i).getX()).at(sorties.at(i).getY()) = 0;

	return carte;
}


int direction(int octet)
{
	int g = 0;
	int zeros = 0;
	int uns = 0;
	while ((octet>>7) == 1 && octet%2 == 1)
	{
		octet <<= 1;
		octet++;
		octet%=256;
		g++;
		
		if(g==8)
			break;
	}

	while (octet%2 == 0)
	{
		octet >>= 1;
		zeros++;
	}
	while (octet%2 == 1)
	{
		octet >>= 1;
		uns++;
	}
	return (7-((zeros+uns/2-g+8)%8));
}

std::vector<std::vector<int>> vitesses(std::vector<std::vector<int>> distances)
{
	int n = distances.size();
	
	int value = 0;
	int octet = 0;
	std::vector<std::vector<int>> Champs(n, std::vector<int>(n,0));
	//~ for(unsigned int i=0; i<Distances_sorties.size(); i++)
	//~ {
	//~ int x = Distances_sorties.at(i).getX();
	//~ int y = Distances_sorties.at(i).getY();
	//~ value = distances[x][y];
	//~ octet = 0;
		
	//~ if(value != 0)
	//~ {
	//~ octet = (x+1<n && y<n && x+1>=0 && y>=0 && distances[x+1][y] < value && distances[x+1][y] > -2);
	//~ octet =  octet << 1;
	//~ octet += (x+1<n && y-1<n && x+1>=0 && y-1>=0 && distances[x+1][y-1] < value && distances[x+1][y-1] > -2);
	//~ octet <<= 1;
	//~ octet += (x<n && y-1<n && x>=0 && y-1>=0 && distances[x][y-1] < value && distances[x][y-1] > -2);
	//~ octet <<= 1;
	//~ octet += (x-1<n && y-1<n && x-1>=0 && y-1>=0 && distances[x-1][y-1] < value && distances[x-1][y-1] > -2);
	//~ octet <<= 1;
	//~ octet += (x-1<n && y<n && x-1>=0 && y>=0 && distances[x-1][y] < value && distances[x-1][y] > -2);
	//~ octet <<= 1;
	//~ octet += (x-1<n && y+1<n && x-1>=0 && y+1>=0 && distances[x-1][y+1] < value && distances[x-1][y+1] > -2);
	//~ octet <<= 1;
	//~ octet += (x<n && y+1<n && x>=0 && y+1>=0 && distances[x][y+1] < value && distances[x][y+1] > -2);
	//~ octet <<= 1;
	//~ octet += (x+1<n && y+1<n && x+1>=0 && y+1>=0 && distances[x+1][y+1] < value && distances[x+1][y+1] > -2);
			
	//~ Champs[x][y] = direction(octet);
	//~ } else {
	//~ Champs[x][y] = -1;
	//~ }
	//~ }
	for(int y=0; y<n; y++) // Balayage de la carte des distances
	{
		for(int x=0; x<n; x++)
		{
			value = distances[x][y];
			octet = 0;
			
			if(value != 0)
			{
				octet = (x+1<n && y<n && x+1>=0 && y>=0 && distances[x+1][y] < value && distances[x+1][y] > -2);
				octet =  octet << 1;
				octet += (x+1<n && y-1<n && x+1>=0 && y-1>=0 && distances[x+1][y-1] < value && distances[x+1][y-1] > -2);
				octet <<= 1;
				octet += (x<n && y-1<n && x>=0 && y-1>=0 && distances[x][y-1] < value && distances[x][y-1] > -2);
				octet <<= 1;
				octet += (x-1<n && y-1<n && x-1>=0 && y-1>=0 && distances[x-1][y-1] < value && distances[x-1][y-1] > -2);
				octet <<= 1;
				octet += (x-1<n && y<n && x-1>=0 && y>=0 && distances[x-1][y] < value && distances[x-1][y] > -2);
				octet <<= 1;
				octet += (x-1<n && y+1<n && x-1>=0 && y+1>=0 && distances[x-1][y+1] < value && distances[x-1][y+1] > -2);
				octet <<= 1;
				octet += (x<n && y+1<n && x>=0 && y+1>=0 && distances[x][y+1] < value && distances[x][y+1] > -2);
				octet <<= 1;
				octet += (x+1<n && y+1<n && x+1>=0 && y+1>=0 && distances[x+1][y+1] < value && distances[x+1][y+1] > -2);
				
				Champs[x][y] = direction(octet);
			} else {
				Champs[x][y] = -1;
			}
		}
	}
	
	return Champs;
}


void calculs_champs(const unsigned int n, const std::vector<Vect> sorties, \
					const std::vector<Vect> murs, std::vector<Vect>* distances_sorties, \
					std::vector<std::vector<int>>* champ_vitesses)
{
	*champ_vitesses = vitesses(distances(n, sorties, murs, distances_sorties));
}
