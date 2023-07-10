# encoding: utf-8
# Revisión 2019 (a Python 3 y base64): Pablo Ventura
# Copyright 2014 Carlos Bederián
# $Id: connection.py 455 2011-05-01 00:32:09Z carlos $

import socket
from constants import *
from base64 import b64encode
import os

def is_number(n):
    """
    checkea si n es un numero
    """
    try:
        float(n)
    except ValueError:
        return False
    return True

class Connection(object):
    """
    Conexión punto a punto entre el servidor y un cliente.
    Se encarga de satisfacer los pedidos del cliente hasta
    que termina la conexión.
    """

    def __init__(self, socket: socket.socket, directory):
        self.new = socket
        self.dir = directory
        self.connected = True
        self.data = ""
        print(f"Connected by: {self.new.getsockname()}")

    def send_err(self, error):
        self.new.send((f"{error} {error_messages[error]}{EOL}").encode())

    def _recv(self):
        """
        Recibe datos y acumula en el buffer interno.

        Para uso privado del servidor.
        """
        try:
            data = self.new.recv(4096).decode("ascii")
            self.data += data

            if len(data) == 0:
                self.connected = False
        
        except UnicodeError:
            self.send_err(BAD_REQUEST)
            self.connected = False
            print("Closing connection...")
            
    def read_line(self):
        """
        Espera datos hasta obtener una línea completa delimitada por el
        terminador del protocolo.

        Devuelve la línea, eliminando el terminador y los espacios en blanco
        al principio y al final.
        """
        while not EOL in self.data and self.connected:
            self._recv()
        if EOL in self.data:
            response, self.data = self.data.split(EOL, 1)
            return response.strip()
        else:
            self.connected = False
            return ""

    def handle(self):
        """
        Atiende eventos de la conexión hasta que termina.
        """
        while self.connected:
            command = self.read_line()
            if '\n' in command:
                self.send_err(BAD_EOL)
                self.connected = False
                print("Closing connection...")
            elif len(command) > 0:
                try:
                    self.command_parser(command)
                except:
                    self.send_err(INTERNAL_ERROR)
                    self.connected = False
                    print("Closing connection...")
        self.send_err(CODE_OK)  
        self.new.close()  

    def command_parser(self, command):
        """
        Traduce, analiza y ejecuta en caso de no haber error,
        el comando ingresado
        """
        cmd = command.split(" ")

        print(f"Request> {command}")
        
        if len(cmd) > 0:
            if cmd[0] == 'quit':
                if(len(cmd) != 1):
                    self.send_err(INVALID_ARGUMENTS)
                else:
                    self.connected = False
                    self.send_err(CODE_OK)
                    print("Closing connection...")

            elif cmd[0] == 'get_file_listing':
                if(len(cmd) != 1):
                    self.send_err(INVALID_ARGUMENTS)
                else:
                    self.send_err(CODE_OK)
                    
                    for file in os.listdir(self.dir):
                        if os.path.isfile(os.path.join(self.dir, file)):
                            self.new.send((str(file)+EOL).encode())
                    self.new.send(EOL.encode())

            elif cmd[0] == 'get_metadata':
                if(len(cmd) != 2):
                    self.send_err(INVALID_ARGUMENTS)
                else:
                    if not os.path.isfile(os.path.join(self.dir, cmd[1])):
                        self.send_err(FILE_NOT_FOUND)
                        
                    elif len(set(cmd[1]) - VALID_CHARS) != 0:
                        self.send_err(INVALID_ARGUMENTS)
                        
                    else:
                        self.send_err(CODE_OK)
                        file = os.stat(self.dir+'/'+cmd[1])
                        self.new.send((str(file.st_size)+EOL).encode())
                    
            elif cmd[0] == 'get_slice':
                if len(cmd) == 4 and is_number(cmd[2]) and is_number(cmd[3]):
                    if not os.path.isfile(os.path.join(self.dir, cmd[1])):
                        self.send_err(FILE_NOT_FOUND)
                        
                    elif len(set(cmd[1]) - VALID_CHARS) != 0:
                        self.send_err(INVALID_ARGUMENTS)
       
                    else:
                        info = os.stat(self.dir+'/'+cmd[1])
                        if info.st_size >= (int(cmd[2]) + int(cmd[3])):
                            file = open(self.dir+'/'+cmd[1], "rb")
                            file.seek(int(cmd[2]))
                            sl = file.read(int(cmd[3]))
                            encoded = b64encode(sl)
                            formated = encoded.decode('ascii')
                            file.close()
                            self.send_err(CODE_OK)
                            self.new.send((formated+EOL).encode())
                        else:
                            self.send_err(BAD_OFFSET)
                else:
                    self.send_err(INVALID_ARGUMENTS)

            else:
                self.send_err(INVALID_COMMAND)