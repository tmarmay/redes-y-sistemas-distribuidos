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

La actividad propuesta por la cátedra fue implementar un protocolo de transferencia de archivos casero **HFTP**, el cual usa como protocolo de transporte **TCP**, este protocolo el cual fue estudiado en la parte teórica de la materia sirve para transportar archivos de un emisor a un receptor, este protocolo en particular se caracteriza por la garantía de que el archivo llegue de manera segura sin errores al receptor, a diferencia del otro protocolo (UDP) que no lo garantiza. HFTP escucha comandos de un cliente y los responde en orden de llegada del pedido (como una queue), en caso de haber errores este protocolo, según la gravedad del error, cierra la conexión o advierte al cliente del error, a estos errores los caracterizamos por un código numérico, 100 a 199 errores fatales (cierran la conexión) y de 200 a 299 errores no fatales (permiten continuar con la conexión y seguir respondiendo pedidos). Además hay un código el cuál nos avisa que la operación se realizó con éxito, éste lo denotamos con 0.  
Éste protocolo tiene a disposición 4 comandos funcionales;  
`get_file_listing`, el cual lista todos los archivos disponibles.  
`get_metadata`, recibe un argumento, el cual es el nombre del archivo, y responde con el tamaño de éste.  
`get_slice`, toma 3 argumentos, el nombre del archivo, el byte de inicio (OFFSET) y el tamaño de la parte esperada (SIZE), éste comando responde con el segmento del archivo que se pidió.  
`quit`, el comando más simple, no toma argumentos y simplemente cierra la conexión.  

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