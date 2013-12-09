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
    if (msg->arrivedOn("desde_fisico")){
        //Procesarlo como si viniera desde abajo
        processMsgFromLowerLayer(msg);
    }
    //Sino, el mensaje viene desde intermedio
    else{
        //Procesarlo como si viniera desde arriba
        processMsgFromHigherLayer(msg);
    }
}

void enlace::processMsgFromHigherLayer(cMessage *dato){
    string nombre_dato = dato->getName();
    int address_dest = par("direccion_dest");

    if(dato->getKind() == 10000){
        if(nombre_dato == "START"){
            delete dato;

            DataFrame *tramaComando = new DataFrame("Trama Comando SABM");

            //Inicio Address
                //Asigna la direccion al sector address de la trama
                for(unsigned int i=0;i<8;i++){
                    if(address_dest<pow(10,(8-(i+1)))){
                        tramaComando->setAddress(i,0);
                    }else{
                        tramaComando->setAddress(i,1);
                    }
                }
            //Fin Address

            //Inicio Control: Unnumbered SABM
                //Unnumbered
                tramaComando->setControl(0,1);
                tramaComando->setControl(1,1);

                //Unnumbered Funcion bits
                tramaComando->setControl(2,1);
                tramaComando->setControl(3,1);

                //bit P/F en 1 para solicitar respuesta
                tramaComando->setControl(4,1);

                //Unnumbered Funcion bits
                tramaComando->setControl(5,1);
                tramaComando->setControl(6,0);
                tramaComando->setControl(7,0);
            //Fin Control

            //Inicio FCS
                for(int i=0;i<16;i++){
                    tramaComando->setFCS(i,0);
                }
            //Fin FCS

            send(tramaComando,"hacia_fisico");
            ev << "Enviado Unnumbered SABM a Host: " << address_dest;
        }else if(nombre_dato == "END"){

        }
    }else{
        send(dato,"hacia_fisico");
    }
}

void enlace::processMsgFromLowerLayer(cMessage *packet){
    //int direccion_host = par("direcion_host");
    int address_dest = par("direccion_dest");
    int M1,M2;
    //int address_host[8];



    //Se registra la direccion del host en modo byte
    /*for(int i=0;i<7;i++){
            address_host[i] = 0;
    }
    address_host[7] = direccion_host;
    */

    //Se transforma el mensaje que ingresa en tipo DataFrame
    DataFrame *dataframe = check_and_cast<DataFrame *>(packet);
    //delete packet;

    M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);
    M2 = pow(2,2)*dataframe->getControl(5) + pow(2,1)*dataframe->getControl(6) + pow(2,0)*dataframe->getControl(7);

    //Si el dataframe corresponde a una trama de informacion
    if(dataframe->getControl(0) == 0){

    }
    else{
        //Si el dataframe corresponde a una trama supervisora
        if(dataframe->getControl(1) == 0){

        }
        //Si el dataframe corresponde a una trama no-numerada (Unnumbered)
        else{
            if(M1 == 0){

            }else if(M1 == 1){

            }else if(M1 == 2){

            }else if(M1 == 3){
                //SABM
                if(M2 == 4){
                    //Mandar de respuesta un UA
                    DataFrame *tramaComando = new DataFrame("Trama Comando UA");

                    //Inicio Address
                        //Asigna la direccion al sector address de la trama
                        for(int i=0;i<8;i++){
                            tramaComando->setAddress(i,dataframe->getAddress(i));
                        }
                    //Fin Address

                    //Inicio Control: Unnumbered UA
                        //Unnumbered
                        tramaComando->setControl(0,1);
                        tramaComando->setControl(1,1);

                        //Unnumbered Funcion bits
                        tramaComando->setControl(2,0);
                        tramaComando->setControl(3,0);

                        //bit P/F en 1 para solicitar respuesta
                        tramaComando->setControl(4,1);

                        //Unnumbered Funcion bits
                        tramaComando->setControl(5,1);
                        tramaComando->setControl(6,1);
                        tramaComando->setControl(7,0);
                    //Fin Control

                    //Inicio FCS
                        for(int i=0;i<16;i++){
                            tramaComando->setFCS(i,0);
                        }
                    //Fin FCS

                    send(tramaComando,"hacia_fisico");
                    ev << "Enviado Unnumbered UA a Host: " << address_dest;
                }
            }
        }
    }

    //send(packet,"hacia_arriba");
}
