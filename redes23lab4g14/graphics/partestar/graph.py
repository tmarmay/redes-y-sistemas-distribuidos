import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
import json
### PARTE ANALISIS ###
#caso1 

def demora_paquetes():
    source,delay,x0,x1,x2,x3,x4,x5,x6,x7,x8 = [],[],[],[],[],[],[],[],[],[],[]
    y0,y1,y2,y3,y4,y5,y6,y7,y8 = [],[],[],[],[],[],[],[],[]

    with open("vector-case-star-2-buffer-source.json","r") as file:
        data = json.load(file)
        source = data["General"]["vectors"][0]["value"]
    with open("vector-case-star-2-buffer-delay.json","r") as file:
        data = json.load(file)
        delay = data["General"]["vectors"][0]["value"]
    
    for i in range(len(delay)):
        if source[i] == 0:
            x0.append(i)
            y0.append(delay[i])
        elif source[i] == 1:
            x1.append(i)
            y1.append(delay[i])
        elif source[i] == 2:
            x2.append(i)
            y2.append(delay[i])
        elif source[i] == 3:
            x3.append(i)
            y3.append(delay[i])
        elif source[i] == 4:
            x4.append(i)
            y4.append(delay[i])
        elif source[i] == 5:
            x5.append(i)
            y5.append(delay[i])
        elif source[i] == 6:
            x6.append(i)
            y6.append(delay[i])
        elif source[i] == 7:
            x7.append(i)
            y7.append(delay[i])
        else: 
            x8.append(i)
            y8.append(delay[i])
    
    plt.plot(x0,y0,'-',x1,y1,'-',x2,y2,'-',x3,y3,'-',x4,y4,'-',x5,y5,'-',x6,y6,'-',x7,y7,'-',x8,y8,'-')
    plt.legend(['De 0 a 50','De 1 a 50','De 2 a 50','De 3 a 50','De 4 a 50','De 5 a 50','De 6 a 50','De 7 a 50','De 8 a 50'], loc='best')
    plt.title("Demora de paquetes")
    plt.xlabel('Tiempo de ejecucion')
    plt.ylabel("Delay")
    plt.grid()
    plt.show()

def uso_buffer():
    b0, b2, b3, b4, b6, b7 = [],[],[],[],[],[]
    with open("b0.json","r") as file:
        data = json.load(file)
        b0 = data["General"]["vectors"][0]["value"]
    with open("b2.json","r") as file:
        data = json.load(file)
        b2 = data["General"]["vectors"][0]["value"]
    with open("b3.json","r") as file:
        data = json.load(file)
        b3 = data["General"]["vectors"][0]["value"]
    with open("b4.json","r") as file:
        data = json.load(file)
        b4 = data["General"]["vectors"][0]["value"]
    with open("b6.json","r") as file:
        data = json.load(file)
        b6 = data["General"]["vectors"][0]["value"]
    with open("b7.json","r") as file:
        data = json.load(file)
        b7 = data["General"]["vectors"][0]["value"]
        
    x0 = np.linspace(0,200,len(b0))
    x2 = np.linspace(0,200,len(b2))
    x3 = np.linspace(0,200,len(b3))
    x4 = np.linspace(0,200,len(b4))
    x6 = np.linspace(0,200,len(b6))
    x7 = np.linspace(0,200,len(b7))
    plt.plot(x0, b0, '-',x2, b2,'-',x3, b3, '-',x4, b4, '-',x6, b6, '-',x7, b7, '-')
    plt.legend(['buffer 0','buffer 2','buffer 3','buffer 4','buffer 6','buffer 7'], loc='best')
    plt.xlabel('Tiempo de ejecucion')
    plt.ylabel('Cantidad de paquetes')
    plt.title('Uso de bufferes internos')
    plt.axis([0,200,0,200])
    plt.grid()
    plt.show()

demora_paquetes()
#uso_buffer()
