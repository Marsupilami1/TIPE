#include "Carte_des_distances.h"

std::vector<std::vector<int>> Champs_de_vitesses;

void voisins(std::vector<std::vector<int>> &Carte, std::vector<vect> &vois, unsigned int d)
{
	unsigned int run = vois.size();
	for(unsigned int i=0; i<run; i++)
	{
		vect obj = vois.at(i);
		int X = obj.get_X();
		int Y = obj.get_Y();
		
		try{ // 1
			if(Carte.at(X+1).at(Y) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X+1, Y});
				Carte[X+1][Y] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 2
			if(Carte.at(X-1).at(Y) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X-1, Y});
				Carte[X-1][Y] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 3
			if(Carte.at(X).at(Y+1) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X, Y+1});
				Carte[X][Y+1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 4
			if(Carte.at(X).at(Y-1) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X, Y-1});
				Carte[X][Y-1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 5
			if(Carte.at(X+1).at(Y+1) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X+1, Y+1});
				Carte[X+1][Y+1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 6
			if(Carte.at(X-1).at(Y-1) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X-1, Y-1});
				Carte[X-1][Y-1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 7
			if(Carte.at(X-1).at(Y+1) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X-1, Y+1});
				Carte[X-1][Y+1] = d;}}
		catch(const std::out_of_range& e){}
		
		try{ // 8
			if(Carte.at(X+1).at(Y-1) == 0){
				//~ vect tt(X+1,Y);
				vois.push_back({X+1, Y-1});
				Carte[X+1][Y-1] = d;}}
		catch(const std::out_of_range& e){}
	}
}

std::vector<std::vector<int>> distances(const unsigned int n, const std::vector<vect> sorties)
{
	std::vector<std::vector<int>> Carte(n, std::vector<int>(n,0)); //Carte vide de taille n*n
	std::vector<vect> vois(0); //liste des voisins
	for(unsigned int i=0; i<sorties.size(); i++)
	{
		Carte.at(sorties.at(i).get_X()).at(sorties.at(i).get_Y()) = -1;
		vois.push_back(sorties.at(i));
	}
	
	for(unsigned int k=0; k<=3*n; k++)
	{
		voisins(Carte, vois, k+1); //Met à jour les voisins
	}
	
	for(unsigned int i=0; i<sorties.size(); i++)
	{
		Carte.at(sorties.at(i).get_X()).at(sorties.at(i).get_Y()) = 0; //Renvoi la carte avec les sorties mises à 0
	}
	
	return Carte;
}

int direction(int octet)
{
	int g = 0; //Rotations
	int zeros = 0;
	int uns = 0; //Compte les 1
	while ((octet>>7) == 1 && octet%2 == 1)
	{
		octet <<= 1;
		octet++;
		octet%=256;
		g++;
		
		if(g==8)
		{
			break;
		}
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
	for(int y=0; y<n; y++) // Balayage de la carte des distances
	{
		for(int x=0; x<n; x++)
		{
			value = distances[x][y];
			octet = 0;
			
			//Construit l'octet
			if(value != 0)
			{
				octet = (x+1<n && y<n && x+1>=0 && y>=0 && distances[x+1][y] < value && distances[x+1][y] > -2); //Dernier bit
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
				octet += (x+1<n && y+1<n && x+1>=0 && y+1>=0 && distances[x+1][y+1] < value && distances[x+1][y+1] > -2); //Premier bit -celui tt à dte-
				
				Champs[x][y] = direction(octet);
			} else {
				Champs[x][y] = -1;
			}
		}
	}
	
	return Champs;
}



void calculs_champs(const unsigned int n, const std::vector<vect> sorties) //Calcul le chmps de vit
{
	Champs_de_vitesses = vitesses(distances(n, sorties));
}
