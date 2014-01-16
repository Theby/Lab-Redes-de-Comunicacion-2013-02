#include "Mesa.h"

using namespace std;

/** Sets y Gets **/
	void Mesa::setJugadores(std::vector<Jugador> Jugadres){
		this->Jugadores = Jugadores;
	}

	void Mesa::setJugadores(Jugador Jugador){
		this->Jugadores.push_back(Jugador);
	}

	std::vector<Jugador> Mesa::getJugadores(){ return this->Jugadores; }
	Jugador Mesa::getJugadores(unsigned int posicion){ return this->Jugadores[posicion]; }

	void Mesa::setApuestas(std::vector<unsigned int> Apuestas){
		this->Apuestas = Apuestas;
	}

	void Mesa::setApuestas(unsigned int apuesta){
		this->Apuestas.push_back(apuesta);
	}

	std::vector<unsigned int> Mesa::getApuesta(){ return this->Apuestas; }
	unsigned int Mesa::getApuesta(unsigned int posicion){ return this->Apuestas[posicion]; }

	void Mesa::setMazo(Mazo Mazo){
		this->Mazo = Mazo;
	}

	Mazo Mesa::getMazo(){ return this->Mazo; }