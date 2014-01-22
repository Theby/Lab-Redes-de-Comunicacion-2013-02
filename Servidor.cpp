#include "Servidor.h"

/** Constructor **/
	Servidor::Servidor(){
		this->fichas_ganadas = 0;
	}

/** Sets y Gets **/
	void Servidor::setCrupier(Jugador Crupier){
		this->Crupier = Crupier;
	}

	Jugador Servidor::getCrupier(){ return this->Crupier; }

	void Servidor::setMazo(Mazo Mazo){
		this->mMazo = Mazo;
	}

	Mazo Servidor::getMazo(){ return this->mMazo; }

	void Servidor::setDeposito(Mazo Deposito){
		this->mDeposito = Deposito;
	}

	Mazo Servidor::getDeposito(){ return this->mDeposito; }

	void Servidor::setFichasGanadas(int fichas_ganadas){
		this->fichas_ganadas = fichas_ganadas;
	}

	int Servidor::getFichasGanadas(){ return this->fichas_ganadas; }