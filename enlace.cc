/* Envía las palabras desde la capa intermedia al otro host
* y recibe las palabras que llegan desde el otro host y las envía al nivel superior
*/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <omnetpp.h>

#include <DataFrame_m.h>
#include <Informacion_m.h>
#include <enlace.h>

#include <FuncionesExtras.h>

using namespace std;

//Define la clase para trabajar directamente con el modulo de enlace
Define_Module( enlace );

string respuesta_a;

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

        par("tramas_no_asentidas").setLongValue(0);
        par("ult_trama_recibida").setLongValue(0);

        //Verde de esperando confirmación
        if (ev.isGUI()){
            getDisplayString().setTagArg("i",1,"green");
            bubble("Esperando Confirmación");
        }

        //Se envia una trama SABM al otro host
        respuesta_a = "SABM";
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

        //Verde de esperando
        if (ev.isGUI()){
            getParentModule()->getDisplayString().setTagArg("i",1,"green");
            getParentModule()->bubble("Esperando Confirmación");
        }

        send(tramaComando,"hacia_fisico");
        ev << "Enviado Unnumbered SABM a Host: " << address_dest;
    }else if(nombre_dato == "END"){
        delete dato;
        //Revisa si faltan tramas por asentir
        int tramas_no_asentidas = par("tramas_no_asentidas");

        if(tramas_no_asentidas == 0){
            //Envia una trama de comando DISC al otro host
            respuesta_a = "DISC";
            //Trama commando DISC
            DataFrame *tramaComando = new DataFrame("Trama Comando DISC");

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

            //Inicio Control: Unnumbered DISC
                //Unnumbered
                tramaComando->setControl(0,1);
                tramaComando->setControl(1,1);

                //Unnumbered Funcion bits
                tramaComando->setControl(2,0);
                tramaComando->setControl(3,0);

                //bit P/F en 1 para solicitar respuesta
                tramaComando->setControl(4,1);

                //Unnumbered Funcion bits
                tramaComando->setControl(5,0);
                tramaComando->setControl(6,1);
                tramaComando->setControl(7,0);
            //Fin Control

            //Inicio FCS
                for(int i=0;i<16;i++){
                    tramaComando->setFCS(i,0);
                }
            //Fin FCS

            send(tramaComando,"hacia_fisico");
            ev << "Enviado Unnumbered DISC a Host: " << address_dest;
        }else{
            DataFrame *unnumberedFrame = new DataFrame("Trama Comando UP");
            //Si faltan tramas por ser asentidas por un RR se manda un UP

            //Inicio Address
                for(unsigned int i=0;i<8;i++){
                    if(address_dest<pow(10,(8-(i+1)))){
                        unnumberedFrame->setAddress(i,0);
                    }else{
                        unnumberedFrame->setAddress(i,1);
                    }
                }
            //Fin Address

            //Inicio Control: Unnumbered UP
                //Unnumbered
                unnumberedFrame->setControl(0,1);
                unnumberedFrame->setControl(1,1);

                //Unnumbered Funcion bits
                unnumberedFrame->setControl(2,0);
                unnumberedFrame->setControl(3,0);

                //bit P/F en 1 para dar respuesta
                unnumberedFrame->setControl(4,1);

                //Unnumbered Funcion bits
                unnumberedFrame->setControl(5,1);
                unnumberedFrame->setControl(6,0);
                unnumberedFrame->setControl(7,0);
            //Fin Control

            //Inicio FCS
                //Queda igual
            //Fin FCS

            send(unnumberedFrame,"hacia_fisico");
            ev << "Se envia UP para confirmar las ultimas tramas" << endl;
        }

    }else{
        //Se recibe un DATO,N por lo que se envia una trama de informacion al otro frame
        srand(time(NULL));
        int error = rand()%100;

        // Instancia de probabilidad de error para las tramas que se envian
        int prob_error = par("error");
        // Instancia de probabilidad que la tramase pierda durante el envio
        int prob_noEnvio = prob_error;

        //Para el bit P/F
        int bit_pf;
        int tramas_libres = par("tramas_libres");

        //Para guardar el id del dato recibido
        int id_dato;

        DataFrame *tramaInformacion;
        if (error > prob_error)
            //Se recibe un DATO,N por lo que se envia una trama de informacion al otro frame
            //Pasar los datos de un paquete a otro (Informacion -> DataFrame)
            tramaInformacion = new DataFrame(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"I,"));
        else{
            //Si no supera el umbral de error, la trama se setea como errornea.
            tramaInformacion = new DataFrame(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"ERROR,"));
            ev << "La trama "<< nombre_dato.c_str() << " se ha generado con error.";
        }

        //Pasar los datos de un paquete a otro (Informacion -> DataFrame)
        Informacion *informacion = check_and_cast<Informacion *>(dato);

        //Obtiene el valor del bit pf para esta trama
        id_dato = FuncionesExtras::getValorId(nombre_dato.c_str());
        if(id_dato%tramas_libres == (tramas_libres-1)){
            bit_pf = 1;
        }else{
            bit_pf = 0;
        }

        //Inicio Address
            //Copiando Address
            for(int i=0;i<8;i++){
                tramaInformacion->setAddress(i,informacion->getAddress_dest(i));
            }
        //Fin Address

        //Inicio Control: Informacion
            //Informacion
            tramaInformacion->setControl(0,0);

            //N(S)
            tramaInformacion->setControl(1,0);
            tramaInformacion->setControl(2,0);
            tramaInformacion->setControl(3,0);

            //bit P/F en 1 para solicitar respuesta
            tramaInformacion->setControl(4,bit_pf);
                //Asignando nombre completo
                    nombre_dato = tramaInformacion->getName();
                    nombre_dato += ',';
                    tramaInformacion->setName(FuncionesExtras::nombrando(nombre_dato.c_str(),bit_pf));

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
        //srand(time(NULL));
        //error = rand()%100;
        //En caso de ser menor a la prob_noEnvio, la trama no se envia para error de trama no recibida
        //if (error > prob_noEnvio){
            send(tramaInformacion,"hacia_fisico");
        /*}else{
            ev << "La trama "<< nombre_dato.c_str() << " se ha perdido.";*/
        //}

        ev << "Mandando trama de Información " << id_dato << " al host: " << address_dest;

        //Guarda el valor de las tramas enviadas y que no han recibido asentimiento RR
        int tramas_no_asentidas = par("tramas_no_asentidas");
        tramas_no_asentidas++;

        par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);

        if(bit_pf == 0){
            //Mandar un ACK,N al modulo de aplicación
            int valor_id = 1 + id_dato;
            cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",valor_id));

            send(ack,"hacia_arriba");
            ev << "Mandando ACK al modulo de aplicación" << endl;
        }
    }
}

void enlace::processMsgFromLowerLayer(cMessage *packet){
    int address_dest = par("direccion_dest");
    string packet_name = packet->getName();
    int bandera = 0;

    //Se transforma el mensaje que ingresa en tipo DataFrame
    DataFrame *dataframe = check_and_cast<DataFrame *>(packet);

    //Si el dataframe corresponde a una trama de informacion
    if(dataframe->getControl(0) == 0){
        if (packet_name == "ERROR"){
            bandera = 1; // bandera de error
        }
        if (bandera == 0){
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

            //Si se recibe el bit P/F en 1 se responde con un RR,N,1
                if(dataframe->getControl(4) == 1){
                    //Se envia RR N al otro host
                    //Usando dataframe para modificar la informacion

                        //Para asignar el valor_id
                        int valor_id;
                        valor_id = FuncionesExtras::getValorId(packet_name.c_str());
                        valor_id++;
                        dataframe->setName(FuncionesExtras::nombrando("RR,",valor_id));

                    //Inicio Address
                        //Queda igual
                    //Fin Address

                    //Inicio Control: Supervisory RR
                        //Supervisory
                        dataframe->setControl(0,1);
                        dataframe->setControl(1,0);

                        //Supervisory Funcion bits
                        dataframe->setControl(2,0);
                        dataframe->setControl(3,0);

                        //bit P/F en 1 para dar respuesta
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
                    par("tramas_no_asentidas").setLongValue(0);
            }else{
                //Asigna el valor de la última trama recibida
                int ult_trama_recibida = par("ult_trama_recibida");
                ult_trama_recibida = FuncionesExtras::getValorId(packet_name.c_str());

                par("ult_trama_recibida").setLongValue(ult_trama_recibida);
                delete dataframe;
            }
        }else{
            //Usando dataframe para modificar la informacion
            dataframe->setName(FuncionesExtras::nombrando("REJ,",FuncionesExtras::getValorId(packet_name.c_str())));

            //Inicio Address
                //Queda igual
            //Fin Address

            //Inicio Control: Supervisory RR
                //Supervisory
                dataframe->setControl(0,1);
                dataframe->setControl(1,0);

                //Supervisory Funcion bits
                dataframe->setControl(2,0);
                dataframe->setControl(3,1);

                //bit P/F en 1 para dar respuesta
                dataframe->setControl(4,1);
                //Asignando nombre completo
                    packet_name = dataframe->getName();
                    packet_name += ',';
                    dataframe->setName(FuncionesExtras::nombrando(packet_name.c_str(),dataframe->getControl(4)));

                //N(R)
                dataframe->setControl(5,0);
                dataframe->setControl(6,0);
                dataframe->setControl(7,0);
            //Fin Control

            //Inicio FCS
                //Queda igual
            //Fin FCS
        }
    }
    else{
        //Si el dataframe corresponde a una trama supervisora
        if(dataframe->getControl(1) == 0){
            int M1;

            M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);

            //Se recibe un RR
            if(M1 == 0){

                if(respuesta_a != "DISC"){
                    if(dataframe->getControl(4) == 1){
                        delete dataframe;

                        //Mandar un ACK,N al modulo de aplicación
                        cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",FuncionesExtras::getValorId(packet_name.c_str())));

                        send(ack,"hacia_arriba");
                        ev << "Mandando ACK al modulo de aplicación" << endl;

                        //Reinicia el valor de las tramas asentidas
                        int tramas_no_asentidas = par("tramas_no_asentidas");
                        tramas_no_asentidas = 0;

                        par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);
                    }else{
                        //Envia una trama de comando DISC al otro host
                        respuesta_a = "DISC";
                        //Trama commando DISC
                        dataframe->setName("Trama Comando DISC");

                        //Inicio Address
                           //Sin cambios
                        //Fin Address

                        //Inicio Control: Unnumbered DISC
                            //Unnumbered
                            dataframe->setControl(0,1);
                            dataframe->setControl(1,1);

                            //Unnumbered Funcion bits
                            dataframe->setControl(2,0);
                            dataframe->setControl(3,0);

                            //bit P/F en 1 para solicitar respuesta
                            dataframe->setControl(4,1);

                            //Unnumbered Funcion bits
                            dataframe->setControl(5,0);
                            dataframe->setControl(6,1);
                            dataframe->setControl(7,0);
                        //Fin Control

                        //Inicio FCS
                            //Sin cambios
                        //Fin FCS

                        send(dataframe,"hacia_fisico");
                        ev << "Se envia trama DISC" << endl;
                    }
                }
            }else if(M1 == 1){
                //Recibe un REJ
                //Retransmite la trama deseada

                dataframe->setName(FuncionesExtras::nombrandoTrama(packet_name.c_str(),"I,"));

                //Inicio Address
                   //Sin cambios
                //Fin Address

                //Inicio Control: Informacion
                    //Informacion
                    dataframe->setControl(0,0);

                    //N(S)
                    dataframe->setControl(1,0);
                    dataframe->setControl(2,0);
                    dataframe->setControl(3,0);

                    //bit P/F en 1 para solicitar respuesta
                        dataframe->setControl(4,1);
                        //Asignando nombre completo
                        packet_name = dataframe->getName();
                        packet_name += ',';
                            dataframe->setName(FuncionesExtras::nombrando(packet_name.c_str(),dataframe->getControl(4)));

                    //N(R)
                        dataframe->setControl(5,0);
                        dataframe->setControl(6,0);
                        dataframe->setControl(7,0);
                //Fin Control

                //Inicio FCS
                    //Sin cambios
                //Fin FCS
                    send(dataframe,"hacia_fisico");

            }
        }
        //Si el dataframe corresponde a una trama no-numerada (Unnumbered)
        else{
            int M1,M2;

            M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);
            M2 = pow(2,2)*dataframe->getControl(5) + pow(2,1)*dataframe->getControl(6) + pow(2,0)*dataframe->getControl(7);

            if(M1 == 0){
                //Se recibe un DISC
                if(M2 == 2){
                    //Negro de desconectado
                    if (ev.isGUI()){
                        getDisplayString().setTagArg("i",1,"black");
                        bubble("Desconectado!");
                    }

                    //Se manda de respuesta un UA Frame y se desconecta
                    dataframe->setName("Trama Comando UA");

                    //Inicio Address
                        //Queda igual
                    //Fin Address

                    //Inicio Control: Unnumbered UA
                        //Unnumbered
                        dataframe->setControl(0,1);
                        dataframe->setControl(1,1);

                        //Unnumbered Funcion bits
                        dataframe->setControl(2,0);
                        dataframe->setControl(3,0);

                        //bit P/F en 1 para dar respuesta
                        dataframe->setControl(4,1);

                        //Unnumbered Funcion bits
                        dataframe->setControl(5,1);
                        dataframe->setControl(6,1);
                        dataframe->setControl(7,0);
                    //Fin Control

                    //Inicio FCS
                        //Queda igual
                    //Fin FCS

                    //Negro de desconectado
                    if (ev.isGUI()){
                        getParentModule()->getDisplayString().setTagArg("i",1,"black");
                        getParentModule()->bubble("Desconectado!");
                    }

                    int master = getParentModule()->par("starter");
                    int address_host = par("direccion_host");
                    if(master != address_host && master!= 2){
                        cMessage *disconnect = new cMessage("DISCONNECT");

                        send(disconnect,"hacia_arriba");
                    }


                    send(dataframe,"hacia_fisico");
                    ev << "Enviado Unnumbered UA a Host: " << address_dest;

                    //Se disconecta
                }
                //Se recibe un UP
                else if(M2 == 4){
                    //Se responde con la última RR
                    int ult_trama_recibida = par("ult_trama_recibida");

                    //Usando dataframe para modificar la informacion

                    //Para asignar el valor_id
                        dataframe->setName(FuncionesExtras::nombrando("RR,",ult_trama_recibida));

                    //Inicio Address
                        //Queda igual
                    //Fin Address

                    //Inicio Control: Supervisory RR
                        //Supervisory
                        dataframe->setControl(0,1);
                        dataframe->setControl(1,0);

                        //Supervisory Funcion bits
                        dataframe->setControl(2,0);
                        dataframe->setControl(3,0);

                        //bit P/F en 1 para dar respuesta
                        dataframe->setControl(4,0);
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
                //Se recibe un UA
                else if(M2 == 6){
                    delete dataframe;
                    if(respuesta_a == "SABM"){
                        respuesta_a == "NONE";
                        //Normal de activo
                        if (ev.isGUI()){
                            getDisplayString().setTagArg("i",1,"");
                            bubble("Conectado!");
                            getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->bubble("Conectado!");
                        }
                        //Mandar un ACK N al modulo de aplicación
                        cMessage *ack = new cMessage("ACK,0");

                        send(ack,"hacia_arriba");
                        ev << "Mandando ACK,0 al modulo de aplicación" << endl;
                    }else if(respuesta_a == "DISC"){
                        respuesta_a == "NONE";
                        //Negro de desconectado
                        if (ev.isGUI()){
                            getDisplayString().setTagArg("i",1,"black");
                            bubble("Desconectado!");
                            getParentModule()->getDisplayString().setTagArg("i",1,"black");
                            getParentModule()->bubble("Desconectado!");
                        }
                    }
                }
            }else if(M1 == 1){

            }else if(M1 == 2){

            }else if(M1 == 3){
                //Se recibe un SABM
                if(M2 == 4){
                    //Normal de activo
                    if (ev.isGUI()){
                        getDisplayString().setTagArg("i",1,"");
                        bubble("Conectado!");
                    }

                    //Se manda de respuesta un UA Frame
                    dataframe->setName("Trama Comando UA");

                    //Inicio Address
                        //Queda igual
                    //Fin Address

                    //Inicio Control: Unnumbered UA
                        //Unnumbered
                        dataframe->setControl(0,1);
                        dataframe->setControl(1,1);

                        //Unnumbered Funcion bits
                        dataframe->setControl(2,0);
                        dataframe->setControl(3,0);

                        //bit P/F en 1 para dar respuesta
                        dataframe->setControl(4,1);

                        //Unnumbered Funcion bits
                        dataframe->setControl(5,1);
                        dataframe->setControl(6,1);
                        dataframe->setControl(7,0);
                    //Fin Control

                    //Inicio FCS
                        //Queda igual
                    //Fin FCS

                    //Normal de conectado
                    if (ev.isGUI()){
                        getParentModule()->getDisplayString().setTagArg("i",1,"");
                        getParentModule()->bubble("Conectado!");

                        int master = getParentModule()->par("starter");
                        int address_host = par("direccion_host");
                        if(master != address_host && master!= 2){
                            cMessage *connect = new cMessage("CONNECT");

                            send(connect,"hacia_arriba");
                        }
                    }

                    send(dataframe,"hacia_fisico");
                    ev << "Enviado Unnumbered UA a Host: " << address_dest;
                }
            }
        }
    }
}
