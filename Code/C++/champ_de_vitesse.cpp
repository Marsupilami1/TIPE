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
	carte.at(murs.at(i).getX()).at(murs.at(i).getY()) = n*n+10;

    for(unsigned int k=0; k<=n*n; k++)
    {
	voisins(carte, distances_sorties, k+1);
    }

    for(unsigned int i=0; i<sorties.size(); i++)
	carte.at(sorties.at(i).getX()).at(sorties.at(i).getY()) = 0;

    return carte;
}


int direction(int octet)
{
    int g = 0;
    int zeros = 0;
    int uns = 0;
    if(octet == 0 || octet == 0b11111111)
	return 0;
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







std::vector<std::vector<Vect>> vitesses(std::vector<std::vector<int>> distances)
{
    int n = distances.size();
    std::vector<Vect> liste_vitesse = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};
    int value = 0;
    int octet = 0;
    std::vector<std::vector<Vect>> Champs(n, std::vector<Vect>(n,{0,0}));

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

		Champs[x][y] = liste_vitesse[direction(octet)];
	    } else {
		Champs[x][y] = Vect(0,0);
	    }
	}
    }

    return Champs;
}

void smooth(std::vector<std::vector<Vect>>* champ_vitesses,	\
	std::vector<Vect>* distances_sorties,			\
	std::vector<std::vector<int>> champ_distances)
{
    unsigned int n = distances_sorties->size();
    unsigned int t = champ_distances.size();
    int tt = t*t;
    // Copie de la matrice pour les calculs
    std::vector<std::vector<Vect>> copy(t, std::vector<Vect>(t, Vect(0,0)));
    for(unsigned int l=0; l<t; l++)
    {
	for(unsigned int c=0; c<t; c++)
	{
	    copy[l][c] = champ_vitesses->at(l).at(c);
	}
    }

    // main
    for(unsigned int i=0; i<n; i++)
    {
	Vect position = distances_sorties->at(i);
	unsigned int x = position.getX();
	unsigned int y = position.getY();
	if (!((x>1 && champ_distances.at(x-1).at(y)>tt) |
		    (x+1<t && champ_distances[x+1][y]>tt)   |
		    (y>1 && champ_distances.at(x).at(y-1)>tt) |
		    (y+1<t && champ_distances.at(x).at(y+1)>tt) |
		    (x+1<t && y+1<t && champ_distances[x+1][y+1]>tt) |
		    (x>1 && y+1<t && champ_distances[x-1][y+1]>tt) |
		    (x+1<t && y>1 && champ_distances[x+1][y-1]>tt) |
		    (x>1 && y>1 && champ_distances[x-1][y-1]>tt) |

		    (x>2 && y>2 && champ_distances[x-2][y-2]>tt) |
		    (x>2 && y>1 && champ_distances[x-2][y-1]>tt) |
		    (x>2 && champ_distances[x-2][y]>tt) |
		    (x>2 && y+1<t && champ_distances[x-2][y+1]>tt) |
		    (x>2 && y+2<t && champ_distances[x-2][y+2]>tt) |
		    (x>1 && y>2 && champ_distances[x-1][y-2]>tt) |
		    (x>1 && y+2<t && champ_distances[x-1][y+2]>tt) |
		    (x+2<t && y>2 && champ_distances[x+2][y-2]>tt) |
		    (x+2<t && y>1 && champ_distances[x+2][y-1]>tt) |
		    (x+2<t && champ_distances[x+2][y]>tt) |
		    (x+2<t && y+1<t && champ_distances[x+2][y+1]>tt) |
		    (x+2<t && y+2<t && champ_distances[x+2][y+2]>tt) |
		    (x+1<t && y>2 && champ_distances[x+1][y-2]>tt) |
		    (x+1<t && y+2<t && champ_distances[x+1][y+2]>tt)))
	    {
		Vect new_dir = copy.at(x).at(y);
		int dx = new_dir.getX();
		int dy = new_dir.getY();
		new_dir += copy.at(x+dx).at(y+dy);
		champ_vitesses->at(x).at(y) = new_dir;
	    }
    }
}

void calculs_champs(const unsigned int n, const std::vector<Vect> sorties, \
	const std::vector<Vect> murs, std::vector<Vect>* distances_sorties, \
	std::vector<std::vector<Vect>>* champ_vitesses)
{
    std::vector<std::vector<int>> champ_distances = distances(n, sorties, murs, distances_sorties);
    *champ_vitesses = vitesses(champ_distances);
    for (unsigned int i=0; i<4; i++) {
	smooth(champ_vitesses, distances_sorties, champ_distances);
    }
}
