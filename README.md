# Redes y Sistemas Distribuidos 
**Famaf 2023**

Breve explicacion sobre cada laboratorio de la materia


### Lab1: Aplicación Cliente (Python)

- La idea del laboratorio es empezar a entender como funciona sockets para que sea mas facil de realizar el lab2

- Puede consultar las consignas en `lab1_enunciado.pdf los archivos para obtener mas informacion detallada. 

**Como correr:**
- python3 hget.py `url`
    - Solo esta permitodo url del tipo `http`


### Lab2: Aplicación Servidor (Python)

- La idea de este laboratorio es levantar un servidor y crear un protocolo llamado **Home-made File Transfer Protocol (HFTP)** para que un cliente pueda conectarse a nuestro servidor y difentes funciones a archivos especificos.
Dicho protocolo va a funcionar en el puerto **19500** y usa a **TCP** como capa de transporte. 
Va a contar con funciones como:
    - `get_file_listing` : Este comando no recibe argumentos y busca obtener la lista de archivos que están actualmente disponibles en el servidor.
    - `get_metadata` : Este comando recibe un argumento FILENAME especificando un nombre de archivo del cual se pretende averiguar el tamaño.
    - `get_slice` : Este comando recibe en el argumento FILENAME el nombre de archivo del que se pretende obtener un slice o parte. La parte se especifica con un OFFSET (byte de inicio) y un SIZE (tamaño de la parte esperada, en bytes), ambos no negativos. Responde con el fragmento de archivo pedido codificado en base64.
    - `quit`: Este comando no recibe argumentos y busca terminar la conexión.
Ademas el servidor es capaz de soportar multiples clientes en simultaneo, con el manejo de `threads`.

> Se puede probar o bien usando el cliente creado en el lab1 o usando telnet.
> Para mas informacion ver `lab2_enunciado.pdf` donde se especifican las consginas y datos mas puntuales.