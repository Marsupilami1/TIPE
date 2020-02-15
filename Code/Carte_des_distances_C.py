import ctypes as ct
import numpy as np
#import matplotlib.pyplot as plt
#import random as rd
#import time

# ~ gcc -shared -Wl,-soname,Carte_des_distances -o Carte_des_distances.so -fPIC Carte_des_distances.c


c = ct.CDLL("/home/administrateur/Documents/Travail/Prepa/TIPE/Simulation/C/Carte_des_distances.so")


c.calculs_champs(20,10,19)

def Recup_matrice() :
	fichier = open("Vitesse.txt","r")
	L = []
	for i in fichier :
		l=[]
		for c in i :
			if c == "X" :
				l.append(-1)
			elif c != "\n" :
				l.append(int(c))
		L.append(l)
	fichier.close()
	return np.transpose(np.array(L))

Vitesses_map = Recup_matrice()


#plt.imshow(Recup_matrice())
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
# ~ *n    -> *n**4
