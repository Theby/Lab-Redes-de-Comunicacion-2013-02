#ifndef _JUGADOR_H_
#define _JUGADOR_H_

class Jugador{
	private:
		//permite identificar que jugador es, el jugador 0 es el Crupier
		int num_jugador;

		//Cartas que tiene en una mano
		std::vector<Carta> Mano_1;

		//Cartas en la otra mano
		std::vector<Carta> Mano_2;

		//Es verdadero si es que se esta usando la otra mano
		bool separado;

		//Numero de cartas en la mano 1
		int num_cartas_mano_1;
		
		//Numero de cartas en la mano 2
		int num_cartas_mano_2;

		//Puntaje de las cartas que tiene en la mano 1
		int puntaje_1;

		//Puntaje de las cartas que tiene en la mano 2
		int puntaje_2;

		//Cantidad de fichas que tiene el jugador
		int fichas;

	public:
		//Constructores
		Jugador(int num_jugador);

		//Sets y Gets
		void setNumJugador(int num_jugador);
		int getNumJugador();

		void setMano1(std::vector<Carta> Mano);
		void setMano1(Carta Carta);
		void setMano1(int posicion, Carta Carta);
		std::vector<Carta> getMano1();
		Carta getMano1(int posicion);

		void setMano2(std::vector<Carta> Mano);
		void setMano2(Carta Carta);
		void setMano2(int posicion, Carta Carta);
		std::vector<Carta> getMano2();
		Carta getMano2(int posicion);

		void setSeparado(bool separado);
		bool getSeparado();

		void setNumCartasMano1(int num_cartas_mano_1);
		int getNumCartasMano1();

		void setNumCartasMano2(int num_cartas_mano_2);
		int getNumCartasMano2();

		void setPuntaje1(int puntaje);
		int getPuntaje1();

		void setPuntaje2(int puntaje);
		int getPuntaje2();

		void setFichas(int fichas);
		int getFichas();

		//Funcionales
		int apostar(int cantidad);
		void vaciarManos();
		void separar();
};

#endif /** _JUGADOR_H_ **//