#include "Cliente.h"


int main(){
	system("clear");

	//Crea un cliente de blackjack
	Cliente C_BlackJack;

	//crea un puntero para guardar el tipo
	char* tipo = (char*)malloc(sizeof(char)*1);
	int tipo_n;

	//Abre el socket y se conecta a el
	C_BlackJack.abreConexionINET((char*)"localhost",(char*)"blackjack");

	//Empieza las consecutivas lecturas
	while(true){
		//Entra en modo de lectura
		C_BlackJack.setAccion("leer");

		//Lee la cabecera del mensaje
		std::cout << "Comenzando a leer del Host" << std::endl;
		if(C_BlackJack.usarSocket(C_BlackJack.getDescriptorServidor(),tipo,sizeof(char)) < 0){
			return -1;
		}
		tipo_n = tipo[0]-48;

		std::cout << "tipo leido: " << tipo_n << std::endl;

		switch(tipo_n){
			//Es el identificador
			case 0:{
				//Para guardar el identificador
				char* c_id = (char*)malloc(sizeof(char)*1);
				int c_id_n;

				//Lee el mensaje
				if(C_BlackJack.usarSocket(C_BlackJack.getDescriptorServidor(),c_id,sizeof(char)) < 0){
					return -1;
				}
				c_id_n = c_id[0]-48;

				//Lo asigna donde corresponde
				C_BlackJack.setIdentificador(c_id_n);
				C_BlackJack.getJugador().setNumJugador(c_id_n);

				std::cout << "Jugador: " << C_BlackJack.getJugador().getNumJugador() << std::endl;

				//Solicita el estado del juego

				break;
			}

			//Es una carta
			case 1:{
				//Para guardar una carta
				char* c_cartas = (char*)malloc(sizeof(char)*5);

				//Lee el mensaje
				if(C_BlackJack.usarSocket(C_BlackJack.getDescriptorServidor(),c_cartas,sizeof(char)*5) < 0){
					return -1;
				}

				Carta prototipo;
				prototipo.deserializar(c_cartas);

				Jugador player = C_BlackJack.getJugador();
				player.setMano1(prototipo);

				C_BlackJack.setJugador(player);

				std::cout << " " + prototipo.getPinta()  << std::endl;
				std::cout << " " + prototipo.getNumero()  << std::endl;
				std::cout << " " + prototipo.getValor()  << std::endl;

				std::cout << " " + C_BlackJack.getJugador().getMano1(0).getPinta()  << std::endl;
				std::cout << " " + C_BlackJack.getJugador().getMano1(0).getNumero()  << std::endl;
				std::cout << " " + C_BlackJack.getJugador().getMano1(0).getValor()  << std::endl;

				break;
			}

			//Son fichas
			case 2:{
				break;
			}

			//Es informacion de interfaz
			case 3:{
				break;
			}
		}
	}

	/* while(1){
		C_BlackJack.setAccion("escribir");
		if(C_BlackJack.usarSocket(C_BlackJack.getDescriptorServidor(),buffer,sizeof(char)) < 0){
			return -1;
		}
		std::cout << "Escrito: " << buffer[0] << std::endl;
		sleep(1);
	} */
	
	return 0;
}