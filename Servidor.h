#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

class Servidor{
	private:
		/**
		 * Guarda la información del Crupier de la partida
		 **/
		Jugador Crupier;

		/**
		 * Mazo actual usado en el juego
		 **/
		Mazo Mazo;

		/**
		 * Socket para iniciar las primitivas de conexión
		 * de modo que es asignado a un socket disponible
		 * o almacenado en una cola
		 **/

		/**
		 * 7 Sockets para jugar contra 7 usuarios
		 **/
		 


	public:
		Servidor();
		~Servidor();

		/* data */
};

#endif /** _SERVIDOR_H_ **/