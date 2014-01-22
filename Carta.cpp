#include "FuncionesExtras.h"
#include "Carta.h"

//using namespace std;

/** Constructores **/
	Carta::Carta(){
		this->valor=0;
		this->pinta=' ';
		this->numero=0;
	}

	Carta::Carta(int valor, char pinta, int numero){
		this->valor = valor;
		this->pinta = pinta;
		this->numero = numero;
	}

/** Sets y Gets **/
	void Carta::setValor(int valor){
		this->valor = valor;
	}

	int Carta::getValor(){ return this->valor; }

	void Carta::setPinta(char pinta){
		this->pinta = pinta;
	}
	
	char Carta::getPinta(){ return this->pinta; }
	
	void Carta::setNumero(int numero){
		this->numero = numero;
	}

	int Carta::getNumero(){ return this->numero; }

/** Serialización **/
	char* Carta::serializar(){

		/** 
		  * Los primeros dos seran para el valor [0 y 1]
		  * El siguiente para la pinta [2]
		  * Finalmente dos para el numero [3 y 4]
		  */
		char* serializado = (char*)malloc(sizeof(char)*5);

		const char* numero = FuncionesExtras::intToString(this->valor);

		if(this->valor > 9){
			serializado[0] = numero[0];
			serializado[1] = numero[1];
		}else{
			serializado[0] = 48;
			serializado[1] = numero[0];
		}

		serializado[2] = this->pinta;

		numero = FuncionesExtras::intToString(this->numero);

		if(this->numero > 9){
			serializado[3] = numero[0];
			serializado[4] = numero[1];
		}else{
			serializado[3] = 48;
			serializado[4] = numero[0];
		}

		return serializado;
	}

	void Carta::deserializar(char* serializado){
		//Asignando Valor
		if(serializado[0] == 0){
			this->valor = serializado[1] - 48;
		}else{
			this->valor = (serializado[0]-48)*10 + (serializado[1] - 48);
		}

		//Asignando pinta
		this->pinta = serializado[2];

		//Asignando numero
		if(serializado[3] == 0){
			this->numero = serializado[4] - 48;
		}else{
			this->numero = (serializado[3]-48)*10 + (serializado[4] - 48);
		}
	}