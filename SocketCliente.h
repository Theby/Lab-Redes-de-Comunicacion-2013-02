#ifndef _SOCKETCLIENTE_H_
#define _SOCKETCLIENTE_H_

#include "Socket.h"
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <iostream>
//#include <sys/types.h> incluidas en socket.h
//#include <sys/socket.h> incluidas en socket.h
//#include <sys/un.h> incluidas en socket.h
//#include <unistd.h> incluidas en socket.h
//#include <errno.h> incluidas en socket.h

class SocketCliente : public Socket{
	private:
		//Estructura que guarda los datos relacionados al servicio que se esta solicitando
		struct servent* Puerto;

		//Estructura que guarda los datos relacionados al host al que se conectara
		struct hostent* Host;

		//Para los datos relacionados con la direccion del host
		struct sockaddr_in Direccion;
		
		//descriptor del servidor
		int DescriptorServidor;

	public:
		//Constructores
		SocketCliente();
	
		//Set y Gets
		void setDescriptorServidor(int DescriptorServidor);
		int getDescriptorServidor();

		//Funcionales
		int abreConexionINET(char *Host_Servidor, char *Servicio);	
};

#endif /** _SOCKETCLIENTE_H_ **/