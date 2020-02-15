from Classes import *
from Carte_des_distances import *
from math import *


def calcul_vitesse(indiv) :
	x, y = floor(indiv.position.x), floor(indiv.position.y)
	v_chemin = Vitesses_map[x][y]
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
	indiv.vitesse = 0.1*indiv.vitesse + 0.9*vit


i0 = individu((2,17),1,5)
i1 = individu((5,2),1,5)
i2 = individu((19,19),1,5)
i3 = individu((17,0),1,5)
i4 = individu((17,0),1,5)

k=1
while individu.Liste != [] :
	for ind in individu.Liste :
		calcul_vitesse(ind)
		ind.move()
		if ind.position.entier() == vecteur((10,10)) :
			individu.Liste.remove(ind)
			print("Moins un : {}".format(k))
			print(individu.Liste)
	k+=1
