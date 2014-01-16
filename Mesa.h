#ifndef _MESA_H_
#define _MESA_H_

class Mesa{
	private:
		//Listas de jugadores en la mesa, incluyendo el Crupier
		std::vector<Jugador> Jugadores;

		//Listas de apuestas donde la posicion representa al jugador
		std::vector<unsigned int> Apuestas;

		//Maso de la mesa
		Mazo Mazo;

	public:
		//Constructor
		Mesa();

		//Sets y Gets
		void setJugadores(std::vector<Jugador> Jugadores);
		void setJugadores(Jugador Jugador);
		std::vector<Jugador> getJugadores();
		Jugador getJugadores(unsigned int posicion);
		void setApuestas(std::vector<unsigned int> Apuestas);
		void setApuestas(unsigned int apuesta);
		std::vector<unsigned int> getApuestas();
		unsigned int getApuestas(unsigned int posicion);
		void setMazo(Mazo Mazo);
		Mazo getMazo();
};

#endif /** _MESA_H_ **/