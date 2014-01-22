#ifndef _SOCKETSERVIDOR_H_
#define _SOCKETSERVIDOR_H_
	
#include "Socket.h"
#include <vector>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <cstdlib>
//#include <sys/types.h> incluidas en socket.h
//#include <sys/socket.h> incluidas en socket.h
//#include <sys/un.h> incluidas en socket.h
//#include <unistd.h> incluidas en socket.h
//#include <errno.h> incluidas en socket.h


class SocketServidor : public Socket{
	public:
		//Estructura que guarda los datos relacionados al servicio que se esta atendiendo
		struct servent* Puerto;

		//descriptor del servidor
		int DescriptorServidor;

		//Vector que guarda el descriptor de todos los clientes
		std::vector<int> DescriptorClientes;

		//Para los datos relacionados con la direccion de los Clientes
		struct sockaddr_in Direccion;

		//Guarda el largo de la direccion
		socklen_t largo;

		//Para guardar las diferentes conexiones de los clientes
		fd_set Clientes;

		//Para el valor maximo del descriptor, se relaciona con cliente y servidor
		int maximo;

	public:
		//Constructor
		SocketServidor();

		//Sets y Gets
		void setDescriptorServidor(int DescriptorServidor);
		int getDescriptorServidor();

		//Funcionales
		int abreSocketINET(char *Servicio);
		//int aceptaCliente(int DescriptorServidor);
};

#endif /* _SOCKETSERVIDOR_H_ */