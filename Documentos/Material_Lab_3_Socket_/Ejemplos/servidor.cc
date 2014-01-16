#include<iostream>
#include<cstdlib>
#include<sys/types.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include<stack>
#include<list>
#include <ctime>

using namespace std;
int puerto=5009;


int main(int argc,char **argv)
{
	//SE ABRE EL DESCRIPTOR DEL SOCKET(SE CREA EL SOCKET)
	int Descriptor;
	Descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1)
	{
    		printf ("Error1\n");
	}


	//AHORA SE AVISA AL SISTEMA OPERATIVO QUE SE TIENE ABIERTO EL SOCKET Y QUE SE VA A ATENDER EL SERVICIO POR EL PUERTO DADO
	struct sockaddr_in Direccion;
	Direccion.sin_family = AF_INET;//TIPO DE CONEXION A ORDENADORES DE CUALQUIER TIPO
	Direccion.sin_port = htons(puerto);
	Direccion.sin_addr.s_addr =INADDR_ANY;//DIRECCION DEL CLIENTE,,,,SE ATIENDE A CUALQUIERA

	if ( bind(Descriptor, (struct sockaddr *)&Direccion, sizeof (Direccion)) == -1)
	{
		printf ("Error2\n");
	} 

	//AHORA SE DICE AL SISTEMA QUE COMIENCE A ATENDEER LAS LLAMADAS QUE LLEGUEN, CON EL DESCRIPTOR DEL SOCKET Y EL NUMERO MAXIMO DE CLIENTES A ENCOLAR
	if (listen(Descriptor, 1) == -1)
	{
		printf ("Error3\n");
	}



	//CON ESTO SE ESPERA Y ATIENDE A CUALQUIER CLIENTE QUE LLEGUE, CON LOS DATOS QUE SE SACAN DEL CLIENTE DE LA LLAMADA ACCEPT
	struct sockaddr_in Cliente;
	int Descriptor_Cliente;
	socklen_t Longitud_Cliente = sizeof(Cliente);

	Descriptor_Cliente = accept(Descriptor, (struct sockaddr*)&Cliente, &Longitud_Cliente);
	if (Descriptor_Cliente == -1)
	{
    		printf ("Error4\n");
	} 

	//********** AHORA SE LEE LO QUE MANDA EL CLIENTE **************
	char buff[20];
	int max_leer=19;

	for(int i = 0; i < 5 ; i++)
	{
		memset(buff,0,max_leer+1);
		if(read(Descriptor_Cliente, buff, max_leer) == -1)
		{
			printf ("Error5 al leer\n");
		}
		printf("\n Mensaje %i recibido: %s",i+1,buff);
	}

	//SE CIERRA EL DESCRIPTOR DEL SOCKET
	printf("\nSE TERMINA LA CONEXION\n");
	close (Descriptor_Cliente); 
	close (Descriptor);
	return 0;
}
