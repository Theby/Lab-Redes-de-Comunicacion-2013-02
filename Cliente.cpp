#include "Cliente.h"

//using namespace std;

/** Constructores **/
	Cliente::Cliente(){
		this->apuestas = (int*)malloc(sizeof(int)*2);
		this->apuestas[0]=0;
		this->apuestas[1]=0;
		this->ready=false;
	}

/** Sets y Gets **/
	void Cliente::setJugadores(Jugador mJugador){
		this->mJugador = mJugador;
	}

	Jugador Cliente::getJugadores(){ return this->mJugador; }

	void Cliente::setApuestas(int posicion, int apuesta){
		this->apuestas[posicion] = apuesta;
	}

	int* Cliente::getApuesta(){ return this->apuestas; }
	int Cliente::getApuesta(int posicion){ return this->apuestas[posicion]; }


	void Cliente::setReady(bool ready){
		this->ready = ready;
	}

	bool Cliente::getReady(){ return this->ready; }