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
        delete msg;
    }
    //Generar un mensaje de respuesta
    generaInfo();
}

void aplicacion::generaInfo()
{
    //Obtiene la dirección del Host en el que se esta ejecutando, la cual es seteada por sistema.ned
    int address_home = par("direccion_host");
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
            if(address_home<pow(10,(8-(i+1)))){
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

    //Se informa al usuario de que se envió la palabra
    ev<<"Host: "<<address_home<<" - Destino: "<< address_dest;
}
