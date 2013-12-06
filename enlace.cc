/* Envía las palabras desde la capa intermedia al otro host
* y recibe las palabras que llegan desde el otro host y las envía al nivel superior
*/

#include <string.h>
#include <omnetpp.h>

//Nombre y tipo de la clase
class enlace : public cSimpleModule
{
    //Métodos
    protected:
        //Procesador de mensaje desde Intermedio
        virtual void processMsgFromHigherLayer(cMessage *dato);

        //Procesador de Mensaje desde el otro host
        virtual void processMsgFromLowerLayer(cMessage *packet);

        virtual void handleMessage(cMessage *msg);
};

//Definición de los métodos
Define_Module( enlace );

//Manejador de mensajes
void enlace::handleMessage(cMessage *msg)
{
    //Si el mensaje llega desde el otro Host
    if (msg->arrivedOn("desde_fisico"))
    {
        //Procesarlo como si viene desde abajo
        processMsgFromLowerLayer(msg);
    }
    //Sino, el mensaje viene desde intermedio
    else
    {
        //Procesarlo como si vieje desde arriba
        processMsgFromHigherLayer(msg);
    }
}

//Si la palabra llega desde intermedio enviar al otro host
void enlace::processMsgFromHigherLayer(cMessage *dato)
{
    //Enviar el paquete a través del canal
    send(dato,"hacia_fisico");
}

//Si la palabra llega desde el otro host enviar hacia intermedio
void enlace::processMsgFromLowerLayer(cMessage *packet)
{
    //Enviar el paquete hacia el nivel intermedio
    send(packet,"hacia_arriba");
}
