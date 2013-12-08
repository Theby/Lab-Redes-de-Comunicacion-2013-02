/*
* Archivo que controla el nivel intermedio
*/

#include <string.h>
#include <omnetpp.h>

#include <DataFrame_m.h>
#include <intermedio.h>

using namespace std;

//Define la clase para trabajar directamente con el modulo intermedio
Define_Module( intermedio );

void intermedio::handleMessage(cMessage* msg)
{
    //Si el mensaje viene desde enlace
    if (msg->arrivedOn("desde_abajo"))
        //Enviar al procesador el mensaje para mandarlo a aplicacion
        processMsgFromLowerLayer(msg);
    //Sino, el mensaje viene desde arriba
    else
        //Procesar el mensaje para enviar a enlace
        processMsgFromHigherLayer(msg);
}

void intermedio::processMsgFromHigherLayer(cMessage *packet)
{
    //Envia el paquete hacia la capa de enlace
    send(packet,"hacia_abajo");
}

void intermedio::processMsgFromLowerLayer(cMessage *packet)
{
    //Envía el paquete hacia la capa de aplicación
    send(packet,"hacia_arriba");
}
