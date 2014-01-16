#ifndef _MAZO_H_
#define _MAZO_H_

class Mazo{
	private:
		//Para guardar el tamaño maximo del mazo
		const int max_num_cartas_mazo = 52;

		//Para guardar todas las cartas linealmente como si fuera un mazo real
		std::vector<Carta> Cartas;

		//Para saber cuantas cartas se han repartido
		int num_cartas_repartidas;

		//Para saber cuantas cartas quedan en el mazo
		int num_cartas_mazo;

	public:
		//Constructores
		Mazo();
		Mazo(std::vector<Carta> Cartas);

		//Sets y Gets
		int getMaxNumCartasMazo();
		void setCartas(std::vector<Carta> Cartas);
		void setCartas(Carta Carta);
		std::vector<Carta> Cartas getCartas();
		Carta getCartas(int posicion);
		void setNumCartasRepartidas(int num_cartas_repartidas);
		int getNumCartasRepartidas();
		void setNumCartasMazo(int num_cartas_mazo);
		int getNumCartasMazo();
};

#endif /** _MAZO_H_ **/