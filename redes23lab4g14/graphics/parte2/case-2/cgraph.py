import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
import json

with open("sender-b7.json","r") as file:
    data = json.load(file)
    procced = data["General"]["vectors"][0]["value"]
    x = []
    y = []
    z = []
    old_x = 0 
    old_y = 0
    for i in range(1,len(procced)):
        if procced[i] == 7:
            x.append(old_x + 1)
            old_x+= 1
            y.append(old_y)
        else:
            y.append(old_y + 1)
            old_y+= 1
            x.append(old_x)

    w = np.linspace(0,200,len(x))
    plt.plot(w, x, '-', w, y, '-')
    plt.title("Equilibrio de la red nodo 7")
    plt.xlabel("Tiempo de ejecucion")
    plt.ylabel("Procesamiento de paquetes")
    plt.legend(['Paquetes propios','Paquetes ajenos'])
    plt.grid()
    plt.show()