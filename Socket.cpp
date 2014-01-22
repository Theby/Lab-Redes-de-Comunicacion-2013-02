#include "Socket.h"

Socket::Socket(){
	this->leido_escrito = 0;
	this->aux = 0;
	this->accion = "escribir" ;
}

void Socket::setLeidoEscrito(int leido_escrito){
	this->leido_escrito = leido_escrito;
}
		
int Socket::getLeidoEscrito(){ return this->leido_escrito; }

void Socket::setAux(int aux){
	this->aux = aux;
}
	
int Socket::getAux(){ return this->aux; }

void Socket::setAccion(std::string accion){
	this->accion = accion;
}

std::string Socket::getAccion(){ return this->accion; }

/**
 * Lee datos del socket para realizar la lectura
 */
int Socket::usarSocket(int fd, char* Datos, int Longitud){
	this->leido_escrito = 0;
	this->aux = 0;

	std::cout << std::endl << "" << fd << "/" << Datos[0] << "/" << Longitud << std::endl << std::endl;
	//Si la orden no corresponde
	if(this->accion != "leer" && this->accion != "escribir"){
		std::cout << std::endl << "La accion debe ser leer o escribir" << std::endl << std::endl;
		return -1;
	}

	/**
	 * Comprueba los datos de entradas
	 **/
	if ((fd == -1) || (Datos == NULL) || (Longitud < 1)){
		std::cout << std::endl << "O fd es -1, o Datos es Null, o Longitud es < 1 causando errores" << std::endl;
		return -1;
	}

	// Mientras queden datos por leer	 
	while (this->leido_escrito < Longitud){
		if (this->accion == "leer"){
			this->aux = read(fd, Datos + this->leido_escrito, Longitud - this->leido_escrito);
		}else if(this->accion == "escribir"){
			this->aux = write(fd, Datos + this->leido_escrito, Longitud - this->leido_escrito);
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
							std::cout << std::endl << "Error al leer" << std::endl << std::endl;
							return -1;
					}
				}else if(this->accion == "escribir"){
					std::cout << std::endl << "Error al escribir" << std::endl << std::endl;
					return -1;
				}
			}
		}
	}

	return this->leido_escrito;
}