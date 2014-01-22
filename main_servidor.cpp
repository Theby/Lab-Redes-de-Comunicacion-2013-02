#include "Servidor.h"

int main(){
	system("clear");

	Servidor S_BlackJack;

	Mazo nuevo_mazo;

	nuevo_mazo.setCartas(nuevo_mazo.generarCartas());

	S_BlackJack.setMazo(nuevo_mazo);
	std::cout << "Mazo con " << S_BlackJack.getMazo().getNumCartasMazo() << std::endl;
	S_BlackJack.getMazo().revolverMazo();


	//Entrega el servicio blackjack ubicado en etc/services
	if(S_BlackJack.abreSocketINET((char*)"blackjack")){
		while(true){
			if(S_BlackJack.aceptaCliente(S_BlackJack.getDescriptorServidor()) < 0){
				std::cout << "Error aceptando al cliente" << std::endl << std::endl;
				return -1;
			}
		}
	}else{
		std::cout << "Error abriendo el socket" << std::endl << std::endl;
		return -1;
	}	

	return 0;
}