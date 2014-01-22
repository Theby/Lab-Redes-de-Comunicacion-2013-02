#include "Cliente.h"

int main(){
	system("clear");

	Cliente C_BlackJack;

	char* buffer = (char*)malloc(sizeof(char)*1);

	C_BlackJack.abreConexionINET((char*)"leen",(char*)"blackjack");

	C_BlackJack.setAccion("leer");
	std::cout << "Comenzando a leer del Host" << std::endl;
	if(C_BlackJack.usarSocket(C_BlackJack.getDescriptorServidor(),buffer,sizeof(char)) < 0){
		return -1;
	}

	std::cout << "Recibí " << buffer << std::endl;

	while(1){
		C_BlackJack.setAccion("escribir");
		if(C_BlackJack.usarSocket(C_BlackJack.getDescriptorServidor(),buffer,sizeof(char)) < 0){
			return -1;
		}
		std::cout << "Escrito: " << buffer[0] << std::endl;
		sleep(1);
	}
	
	return 0;
}