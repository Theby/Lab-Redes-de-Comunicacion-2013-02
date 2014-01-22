#include "Mazo.h"

//using namespace std;

/** Privadas **/
	int Mazo::randomNumber(int i){ 
		int number = std::rand()%i;//std::rand()%i;

		if(number<0)
			number *= -1;

		return number;
	}

/** Constructores **/
	Mazo::Mazo(){
		this->max_num_cartas_mazo = 208;
		this->num_cartas_repartidas=this->max_num_cartas_mazo;
		this->num_cartas_mazo=0;
	}

	Mazo::Mazo(std::vector<Carta> Cartas){
		this->max_num_cartas_mazo = 208;
		this->Cartas = Cartas;
		this->num_cartas_mazo = Cartas.size();
		this->num_cartas_repartidas = this->max_num_cartas_mazo-this->num_cartas_mazo;
	}

/** Sets y Gets **/
	void Mazo::setCartas(std::vector<Carta> Cartas){
		this->Cartas = Cartas;
		this->num_cartas_mazo = Cartas.size();
		this->num_cartas_repartidas = this->max_num_cartas_mazo-this->num_cartas_mazo;
	}

	void Mazo::setCartas(Carta mCarta){
		this->Cartas.push_back(mCarta);
		this->num_cartas_mazo += 1;
		this->num_cartas_repartidas -= 1;
	}

	void Mazo::setCartas(int posicion,Carta mCarta){
		this->Cartas[posicion] = mCarta;
	}

	std::vector<Carta> Mazo::getCartas(){ return this->Cartas; }

	Carta Mazo::getCartas(int posicion){ return this->Cartas[posicion]; }

	void Mazo::setNumCartasRepartidas(int num_cartas_repartidas){
		this->num_cartas_repartidas = num_cartas_repartidas;
	}

	int Mazo::getNumCartasRepartidas(){ return this->num_cartas_repartidas; }
	
	void Mazo::setNumCartasMazo(int num_cartas_mazo){
		this->num_cartas_mazo = num_cartas_mazo;
	}

	int Mazo::getNumCartasMazo(){ return this->num_cartas_mazo; }

/** Funcionales **/
	void Mazo::revolverMazo(){
		srand(unsigned(time(0)));

		//Valores entre 3 y 5
		int veces = this->randomNumber(3)+3;

		for (int i = 0; i < veces; i++){
			std::random_shuffle(this->Cartas.begin(), this->Cartas.end());
		}		
	}

	Carta Mazo::repartirCarta(){
		Carta CartaAux = this->Cartas.back();

		this->Cartas.pop_back();
		this->num_cartas_repartidas += 1;
		this->num_cartas_mazo -= 1;

		return CartaAux;
	}

	std::vector<Carta> Mazo::generarCartas(){
		std::vector<Carta> Cartas;

		//Genera de manera ordenada las 52 cartas
		for (int j = 0; j < 4; j++){
			for (int i = 0; i < 13; i++){
				Carta moldeC;

				moldeC.setPinta(3+j);
				if(i == 0){
					moldeC.setValor(11);
				}else if(i > 0 && i < 10){
					moldeC.setValor(i+1);
				}else{
					moldeC.setValor(10);
				}
				moldeC.setNumero(i+1);

				Cartas.push_back(moldeC);
			}
		}

		//Replica las cartas 3 veces para tener 208 cartas
		for (int i = 0; i < 3; i++){
			for (int k = 0; k < 52; k++){
				Carta replica = Cartas[k];
				Cartas.push_back(replica);
			}
		}

		return Cartas;
	}