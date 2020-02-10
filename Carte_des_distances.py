import numpy as np
import matplotlib.pyplot as plt
import random as rd
import time


def distances(matrice, x, y) : # ~ Établie la carte des distances
	matrice[x][y] = -1 # ~ Sortie
	vois=[(x,y)] # ~ Liste des voisins
	for k in range(matrice.shape[0]*matrice.shape[1]+1) : # ~ Il faudrait mieux évaluer le chemins max
		voisins(vois, matrice, k+1) # ~ Met à jours les voisins de la zone déjà explorée
	return matrice # ~ Renvoie la matrice

def voisins(liste, tab, d) : # ~ Met à jours les voisins de la zone déjà explorée
	l,c = tab.shape # ~ Taille de la matrice
	run = liste[:] # ~ Liste des voisins itérée, on itère pas sur "liste" car on .append() dans la boucle
	for i in run : # ~ Pour toutes les cellules de la zone explorée, il faudrait restreindre à la frontière
		x,y = i # ~ Coordonnées de la cellule en question
		'''Ici, on regarde les quatres cellules voisines, elles prennent la valeur du nombre d'itération'''
		if x+1<c and y<l and x+1>=0 and y>=0 and tab[x+1][y] == 0 and not ((x+1,y)in liste) :
			tab[x+1][y] = d
			liste.append((x+1,y))
		if x<c and y-1<l and x>=0 and y-1>=0 and tab[x][y-1] == 0 and not ((x,y-1)in liste) :
			tab[x][y-1] = d
			liste.append((x,y-1))
		if x-1<c and y<l and x-1>=0 and y>=0 and tab[x-1][y] == 0 and not ((x-1,y)in liste) :
			tab[x-1][y] = d
			liste.append((x-1,y))
		if x<c and y+1<l and x>=0 and y+1>=0 and tab[x][y+1] == 0 and not ((x,y+1)in liste) :
			tab[x][y+1] = d
			liste.append((x,y+1))
	# ~ Ne renvoie rien car modifie directement


def vitesse(distance) :
	l,c = distance.shape
	champ = np.zeros((l,c))
	for y in range(l) :
		for x in range(c) :
			value = distance[x][y]-1
			if x+1<c and y<l and x+1>=0 and y>=0 and distance[x+1][y] ==  value :
				champ[x][y] = 0
			elif x<c and y-1<l and x>=0 and y-1>=0 and distance[x][y-1] == value :
				champ[x][y] = 1
			elif x-1<c and y<l and x-1>=0 and y>=0 and distance[x-1][y] == value :
				champ[x][y] = 2
			elif x<c and y+1<l and x>=0 and y+1>=0 and distance[x][y+1] == value :
				champ[x][y] = 3
	return champ





plateau = np.zeros((10,10)) # ~ Matrice 30,30


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

# ~ t0 = time.time()
plateau = distances(plateau,5,5) # ~ Calcul les distances pour la sortie en 15,0
# ~ print(time.time()-t0)
# ~ plt.imshow(plateau) # ~ Plt fait l'imagne tout seul
# ~ plt.show() # ~ On affiche l'image
print(plateau)
matrix = vitesse(plateau)
print(matrix)
print(plateau)

l,k = matrix.shape
for y in range(l) :
	c=""
	for x in range(k) :
		v = matrix[x][y]
		if v == 0 :
			c+=">"
		elif v == 1 :
			c+="^"
		elif v == 2 :
			c+="<"
		elif v == 3 :
			c+="v"
	print(c)

# ~ 10,10 -> 0.06s
# ~ 20,20 -> 0.97s
# ~ 25,25 -> 2.37s
# ~ 30,30 -> 4.6s
# ~ 40,40 -> 14.4s
# ~ 50,50 -> 35.7s
# ~ *n    -> *n**4
