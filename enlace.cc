/* Envía las palabras desde la capa intermedia al otro host
* y recibe las palabras que llegan desde el otro host y las envía al nivel superior
*/

#include <string.h>
#include <omnetpp.h>

#include <DataFrame_m.h>
#include <enlace.h>

using namespace std;

//Define la clase para trabajar directamente con el modulo de enlace
Define_Module( enlace );

void enlace::handleMessage(cMessage *msg)
{
    //Si el mensaje llega desde el otro Host
    if (msg->arrivedOn("desde_fisico"))
    {
        //Procesarlo como si viniera desde abajo
        processMsgFromLowerLayer(msg);
    }
    //Sino, el mensaje viene desde intermedio
    else
    {
        //Procesarlo como si viniera desde arriba
        processMsgFromHigherLayer(msg);
    }
}

void enlace::processMsgFromHigherLayer(cMessage *dato)
{
    send(dato,"hacia_fisico");
}

void enlace::processMsgFromLowerLayer(cMessage *packet)
{
    send(packet,"hacia_arriba");
}
