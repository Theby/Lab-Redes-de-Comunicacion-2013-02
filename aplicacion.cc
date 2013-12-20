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
    //Obtiene la direccion del host del modulo
    int nombreHost = par("direccion_host");

    //Si es primera vez que se inicia
    if(trama_id == -1){
        //creando un mensaje START
        cMessage *start = new cMessage("START");

        //Enviando el mensaje a Enlace
        send(start, "hacia_abajo");
        ev << "Host " << nombreHost << ": " << "Enviando mensaje START a Enlace" << endl;
    }else{
        //Numero maximo de tramas a enviar
        int numTramas = par("numTramas");

        //Si se ha enviado el número máximo de tramas
        if(numTramas == trama_id){
            //Negro de desconectado
            if (ev.isGUI()){
                getDisplayString().setTagArg("i",1,"black");
                bubble("Desconectado!");
            }

            //creando un mensaje END
            cMessage *end = new cMessage("END");

            //Enviando el mensaje a Enlace
            send(end, "hacia_abajo");
            ev << "Host " << nombreHost << ": " << "Enviando mensaje END a Enlace" << endl;
        }else{
            //Obtiene la dirección del destino al cual se le enviará la información, la cual es seteada por sistema.ned
            int address_dest = par("direccion_dest");

            //Obtiene el tamaño de la trama, el cual por defecto es 4 en sistema.ned
            int tamTrama = par("tamTrama");

            //Crea la información a enviar (ALEATORIO)
            srand(time(NULL));
            int* informacionRand = (int*)malloc(sizeof(int)*tamTrama);

            for(int i=0;i<tamTrama;i++){
                 informacionRand[i] = rand()%2;
            }

            //Creando trama de comunicación con enlace
            Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrando("DATO,",trama_id));

            //Asignando valores correspondientes
            tramaComunicacion->createFrame(address_dest,informacionRand,tamTrama);

            //Se envia el mensaje a intermedio
            send(tramaComunicacion, "hacia_abajo");

            //Informando al Usuario el dato enviado
            ev << "Host " << nombreHost << ": " << "Enviado trama " << trama_id << " al modulo de enlace " << address_dest;
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
        //Les da un color de conectado al modulo de aplicación
        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"green");

        //Generar palabra para envío
        generaInfo(-1);
    }else if(master == 2){
        //Les da un color de conectado al modulo de aplicación
        if (ev.isGUI()) getDisplayString().setTagArg("i",1,"green");

        //Ambos generan mensajes
        generaInfo(-1);
    }
}

void aplicacion::handleMessage(cMessage* msg){
    //Obtiene la direccion del host del modulo
    int nombreHost = par("direccion_host");

    string ack = "ACK";
    string dato = "DATO";
    string msg_name = msg->getName();

    //Normal de activo
    if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

    //Si el mensaje ha llegado desde intermedio
    if (msg->arrivedOn("desde_abajo")){
        //Es ACK
        if(msg_name[0] == ack[0] && msg_name[1] == ack[1] && msg_name[2] == ack[2]){
            delete msg;

            //Mensaje de conectado
            if(msg_name[4] == '0'){
                if (ev.isGUI()) bubble("Conectado!");
            }

            ev << "Host " << nombreHost << ": " << "El mensaje: " << msg_name << " fue correctamente recivido." << endl;
            //Comienza a generar informacion
            generaInfo(FuncionesExtras::getValorId(msg_name.c_str()));
        //Es DATO
        }else if(msg_name[0] == dato[0] && msg_name[1] == dato[1] && msg_name[2] == dato[2] && msg_name[3] == dato[3]){
            ev << "Host " << nombreHost << ": " << "El dato: " << msg_name << " fue correctamente recivido." << endl;
            delete msg;
        }else if(msg_name == "CONNECT"){
            //Mensaje de coenctado
            if (ev.isGUI()) bubble("Conectado!");
            delete msg;
        }else if(msg_name == "DISCONNECT"){
            if (ev.isGUI()){
                //Negro de desconectado
                getDisplayString().setTagArg("i",1,"black");
                bubble("Desconectado!");
            }
            delete msg;
        }else{
            ev << "Host " << nombreHost << ": " << "El mensaje: " << msg_name << " no es valido y ha sido eliminado" << endl;
            delete msg;
        }
    }else{
        ev << "Host " << nombreHost << ": " << "Mensaje llegado desde un lugar desconocido" << endl;
        delete msg;
    }
}


