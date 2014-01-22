#include "SocketCliente.h"
		
/** Constructores **/
	SocketCliente::SocketCliente(){
		this->DescriptorServidor = 0;
	}

/** Sets y Gets **/
	void SocketCliente::setDescriptorServidor(int DescriptorServidor){
		this->DescriptorServidor = DescriptorServidor;
	}

	int SocketCliente::getDescriptorServidor(){ return this->DescriptorServidor; }

/** Funcionales **/
	//Se conecta a un servidor remoto mediane INET
	int SocketCliente::abreConexionINET(char *Host_Servidor, char *Servicio){
		system("clear");
		std::cout << "Comenzando conexion con el Servidor" << std::endl << std::endl;
		
		std::cout << "Obtieniendo direccion del servicio " << Servicio << "/tcp" << std::endl;
		this->Puerto = getservbyname (Servicio, "tcp");
		if (this->Puerto == NULL){
			std::cout << "Error al solicitar el servicio " << Servicio << "." << std::endl;
			std::cout << "Revise en /etc/services si el servicio realmente existe." << std::endl;
			return -1;
		}
		std::cout << "Servicio obtenido satisfactoriamente" << std::endl << std::endl;

		std::cout << "Obteniendo direccion IP del host " << Host_Servidor << std::endl;
		this->Host = gethostbyname(Host_Servidor);
		if (this->Host == NULL){
			std::cout << "Error al solicitar la direcion del Host " << Host_Servidor << "." << std::endl;
			std::cout << "Revise en /etc/hosts si el Host esta realmente registrado." << std::endl;
			return -1;
		}
		std::cout << "Direccion obtenida satisfactoriamente" << std::endl << std::endl;

		std::cout << "socket (AF_INET, SOCK_STREAM, 0)" << std::endl;
		this->DescriptorServidor = socket (AF_INET, SOCK_STREAM, 0);
		if (this->DescriptorServidor == -1){
			std::cout << "Error al abrir el socket" << std::endl;
			return -1;
		}
		std::cout << "Descriptor obtenido satisfactoriamente" << std::endl << std::endl;


		this->Direccion.sin_family = AF_INET;
		this->Direccion.sin_addr.s_addr = ((struct in_addr *)(this->Host->h_addr))->s_addr;
		this->Direccion.sin_port = this->Puerto->s_port;
		std::cout << "connect (Descriptor, (struct sockaddr *)&Direccion, sizeof (Direccion))" << std::endl;
		if (connect (this->DescriptorServidor, (struct sockaddr *)&this->Direccion, sizeof (this->Direccion)) == -1)		{
			std::cout << "No ha sido posible conectarse al socket" << std::endl;
			return -1;
		}
		std::cout << "Conectado satisfactoriamente al socket del Host" << std::endl << std::endl;

		system("clear");
		return this->DescriptorServidor;
	}
