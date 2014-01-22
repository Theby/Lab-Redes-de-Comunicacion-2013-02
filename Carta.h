#ifndef _CARTA_H_
#define _CARTA_H_

#include <cstdlib>

class Carta{
	private:
		//Para guardar cuanto vale esa carta en el BlackJack
		int valor;

		//Para saber si es corazón, pica, diamante o trebol
		char pinta;

		//Para saber si es AS(1), 2, 3, ..., jota(11), queena(12) o kayser(13).
		int numero;

	public:
		//Constructores
		Carta();
		Carta(int valor, char pinta, int numero);

		//Sets y Gets
		void setValor(int valor);
		int getValor();

		void setPinta(char pinta);
		char getPinta();

		void setNumero(int numero);
		int getNumero();

		//Serializacion
		char* serializar();
		void deserializar(char* serializado);
};

#endif /** _CARTA_H_ **/