/*
* En éste módulo se generan palabras de información, la cual viaja
* a los modulos inferiores para ser enviada al otro Host a través del canal
*/

#include <string.h>
#include <omnetpp.h>
#include <cstdlib>
#include <iostream>

#include <DataFrame_m.h>
#include <aplicacion.h>

using namespace std;

//Entero para dar turno de transmisión
int turno=0;

//Define la clase para trabajar directamente con el modulo de aplicacion
Define_Module( aplicacion );

void aplicacion::initialize()
{
    //Si es mi turno
    if(turno==0)
    {
        //Generar palabra para envío
        generaInfo();
        //Entregar el turno
        turno=1;
    }
}

void aplicacion::handleMessage(cMessage* msg)
    {
    //Si el mensaje ha llegado desde intermedio
    if (msg->arrivedOn("desde_abajo"))
    {
        //Borrar el mensaje
        DataFrame* tramaInformacion = check_and_cast<DataFrame *>(msg);
        ev<<"Dato "<< tramaInformacion->getAddress() <<" recibido. Nombre " << tramaInformacion->getName();
        delete msg;
    }
    //Generar un mensaje de respuesta
    generaInfo();
}

void aplicacion::generaInfo()
{
    //Obtiene la dirección del Host en el que se esta ejecutando, la cual es seteada por sistema.ned
    int direccion = par("direccion");

    //Obtiene el tamaño de la trama, el cual por defecto es 4 en sistema.ned
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

    DataFrame *tramaInformacion = new DataFrame(mens);
    tramaInformacion->setAddress(1011);

    //Se envia el mensaje a intermedio
    send(tramaInformacion, "hacia_abajo");

    //Se informa al usuario de que se envió la palabra
    ev<<"Host "<<direccion<<" - LA PALABRA QUE SE ENVIO DESDE APLICACION ES: "<< mens << " DE TAMAÑO: " << tamT;
}
