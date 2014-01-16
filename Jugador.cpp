#include "Jugador.h"

using namespace std;

/** Constructores **/
	Jugador::Jugador(){
		this->puntaje = 0;
		this->fichas = 100;
	}

/** Sets y Gets **/
		void setNumJugador(unsigned int num_jugador){
			this->num_jugador = num_jugador;
		}

		unsigned int getNumJugador(){ return this->num_jugador; }

		void Jugador::setMano(std::vector<Carta> Mano){
			this->Mano = Mano;
		}

		void Jugador::setMano(Carta Carta){
			this->Mano.push_back(Carta);
		}

		std::vector<Carta> Jugador::getMano(){ return this->Mano; }
		Carta Jugador::getMano(unsigned int posicion){ return this->Mano[posicion]; 

		void Jugador::setPuntaje(unsigned int puntaje){
			this->puntaje = puntaje;
		}

		unsigned int getPuntaje(){ return this->puntaje; }

		void setFichas(unsigned int ficha){
			this->fichas = fichas;
		}

		unsigned int getFichas(){ return this->fichas; }


