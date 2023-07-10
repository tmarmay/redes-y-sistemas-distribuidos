import numpy as np
import matplotlib.pyplot as plt
from scipy import interpolate

#parte de analisis
def caso1p1():
    # datos originales
    x = [1979, 200, 659, 390, 285, 989, 1307,1162,1664]
    y = [1199, 200, 658, 390, 285, 989, 1198,1159,1199]

    # funcion de interpolacion
    f = interpolate.interp1d(x,y,kind="cubic")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=100)
    hy = f(hx)

    # graficar los resultados
    plt.plot(x, y, 'o', hx, hy, '-')
    plt.legend(['datos originales', 'interpolación'], loc='best')
    plt.title("Caso1")    
    plt.xlabel("carga trasmitida")
    plt.ylabel("carga recibida")
    plt.show()

def caso2p1():
    # datos originales
    x = [200,285,390,659,989,1307,1979,1162,1664]
    y = [200,285,390,659,989,1199,1199,1160,1199]

    # funcion de interpolacion
    f = interpolate.interp1d(x,y,kind="cubic")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=100)
    hy = f(hx)

    # graficar los resultados
    plt.plot(x, y, 'o', hx, hy, '-')
    plt.legend(['datos originales', 'interpolación'], loc='best')
    plt.title("Caso2")
    plt.xlabel("carga trasmitida")
    plt.ylabel("carga recibida")
    plt.show()

def perdidasp1():
    # datos originales
    x = [1979, 200, 659, 390, 285, 989, 1307,1162,1664]
    y = [780, 0, 1, 0, 0, 0, 109, 3, 465]

    # funcion de interpolacion
    f = interpolate.interp1d(x,y,kind="cubic")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=100)
    hy = f(hx)

    # graficar los resultados
    plt.plot(x, y, 'o', hx, hy, '-')
    plt.legend(['datos originales', 'interpolación'], loc='best')
    plt.title("Perdidas primera parte") 
    plt.xlabel("paquetes generados")
    plt.ylabel("paquetes perdidos") 
    plt.show()


#Parte de diseno
def caso1p2():
    # datos originales
    x = [200,285,390,659,989,1175,1161,1176,1178]
    y = [200,285,390,658,989,1175,1159,1174,1177]

    # funcion de interpolacion
    f = interpolate.interp1d(x,y,kind="cubic")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=100)
    hy = f(hx)

    # graficar los resultados
    plt.plot(x, y, 'o', hx, hy, '-')
    plt.legend(['datos originales', 'interpolación'], loc='best')
    plt.title("Caso1")    
    plt.xlabel("carga trasmitida")
    plt.ylabel("carga recibida") 
    plt.show()

def caso2p2():
    # datos originales
    x = [200,285,390,659,989,1161,1177,1179]
    y = [200,285,390,659,989,1160,1176,1198]

    # funcion de interpolacion
    f = interpolate.interp1d(x,y,kind="cubic")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=100)
    hy = f(hx)

    # graficar los resultados
    plt.plot(x, y, 'o', hx, hy, '-')
    plt.legend(['datos originales', 'interpolación'], loc='best')
    plt.title("Caso2")
    plt.xlabel("carga trasmitida")
    plt.ylabel("carga recibida")
    plt.show()


def perdidasp2():
    # datos originales
    x = [1178,1176,1161,1175,989,659,390,285,200]
    y = [1, 2, 2, 0, 0, 1, 0, 0, 0]

    # funcion de interpolacion
    f = interpolate.interp1d(x,y,kind="linear")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=300)
    hy = f(hx)

    # graficar los resultados
    plt.plot(x, y, 'o', hx, hy, '-')
    plt.legend(['datos originales', 'interpolación'], loc='best')
    plt.title("Perdidas segunda parte")   
    plt.xlabel("paquetes generados")
    plt.ylabel("paquetes perdidos") 
    plt.show()



#Diferencias p1 y p2

def relacion_paquetes_generados():
    # datos originales
    x = [1,0.7,0.5,0.3,0.2,0.17,0.15,0.12,0.1]
    yp1 = [200,285,390,359,989,1162,1307,1664,1979]
    yp2 = [200,285,390,659,989,1161,1175,1176,1178]

    # funcion de interpolacion
    f1 = interpolate.interp1d(x,yp1,kind="cubic")
    f2 = interpolate.interp1d(x,yp2,kind="cubic")

    # puntos para la interpolación
    hx = np.linspace(min(x),max(x),num=100)
    hy1 = f1(hx)
    hy2 = f2(hx)

    # graficar los resultados
    plt.plot(hx, hy1, '-', hx, hy2, '-')
    plt.legend(['primera parte', 'segunda parte'], loc='best')
    plt.title("Relacion paquetes generados parte1 y parte2")    
    plt.xlabel("intervalo de generacion")
    plt.ylabel("paquetes generados")
    plt.show()
