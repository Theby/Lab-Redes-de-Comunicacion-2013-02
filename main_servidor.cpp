#include "Servidor.h"

int main(){
	system("clear");

	Servidor S_BlackJack;

	//Entrega el servicio blackjack ubicado en etc/services
	if(S_BlackJack.abreSocketINET((char*)"blackjack")){
		while(true){
			if(S_BlackJack.aceptaCliente(S_BlackJack.getDescriptorServidor()) < 0){
				return -1;
			}
		}
	}else{
		return -1;
	}	

	return 0;
}