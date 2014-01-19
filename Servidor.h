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
		 * Mazo usado cómo deposito, en el cuál se almacenan las cartas retiradas de los jugadores
		 **/
		Mazo Deposito;

		/** 
		 * Variable que guarda la cantidad total de fichas retiradas de los juegadores
		 * Por otra parte el Servidor tiene infinitas fichas
		 **/
		int fichas_ganadas;

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
};

#endif /** _SERVIDOR_H_ **/