#ifndef _CLIENTE_H_
#define _CLIENTE_H_

#include "Jugador.h"
#include "SocketCliente.h"

class Cliente : public SocketCliente{
	private:
		//Listas de jugadores
		Jugador mJugador;

		//Cantidad apostada, para la mano 1 y la mano 2
		int* apuestas;

		//Para saber si el jugador esta listo con su turno
		bool ready;

	public:
		//Constructor
		Cliente();

		//Sets y Gets
		void setJugadores(Jugador mJugador);
		Jugador getJugadores();

		void setApuestas(int posicion, int apuesta);
		int* getApuesta();
		int getApuesta(int posicion);

		void setReady(bool ready);
		bool getReady();
};

#endif /** _CLIENTE_H_ **/