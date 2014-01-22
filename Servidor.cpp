#include "Servidor.h"

/** Constructor **/
	Servidor::Servidor(){
		this->fichas_ganadas = 0;
	}

/** Sets y Gets **/
	void Servidor::setCrupier(Jugador Crupier){
		this->Crupier = Crupier;
	}

	Jugador Servidor::getCrupier(){ return this->Crupier; }

	void Servidor::setMazo(Mazo Mazo){
		this->mMazo = Mazo;
	}

	Mazo Servidor::getMazo(){ return this->mMazo; }

	void Servidor::setDeposito(Mazo Deposito){
		this->mDeposito = Deposito;
	}

	Mazo Servidor::getDeposito(){ return this->mDeposito; }

	void Servidor::setFichasGanadas(int fichas_ganadas){
		this->fichas_ganadas = fichas_ganadas;
	}

	int Servidor::getFichasGanadas(){ return this->fichas_ganadas; }

	/**
	 * Se le pasa un socket de servidor y acepta conexiones mediante el
	 * uso de select()
	 */
	int Servidor::aceptaCliente(int DescriptorServidor){
		std::cout << std::endl << "~~~ Comenzando proceso de aceptacion de clientes ~~~" << std::endl << std::endl;
		
		char* buffer_lectura = (char*)malloc(sizeof(char)*1);
		char* buffer_num_cliente = (char*)malloc(sizeof(char)*1);
		char* buffer_carta;
		char* tipo = (char*)malloc(sizeof(char)*1);
		int contador;

		//Limpia el vector de descriptores de los clientes ya desconetados
		contador = 0;
		std::cout << "Limpiando DescriptorClientes de clientes desconetados" << std::endl;
		for (int it = 0; it < this->DescriptorClientes.size(); it++){
			if(this->DescriptorClientes[it] == -1){
				this->DescriptorClientes.erase(this->DescriptorClientes.begin()+it);
				contador++;
			}
		}
		std::cout << "Proceso terminado satisfactoriamente con " << contador << " clientes_borrados" << std::endl << std::endl;

		//Inicializa el arreglo para clientes
		std::cout << "FD_FZERO(&Clientes)" << std::endl;
		FD_ZERO(&this->Clientes);
		std::cout << "Inicialización de Clientes completada" << std::endl << std::endl;

		//Asigna el descriptor del servidor al arreglo en la posicion 0
		std::cout << "FD_SET(DescriptorServidor, &Clientes)" << std::endl;
		FD_SET(DescriptorServidor, &this->Clientes);
		std::cout << "DescriptorServidor asignado a Clientes de forma satisfactoria" << std::endl << std::endl;

		//Asigna el valor maximo
		this->maximo = 0;
		if(DescriptorServidor >= this->maximo){ 
			this->maximo = DescriptorServidor + 1;
		}
		std::cout << "Valor actual de maximo: " << this->maximo << std::endl;
		std::cout << "Valor de DescriptorServidor: " << this->DescriptorServidor << std::endl << std::endl;

		//Añade los iteradores al arreglo para los servidores conectados
		std::cout << "Comenzando ciclo iterativo de asignacion de descriptores de clientes" << std::endl;
		contador = 0;
		for (int it = 0; it < DescriptorClientes.size(); it++){
			std::cout << "   -FD_SET(" << DescriptorClientes[it] << ", &Clientes)" << std::endl;
			FD_SET(DescriptorClientes[it], &this->Clientes);
			std::cout << "    Asignacion realizada con exito" << std::endl;
			//Vuelve a asignar el maximo en caso de que haya uno mayor
			if(DescriptorClientes[it] >= this->maximo){
				this->maximo = DescriptorClientes[it] + 1;
				std::cout << "    Nuevo valor de maximo: " << this->maximo << std::endl;
			}
			contador++;
		}
		std::cout << "Ciclo terminado, con " << contador << " Clientes actualmente conectados" << std::endl << std::endl;

		//Si se ha leido algo
		std::cout << "select(" << this->maximo << ",&Clientes,NULL,NULL,NULL)" << std::endl;
		if(select(this->maximo,&this->Clientes,NULL,NULL,NULL) > 0){

			std::cout << "FD_ISSET(DescriptorServidor,&Clientes)" << std::endl;
			if(FD_ISSET(DescriptorServidor,&this->Clientes)){
				std::cout << "Nuevo Cliente detectado" << std::endl;
				//Se ha conectado un nuevo usuario
				//Agrega el descriptor del cliente dado po accept
				std::cout << "   accept(DescriptorServidor,(struct sockaddr *)&Direccion,&largo)" << std::endl;
				this->largo = sizeof((struct sockaddr *)&this->Direccion);
				this->DescriptorClientes.push_back(accept(DescriptorServidor,(struct sockaddr *)&this->Direccion,&this->largo));
				std::cout << "   accept realizado satisfactoriamente" << std::endl;

				if(DescriptorClientes.size() > 7){
					std::cout << "Cliente rechazado ya que no hay más capacidad permitida" << std::endl;
					close(DescriptorClientes.back());
					DescriptorClientes.erase(DescriptorClientes.end());
				}else{		
					contador = 0;		
					this->setAccion("escribir");

					tipo[0] = '0';

					buffer_num_cliente[0] = DescriptorClientes.back()+48%10;

					//escribe al cliente su identificador
					std::cout << "   Comenzando escritura en el socket" << std::endl;
					std::cout << "   Se enviará el numero de cliente: " << DescriptorClientes.size() << std::endl;
					std::cout << "      Comenzando a escribir tipo usando el socket" << std::endl;
					contador = this->usarSocket(this->DescriptorClientes.back(),tipo,sizeof(char)*1);
					std::cout << "      Terminado de escribir el tipo en el socket, con " << contador << " escrituras" << std::endl;
					std::cout << "      Comenzando a escribir identificador usando el socket" << std::endl;
					contador = this->usarSocket(this->DescriptorClientes.back(),buffer_num_cliente,sizeof(char)*1);
					std::cout << "      Terminado de escribir identificador en el socket, con " << contador << " escrituras" << std::endl;
					std::cout << "   Cliente aceptado" << std::endl;

					for(int i=0; i<3 ; i++){
						tipo[0] = '1';
						Mazo test = this->getMazo();
						Carta prototipo = test.repartirCarta();
						this->setMazo(test);
						buffer_carta = prototipo.serializar();

						std::cout << " " << prototipo.getPinta()  << std::endl;
						std::cout << " " << prototipo.getNumero()  << std::endl;
						std::cout << " " << prototipo.getValor()  << std::endl;

						//Manda una carta
						std::cout << "   Comenzando escritura en el socket" << std::endl;
						std::cout << "   Se enviará una carta: " << std::endl;
						std::cout << "      Comenzando a escribir tipo usando el socket" << std::endl;
						contador = this->usarSocket(this->DescriptorClientes.back(),tipo,sizeof(char)*1);
						std::cout << "      Terminado de escribir el tipo en el socket, con " << contador << " escrituras" << std::endl;
						std::cout << "      Comenzando a escribir carta usando el socket" << std::endl;
						contador = this->usarSocket(this->DescriptorClientes.back(),buffer_carta,sizeof(char)*5);
						std::cout << "      Terminado de escribir carta en el socket, con " << contador << " escrituras" << std::endl;
						
					}
					std::cout << "Terminado de repartir cartas" << std::endl;

				}
				std::cout << "Proceso de aceptacion del Cliente terminado" << std::endl << std::endl; 
			}

			std::cout << "Comenzando ciclo iterativo para comprobar cuales clientes" << std::endl << "han solicitado conexion" << std::endl;
			contador = 0;
			for (int it = 0; it < DescriptorClientes.size(); it++){
				//Si ese cliente realizó alguna acción
				std::cout << "   -FD_ISSET(" << DescriptorClientes[it] << ", &Clientes)" << std::endl;
				if(FD_ISSET(DescriptorClientes[it], &this->Clientes)){
					std::cout << "    Atendiendo al cliente: " << it << std::endl;

					this->setAccion("leer");

					//Si se lee el mensaje
					std::cout << "    Comenzando lectura desde el socket" << std::endl;
					if(this->usarSocket(this->DescriptorClientes[it],buffer_lectura,sizeof(char))){
					
						std::cout << "Mensaje leido: " << buffer_lectura[0] << std::endl << std::endl;

					//Si el cliente se desconecto
					}else{
						std::cout << "    Cliente desconectado, marcando con -1" << std::endl;
						this->DescriptorClientes[it] = -1;
					}

					std::cout << "    Cliente: " << it << " Atendido" << std::endl;
					contador++;
				}
				std::cout << "    Cliente " << it << " Revisado" << std::endl;
			}
			std::cout << "Ciclo terminado, atendido a " << contador << " Clientes" << std::endl << std::endl;

		//Si hubo un error
		}else{
			std::cout << "Error al realizar el select" << std::endl << std::endl;
			return -1;
		}

		//Todo terminó bien
		return 1;
	}