import subprocess
import matplotlib.pyplot as plt

subprocess.call("./bin/Debug/TIPE_w_sfml.exe") #Execute le cpp

resultats=open("./TIPE.txt","r") #Résultats notés au format tps//nb_indiv

indiv,temps=[],[]

for i in resultats:
    tps,ind=i.rstrip().split('//')
    temps.append(tps)
    indiv.append(ind)

print(temps,indiv)
    
resultats.close()

plt.grid()
plt.plot(indiv,temps)
plt.title("Résultat de la simulation")
plt.show()
