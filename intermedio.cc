/*
* Archivo que controla el nivel intermedio
*/

#include <string.h>
#include <omnetpp.h>

//Nombre de la clase y tipo
class intermedio : public cSimpleModule
{
    //Métodos
    protected:
        //Procesador de mensajes desde la capa superior
        virtual void processMsgFromHigherLayer(cMessage *packet);

        //Procesador de mensajes desde la capa inferior
        virtual void processMsgFromLowerLayer(cMessage *packet);

        //Receptor de mensajes
        virtual void handleMessage(cMessage *msg);
};

//Definición de funciones
Define_Module( intermedio );

//Manejador de mensajes
void intermedio::handleMessage(cMessage *msg)
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

//Procesador desde arriba
void intermedio::processMsgFromHigherLayer(cMessage *packet)
{
    //Envia el paquete hacia la capa de enlace
    send(packet,"hacia_abajo");
}

//Procesador desde abajo
void intermedio::processMsgFromLowerLayer(cMessage *packet)
{
    //Envía el paquete hacia la capa de aplicación
    send(packet,"hacia_arriba");
}
