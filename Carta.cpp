#include "Carta.h"

using namespace std;

/** Constructores **/
	Carta::Carta(){
		this->valor=0;
		this->pinta.push_back('');
		this->numero=0;
	}

	Carta::Carta(int valor, string pinta, int numero){
		this->valor = valor;
		this->pinta = pinta;
		this->numero = numero;
	}

/** Sets y Gets **/
	void Carta::setValor(int valor){
		this->valor = valor;
	}

	int Carta::getValor(){ return this->valor; }

	void Carta::setPinta(string pinta){
		this->pinta = pinta;
	}

	string Carta::getPinta(){ return this->pinta; }
	
	void Carta::setNumero(int numero){
		this->numero = numero;
	}
		
	int Carta::getNumero(){ return this->numero; }
