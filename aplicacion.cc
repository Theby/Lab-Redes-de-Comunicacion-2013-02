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

//Identificador del iniciador de la transmisión
int master;

//Define la clase para trabajar directamente con el modulo de aplicacion
Define_Module( aplicacion );

void aplicacion::initialize()
{
    //obtiene el valor de maste para saber quien parte
    master = par("starter");

    //obtiene el valor del host en el que se ubica
    int address_host = par("direccion_host");

    //Es el turno de quien tenga la misma address que numero de master
    if(address_host == master){
        //Ya nadie es el master
        master=42;

        //Generar palabra para envío
        generaInfo();
    }
}

void aplicacion::handleMessage(cMessage* msg){
    string ack = "ACK";
    string dato = "DATO";
    string msg_name = msg->getName();

    //Si el mensaje ha llegado desde intermedio
    if (msg->arrivedOn("desde_abajo")){
        if(msg_name == ack){
            ev << "El mensaje: " << msg_name << " correctamente recivido." << endl;
            delete msg;
            generaInfo();
        }else if(msg_name == dato){
            ev << "El dato: " << msg_name << " correctamente recivido." << endl;
            delete msg;
        }else{
            ev << "El mensaje: " << msg_name << " No es valido y ha sido eliminado" << endl;
            delete msg;
        }
    }else{
        ev << "Mensaje llegado desde un lugar desconocido" << endl;
    }
}

void aplicacion::generaInfo()
{
    //Obtiene la dirección del destino al cual se le enviará la información, la cual es seteada por sistema.ned
    int address_dest = par("direccion_dest");

    //Obtiene el tamaño de la trama, el cual por defecto es 4 en sistema.ned
    int tamTrama = par("tamTrama");

    //Puntero al mensaje
    char *mens;
        //Reserva de memoria para el mensaje
        mens = (char*)malloc(sizeof(char)*tamTrama);

        //Se crea el mensaje solo con valores 0
        strcpy(mens, "0");

        //Se concatenan ceros al mensaje
        for(int i=1;i<tamTrama;i++){
            strcat(mens, "0");
        }

    //Creando tramas completas
    DataFrame *tramaInformacion = new DataFrame(mens);

    //Inicio Address
        //Asigna la direccion al sector address de la trama
        for(unsigned int i=0;i<8;i++){
            if(address_dest<pow(10,(8-(i+1)))){
                tramaInformacion->setAddress(i,0);
            }else{
                tramaInformacion->setAddress(i,1);
            }
        }
    //Fin Address

    //Inicio Control
        //De informacion: control[0] = 0
        tramaInformacion->setControl(0,0);

        //N(S)
        tramaInformacion->setControl(1,0);
        tramaInformacion->setControl(2,0);
        tramaInformacion->setControl(3,0);

        //bit P/F
        tramaInformacion->setControl(4,0);

        //N(R)
        tramaInformacion->setControl(5,0);
        tramaInformacion->setControl(6,0);
        tramaInformacion->setControl(7,0);
    //Fin Control

    //Inicio Informacion
        tramaInformacion->setInformationArraySize(tamTrama);

        for(int i=0;i<tamTrama;i++){
            tramaInformacion->setInformation(i,1);
        }
    //Fin Informacion

    //Inicio FCS
        for(int i=0;i<16;i++){
            tramaInformacion->setFCS(i,0);
        }
    //Fin FCS

    //Se envia el mensaje a intermedio
    send(tramaInformacion, "hacia_abajo");

    //Informando al Usuario el dato enviado
    ev <<"Destino: "<< address_dest;
}
