/*
* En éste módulo se generan palabras de información, la cual viaja
* a los modulos inferiores para ser enviada al otro Host a través del canal
*/

#include <string.h>
#include <omnetpp.h>
#include <cstdlib>
#include <iostream>

#include <DataFrame_m.h>
#include <Informacion_m.h>
#include <aplicacion.h>

#include <FuncionesExtras.h>

using namespace std;

void aplicacion::generaInfo(int trama_id){
    //Si es primera vez que se inicia
    if(trama_id == -1){
        //creando un mensaje START
        cMessage *start = new cMessage("START");

        //Enviando el mensaje a Enlace
        send(start, "hacia_abajo");
        ev << "Enviando mensaje START a Enlace" << endl;
    }else{
        //Numero maximo de tramas a enviar
        int numTramas = par("numTramas");

        if(numTramas == trama_id){
            //creando un mensaje END
            cMessage *end = new cMessage("END");

            //Enviando el mensaje a Enlace
            send(end, "hacia_abajo");
            ev << "Enviando mensaje END a Enlace" << endl;
        }else{
            //Obtiene la dirección del destino al cual se le enviará la información, la cual es seteada por sistema.ned
            int address_dest = par("direccion_dest");

            //Obtiene el tamaño de la trama, el cual por defecto es 4 en sistema.ned
            int tamTrama = par("tamTrama");

            //Creando trama de comunicación con enlace
            Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrando("DATO,",trama_id));

            //Inicio Address
                //Asigna la direccion al sector address de la trama
                for(unsigned int i=0;i<8;i++){
                    if(address_dest<pow(10,(8-(i+1)))){
                        tramaComunicacion->setAddress_dest(i,0);
                    }else{
                        tramaComunicacion->setAddress_dest(i,1);
                    }
                }
            //Fin Address

            //Inicio Informacion
                tramaComunicacion->setInformacionArraySize(tamTrama);

                for(int i=0;i<tamTrama;i++){
                    tramaComunicacion->setInformacion(i,1);
                }
            //Fin Informacion

            //Se envia el mensaje a intermedio
            send(tramaComunicacion, "hacia_abajo");

            //Informando al Usuario el dato enviado
            ev <<"Destino: "<< address_dest;
        }
    }
}

//Define la clase para trabajar directamente con el modulo de aplicacion
Define_Module( aplicacion );

void aplicacion::initialize(){
    //obtiene el valor de master para saber quien parte
    int master = par("starter");

    //obtiene el valor del host en el que se ubica
    int address_host = par("direccion_host");

    //Es el turno de quien tenga la misma address que numero de master
    if(address_host == master){
        //Ya nadie es el master
        master=42;

        //Generar palabra para envío
        generaInfo(-1);
    }else if(master == 2){
        //Ya nadie es el master
        master=42;

        //Ambos generan mensajes
        generaInfo(-1);
    }
}

void aplicacion::handleMessage(cMessage* msg){
    string ack = "ACK";
    string dato = "DATO";
    string msg_name = msg->getName();

    //Si el mensaje ha llegado desde intermedio
    if (msg->arrivedOn("desde_abajo")){
        //Es ACK
        if(msg_name[0] == ack[0] && msg_name[1] == ack[1] && msg_name[2] == ack[2]){
            delete msg;

            ev << "El mensaje: " << msg_name << " fue correctamente recivido." << endl;
            generaInfo(FuncionesExtras::getValorId(msg_name.c_str()));
        //Es DATO
        }else if(msg_name[0] == dato[0] && msg_name[1] == dato[1] && msg_name[2] == dato[2] && msg_name[3] == dato[3]){
            ev << "El dato: " << msg_name << " fue correctamente recivido." << endl;
            delete msg;
        }else{
            ev << "El mensaje: " << msg_name << " no es valido y ha sido eliminado" << endl;
            delete msg;
        }
    }else{
        ev << "Mensaje llegado desde un lugar desconocido" << endl;
    }
}


