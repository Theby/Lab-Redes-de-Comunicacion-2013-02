#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include "Mazo.h"
#include "Jugador.h"
#include "SocketServidor.h"

class Servidor : public SocketServidor{
	private:
		/**
		 * Guarda la información del Crupier de la partida
		 **/
		Jugador Crupier;

		/**
		 * Mazo actual usado en el juego
		 **/
		Mazo mMazo;

		/**
		 * Mazo usado cómo deposito, en el cuál se almacenan las cartas retiradas de los jugadores
		 **/
		Mazo mDeposito;

		/** 
		 * Variable que guarda la cantidad total de fichas retiradas de los juegadores
		 * Por otra parte el Servidor tiene infinitas fichas
		 **/
		int fichas_ganadas;		

	public:
		//Constructores
		Servidor();

		//Sets y Gets
		void setCrupier(Jugador Crupier);
		Jugador getCrupier();

		void setMazo(Mazo Mazo);
		Mazo getMazo();

		void setDeposito(Mazo Deposito);
		Mazo getDeposito();

		void setFichasGanadas(int fichas_ganadas);
		int getFichasGanadas();

		int aceptaCliente(int DescriptorServidor);
};

#endif /** _SERVIDOR_H_ **/