#include "Jugador.h"

//using namespace std;

/** Constructores **/
	Jugador::Jugador(){
		this->num_jugador = 0;
		this->separado=false;
		this->puntaje_1 = 0;
		this->puntaje_2 = 0;
		this->fichas = 100;
	}

	Jugador::Jugador(int num_jugador){
		this->num_jugador = num_jugador;
		this->separado=false;
		this->puntaje_1 = 0;
		this->puntaje_2 = 0;
		this->fichas = 100;
	}

/** Sets y Gets **/
	void Jugador::setNumJugador(int num_jugador){
		this->num_jugador = num_jugador;
	}

	int Jugador::getNumJugador(){ return this->num_jugador; }

	void Jugador::setMano1(std::vector<Carta> Mano){
		this->Mano_1 = Mano;

		this->puntaje_1=0;
		for (int i = 0; i < Mano_1.size(); i++)		{
			this->puntaje_1 += this->Mano_1[i].getValor();
		}

		this->num_cartas_mano_1 = this->Mano_1.size();
	}

	void Jugador::setMano1(Carta Carta){
		this->Mano_1.push_back(Carta);
		this->puntaje_1 += Carta.getValor();
		this->num_cartas_mano_1 += 1;
	}

	void Jugador::setMano1(int posicion, Carta Carta){
		int valor_anterior = this->Mano_1[posicion].getValor();
		this->Mano_1[posicion] = Carta;
		this->puntaje_1 -= valor_anterior;
		this->puntaje_1 += Carta.getValor();
	}

	std::vector<Carta> Jugador::getMano1(){ return this->Mano_1; }

	Carta Jugador::getMano1(int posicion){ return this->Mano_1[posicion]; }

	void Jugador::setMano2(std::vector<Carta> Mano){
		this->Mano_2 = Mano;

		this->puntaje_2=0;
		for (int i = 0; i < Mano_2.size(); i++)		{
			this->puntaje_2 += this->Mano_2[i].getValor();
		}

		this->num_cartas_mano_2 = this->Mano_2.size();
	}

	void Jugador::setMano2(Carta Carta){
		this->Mano_2.push_back(Carta);
		this->puntaje_2 += Carta.getValor();
		this->num_cartas_mano_2 += 1;
	}

	void Jugador::setMano2(int posicion, Carta Carta){
		int valor_anterior = this->Mano_2[posicion].getValor();
		this->Mano_2[posicion] = Carta;
		this->puntaje_2 -= valor_anterior;
		this->puntaje_2 += Carta.getValor();
	}

	std::vector<Carta> Jugador::getMano2(){ return this->Mano_2; }

	Carta Jugador::getMano2(int posicion){ return this->Mano_2[posicion]; }

	void Jugador::setSeparado(bool separado){
		this->separado = separado;
	}

	bool Jugador::getSeparado(){ return this->separado; }

	void Jugador::setNumCartasMano1(int num_cartas_mano_1){
		this->num_cartas_mano_1 = num_cartas_mano_1;
	}

	int Jugador::getNumCartasMano1(){ return this->num_cartas_mano_1; }

	void Jugador::setNumCartasMano2(int num_cartas_mano_2){
		this->num_cartas_mano_2 = num_cartas_mano_2;
	}

	int Jugador::getNumCartasMano2(){ return this->num_cartas_mano_2; }

	void Jugador::setPuntaje1(int puntaje){
		this->puntaje_1 = puntaje;
	}

	int Jugador::getPuntaje1(){ return this->puntaje_1; }

		void Jugador::setPuntaje2(int puntaje){
		this->puntaje_2 = puntaje;
	}

	int Jugador::getPuntaje2(){ return this->puntaje_2; }

	void Jugador::setFichas(int ficha){
		this->fichas = fichas;
	}

	int Jugador::getFichas(){ return this->fichas; }

/** Funcionales **/
	int Jugador::apostar(int cantidad){
		if(cantidad < this->fichas){
			this->fichas -= cantidad;
			return cantidad;
		}else{
			return -1;
		}
	}

	void Jugador::vaciarManos(){
		//Se deben enviar al Crupier para que las
		//ponga en el poso de cartas
		//Luego

		this->Mano_1.clear();
		this->puntaje_1=0;
		this->num_cartas_mano_1=0;

		this->Mano_2.clear();
		this->puntaje_2=0;
		this->num_cartas_mano_2=0;
	}

	void Jugador::separar(){

		this->separado = true;

		this->Mano_2.push_back(this->Mano_1.back());
		this->num_cartas_mano_2 += 1;
		this->puntaje_2 += this->Mano_1.back().getValor();

		this->puntaje_1 -= this->Mano_1.back().getValor();
		this->Mano_1.pop_back();
		this->num_cartas_mano_1 -= 1;
	}
