/* Envía las palabras desde la capa intermedia al otro host
* y recibe las palabras que llegan desde el otro host y las envía al nivel superior
*/

#include <string.h>
#include <omnetpp.h>

#include <DataFrame_m.h>
#include <Informacion_m.h>
#include <enlace.h>

#include <FuncionesExtras.h>

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

    if(nombre_dato == "START"){
        delete dato;

        //Trama Comando SABM
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
        delete dato;

    }else{
        //Pasar los datos de un paquete a otro (Informacion -> DataFrame)

        DataFrame *tramaInformacion = new DataFrame(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"I,"));
        Informacion *informacion = check_and_cast<Informacion *>(dato);

        //Copiando Address
        for(int i=0;i<8;i++){
            tramaInformacion->setAddress(i,informacion->getAddress_dest(i));
        }

        //Inicio Control: Informacion
            //Informacion
            tramaInformacion->setControl(0,0);

            //N(S)
            tramaInformacion->setControl(1,0);
            tramaInformacion->setControl(2,0);
            tramaInformacion->setControl(3,0);

            //bit P/F en 1 para solicitar respuesta
            tramaInformacion->setControl(4,1);
                //Asignando nombre completo
                    nombre_dato = tramaInformacion->getName();
                    nombre_dato += ',';
                    tramaInformacion->setName(FuncionesExtras::nombrando(nombre_dato.c_str(),tramaInformacion->getControl(4)));

            //N(R)
            tramaInformacion->setControl(5,0);
            tramaInformacion->setControl(6,0);
            tramaInformacion->setControl(7,0);
        //Fin Control

        //Inicio Informacion
            //Copiando Información
            tramaInformacion->setInformationArraySize(informacion->getInformacionArraySize());
            for(unsigned int i=0;i<tramaInformacion->getInformationArraySize();i++){
                tramaInformacion->setInformation(i,informacion->getInformacion(i));
            }
        //Fin Informacion

        //Inicio FCS
            for(int i=0;i<16;i++){
                tramaInformacion->setFCS(i,0);
            }
        //Fin FCS

        delete informacion;
        send(tramaInformacion,"hacia_fisico");
    }
}

void enlace::processMsgFromLowerLayer(cMessage *packet){
    int address_dest = par("direccion_dest");
    string packet_name = packet->getName();

    //Se transforma el mensaje que ingresa en tipo DataFrame
    DataFrame *dataframe = check_and_cast<DataFrame *>(packet);

    //Si el dataframe corresponde a una trama de informacion
    if(dataframe->getControl(0) == 0){
        //Se envia la información recibida a aplicacion
            //Crea el packete de informacion para mandarlo a Aplicacion
            Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrandoTrama(packet_name.c_str(),"DATO,"));

            //Inicia Address
                //Copia Address
                for(unsigned int i=0;i<dataframe->getAddressArraySize();i++){
                    tramaComunicacion->setAddress_dest(i,dataframe->getAddress(i));
                }
            //Fin Address

            //Inicia Informacion
                //Copia Informacion
                tramaComunicacion->setInformacionArraySize(dataframe->getInformationArraySize());
                for(unsigned int i=0;i<dataframe->getInformationArraySize();i++){
                    tramaComunicacion->setInformacion(i,dataframe->getInformation(i));
                }
            //Fin Informacion

            send(tramaComunicacion,"hacia_arriba");
            ev << "Mandando mensaje recibido a aplicacion";

        //Se envia RR N al otro host
            //Usando dataframe para modificar la informacion
            dataframe->setName(FuncionesExtras::nombrando("RR,",1+FuncionesExtras::getValorId(packet_name.c_str())));

            //Inicio Address
                //Queda igual
            //Fin Address

            //Inicio Control: Unnumbered UA
                //Supervisory
                dataframe->setControl(0,1);
                dataframe->setControl(1,0);

                //Supervisory Funcion bits
                dataframe->setControl(2,0);
                dataframe->setControl(3,0);

                //bit P/F en 1 para solicitar respuesta
                dataframe->setControl(4,1);
                //Asignando nombre completo
                    packet_name = dataframe->getName();
                    packet_name += ',';
                    dataframe->setName(FuncionesExtras::nombrando(packet_name.c_str(),dataframe->getControl(4)));

                //N(R)
                dataframe->setControl(5,1);
                dataframe->setControl(6,1);
                dataframe->setControl(7,0);
            //Fin Control

            //Inicio FCS
                //Queda igual
            //Fin FCS

            send(dataframe,"hacia_fisico");
            ev << "Enviado Supervisory RR" << " a Host: " << address_dest;
    }
    else{
        //Si el dataframe corresponde a una trama supervisora
        if(dataframe->getControl(1) == 0){
            int M1;

            M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);

            //Se recibe un RR
            if(M1 == 0){
                //Mandar un ACK,N al modulo de aplicación
                cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",FuncionesExtras::getValorId(packet_name.c_str())));

                send(ack,"hacia_arriba");
                ev << "Mandando ACK al modulo de aplicación" << endl;
            }
        }
        //Si el dataframe corresponde a una trama no-numerada (Unnumbered)
        else{
            int M1,M2;

            M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);
            M2 = pow(2,2)*dataframe->getControl(5) + pow(2,1)*dataframe->getControl(6) + pow(2,0)*dataframe->getControl(7);

            if(M1 == 0){
                //Se recibe un UA
                if(M2 == 6){
                    //Mandar un ACK N al modulo de aplicación
                    cMessage *ack = new cMessage("ACK,0");

                    send(ack,"hacia_arriba");
                    ev << "Mandando ACK,0 al modulo de aplicación" << endl;
                }
            }else if(M1 == 1){

            }else if(M1 == 2){

            }else if(M1 == 3){
                //Se recibe un SABM
                if(M2 == 4){
                    //Mandar de respuesta un UA al otro Host
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
