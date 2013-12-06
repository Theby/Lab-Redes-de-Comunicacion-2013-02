/*
* En éste módulo se generan palabras de información, la cual viaja
* a los modulos inferiores para ser enviada al otro Host a través del canal
*/

#include <string.h>

//librería de OMNeT++
#include <omnetpp.h>

#include <cstdlib>
#include <iostream>

using namespace std;

//Nombre de la clase y tipo de la librería omnetpp.h
class aplicacion : public cSimpleModule
{
    //Métodos de la clase
    public:
        //Inicializar módulo
        virtual void initialize();

        //Manejador de mensajes
        virtual void handleMessage(cMessage *msg);

        //Generador de palabras
        virtual void generaPalabraInfo();
};

//Entero que define quien está enviando y quien está recibiendo
int turno=0;

//Definición de las funciones de aplicación
Define_Module(aplicacion);

//Inicializar
void aplicacion::initialize()
{
    //Si es mi turno
    if(turno==0)
    {
        //Generar palabra para envío
        generaPalabraInfo();
        //Entregar el turno
        turno=1;
    }
}

//Manejar mensajes
void aplicacion::handleMessage(cMessage *msg)
    {
    //Si el mensaje ha llegado desde intermedio
    if (msg->arrivedOn("desde_abajo"))
    {
        //Borrar el mensaje
        delete msg;//cuando llega un mensaje solo se descarta
    }
    //Generar un mensaje de respuesta
    generaPalabraInfo();
}

//Generador de palabras/tramas
void aplicacion::generaPalabraInfo()
{
    //Se entrega la dirección
    int direccion = par("direccion");
    //Se entrega el tamaño
    int tamT = par("tamTrama");
    //Puntero al mensaje
    char *mens;

    //Reserva de memoria para el mensaje
    mens = (char*)malloc(sizeof(char)*tamT);

    //Se crea el mensaje solo con valores 0
    strcpy(mens, "0");

    //Se concatenan ceros al mensaje
    for(int i=1;i<tamT;i++)
    {
        strcat(mens, "0");
    }

    //Se crea el mensaje
    cMessage *palabra = new cMessage(mens);

    //Se envia el mensaje a intermedio hacia abajo
    send(palabra, "hacia_abajo");

    //Se informa al usuario de que se envió la palabra
    ev<<"Host "<<direccion<<" - LA PALABRA QUE SE ENVIO DESDE APLICACION ES: "<<mens;
}
