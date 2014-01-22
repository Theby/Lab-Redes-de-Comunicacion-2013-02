#ifndef _MAZO_H_
#define _MAZO_H_

#include "Carta.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

class Mazo{
	private:
		//Para guardar el tamaño maximo del mazo, son 4 mazos
		int max_num_cartas_mazo;

		//Para guardar todas las cartas linealmente como si fuera un mazo real
		std::vector<Carta> Cartas;

		//Para saber cuantas cartas se han repartido
		int num_cartas_repartidas;

		//Para saber cuantas cartas quedan en el mazo
		int num_cartas_mazo;

		//Para generar un numero al azar, usado al revolver
		int randomNumber(int i);

	public:
		//Constructores
		Mazo();
		Mazo(std::vector<Carta> Cartas);

		//Sets y Gets
		int getMaxNumCartasMazo();

		void setCartas(std::vector<Carta> Cartas);
		void setCartas(Carta mCarta);
		void setCartas(int posicion,Carta mCarta);
		std::vector<Carta> getCartas();
		Carta getCartas(int posicion);

		void setNumCartasRepartidas(int num_cartas_repartidas);
		int getNumCartasRepartidas();

		void setNumCartasMazo(int num_cartas_mazo);
		int getNumCartasMazo();

		//Funcionales
		void revolverMazo();
		Carta repartirCarta();
		std::vector<Carta> generarCartas();
};

#endif /** _MAZO_H_ **/