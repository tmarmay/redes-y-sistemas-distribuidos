# Laboratorio 2: Aplicación servidor

## Integrantes:
+ Cuevas Ignacio
+ Guglieri Juan Cruz
+ Marmay Tomas


## Índice:
0. Introducción
1. Implementación, Explicación de nuestro proyecto
2. Preguntas y Respuestas
3. Conclusión

## 0. Introducción

La idea de este laboratorio es levantar un servidor y crear un protocolo llamado **Home-made File Transfer Protocol (HFTP)** para que un cliente pueda conectarse a nuestro servidor y difentes funciones a archivos especificos.
Dicho protocolo va a funcionar en el puerto **19500** y usa a **TCP** como capa de transporte. 
Va a contar con funciones como:
    - `get_file_listing` : Este comando no recibe argumentos y busca obtener la lista de archivos que están actualmente disponibles en el servidor.
    - `get_metadata` : Este comando recibe un argumento FILENAME especificando un nombre de archivo del cual se pretende averiguar el tamaño.
    - `get_slice` : Este comando recibe en el argumento FILENAME el nombre de archivo del que se pretende obtener un slice o parte. La parte se especifica con un OFFSET (byte de inicio) y un SIZE (tamaño de la parte esperada, en bytes), ambos no negativos. Responde con el fragmento de archivo pedido codificado en base64.
    - `quit`: Este comando no recibe argumentos y busca terminar la conexión.
Ademas el servidor es capaz de soportar multiples clientes en simultaneo, con el manejo de `threads`.

> Se puede probar o bien usando el cliente creado en el lab1 o usando telnet.
> Para mas informacion ver [consignas](lab2-enunciado.pdf) donde se especifican las consginas y datos mas puntuales.

## 1. Implementación, Explicación de nuestro proyecto

Al haber hecho el laboratorio 1, ya estábamos un poco familiarizados con el módulo socket y sus funciones, lo primero que hicimos fue establecer la conexión entre el servidor y un cliente (luego hicimos para varios clientes), esto inicializando el servidor para que espere las conexiones de los clientes en ese socket con la dirección y puerto predeterminados (dir: 0.0.0.0 y puerto 19500).  
La parte más complicada fue la creación de los comandos y el manejo de errores. Decidimos crear una función el cual traduce y ejecuta comandos ingresados por el cliente, el cual llamamos `command_parser`; éste divide el string ingresado para analizar palabra por palabra, la primera palabra siempre deberá ser el comando (`quit`, `get_slice`, etc.) y dependiendo de cada comando deberá recibir más de una palabra, por ejemplo, para poder ejecutar `get_slice` el string debe estar formado por 4 palabras, si no es así salta el error `INVALID_ARGUMENTS`. Por otro lado, si el formato del pedido es correcto se deberá responder a ese pedido con `CODE_OK`.  
La función explicada anteriormente es llamada por `handle` el cual lee el texto ingresado y decide si el pedido está hecho correctamente (terminado correctamente: `EOL`), además puede surgir un error fatal interno y `handle` lo maneja. esta función además hace el llamado a la función `read_line` el cual toma todo el texto ingresado hasta EOL (end of line: `\r\n`), el cual este también llama a otra función `_recv` (el guión bajo en una función de python indica que es una función estática, `static` en c). Éste cumple una funcionalidad más interna el cual deduce si el texto ingresado cumple el formato "ascii".  
Estas funciones se pueden usar siempre y cuando exista conexión entre el cliente y el servidor, como explicamos anteriormente esta conexión se puede interrumpir cuando aparece un error fatal, por un error interno (el cual también es fatal) o por el uso del comando `quit`.  


## 2. Preguntas y Respuestas

+ ¿Qué estrategias existen para poder implementar este mismo servidor pero con capacidad de atender múltiples clientes simultáneamente? Investigue y responda brevemente qué cambios serían necesarios en el diseño del código.

Básicamente, cuando nos llega una solicitud de un cliente al servidor, el programa deja de "escuchar" por nuevas solicitudes de otros clientes para poder atender la que llegó. Por lo que si nosotros cuando nos llega una solicitud de un cliente se la damos a alguien más para que conteste los pedidos del cliente, mientras el programa principal sigue atento por nuevas solicitudes entrantes, podríamos tener un servidor con múltiples clientes.  
Para poder implementar esto basta usar hilos, es decir, cuando un cliente se conecte al servidor hacer que un hilo nuevo lo atienda mientras que el hilo principal sigue a la escucha de nuevos clientes. 

+ Pruebe ejecutar el servidor en una máquina del laboratorio, mientras utiliza el cliente desde otra, hacia la ip de la máquina servidor. ¿Qué diferencia hay si se corre el servidor desde la IP “localhost”, “127.0.0.1” o la ip “0.0.0.0”?

Primero podríamos decir que cuando aparece localhost se hace la traducción a 127.0.0.1, por lo cual son direcciones equivalentes(como pasaba con dns).  
Respondiendo a la pregunta, la diferencia está en que si corro el servidor desde la ip 127.0.0.1 o localhost solo voy a poder conectarme al servidor desde la misma computadora que lo corre, porque solo va a aceptar conexiones de clientes cuya ip sea 127.0.0.1 o localhost. Y si corro el servidor en 0.0.0.0 significa que el mismo esta abierto a escuchar conexiones provenientes de cualquier clientes independientemente de la ip


## 3. Conclusión 

Con este laboratorio aprendimos más a profundidad cómo usar la librería de sockets, como en un protocolo para cada caso posible de error se debe devolver un comando ya definido para que entre las dos computadoras puedan comunicarse y como se encodea la información para que sea más segura.  
Además pudimos observar y crear algo que usamos con mucha cotidianeidad en nuestra vida, y que a la vez es muy importante que es el pedido y entrega de datos mediante la red.