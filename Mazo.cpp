#include "Mazo.h"

using namespace std;

/** Constructores **/
	Mazo::Mazo(){
		this->Cartas = new vector();
		this->num_cartas_repartidas=0;
		this->num_cartas_mazo=0;
	}

	Mazo::Mazo(std::vector<Carta> Cartas){
		this->Cartas = Cartas;
		this->num_cartas_repartidas=0;
		this->num_cartas_mazo=Cartas.size();
	}