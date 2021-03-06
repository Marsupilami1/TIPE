import numpy as np
import matplotlib.pyplot as plt
import random as rd
import time


def distances(matrice, x, y) : # ~ Établie la carte des distances
	matrice[x][y] = -1 # ~ Sortie
	vois=[(x,y)] # ~ Liste des voisins
	for k in range(matrice.shape[0]*matrice.shape[1]+1) : # ~ Il faudrait mieux évaluer le chemins max
		voisins(vois, matrice, k+1) # ~ Met à jours les voisins de la zone déjà explorée
	matrice[x][y] = 0
	return matrice # ~ Renvoie la matrice

def ingrid(x,y,c,l) :
	return x<c and y<l and x>=0 and y>=0

def voisins(liste, tab, d) : # ~ Met à jours les voisins de la zone déjà explorée
	l,c = tab.shape # ~ Taille de la matrice
	run = liste[:] # ~ Liste des voisins itérée, on itère pas sur "liste" car on .append() dans la boucle
	for i in run : # ~ Pour toutes les cellules de la zone explorée, il faudrait restreindre à la frontière
		x,y = i # ~ Coordonnées de la cellule en question
		'''Ici, on regarde les huit cellules voisines, elles prennent la valeur du nombre d'itération'''
		if ingrid(x+1,y,c,l) and tab[x+1][y] == 0 :
			tab[x+1][y] = d
			liste.append((x+1,y))
		if ingrid(x,y-1,c,l) and tab[x][y-1] == 0 :
			tab[x][y-1] = d
			liste.append((x,y-1))
		if ingrid(x-1,y,c,l) and tab[x-1][y] == 0 :
			tab[x-1][y] = d
			liste.append((x-1,y))
		if ingrid(x,y+1,c,l) and tab[x][y+1] == 0 :
			tab[x][y+1] = d
			liste.append((x,y+1))
		# ~ Diagonales
		if ingrid(x+1,y+1,c,l) and tab[x+1][y+1] == 0 :
			tab[x+1][y+1] = d
			liste.append((x+1,y+1))
		if ingrid(x-1,y-1,c,l) and tab[x-1][y-1] == 0 :
			tab[x-1][y-1] = d
			liste.append((x-1,y-1))
		if ingrid(x-1,y+1,c,l) and tab[x-1][y+1] == 0 :
			tab[x-1][y+1] = d
			liste.append((x-1,y+1))
		if ingrid(x+1,y-1,c,l) and tab[x+1][y-1] == 0 :
			tab[x+1][y-1] = d
			liste.append((x+1,y-1))
	# ~ Ne renvoie rien car modifie directement


def vitesse(distance) :
	l,c = distance.shape
	champ = np.zeros((l,c))
	champ += -1
	for y in range(l) :
		for x in range(c) :
			value = distance[x][y]
			if value != 0 :
				if ingrid(x+1,y,c,l) and distance[x+1][y] < value and distance[x+1][y] > -2 :
					champ[x][y] = 0
				elif ingrid(x+1,y-1,c,l) and distance[x+1][y-1] < value and distance[x+1][y-1] > -2 :
					champ[x][y] = 1
				elif x<c and y-1<l and x>=0 and y-1>=0 and distance[x][y-1] < value and distance[x][y-1] > -2 :
					champ[x][y] = 2
				elif ingrid(x-1,y-1,c,l) and distance[x-1][y-1] < value and distance[x-1][y-1] > -2 :
					champ[x][y] = 3
				elif x-1<c and y<l and x-1>=0 and y>=0 and distance[x-1][y] < value and distance[x-1][y] > -2 :
					champ[x][y] = 4
				elif ingrid(x-1,y+1,c,l) and distance[x-1][y+1] < value and distance[x-1][y+1] > -2 :
					champ[x][y] = 5
				elif x<c and y+1<l and x>=0 and y+1>=0 and distance[x][y+1] < value and distance[x][y+1] > -2 :
					champ[x][y] = 6
				elif ingrid(x+1,y+1,c,l) and distance[x+1][y+1] < value and distance[x+1][y+1] > -2 :
					champ[x][y] = 7
	
	return champ





plateau = np.zeros((20,20)) # ~ Matrice 20,20


'''Pour des couloirs'''
# ~ plateau[14][0] = -10
# ~ plateau[14][1] = -10
# ~ plateau[14][2] = -10
# ~ plateau[16][0] = -10
# ~ plateau[16][1] = -10
# ~ plateau[16][2] = -10
# ~ plateau[13][2] = -10
# ~ for i in range(12,0,-1) :
	# ~ plateau[i][2] = -10
# ~ for i in range(2,29) :
	# ~ plateau[1][i] = -10
# ~ for i in range(25,0,-1) :
	# ~ plateau[i][20] = -10
''''''


Distances_map = distances(plateau,10,10) # ~ Calcul les distances
Vitesses_map = vitesse(Distances_map) # ~ Calcul du champ de vitesse
print("Tableaux prets")

# ~ plt.subplot(121)
# ~ plt.imshow(Distances_map) # ~ Plt fait l'imagne tout seul
# ~ plt.subplot(122)
# ~ plt.imshow(Vitesses_map)
# ~ plt.show()

# ~ Python, distances seules
# ~ 10,10 -> 0.06s
# ~ 20,20 -> 0.97s
# ~ 25,25 -> 2.37s
# ~ 30,30 -> 4.6s
# ~ 40,40 -> 14.4s
# ~ 50,50 -> 35.7s
# ~ *n    -> *n**4
