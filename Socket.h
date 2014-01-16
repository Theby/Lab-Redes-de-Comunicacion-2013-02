#ifndef _SOCKET_H
#define _SOCKET_H

class Socket{
    private:
    	//Datos leidos o escritos en total
		int leido_escrito = 0;

		//Datos leidos en cada intento de lectura
		int aux = 0;

		//Modo del socket, ya sea para leer o escribir
		string accion, 

	public:
		Socket();
		~Socket();

		void setLeidoEscrito(int leido_escrito);
		int getLeidoEscrito();

		void setAux(int aux);
		int getAux();

		int usarSocket(int fd, char *Datos, int Longitud);
};

#endif /** _SOCKET_H_ **/
