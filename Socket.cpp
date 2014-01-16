#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include "Socket.h"

void setLeidoEscrito(int leido_escrito){
	this->leido_escrito = leido_escrito;
}
		
int getLeidoEscrito(){ return this->leido_escrito; }

void setAux(int aux){
	this->aux = aux;
}
	
int getAux(){ return this->aux; }

/**
 * Lee datos del socket para realizar la lectura
 */
int Socket::usarSocket(int fd, char *Datos, int Longitud){
	//Si la orden no corresponde
	if(this->accion != "leer" && this->accion != "escribir"){
		return -1;
	}

	/**
	 * Comprueba los datos de entradas
	 **/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1)){
		return -1;
	}

	// Mientras queden datos por leer	 
	while (this->leido_escrito < Longitud){
		if (this->accion == "leer"){
			this->aux = read (fd, Datos + this->leido_escrito, Longitud - this->leido_escrito);
		}else if(this->accion == "escribir"){
			this->aux = write (fd, Datos + this->leido_escrito, Longitud - this->leido_escrito);
		}
		
		if (this->aux > 0){
			//Se actualizan los datos leido_escritos			 
			this->leido_escrito = (this->leido_escrito + this->aux);

		//Si se ha cerrado el socket
		}else{
			if (this->aux == 0){
				return this->leido_escrito;
			}else if(this->aux == -1){
				if(this->accion == "leer"){
					/**
					 * En caso de error, la variable errno nos indica el tipo
					 * de error. 
					 * El error EINTR se produce si ha habido alguna
					 * interrupcion del sistema antes de leer ningun dato. No
					 * es un error realmente.
					 * El error EGAIN significa que el socket no esta disponible
					 * de momento, que lo intentemos dentro de un rato.
					 * Ambos errores se tratan con una espera de 100 microsegundos
					 * y se vuelve a intentar.
					 * El resto de los posibles errores provocan que salgamos de 
					 * la funcion con error.
					 */
					switch (errno){
						case EINTR:
						case EAGAIN:
							usleep (100);
							break;
						default:
							return -1;
					}
				}else if(this->accion == "escribir"){
					return -1;
				}
			}
		}
	}

	return this->leido_escrito;
}