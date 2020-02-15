from math import sqrt, floor


class vecteur :
	'''
	Classe définissant un type vecteur, possibilité de :
	- calcul de norme
	- addition
	- multiplication par un scalaire
	- produit scalaire
	- print()
	'''
	def __init__(self, position) : # ~ Position est un tuple
		self.x, self.y = position
	
	def entier(self) :
		return vecteur((floor(self.x),floor(self.y)))
	
	def __add__(self, vect2) :
		return vecteur((self.x+vect2.x, self.y+vect2.y))
		
	def __iadd__(self, vect2) :
		return vecteur((self.x+vect2.x, self.y+vect2.y))
	
	def __sub__(self, vect2) :
		return vecteur((self.x-vect2.x, self.y-vect2.y))
		
	def __isub__(self, vect2) :
		return vecteur((self.x-vect2.x, self.y-vect2.y))
	
	def __rmul__(self, scalaire) :
		return vecteur((scalaire*self.x, scalaire*self.y))
	
	def __truediv__(self, scalaire) :
		return vecteur((self.x/scalaire, self.y/scalaire))
	
	def __mul__(self, vect2) :
		return self.x*vect2.x + self.y*vect2.y
		
	
	def __eq__(self, vect2) :
		return self.x == vect2.x and self.y == vect2.y
	
	def __ne__(self, vect2) :
		return not(self.x == vect2.x and self.y == vect2.y)
		
	def __repr__(self) :
		return ("Vecteur : ( {} , {} )".format(self.x, self.y))
	
	def norme(self) :
		return sqrt(self.x*self.x + self.y*self.y)
	
	def normalise(self) :
		return self/self.norme()
	

class individu :
	
	Liste = []
	
	def __init__(self, position, rayon, rayon_CdV) : # ~ position est un tuple, CdV = champ de vision
		self.position = vecteur(position)
		self.vitesse = vecteur((0,0))
		self.rayon = rayon
		self.CdV = rayon_CdV
		individu.Liste.append(self)
	
	def move(self) :
		self.position += self.vitesse

class pylone :
	def __init__(self, position, rayon) :
		self.position = vecteur(position)
		self.rayon = rayon
