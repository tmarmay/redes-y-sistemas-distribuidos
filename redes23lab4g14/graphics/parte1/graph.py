import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate
import json
### PARTE ANALISIS ###
#caso1 

def demora_paquetes():
    source,delay,x0,x2,y0,y2 = [],[],[],[],[],[]

    with open("source-c1.json","r") as file:
        data = json.load(file)
        source = data["General"]["vectors"][0]["value"]
    with open("delay-c1.json","r") as file:
        data = json.load(file)
        delay = data["General"]["vectors"][0]["value"]
    
    for i in range(len(delay)):
        if source[i] == 2:
            x2.append(i)
            y2.append(delay[i])
        else: 
            x0.append(i)
            y0.append(delay[i])
    
    plt.plot(x0,y0,'-',x2,y2,'-')
    plt.legend(['De nodo 0 a nodo 5','De nodo 2 a nodo 5'], loc='best')
    plt.title("Demora de paquetes")
    plt.xlabel('Tiempo de ejecucion')
    plt.ylabel("Delay")
    plt.grid()
    plt.show()
#demora_paquetes()
def uso_buffer():
    b0, b1, b2, b6, b7 = [],[],[],[],[]
    with open("b0.json","r") as file:
        data = json.load(file)
        b0 = data["General"]["vectors"][0]["value"]
    with open("b1.json","r") as file:
        data = json.load(file)
        b1 = data["General"]["vectors"][0]["value"]
    with open("b2.json","r") as file:
        data = json.load(file)
        b2 = data["General"]["vectors"][0]["value"]
    with open("b6.json","r") as file:
        data = json.load(file)
        b6 = data["General"]["vectors"][0]["value"]
    with open("b7.json","r") as file:
        data = json.load(file)
        b7 = data["General"]["vectors"][0]["value"]
        
    x0 = np.linspace(0,200,len(b0))
    x1 = np.linspace(0,200,len(b1))
    x2 = np.linspace(0,200,len(b2))
    x6 = np.linspace(0,200,len(b6))
    x7 = np.linspace(0,200,len(b7))
    plt.plot(x0, b0, '-',x1, b1, '-',x2, b2, '-',x6, b6, '-',x7, b7, '-')
    plt.legend(['buffer 0','buffer 1','buffer 2','buffer 6','buffer 7'], loc='best')
    plt.xlabel('Tiempo de ejecucion')
    plt.ylabel('Cantidad de paquetes')
    plt.title('Uso de bufferes internos')
    plt.grid()
    plt.show()
#uso_buffer()
def cant_saltos():
    nodos = ["Nodo 0", "Nodo 2"]
    saltos = [3, 5]

    fig, ax = plt.subplots(figsize=(6, 4))  # Ajustar el tamaño de la figura

    # Ajustar el ancho de las barras
    ancho = 0.3  # Reducir el ancho de las barras

    # Crear el gráfico de barras horizontales con colores personalizados
    colores = ['violet', 'cornflowerblue']  # Colores personalizados
    ax.barh(nodos, saltos, height=ancho, color=colores)

    ax.set_xlabel('Saltos')
    ax.set_title('Cantidad de saltos a nodo 5')

    plt.tight_layout()  # Ajustar el espaciado entre los elementos del gráfico
    plt.show()
cant_saltos()
def uso_link():
    #Defino valor eje y
    l0,l1,l2,l5,l6,l7 = [],[],[],[],[],[]
    num_nod = [0,1,2,5,6,7]
    for i in range(len(num_nod)):
        with open(f"l{num_nod[i]}.json","r") as file:
            data = json.load(file)
            a = data["General"]["vectors"][0]["value"]
            locals()[f"l{num_nod[i]}"].append(a.count(0))
            locals()[f"l{num_nod[i]}"].append(a.count(1)) 
 
    categorias = ['Interfaz 0 (der)', 'Interfaz 1 (izq)']

    # Calcular las posiciones de las barras agrupadas en el eje x
    num_nodos = 6
    ancho_barras = 0.1

    posicion_nodos = np.arange(len(categorias))
    posiciones_barras = [posicion_nodos + i * ancho_barras for i in range(num_nodos)]

    # Crear el gráfico de barras con barras agrupadas
    for i in range(len(num_nod)):
        plt.bar(posiciones_barras[i], eval(f"l{num_nod[i]}"), width=ancho_barras, label=f"Nodo {num_nod[i]}")

    plt.ylabel('Cantidad de veces utilizado')
    plt.title('Uso de enlaces')
    plt.xticks(posicion_nodos + (num_nodos - 1) * ancho_barras / 2, categorias)
    plt.legend()
    plt.show()

#uso_link()

'''
La utilizacion de los encales por nodos, 
se pudo medir gracias a entender como funciona la red.
Supongamos que estamos en un nodo i y recibimos un mensaje,
si el mensaje no es para nosotros significa que vino por la interfaz 1
y lo voy a tener que mandar por la interfaz 0

'''
























