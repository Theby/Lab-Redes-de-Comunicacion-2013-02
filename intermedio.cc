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
    string packet_name = packet->getName();

    if(packet_name == "START"){
        //Verde de esperando confirmación
        if (ev.isGUI()){
            getDisplayString().setTagArg("i",1,"green");
            bubble("Esperando Confirmación");
        }
    }
    //Negro de desconectado
    else if(packet_name == "END"){
        getDisplayString().setTagArg("i",1,"black");
        bubble("Desconectado!");
    }
    //Envia el paquete hacia la capa de enlace
    send(packet,"hacia_abajo");
}

void intermedio::processMsgFromLowerLayer(cMessage *packet)
{
    string packet_name = packet->getName();

    //Normal de activo
    if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

    if((packet_name[0]=='A' && packet_name[1]=='C' && packet_name[2]=='K' && packet_name[4]=='0') || packet_name == "CONNECT"){
        if (ev.isGUI()) bubble("Conectado!");
    }else if(packet_name == "DISCONNECT"){
        //Negro de desconectado
        getDisplayString().setTagArg("i",1,"black");
        bubble("Desconectado!");
    }

    //Envía el paquete hacia la capa de aplicación
    send(packet,"hacia_arriba");
}
