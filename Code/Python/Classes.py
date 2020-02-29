from math import sqrt, floor
import random as rd

"""
TIPE 2020-2021 :
ENJEUX SOCIETAUX ~ Etude de mouvements de foules
"""

"""
Nom : Classes.py
Création : 15/02/2020
Dernière modification : 20/02/2020
Modifié par : ANDRIEUX Martin
"""

"""
Description :
Contient diverses classes utiles aux autres programmes :
 - vecteur
 - individu
 - pylone
"""

#___________________________________


class vecteur :
	"""
	Classe définissant un type vecteur, possibilité de :
	 - calcul de norme
	 - addition, soustraction
	 - multiplication par un scalaire
	 - produit scalaire
	 - Tests d'égalité
	 - normalistaion
	 - vecteur au coordonnées entières
	 - print()
	"""
	
	Taille_grille = 0
	
	def __init__(self, position) : # Création (position est un tuple)
		self.x, self.y = position
	
	def entier(self) : # Vecteur aux coordonnées entières
		return vecteur((floor(self.x),floor(self.y)))
	
	def __add__(self, vect2) : # u + v
		return vecteur((self.x+vect2.x, self.y+vect2.y))
		
	def __iadd__(self, vect2) : # u += v
		return vecteur((self.x+vect2.x, self.y+vect2.y))
	
	def __sub__(self, vect2) : # u - v
		return vecteur((self.x-vect2.x, self.y-vect2.y))
		
	def __isub__(self, vect2) : # u -= v
		return vecteur((self.x-vect2.x, self.y-vect2.y))
	
	def __rmul__(self, scalaire) : # k * u
		return vecteur((scalaire*self.x, scalaire*self.y))
	
	def __irmul__(self, scalaire) : # u *= k
		return vecteur((scalaire*self.x, scalaire*self.y))
	
	def __truediv__(self, scalaire) : # u/k
		return vecteur((self.x/scalaire, self.y/scalaire))
	
	def __mul__(self, vect2) : # u . v
		return self.x*vect2.x + self.y*vect2.y
	
	def __eq__(self, vect2) : # u == v
		return self.x == vect2.x and self.y == vect2.y
	
	def __ne__(self, vect2) : # u != v
		return not(self.x == vect2.x and self.y == vect2.y)
		
	def __repr__(self) : # print(u)
		return ("Vecteur : ( {} , {} )".format(self.x, self.y))
	
	def norme(self) : # | u |
		return sqrt(self.x*self.x + self.y*self.y)
	
	def normalise(self) : # u / |u| -> vecteur unitaire
		if self == vecteur((0,0)) :
			return self
		return self/self.norme()


class individu :
	"""
	Classe définissant les individus présents sur le plateau, 
	un individu est défini par :
	 - une position (vecteur)
	 - un rayon
	 - un rayon de champs de vision
	 - une couleur (rouge par défaut)
	'individu.Liste' contient tous les individus créés
	Possibilité de déplacer un individu selon sa vitesse
	"""
	
	Liste = [] # Liste des individus créés
	
	def __init__(self, position, rayon, rayon_CdV, couleur = "red") : # ~ position est un tuple, CdV = champs de vision
		self.position = vecteur(position)
		self.vitesse = vecteur((0,0))
		self.rayon = rayon
		self.CdV = rayon_CdV
		self.couleur = couleur
		individu.Liste.append(self) # Ajout à la liste
	
	def move(self) : # permet de déplacer un individu (selon sa vitesse)
		self.position += self.vitesse
	
class pylone :
	
	"""
	Classe définissant un pylone (mur circulaire infranchissable),
	un pylone est défini par :
	 - une position
	 - un rayon
	"""
	
	def __init__(self, position, rayon) :
		self.position = vecteur(position)
		self.rayon = rayon
