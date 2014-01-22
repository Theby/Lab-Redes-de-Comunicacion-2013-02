#include "Cliente.h"


int main(){
	system("clear");

	//Crea un cliente de blackjack
	Cliente C_BlackJack;
	bool reading = false;
	//crea un puntero para guardar el tipo
	char* tipo = (char*)malloc(sizeof(char)*1);
	int tipo_n;
	char pinta_c;
	int numero_c;
	int valor_c;
	int cartas_recibidas = 0;
	char opcion = 'k';
	int valor_t=0;
	int valor_t_c=0;

	while(opcion != '1'){
		system("clear");
		std::cout << "BLACKJACK" << std::endl << std::endl;

		std::cout << "1.- Conectarse" << std::endl;

		std::cin >> opcion;
	}

	//Abre el socket y se conecta a el
	C_BlackJack.abreConexionINET((char*)"localhost",(char*)"blackjack");

	//Empieza las consecutivas lecturas
	while(true){
		system("clear");

		//Entra en modo de lectura
		C_BlackJack.setAccion("leer");
		reading = true;
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
				cartas_recibidas++;

				if(cartas_recibidas < 3){
					Jugador player = C_BlackJack.getJugador();
					player.setMano1(prototipo);
					C_BlackJack.setJugador(player);
				}else{
					C_BlackJack.CartasCrupier.push_back(prototipo);
					reading=false;
				}
				
				std::cout << " " << prototipo.getPinta()  << std::endl;
				std::cout << " " << prototipo.getNumero()  << std::endl;
				std::cout << " " << prototipo.getValor()  << std::endl;

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

		opcion = 'k';
		system("clear");
		while(reading == false){
			std::cout << "BLACKJACK" << std::endl << std::endl;

			std::cout << "1.- Apostar" << std::endl;
			std::cout << "2.- Pedir una carta" << std::endl;
			std::cout << "3.- Salir" << std::endl << std::endl;

			std::cout << "Tu apuesa actual:" << C_BlackJack.getApuesta(0) << std::endl;
			std::cout << "Numero de fichas restantes: "  << C_BlackJack.getJugador().getFichas()-C_BlackJack.getApuesta(0) << std::endl << std::endl;
			std::cout << "Tus cartas:" << std::endl;
			valor_t = 0;

			pinta_c = C_BlackJack.getJugador().getMano1(0).getPinta();

			switch(pinta_c){
				case 'A':{
					std::cout << "  1) Corazon - ";
					break;
				}
				case 'B':{
					std::cout << "  1) Pica - ";
					break;
				}
				case 'C':{
					std::cout << "  1) Trebol - ";
					break;
				}
				case 'D':{
					std::cout << "  1) Diamante - ";
					break;
				}
			}

			numero_c = C_BlackJack.getJugador().getMano1(0).getNumero();

			switch(numero_c){
				case 1:{
					std::cout << "AS - ";
					break;
				}
				case 11:{
					std::cout << "JOTA - ";
					break;
				}
				case 12:{
					std::cout << "QUEEN - ";
					break;
				}
				case 13:{
					std::cout << "KAYSER - ";
					break;
				}
				default:{
					std::cout << "" << numero_c << " - ";
					break;
				}
			}

			valor_c = C_BlackJack.getJugador().getMano1(0).getValor();
			valor_t += valor_c;
			std::cout << "Valor: " << valor_c << std::endl << std::endl;

			pinta_c = C_BlackJack.getJugador().getMano1(1).getPinta();

			switch(pinta_c){
				case 'A':{
					std::cout << "  2) Corazon - ";
					break;
				}
				case 'B':{
					std::cout << "  2) Pica - ";
					break;
				}
				case 'C':{
					std::cout << "  2) Trebol - ";
					break;
				}
				case 'D':{
					std::cout << "  2) Diamante - ";
					break;
				}
			}

			numero_c = C_BlackJack.getJugador().getMano1(1).getNumero();

			switch(numero_c){
				case 1:{
					std::cout << "AS - ";
					break;
				}
				case 11:{
					std::cout << "JOTA - ";
					break;
				}
				case 12:{
					std::cout << "QUEEN - ";
					break;
				}
				case 13:{
					std::cout << "KAYSER - ";
					break;
				}
				default:{
					std::cout << "" << numero_c << " - ";
					break;
				}
			}

			valor_c = C_BlackJack.getJugador().getMano1(1).getValor();
			valor_t += valor_c;
			std::cout << "Valor: " << valor_c << std::endl << std::endl;
			std::cout << "Valor total: " << valor_t << std::endl << std::endl;

			std::cout << "Cartas Crupier: " << std::endl;
			valor_t_c = 0;

			pinta_c = C_BlackJack.CartasCrupier[0].getPinta();

			switch(pinta_c){
				case 'A':{
					std::cout << "  1) Corazon - ";
					break;
				}
				case 'B':{
					std::cout << "  1) Pica - ";
					break;
				}
				case 'C':{
					std::cout << "  1) Trebol - ";
					break;
				}
				case 'D':{
					std::cout << "  1) Diamante - ";
					break;
				}
			}

			numero_c = C_BlackJack.CartasCrupier[0].getNumero();

			switch(numero_c){
				case 1:{
					std::cout << "AS - ";
					break;
				}
				case 11:{
					std::cout << "JOTA - ";
					break;
				}
				case 12:{
					std::cout << "QUEEN - ";
					break;
				}
				case 13:{
					std::cout << "KAYSER - ";
					break;
				}
				default:{
					std::cout << "" << numero_c << " - ";
					break;
				}
			}

			valor_c = C_BlackJack.CartasCrupier[0].getValor();
			valor_t_c += valor_c;
			std::cout << "Valor: " << valor_c << std::endl << std::endl;

			if(C_BlackJack.CartasCrupier.size() == 2){
				pinta_c = C_BlackJack.CartasCrupier[1].getPinta();

				switch(pinta_c){
					case 'A':{
						std::cout << "  2) Corazon - ";
						break;
					}
					case 'B':{
						std::cout << "  2) Pica - ";
						break;
					}
					case 'C':{
						std::cout << "  2) Trebol - ";
						break;
					}
					case 'D':{
						std::cout << "  2) Diamante - ";
						break;
					}
				}

				numero_c = C_BlackJack.CartasCrupier[1].getNumero();

				switch(numero_c){
					case 1:{
						std::cout << "AS - ";
						break;
					}
					case 11:{
						std::cout << "JOTA - ";
						break;
					}
					case 12:{
						std::cout << "QUEEN - ";
						break;
					}
					case 13:{
						std::cout << "KAYSER - ";
						break;
					}
					default:{
						std::cout << "" << numero_c << " - ";
						break;
					}
				}

				valor_c = C_BlackJack.CartasCrupier[1].getValor();
				valor_t_c += valor_c;
				std::cout << "Valor: " << valor_c << std::endl << std::endl;				
			}

			std::cout << "Valor total: " << valor_t_c << std::endl;

			std::cin >> opcion;
			switch(opcion){
				case '1':{

					int apuesta_n = 0;
					while(apuesta_n == 0){
						std::cout << "" << std::endl;
						std::cout << "Ingrese valor a apostar : ";

						std::cin >> apuesta_n;

						if(apuesta_n < 0 || apuesta_n > 100){ 
							apuesta_n = 0;
						}
					}

					C_BlackJack.setApuestas(0,C_BlackJack.getApuesta(0)+apuesta_n);
					system("clear");
					break;
				}
				case '2':{
					break;
				}
				case '3':{
					return 0;
					break;
				}
			}
		}

	}
	
	return 0;
}