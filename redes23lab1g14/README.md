### Lab1: Aplicación Cliente (Python)

- La idea del laboratorio es empezar a entender como funciona sockets para que sea mas facil de realizar el lab2

- Puede consultar las consignas en `lab1_enunciado.pdf los archivos para obtener mas informacion detallada. 

- Para mas informacion consultar [consignas](lab1-enunciado.pdf)

**Como correr:**
- python3 hget.py `url`
    - Solo esta permitodo url del tipo `http`

### Punto estrella
Primero hay que definir como se referencian las paginas web. Para poder conectar con una pagina web tenemos que usar su ip y un puerto. Como no es comun para las personas tener que acordarnos de numeros y en su lugar es mas facil acordarse de un nombre, lo que se hace es usar una "etiqueta" para referenciarse a la ip de un sitio web. A dicha etiqueta se la llama dominio y a la tecnologia que nos transforma el dominio en una ip se la llama dns.
Para que un emoji funcione como dominio (ej: https://💩.la) tiene que ser convertido a Punycode.   Punycode es un sistema de codificación empleado para convertir una cadena de texto en formato  Unicode a un formato ASCII limitado. En lo que respecta a dominios, se utiliza para convertir dominios multilingües (IDN) que tengan caracteres especiales que no están incluidos en ASCII.
Los caracteres ASCII limitados que permiten los dominios son los siguientes:
-   Letras minúsculas de la “a” la “z”.
-   Números (0-9).
-   El carácter especial -.

Por ejemplo, para un dominio compuesto por caracteres chinos.  Punycode se encargará de codificar esos caracteres y hacerlos compatibles con el formato ASCII, también, se codificaría en punycode un dominio que tenga una ñ o ç


#### Desventajas 

- Los dominios con emoji son limitados
- Se especula que los dominios que contienen emoji, especialmente en los celulares, son usados para ataques phishing
