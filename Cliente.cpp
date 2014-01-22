#include "Cliente.h"

//using namespace std;

/** Constructores **/
	Cliente::Cliente(){
		this->apuestas = (int*)malloc(sizeof(int)*2);
		this->apuestas[0]=0;
		this->apuestas[1]=0;
		this->ready=false;
		this->identificador=0;
	}

/** Sets y Gets **/
	void Cliente::setJugador(Jugador mJugador){
		this->mJugador = mJugador;
	}

	Jugador Cliente::getJugador(){ return this->mJugador; }

	void Cliente::setApuestas(int posicion, int apuesta){
		this->apuestas[posicion] = apuesta;

		int nuevo_valor = this->mJugador.getFichas();
		nuevo_valor -= apuesta;	

		this->mJugador.setFichas(apuesta);
	}

	int* Cliente::getApuesta(){ return this->apuestas; }
	int Cliente::getApuesta(int posicion){ return this->apuestas[posicion]; }


	void Cliente::setReady(bool ready){
		this->ready = ready;
	}

	bool Cliente::getReady(){ return this->ready; }

	void Cliente::setIdentificador(int identificador){
		this->identificador = identificador;
	}

	int Cliente::getIdentificador(){ return this->identificador; }