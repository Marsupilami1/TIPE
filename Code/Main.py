import matplotlib.pyplot as plt
from Classes import vecteur, individu
from Deplacement import calcul_vitesse
import random as rd

"""
TIPE 2020-2021 :
ENJEUX SOCIETAUX ~ Etude de mouvements de foules
"""

"""
Nom : Main.py
Création : 15/02/2020
Dernière modification : 20/02/2020
Modifié par : ANDRIEUX Martin
"""

"""
Description :
Fichier à executer, appelle toutes les fonctions utiles dans les autres fichiers.
Ce fichier doit rester le plus simple possible, en cas d'ajout d'une fonction importante, veuillez
créer un nouveau fichier.
"""

for i in range (20) : # Génération de n individus
	individu((rd.randrange(2,38),rd.randrange(2,38)),0.2,10,(rd.random(),rd.random(),rd.random()))

# Générations d'individus particuliers'
#i1 = individu((3,6),0.5,2,"red")
#i2 = individu((3,10),0.5,2,"black")
#i3 = individu((10,5),0.5,2,"purple")
#i4 = individu((38,2),0.3,2,"yellow")
#i5 = individu((14,34),0.3,2,"green")
#i6 = individu((35,35),0.3,2,"orange")
#i7 = individu((20,1),0.5,2,"pink")


# Processus d'itération
k=1
while individu.Liste != [] : # Tant qu'il reste des individus :
	for ind in individu.Liste : # On les déplace un par un
		calcul_vitesse(ind)
		ind.move()
		plt.scatter(ind.position.x, ind.position.y, color=ind.couleur) # c'est plus joli
		if ind.position.entier() == vecteur((0,20)) : # Si on est sorti
			individu.Liste.remove(ind) # On est supprimé de la liste
			print("{} est sorti en {} pas".format(ind.couleur,k)) # c'est plus joli
	k+=1

plt.show()
