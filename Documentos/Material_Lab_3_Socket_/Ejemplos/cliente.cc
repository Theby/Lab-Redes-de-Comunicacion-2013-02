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
int puerto= 5009;


int main(int argc,char **argv)
{

	//SE CONSULTA POR DIRECCION IP DEL SERVIDOR
	printf("\nIngrese direccion IP del servidor: ");
	char ip[15];
	gets(ip);


	//*********** SE DEFINE LA ESTRUCTURA DEL CLIENTE Y SE CONECTA AL SERVIDOR ************************
	int Descriptor;
	Descriptor = socket (AF_INET, SOCK_STREAM, 0);
	if (Descriptor == -1)
	{
    		printf ("Error1\n");
	}
	
	struct sockaddr_in Direccion;
	Direccion.sin_family = AF_INET;
	Direccion.sin_addr.s_addr = inet_addr(ip);//((structin_addr*)(Host->h_addr))->s_addr;
	Direccion.sin_port = htons(puerto);

	if (connect(Descriptor, (struct sockaddr *)&Direccion, sizeof(Direccion) ) == -1)
	{
    		printf ("Error2\n");
	}

	//************* AHORA SE COMIENZA CON EL ENVIO DE LOS 5 MENSAJES ****************************

	char buff[20];
	int max=19;

	//strcpy(buff,"Hola");
	//write(Descriptor, buff, max);
	


	for(int i = 0; i < 5 ; i++)
	{
		memset(buff,0,max+1);		
		printf("\nIngrese Mensaje %i: ",i+1);
		fflush( stdout );
		gets(buff);
		write(Descriptor, buff, max);		
	}
	close(Descriptor); 
}

