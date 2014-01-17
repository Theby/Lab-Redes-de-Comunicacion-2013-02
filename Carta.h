#ifndef _CARTA_H_
#define _CARTA_H_

class Carta{
	private:
		//Para guardar cuanto vale esa carta en el BlackJack
		int valor;

		//Para saber si es corazón, pica, diamante o trebol
		string pinta;

		//Para saber si es AS(1), 2, 3, ..., jota(11), queena(12) o kayser(13).
		int numero;

	public:
		//Constructores
		Carta();
		Carta(int valor, string pinta, int numero);

		//Sets y Gets
		void setValor(int valor);
		int getValor();

		void setPinta(string pinta);
		string getPinta();

		void setNumero(int numero);
		int getNumero();
};

#endif /** _CARTA_H_ **/