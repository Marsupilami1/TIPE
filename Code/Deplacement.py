from Carte_des_distances_C import Vitesses_map
from Classes import vecteur, individu
from math import floor

"""
TIPE 2020-2021 :
ENJEUX SOCIETAUX ~ Etude de mouvements de foules
"""

"""
Nom : Deplacement.py
Création : 14/02/2020
Dernière modification : 20/02/2020
Modifié par : ANDRIEUX Martin
"""

"""
Description :
Ce fichier contient les fonctions utiles au calcul de vitesses, notamment :
 - calcul_vitesse(indiv)
"""

#___________________________________


def calcul_vitesse(indiv) : # Calcule la vitesse pour le prochain pas de l'individu
	
	# Vitesse indiquée par le champs de vitesse (voir Carte_des_distances_C.py)
	x, y = floor(indiv.position.x), floor(indiv.position.y) # Obtention des coordonnées de la case sur laquelle se trouve l'individu
	v_chemin = Vitesses_map[x][y] # Récupération de la vitesse puis convertion en vecteur (voir Carte_des_distancesbis.c)
	if v_chemin == 0 :
		vit = vecteur((1,0))
	elif v_chemin == 1 :
		vit = vecteur((1,-1))
	elif v_chemin == 2 :
		vit = vecteur((0,-1))
	elif v_chemin == 3 :
		vit = vecteur((-1,-1))
	elif v_chemin == 4 :
		vit = vecteur((-1,0))
	elif v_chemin == 5 :
		vit = vecteur((-1,1))
	elif v_chemin == 6 :
		vit = vecteur((0,1))
	elif v_chemin == 7 :
		vit = vecteur((1,1))
	elif v_chemin == -1 :
		vit = vecteur((0,0))
	
	# Vitesse due à l'influence des autres individus
	vit2 = vecteur((0,0)) # D'abord nulle
	for indiv2 in individu.Liste : # Pour chaque individu (Liste : voir Classes.py)
		if indiv2 != indiv and (indiv2.position - indiv.position).norme() <= indiv.CdV and (indiv2.position - indiv.position)*indiv.vitesse > 0:
			"""
			L'individu est influencé par un autre si :
			 - l'autre n'est pas lui-même
			 - l'autre est dans son champs de vision
			 Un individu est dans le champs de vision d'un autre si :
			  - il est assez proche pour être vu (voir champs de vision dans Classes.py)
			  - il est en face de l'individu (vecteurs vitesse et difference de position dans le même sens -> u.v > 0)
			"""
			vit2 += indiv2.vitesse # La vitesse est d'autant plus grande que le nombre d'individus est grand
			# L'individu en cours voit sa vitesse se raprocher de celle de l'individu qu'il voit
	
	indiv.vitesse = 0.9*(2*vit+1.0*vit2+0.0*indiv.vitesse).normalise() # Coefficients attribués à chaque vitesse (dont celle actuelle) -> il faut trouver un équilibre
	
	# Gestion de sortie de plateau
	n = Vitesses_map.shape[0] # Taille du plateau
	new_position = indiv.vitesse+indiv.position # Nouvelle position
	# Si hors de la grille :
	if not(new_position.x > indiv.rayon and new_position.x < n - indiv.rayon and new_position.y > indiv.rayon  and new_position.y < n - indiv.rayon) :
		indiv.vitesse = vit # Vitesse du champs de vitesse
