#ifndef _JUGADOR_H_
#define _JUGADOR_H_

class Jugador{
	private:
		//permite identificar que jugador es, el jugador 0 es el Crupier
		unsigned int num_jugador;

		//Cartas que tiene en la mano
		std::vector<Carta> Mano;

		//Puntaje de las cartas que tiene en lamano
		unsigned int puntaje;

		//Cantidad de fichas que tiene el jugador
		unsigned int fichas;

	public:
		//Constructores
		Jugador();

		//Sets y Gets
		void setNumJugador(unsigned int num_jugador);
		unsigned int getNumJugador();
		void setMano(std::vector<Carta> Mano);
		void setMano(Carta Carta);
		std::vector<Carta> getMano();
		Carta getMano(unsigned int posicion);
		void setPuntaje(unsigned int puntaje);
		unsigned int getPuntaje();
		void setFichas(unsigned int fichas);
		unsigned int getFichas();
};

#endif /** _JUGADOR_H_ **//