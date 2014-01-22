#ifndef _SOCKET_H
#define _SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>

class Socket{
    private:
    	//Datos leidos o escritos en total
		int leido_escrito;

		//Datos leidos en cada intento de lectura
		int aux;

		//Modo del socket, ya sea para leer o escribir
		std::string accion;

	public:
		Socket();

		void setLeidoEscrito(int leido_escrito);
		int getLeidoEscrito();

		void setAux(int aux);
		int getAux();

		void setAccion(std::string accion);
		std::string getAccion();

		int usarSocket(int fd, char *Datos, int Longitud);
};

#endif /** _SOCKET_H_ **/
