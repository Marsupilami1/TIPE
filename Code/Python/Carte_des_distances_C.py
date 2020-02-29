import ctypes as ct
import numpy as np
#import matplotlib.pyplot as plt

"""
TIPE 2020-2021 :
ENJEUX SOCIETAUX ~ Etude de mouvements de foules
"""

"""
Nom : Carte_des_distances_C.py
Création : 15/02/2020
Dernière modification : 20/02/2020
Modifié par : ANDRIEUX Martin
"""

"""
Descrition :
Ce programme permet d'obtenir le champs de vitesse, utilisé dans le fichier Deplacement.py
PYTHON ne fait pas les calculs, ces derniers sont effectués par un programme C : Carte_des_distancesbis.c
L'interaction entre PYTHON et le fichier .so (obtenue avec la commande qui suit, dans le bon répertoire)
est gérée par la bibliothèque "ctypes" (importée en tant que "ct")
gcc -shared -Wl,-soname,Carte_des_distances -o Carte_des_distances.so -fPIC Carte_des_distances.c
"""



def Recup_matrice(taille, sortie_ligne, sortie_colonne) : # Demande la taille de la grille et les coordonnées de la sortie
	c = ct.CDLL("/home/administrateur/Documents/Travail/Prepa/TIPE/Simulation/C/Carte_des_distancesbis.so") # Fichier .so
	c.calculs_champs(taille,sortie_ligne,sortie_colonne) # Appel de la fonction C "calculs_champs()"
	fichier = open("Vitesse.txt","r") # On récupère le fichier dans lequel est écrit le champs de vitesses (enregistré dans le répertoire courant)
	# On reconstruit le tableau à partir du fichier texte, voir construction du fichier texte...
	L = []
	for i in fichier :
		l=[]
		for c in i :
			if c == "X" :
				l.append(-1) # Vitesse sur la sortie (peu d'importance car disparition sur cette case)
			elif c != "\n" :
				l.append(int(c))
		L.append(l)
	fichier.close()
	return np.transpose(np.array(L)) # Transposée car inversion de lignes et colonnes (tant de calcul négligeable)

Vitesses_map = Recup_matrice(40,0,20) # Définition de la variable "globale" (tout de même appellée) 'Vitesse_map'


# Si volonté d'affichier le champs de vitesse :
#plt.imshow(Vitesses_map)
#plt.show()


# ~ C avec champ de vitesses
# ~ 10,10 -> 0.0006s
# ~ 20,20 -> 0.006s
# ~ 25,25 -> 0.014s
# ~ 30,30 -> 0.030s
# ~ 40,40 -> 0,095s
# ~ 50,50 -> 0.23s
# ~ 100,100 -> 3,72s
# ~ 200,200 -> 60.8s
# ~ *n -> *n**4
