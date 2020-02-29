#include <stdio.h>
#include <stdlib.h>

/* TIPE 2020-2021 :
 * ENJEUX SOCIETAUX ~ Etude de mouvements de foules
 */

/* Nom : Carte_des_distancesbis.c
 * Création : 19/02/2020
 * Dernière modification : 19/02/2020
 * Modifié par : ANDRIEUX Martin
 */


/* Description :
 * Ce programme a pour unique but d’accélérer la vitesse de calcul
 * du champs de vitesses.
 * Le champs est créé sous forme de fichier texte, ensuite récupéré par PYTHON
 * et converti sous forme de tableau NUMPY.
 * La fonction de création est appelée depuis le programme python avec le package ctypes.
 * Une version de ce programme a ete ecrite en PYTHON, celle-ci est néanmoins plus lente
 * et fourni un résultat moins probant (car le chemin suivit est peu proche du chemin réel).
 */

/* (?) : mérite une optimisation
 * (!) : nécessite une correction
 */

//___________________________________


/* Définition d'une structure "tuple", permet de stocker des coordonnées.
 * Un tuple contient 2 "int" : tuple.x et tuple.y.
 */
typedef struct tuple tuple;
struct tuple
{
	int x;
	int y;
};


// Prototypes de fonctions, voir définitions après le main()
int msb(int nombre); // Retourne le bit de poids fort (huitième bit de l'octet)
int voisins(int n, int taille, tuple *liste, int (*tab)[n], int d); // Gère une liste de voisins d'une cellule
void Distance(int n, int (*matrice)[n], int x, int y); // Calcul de la carte des distances
int vect(int a); // Permet une vitesse plus proche de celle réellement adoptée
void vitesse(int n, int (*distance)[n]); // Calcul du champs de vitesse
void calculs_champs(int n, int ligne, int colonne); // Fonction appelée par PYTHON


// MAIN
int main(int argc, char *argv[])
{
	/* La fonction main est vide, simplement car PYTHON peut appeler
	 * une fonction particulière directement. Comme ce programme n'a
	 * pas vocation à être exécuté, il n'est pas nécessaire de remplir
	 * le main.
	 * 
	 * Si volonté de test : appeler 
	 * calculs_champs(int n, int ligne, int colonne);
	 * 
	 * Il faudra bien sûr recompiler...
	 */
	return 0;
}


// FONCTIONS
int msb(int nombre) 
{
	/* Retourne le MSB (most significant bit) d'un octet passé en argument
	 * Ne fonctionne qu'avec des octets (ou binaires de moins de 8 bits)
	 */
	return nombre >> 7; // Décalage à droite du binaire, 7 fois
}


int voisins(int n, int taille, tuple *liste, int (*tab)[n], int d)
{
	/* Cette fonctions gère les voisins d'une cellule ou d'une
	 * zone de cellules.
	 * Utilisée par la fonction Distance(), à lire avant celle-ci
	 * n : taille de la grille
	 * taille : longueur de la liste de cellules
	 * *liste : liste de cellules
	 * (*tab)[n] : Carte des distances en cours de création
	 * d : numéro du pas
	 */
	int run = taille; // Sauvegarde de la longueur de la liste, car celle ci va augmenter
	for(int i=0; i<=run; i++) // Pour chaque cellules de la liste :
	{
		tuple obj = liste[i]; // On récupère la ligne et la colonne de la cellule
		int X = obj.x; // 'X' est la ligne
		int Y = obj.y; // 'Y' est la colonne
		
		/* On test les huit cellules voisines, considérées commes
		 * nouvelles cellules ssi :
		 * - Elles existent (4 premiers tests)
		 * - Elles ne sont pas déjà dans la zone (tab[][]==0)
		 * && est le signe logique "et", "and" en PYTHON
		 */
		if(X+1<n && Y<n && X+1>=0 && Y>=0 && tab[X+1][Y] == 0) // Tests
		{
			tab[X+1][Y] = d; // Affectation de la cellule
			
			tuple tt; // Ajout de la cellule à la liste
			tt.x = X+1;
			tt.y = Y;
			liste[taille+1] = tt; // Ajout à la fin
			taille++; // La taille a changé
		}
		if(X-1<n && Y<n && X-1>=0 && Y>=0 && tab[X-1][Y] == 0)
		{
			tab[X-1][Y] = d;
			tuple tt;
			tt.x = X-1;
			tt.y = Y;
			liste[taille+1] = tt;
			taille++;
		}
		if(X<n && Y+1<n && X>=0 && Y+1>=0 && tab[X][Y+1] == 0)
		{
			tab[X][Y+1] = d;
			tuple tt;
			tt.x = X;
			tt.y = Y+1;
			liste[taille+1] = tt;
			taille++;
		}
		if(X<n && Y-1<n && X>=0 && Y-1>=0 && tab[X][Y-1] == 0)
		{
			tab[X][Y-1] = d;
			tuple tt;
			tt.x = X;
			tt.y = Y-1;
			liste[taille+1] = tt;
			taille++;
		}
		// Cellules en diagonale
		if(X+1<n && Y+1<n && X+1>=0 && Y+1>=0 && tab[X+1][Y+1] == 0)
		{
			tab[X+1][Y+1] = d;
			tuple tt;
			tt.x = X+1;
			tt.y = Y+1;
			liste[taille+1] = tt;
			taille++;
		}
		if(X-1<n && Y-1<n && X-1>=0 && Y-1>=0 && tab[X-1][Y-1] == 0)
		{
			tab[X-1][Y-1] = d;
			tuple tt;
			tt.x = X-1;
			tt.y = Y-1;
			liste[taille+1] = tt;
			taille++;
		}
		if(X-1<n && Y+1<n && X-1>=0 && Y+1>=0 && tab[X-1][Y+1] == 0)
		{
			tab[X-1][Y+1] = d;
			tuple tt;
			tt.x = X-1;
			tt.y = Y+1;
			liste[taille+1] = tt;
			taille++;
		}
		if(X+1<n && Y-1<n && X+1>=0 && Y-1>=0 && tab[X+1][Y-1] == 0)
		{
			tab[X+1][Y-1] = d;
			tuple tt;
			tt.x = X+1;
			tt.y = Y-1;
			liste[taille+1] = tt;
			taille++;
		}
	}
	return taille; // On renvoie la nouvelle taille de la liste, car ajout de nouvelles cellules
}

void Distance(int n, int (*matrice)[n], int x, int y)
{
	/* Calcul de la carte des distances
	 * n : taille de la grille
	 * (*matrice)[n] : tableau bidimensionnel (ici carré) de n lignes
	 * x : ligne de la sortie
	 * y : colonne de la sortie
	 */
	/* Pour calculer cette carte, on part de la cellule de sortie, puis,
	 * à chaque itération, les cellules voisines prennent une valeur
	 * correspondante au nombre de "pas" necessaires pour parvenir à cette cellule.
	 * Les cellules voisines sont ensuites rajoutées à la zone.
	 * Chaque itération correspond à un "pas"
	 */
	 
	matrice[x][y] = -1; // On isole la sortie
	tuple vois[n*n]; // Création d'une liste de cellules de longueur n*n, (?) -> longueur max ?
	
	tuple origine; // Création de la première cellule de la liste : la sortie
	origine.x = x; // Ligne de la sortie
	origine.y = y; // Colonne de la sortie
	vois[0] = origine; // Affectation
	
	int taille = 0; // 'taille' est la longueur utile de la liste courante (ou le nombre de cellules dans la zone)
	// Les indexations commencent à zéro, il y a donc une cellule dans la zone
	
	// Coeur de la fonction : on calcule chaque "pas" un par un, tout est fait par la fonction voisins()
	for(int k=0; k<=n*n; k++) // 'k' est l'indice du pas courant
	{
		taille = voisins(n, taille, vois, matrice, k+1); // Appel de la fonction voisins
	}
	
	matrice[x][y] = 0; // La sortie était isolée, on lui donne sa vraie valeur : 0
	// Ne renvoie rien car modifie directement.
}


int vect(int a)
{
	/* Cette fonction a pour role de renvoyer la vitesse correspondante à un octet donné
	 * La vitesse est un nombre entier entre 0 et 7 inclus. répartis ainsi :
	 * 3|2|1	C : cellule actuelle
	 * 4|C|0	'octet' est de la forme (01234567)
	 * 5|6|7
	 * 
	 * Si les cellules accessibles sont [0,1,7], nous ne pouvons pas calculer une simplie moyenne,
	 * (8/3 = 2, or 2 est inaccessible).
	 * 
	 * L'astuce est la suivante :
	 * # octet = 1100 0001
	 * Nous allons "faire tourner" 'octet' pour éliminer cette cassure dans la suite de 1
	 * # octet << 2 (il faut veiller à ajouter les 1 de gauche et à tronquer)
	 * # octet = 0000 0111
	 * la moyenne est désormais un choix judicieux, il ne reste plus qu'a lui faire subir les
	 * transformations inverses
	 */
	
	int g = 0; // nombre de rotations
	int zeros = 0; // nombre de 0 avant le premier 1
	int uns = 0; // nombre de 1 avant la deuxième série de 0
	
	while (msb(a) == 1 && a%2 == 1) // Tant que les deux extrémités sont des 1 :
	{
		// On tourne, càd
		a <<= 1; // On translate
		a++; // On ajoute le 1
		a%=256; // On tronque pour garder un octet
		
		g++; // On ajoute une rotation au compteur
		
		if(g==8) // Cas rare, mais ce serait con de se taper une boucle infinie
		{
			break; // OMG !!
		}
	}
	
	// Il faut calculer la moyenne :
	while (a%2 == 0) // On compte le nombre de zéros avant la première série de 1 (exple 0011 1000 -> 3 zeros)
	{
		a >>= 1;
		zeros++;
	}
	while (a%2 == 1) // On compte le nombre de 1 qui suivent (exple 0011 1000 -> 3 uns)
	{
		a >>= 1;
		uns++;
	}
	
	// Calcul hautement savant : bit moyen = zeros + uns/2 (tout est tjrs entier)
	// Retranslation : _ - g ('g' est le nombre de translations)
	// On veut une réponse entre 0 et 7, pas de modulo de nombre négatif : (_+8) % 8
	// On a compté en 76543210, donc 7-_
	return (7-((zeros+uns/2-g+8)%8));
}

void vitesse(int n, int (*distance)[n])
{
	/* Calcul du champs de vitesses
	 * n : taille de la grille
	 * (*distance)[n] : carte des distances
	 */
	/* Cette fonction calcul le champs de vitesses à partir de la
	 * carte des distances (qui n'est ni plus ni moins qu'un champs scalaire).
	 * Le tout est enregistré dans un fichier texte afin d'être facilement lisible
	 * par PYTHON.
	 */
	int value = 0; // Variable locale (valeur de la cellule dans la carte des distances)
	int octet = 0; // Nombre symbolisant les cases accessibles depuis une case donnée
	
	FILE* fichier = fopen("Vitesse.txt", "w"); // Ouverture d'un fichier texte en mode écriture, enregistré dans le répertoire courant
	for(int y=0; y<n; y++) // Balayage de la carte des distances
	{
		for(int x=0; x<n; x++)
		{
			value = distance[x][y]; // Valeur de la cellule courante
			octet = 0; // Initialisation
			/* Nous sommes actuellement sur une cellule de la carte des distances,
			 * Nous devons choisir une "vitesse" (plutôt une orientation) qui permettrait
			 * d'arriver au prochain pas sur une cellule plus proche de la sortie.
			 * Nous disposons des distances entre chaque cellule et la sortie.
			 * 
			 * Il serait possible de regarder les huit cellules voisines et de choisir une vitesse vers
			 * l'une de celles plus proche de la sortie. (Une telle cellule existe forcément, par construction).
			 * Seulement, cette méthode (qui est celle de la version PYTHON de ce programme), a l'inconvénient
			 * de fournir un champs de vitesse peu satisfaisant. Les trajectoires ayant en effet tendance à
			 * être loin d'une trajectoire réelle. (fortement diagonales alors qu'une ligne droite serait privilégiée
			 * par un individu réel).
			 * 
			 * Une autre solution est donc mise en place :
			 * Plutôt que de choisir une trajectoire parmi celles possibles, nous prendrons celle la
			 * plus proche de la moyenne. Pour cela, nous instaurons un nombre binaire 'octet' qui contient les
			 * possibilités de déplacements d'une cellule donnée.
			 * Une cellule sera dite accessible si :
			 * - Elle existe (4 premiers tests)
			 * - Elle est plus proche de la sortie
			 * - Elle n'est pas un mur (!) -> plus de gestion de murs
			 */
			if(value != 0) // Si la cellule n'est pas la sortie :
			{
				octet = (x+1<n && y<n && x+1>=0 && y>=0 && distance[x+1][y] < value && distance[x+1][y] > -2); // Dernier bit
				octet =  octet << 1; // Décalage (0010 << 1 = 00100)
				octet += (x+1<n && y-1<n && x+1>=0 && y-1>=0 && distance[x+1][y-1] < value && distance[x+1][y-1] > -2); // Avant-dernier bit
				octet <<= 1; // Equivalent à la ligne d'avant
				octet += (x<n && y-1<n && x>=0 && y-1>=0 && distance[x][y-1] < value && distance[x][y-1] > -2);
				octet <<= 1;
				octet += (x-1<n && y-1<n && x-1>=0 && y-1>=0 && distance[x-1][y-1] < value && distance[x-1][y-1] > -2);
				octet <<= 1;
				octet += (x-1<n && y<n && x-1>=0 && y>=0 && distance[x-1][y] < value && distance[x-1][y] > -2);
				octet <<= 1;
				octet += (x-1<n && y+1<n && x-1>=0 && y+1>=0 && distance[x-1][y+1] < value && distance[x-1][y+1] > -2);
				octet <<= 1;
				octet += (x<n && y+1<n && x>=0 && y+1>=0 && distance[x][y+1] < value && distance[x][y+1] > -2);
				octet <<= 1;
				octet += (x+1<n && y+1<n && x+1>=0 && y+1>=0 && distance[x+1][y+1] < value && distance[x+1][y+1] > -2); // Premier bit
				
				/* Traitons maintenant cet octet :
				 * 
				 * Considérons l'exemple suivant :
				 * O|X|X	O : cellule accessible		C : cellule actuelle
				 * O|C|X	X : cellule inaccessible	octet = 0001 1100 (sens de rotation direct, départ à droite)
				 * O|X|X	Dans cette situation, le pas le plus "naturel" est celui vers la gauche.
				 * 
				 * La difficulté survient lorsque les cellules conjointes sont de part et d'autre de la
				 * première cellule testée. Les explications suivent dans la définitions de la fonction vect().
				 */
				fprintf(fichier,"%i",vect(octet)); // Ecriture de la valeur de la vitesse dans le fichier texte
				
			} else { // Si la cellule est la sortie
				fputs("X", fichier);
			}
		}
		fputs("\n", fichier);
	}
	fclose(fichier);
}

void calculs_champs(int n, int ligne, int colonne)
{
	/* Fonctions appelée directement depuis PYTHON
	 * n : taille de la grille carrée
	 * ligne : ligne de la sortie
	 * colonne : colonne de la sortie
	 */
	 
	int plateau[n][n]; // création d'un tableau bidimensionnel (future carte des distances)
	for(int i=0; i<n; i++)
	{
		for(int j=0; j<n; j++)
		{
			plateau[i][j]=0; // Toutes les cases à zéro
		}
	}
	
	Distance(n,plateau,ligne,colonne); // Calcul de la carte des distances
	vitesse(n, plateau); // Calcul du champs de vitesses
}

