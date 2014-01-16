#ifndef _MESA_H_
#define _MESA_H_

class Mesa{
	private:
		//Listas de jugadores en la mesa
		std::vector<Jugador> Jugadores;

		//Listas de apuestas donde la posicion representa al jugador
		std::vector<unsigned int> Apuestas;

		//Para saber si el jugador esta listo con su turno
		std::vector<bool> Ready;

	public:
		//Constructor
		Mesa();

		//Sets y Gets
		void setJugadores(std::vector<Jugador> Jugadores);
		void setJugadores(Jugador Jugador);
		void setJugadores(unsigned int posicion, Jugador Jugador);
		std::vector<Jugador> getJugadores();
		Jugador getJugadores(unsigned int posicion);

		void setApuestas(std::vector<unsigned int> Apuestas);
		void setApuestas(unsigned int apuesta);
		void setApuestas(unsigned int posicion, unsigned int apuesta);
		std::vector<unsigned int> getApuestas();
		unsigned int getApuestas(unsigned int posicion);

		void setReady(std::vector<bool> Ready);
		void setReady(bool ready);
		void setReady(unsigned int posicion, bool ready);
		std::vector<bool> getReady();
		bool getReady(unsigned int posicion);

};

#endif /** _MESA_H_ **/