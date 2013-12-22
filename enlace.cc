/* Envía las palabras desde la capa intermedia al otro host
* y recibe las palabras que llegan desde el otro host y las envía al nivel superior
*/

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <omnetpp.h>

#include <DataFrame_m.h>
#include <Informacion_m.h>
#include <enlace.h>

#include <FuncionesExtras.h>

using namespace std;

//Define la clase para trabajar directamente con el modulo de enlace
Define_Module( enlace );

void enlace::initialize(){
    int error = par("error");
    int numTramas_env = par("numTramas_env");
    int numTramas_rec = par("numTramas_rec");
    int tamVentana = par("tamVentana");
    int tramas_libres = par("tramas_libres");

    bool not_valid_enviroment = false;

    if(error <0 || error>100){
        opp_error("\nLa probabilidad de error debe ser entre 0 y 100 inclusive.\n");
        not_valid_enviroment = true;
    }

    if(numTramas_env < 0){
        opp_error("\nLa cantidad de tramas no puede ser negativa.\n");
        not_valid_enviroment = true;
    }

    if(numTramas_rec < 0){
        opp_error("\nLa cantidad de tramas no puede ser negativa.\n");
        not_valid_enviroment = true;
    }

    if(tamVentana < 0){
        opp_error("\nEl tamaño de la ventana no puede ser negativo.\n");
        not_valid_enviroment = true;
    }

    if(tramas_libres < 0){
        opp_error("\nLa cantidad de tramas a esperar antes de un RR no puede ser negativa.\n");
        not_valid_enviroment = true;
    }

    if(tramas_libres > tamVentana){
        opp_error("\nLa cantidad de tramas a esperar antes de un RR no puede ser mayor que el tamaño de la ventana.\n");
        not_valid_enviroment = true;
    }

    if(not_valid_enviroment){
        endSimulation();
    }
}

//Numero de la trama enviada con error
int trama_con_error;
std::queue<DataFrame*> ventanaDeslizante;

void enlace::handleMessage(cMessage *msg){
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
    //Obtiene el estado del modulo
    string en_respuesta_a = par("en_respuesta_a");

    //Obtiene la direccion del host del modulo
    int nombreHost = par("direccion_host");

    int address_dest = par("direccion_dest");
    string nombre_dato = dato->getName();

    //Para guardar la cantidad de tramas de la ventana
    unsigned int cant_tramasVentana;

    //Si es una trama START
    if(nombre_dato == "START"){
        delete dato;

        //Para saber cual fue la última trama asentida y si es que faltan por asentir
        par("tramas_no_asentidas").setLongValue(0);
        par("ult_trama_recibida").setLongValue(0);

        //Verde de esperando confirmación
        if (ev.isGUI()){
            getDisplayString().setTagArg("i",1,"green");
            bubble("Esperando Confirmación");
        }

        //El host entra en modo de "esperando respuseta a un SABM"
        //Entra en estado de "esperando respuesta a DISC"
        en_respuesta_a = "SABM";
        par("en_respuesta_a").setStringValue(en_respuesta_a);

        //Trama Comando SABM
        DataFrame *tramaComando = new DataFrame("SABM, Trama Comando");

        //Asigna los valores para crear una trama SABM
        tramaComando->createFrame(address_dest);        

        //Verde de esperando confirmacion
        if (ev.isGUI()){
            getParentModule()->getDisplayString().setTagArg("i",1,"green");
            getParentModule()->bubble("Esperando Confirmación");
        }

        send(tramaComando,"hacia_fisico");
        ev << "Host " << nombreHost << ": " << "Enviado Unnumbered SABM a Host: " << address_dest;

    //Si es una trama END
    }else if(nombre_dato == "END"){
        delete dato;

        //Revisa si faltan tramas por asentir
        int tramas_no_asentidas = par("tramas_no_asentidas");

        //Si no falta asentir ninguna trama
        if(tramas_no_asentidas == 0){
            //Entra en estado de "esperando respuesta a DISC"
            en_respuesta_a = "DISC";
            par("en_respuesta_a").setStringValue(en_respuesta_a);

            //Trama commando DISC
            DataFrame *tramaComando = new DataFrame("DISC,Trama Comando");

            //Crea los valores necesarios para una trama DISC
            tramaComando->createFrame(address_dest);

            //Envia la trama por el canal fisico
            send(tramaComando,"hacia_fisico");
            ev << "Host " << nombreHost << ": " << "Enviado Unnumbered DISC a Host: " << address_dest;

        //Si aun faltan tramas por asentir
        }else{
            //Entra en estado de "esperando respuesta a UP"
            en_respuesta_a = "UP";
            par("en_respuesta_a").setStringValue(en_respuesta_a);

            //Si faltan tramas por ser asentidas por un RR se manda un UP para exigir respuesta
            DataFrame *unnumberedFrame = new DataFrame("UP, Trama Comando");
            
            //Asignando valores necesarios para crear la trama
            unnumberedFrame->createFrame(address_dest);

            //Enviando a travez del canal fisico
            send(unnumberedFrame,"hacia_fisico");
            ev << "Host " << nombreHost << ": " << "Se envia UP para confirmar las ultimas tramas" << endl;
        }

    //Si es trama de informacion
    }else{
        //Se recibe un DATO,N por lo que se envia una trama de informacion al otro frame
        srand(time(NULL));
        int prob_error = rand()%100;

        // Probabilidad de que se envie de forma erronea una trama
        int error = par("error");
        // Instancia de probabilidad que la trama se pierda durante el envio
        //int prob_noEnvio = prob_error;

        //Para el bit P/F
        int bit_pf;
        int tramas_libres = par("tramas_libres");

        //Pasar los datos de un paquete a otro (Informacion -> DataFrame)
        Informacion *informacion = check_and_cast<Informacion *>(dato);

        //Obtiene el valor del bit pf para esta trama
        int id_dato = FuncionesExtras::getValorId(nombre_dato.c_str());
        if(id_dato%tramas_libres == (tramas_libres-1)){
            bit_pf = 1;
        }else{
            bit_pf = 0;
        }

        DataFrame *tramaInformacion = new DataFrame(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"I,"));
        DataFrame *copiaParaVentana = new DataFrame(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"I,"));

        /*
         * Para crear la trama se pasan los siguientes argumentos
         * - destino de la trama en decimal
         * - tamaño del array de información
         * - vector del array de información
         * - N(R) en decimal
         * - Bit P/F
         * - N(S) en decimal
         */
        tramaInformacion->createFrame(address_dest,informacion->getInformacionArraySize(),informacion->getInformacion(),0,bit_pf,0);
        copiaParaVentana->createFrame(address_dest,informacion->getInformacionArraySize(),informacion->getInformacion(),0,bit_pf,0);

        //Se almacena la trama en la ventana
        ventanaDeslizante.push(copiaParaVentana);

        //Actualiza el valor de las tramas en la ventana
        cant_tramasVentana = par("cant_tramasVentana");
        cant_tramasVentana++;
        par("cant_tramasVentana").setLongValue(cant_tramasVentana);


        if (prob_error < error && error != 0){
            //Si no supera el umbral de error, la trama se setea como erronea.
            tramaInformacion->setName(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"ERROR,"));
            ev << "Host " << nombreHost << ": " << "La trama "<< nombre_dato.c_str() << " se ha enviado con error.";
        }           
        
        //Guarda el valor de las tramas enviadas y que no han recibido asentimiento RR
        int tramas_no_asentidas = par("tramas_no_asentidas");
        tramas_no_asentidas++;
        par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);

        delete informacion;
        //srand(time(NULL));
        //error = rand()%100;
        //En caso de ser menor a la prob_noEnvio, la trama no se envia para error de trama no recibida
        //if (error > prob_noEnvio){
            //Enviando trama por el medio fisico
            send(tramaInformacion,"hacia_fisico");
            ev << "Host " << nombreHost << ": " << "Mandando trama de Información " << id_dato << " al host: " << address_dest;
        /*}else{
            ev << "Host " << nombreHost << ": " << "La trama "<< nombre_dato.c_str() << " se ha perdido.";*/
        //}        

        //Cantidad de tramas maximas en la ventana
        int tamVentana = par("tamVentana");
        ev << "Tramas en la ventana: " << cant_tramasVentana << " - Ventana de: " << tamVentana << " - queue: " << ventanaDeslizante.size() << endl;
        //Si aún hay espacio en la ventana se manda un ACK para recibir más información
        if(ventanaDeslizante.size() != tamVentana){
            //Crea un ACK,N
            int valor_id = par("ult_ack_enviado");
            valor_id++;
            cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",valor_id));

            //Actualiza el valor del último ACK pedido
            par("ult_ack_enviado").setLongValue(valor_id);

            //Envia ACK,N al modulo de aplicacion
            send(ack,"hacia_arriba");
            ev << "Host " << nombreHost << ": " << "Mandando ACK al modulo de aplicación" << endl;
        }
    }
}

void enlace::processMsgFromLowerLayer(cMessage *packet){
    //Obtiene el estado del modulo
    string en_respuesta_a = par("en_respuesta_a");

    //Obtiene la direccion del host del modulo
    int nombreHost = par("direccion_host");

    int address_dest = par("direccion_dest");
    string packet_name = packet->getName();

    int bandera = 0;

    //Se transforma el mensaje que ingresa en tipo DataFrame
    DataFrame *dataframe = check_and_cast<DataFrame *>(packet);

    //Si el dataframe corresponde a una trama de informacion
    if(dataframe->getControl(0) == 0){
        //Si se está en respuesta a un ERROR
        if(en_respuesta_a == "ERROR"){
            int valor_trama = FuncionesExtras::getValorId(packet_name.c_str());

            //Si corresponde al error se anula el modo de respuesta y se pueden seguir recibiendo tramas tras registrar el error
            if(dataframe->getControl(4) == 1 && valor_trama == trama_con_error){
                en_respuesta_a = "NONE";
                par("en_respuesta_a").setStringValue(en_respuesta_a);

            //Si no las tramas se rechazan hasta que recibe el ERROR
            }else{
                delete packet;
            }
        }

        //Si no está a la espera de ERROR
        if(en_respuesta_a != "ERROR"){
            if (packet_name[0] == 'E' && packet_name[1] == 'R' && packet_name[2] == 'R' && packet_name[3] == 'O' && packet_name[4] == 'R'){
                bandera = 1; // bandera de error
                trama_con_error = FuncionesExtras::getValorId(packet_name.c_str());
            }

            //Si no hay errores
            if (bandera == 0){
            //Se envia la información recibida a aplicacion
                //Crea el packete de informacion para mandarlo a Aplicacion
                Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrandoTrama(packet_name.c_str(),"DATO,"));

                //Asignando valores correspondientes
                tramaComunicacion->createFrame(address_dest,dataframe->getInformation(),dataframe->getInformationArraySize());

                //Lo envia hacia aplicacion
                send(tramaComunicacion,"hacia_arriba");
                ev << "Host " << nombreHost << ": " << "Mandando mensaje recibido a aplicacion";

                //Asigna el valor de la última trama recibida
                int ult_trama_recibida = FuncionesExtras::getValorId(packet_name.c_str());
                par("ult_trama_recibida").setLongValue(ult_trama_recibida);

                //Si se recibe el bit P/F en 1 se responde con un RR,N,1
                if(dataframe->getControl(4) == 1){
                    //Se envia RR N al otro host
                    //Usando dataframe para modificar la informacion

                    //Para asignar el valor_id
                    int valor_id;
                    valor_id = FuncionesExtras::getValorId(packet_name.c_str());
                    valor_id++;
                    dataframe->setName(FuncionesExtras::nombrando("RR,",valor_id));

                    //Actualiza el valor del ultimo RR enviado
                    par("ult_rr_enviado").setLongValue(valor_id);

                    //Asignando los valores necesesarios para RR
                    dataframe->createFrame(address_dest,0,NULL,0,1,0);

                    send(dataframe,"hacia_fisico");
                    ev << "Host " << nombreHost << ": " << "Enviado Supervisory RR" << " a Host: " << address_dest;
                    par("tramas_no_asentidas").setLongValue(0);
                }

            //Si hay errores
            }else{
                //Manda un REJ de error
                en_respuesta_a = "ERROR";
                par("en_respuesta_a").setStringValue(en_respuesta_a);

                //Usando dataframe para modificar la informacion
                dataframe->setName(FuncionesExtras::nombrando("REJ,",FuncionesExtras::getValorId(packet_name.c_str())));

                dataframe->createFrame(address_dest);

                send(dataframe,"hacia_fisico");
                
            }
        }
    
    //Si no, puede ser de control o no-numerada
    }else{
        //Si el dataframe corresponde a una trama supervisora
        if(dataframe->getControl(1) == 0){
            int M1;

            M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);

            //Se recibe un RR
            if(M1 == 0){

                if(en_respuesta_a != "DISC"){

                    //Revisa el valor del bit P/F
                    if(dataframe->getControl(4) == 1 && en_respuesta_a != "UP"){
                        delete dataframe;

                        //Trama a la que corresponde el RR
                        int id_tramaRR = FuncionesExtras::getValorId(packet_name.c_str());

                        //Para saber cuantas tramas se asentieron
                        int tramas_asentidas = 0;

                        //Datos del primer elemento de la trama
                        DataFrame *primer_elemento = ventanaDeslizante.front();
                        string nombre_primer_elemento = primer_elemento->getName();
                        int id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                        //Recorre la Ventana removiendo los elementos asentidos
                        for(int i=0;id_tramaRR > id_tramaVD;i++){
                            if(id_tramaVD < id_tramaRR){
                                ventanaDeslizante.pop();
                                tramas_asentidas++;
                            }

                            if(!ventanaDeslizante.empty()){
                                primer_elemento = ventanaDeslizante.front();
                                nombre_primer_elemento = primer_elemento->getName();

                                id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                            }else{
                                break;
                            }
                        }

                        //Actualiza el valor de las tramas en la ventana
                        int cant_tramasVentana = ventanaDeslizante.size();
                        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                        /*if(!ventanaDeslizante.empty()){
                            //Obtiene el valor de id de la ultima trama para solicitar las tramas a aplicacion que corresponden
                            primer_elemento = ventanaDeslizante.back();
                            nombre_primer_elemento = primer_elemento->getName();
                            id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                            id_tramaVD++;
                        }else{
                            id_tramaVD = id_tramaRR;
                        }*/

                        //Obtiene el valor del ultimo ACK pedido y pide el siguiente
                        id_tramaVD = par("ult_ack_enviado");
                        id_tramaVD++;

                        //Mandar un ACK,N al modulo de aplicación
                        cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",id_tramaVD));

                        //Actualiza el valor del ultimo ack pedido
                        int ult_ack_enviado = id_tramaVD;
                        par("ult_ack_enviado").setLongValue(ult_ack_enviado);

                        //Reinicia el valor de las tramas asentidas
                        int tramas_no_asentidas = par("tramas_no_asentidas");
                        tramas_no_asentidas -= tramas_asentidas;
                        par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);

                        //Manda el mensaje a aplicacion
                        send(ack,"hacia_arriba");
                        ev << "Host " << nombreHost << ": " << "Mandando ACK al modulo de aplicación" << endl;

                    }else if(en_respuesta_a == "UP"){
                        //Trama a la que corresponde el RR
                        int id_tramaRR = FuncionesExtras::getValorId(packet_name.c_str());

                        //Para saber cuantas tramas se asentieron
                        int tramas_asentidas = 0;

                        //Datos del primer elemento de la trama
                        DataFrame *primer_elemento = ventanaDeslizante.front();
                        string nombre_primer_elemento = primer_elemento->getName();
                        int id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                        //Recorre la Ventana removiendo los elementos asentidos
                        for(int i=0;id_tramaRR > id_tramaVD;i++){
                            if(id_tramaVD < id_tramaRR){
                                ventanaDeslizante.pop();
                                tramas_asentidas++;
                            }

                            if(!ventanaDeslizante.empty()){
                                primer_elemento = ventanaDeslizante.front();
                                nombre_primer_elemento = primer_elemento->getName();

                                id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                            }else{
                                break;
                            }
                        }

                        //Actualiza el valor de las tramas en la ventana
                        int cant_tramasVentana = ventanaDeslizante.size();
                        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                        if(ventanaDeslizante.empty()){
                            //Envia una trama de comando DISC al otro host
                            en_respuesta_a = "DISC";
                            par("en_respuesta_a").setStringValue(en_respuesta_a);

                            //Trama commando DISC
                            dataframe->setName("DISC, Trama Comando");

                            //Fija los datos necesarios para la trama DISC
                            dataframe->createFrame(address_dest);

                            send(dataframe,"hacia_fisico");
                            ev << "Host " << nombreHost << ": " << "Se envia trama DISC" << endl;
                        }
                    }
                }
            
            //Recibe un REJ
            }else if(M1 == 1){
                //Retransmite a partir de la trama deseada

                //Obtiene el id de la trama REJ
                int id_tramaREJ = FuncionesExtras::getValorId(packet_name.c_str());

                //Trama a la que corresponden los elementos de la Ventana Deslizante
                int id_tramaVD = 0;

                //Para ir guardando los datos de la ventana deslizante
                DataFrame *primer_elemento;
                string nombre_primer_elemento;

                //Para saber cuantas tramas se asentieron
                int tramas_asentidas = 0;

                //Datos del primer elemento de la ventana
                primer_elemento = ventanaDeslizante.front();
                nombre_primer_elemento = primer_elemento->getName();
                id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                //Recorre la Ventana removiendo los elementos asentidos previos al REJ
                for(int i=0;id_tramaREJ > id_tramaVD;i++){
                    if(id_tramaVD < id_tramaREJ){
                        ventanaDeslizante.pop();
                        tramas_asentidas++;
                    }

                    primer_elemento = ventanaDeslizante.front();
                    nombre_primer_elemento = primer_elemento->getName();

                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                }
                
                //Actualiza el valor de las tramas en la ventana
                int cant_tramasVentana = ventanaDeslizante.size();
                par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                //Reinicia el valor de las tramas asentidas
                int tramas_no_asentidas = par("tramas_no_asentidas");
                tramas_no_asentidas -= tramas_asentidas;
                par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);

                //Re-envia la trama que tenía el error
                send(ventanaDeslizante.front(),"hacia_fisico");
            }
        
        }
        //Si el dataframe corresponde a una trama no-numerada (Unnumbered)
        else{
            //Para obtener los identificadores de la trama
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
                    dataframe->setName("UA, Trama Comando");

                    //Seteando los valores necesarios para la trama UA
                    dataframe->createFrame(address_dest);

                    //Negro de desconectado
                    if (ev.isGUI()){
                        getParentModule()->getDisplayString().setTagArg("i",1,"black");
                        getParentModule()->bubble("Desconectado!");
                    }

                    int master = getParentModule()->par("starter");
                    int address_host = par("direccion_host");

                    //El modulo que recibio el DISC manda un DISCONNECT a los modulos superiores si es que no están ambos transmitiendo
                    if(master != address_host && master!= 2){
                        cMessage *disconnect = new cMessage("DISCONNECT");

                        //Se envia hacia los modulos superiores
                        send(disconnect,"hacia_arriba");
                    }

                    //Enia el UA por el medio físico
                    send(dataframe,"hacia_fisico");
                    ev << "Host " << nombreHost << ": " << "Enviado Unnumbered UA a Host: " << address_dest;
                
                //Se recibe un UP
                }else if(M2 == 4){
                    //Se responde con la última RR
                    int ult_trama_recibida = par("ult_trama_recibida");
                    ult_trama_recibida++;
                    int ult_rr_enviado = par("ult_rr_enviado");

                    if(ult_trama_recibida > ult_rr_enviado){
                        //Para asignar el valor_id
                        dataframe->setName(FuncionesExtras::nombrando("RR,",ult_trama_recibida));

                        //Setea los valores necesarios para la trama RR
                        dataframe->createFrame(address_dest);

                        //Actualiza el valor del ultimo RR enviado
                        par("ult_rr_enviado").setLongValue(ult_trama_recibida);

                        //Envia la trama por el medio fisico
                        send(dataframe,"hacia_fisico");
                        ev << "Host " << nombreHost << ": " << "Enviado Supervisory RR" << " a Host: " << address_dest;
                    }else{
                        ev << "Host " << nombreHost << ": " << "UP no respondido debido a que ya se ha enviado un RR adecuado" << address_dest;
                    }

                //Se recibe un UA
                }else if(M2 == 6){
                    delete dataframe;

                    //Si la trama UA fue recibida por enviar un SABM
                    if(en_respuesta_a == "SABM"){
                        en_respuesta_a == "NONE";
                        par("en_respuesta_a").setStringValue(en_respuesta_a);

                        //Normal de activo
                        if (ev.isGUI()){
                            getDisplayString().setTagArg("i",1,"");
                            bubble("Conectado!");
                            getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->bubble("Conectado!");
                        }

                        //Mandar un ACK N al modulo de aplicación
                        cMessage *ack = new cMessage("ACK,0");

                        //Actualiza el valor del último ACK pedido
                        int ult_ack_enviado = 0;
                        par("ult_ack_enviado").setLongValue(ult_ack_enviado);

                        send(ack,"hacia_arriba");
                        ev << "Host " << nombreHost << ": " << "Mandando ACK,0 al modulo de aplicación" << endl;

                    //Si la trama UA fue recibida por enviar DISC
                    }else if(en_respuesta_a == "DISC"){
                        en_respuesta_a == "NONE";
                        par("en_respuesta_a").setStringValue(en_respuesta_a);

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
                    dataframe->setName("UA, Trama Comando");

                    //Fija los valores necesarios para la trama UA
                    dataframe->createFrame(address_dest);

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

                    //Envia la trama por el canal físico
                    send(dataframe,"hacia_fisico");
                    ev << "Host " << nombreHost << ": " << "Enviado Unnumbered UA a Host: " << address_dest;
                }
            }
        }
    }
}
