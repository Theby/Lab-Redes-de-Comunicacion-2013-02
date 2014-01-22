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

		//Para saber que numero de cliente es
		int identificador;

	public:
		//Para las cartas del crupier
		std::vector<Carta> CartasCrupier;

		//Constructor
		Cliente();

		//Sets y Gets
		void setJugador(Jugador mJugador);
		Jugador getJugador();

		void setApuestas(int posicion, int apuesta);
		int* getApuesta();
		int getApuesta(int posicion);

		void setReady(bool ready);
		bool getReady();

		void setIdentificador(int identificador);
		int getIdentificador();
};

#endif /** _CLIENTE_H_ **/