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

//queue para el host 0
std::queue<DataFrame*> ventanaDeslizante00;
std::queue<DataFrame*> canalLibre00;

//queue para el host 1
std::queue<DataFrame*> ventanaDeslizante01;
std::queue<DataFrame*> canalLibre01;

void enlace::sending(cMessage* msg, const char* ngate){
    cChannel *canal = gate(ngate)->getTransmissionChannel();;
    simtime_t canalFinishTime = canal->getTransmissionFinishTime();

    DataFrame *mensaje = check_and_cast<DataFrame *>(msg);

    // Canal libre
    if (canalFinishTime <= simTime()){
        send(mensaje, ngate);

    }else{
        //Mensaje que avisara cuando se libere el canal
        cMessage *canalLibre = new cMessage("FREE");

        //Obtiene la direccion del host del modulo
        int nombreHost = par("direccion_host");

        //Guarda el mensaje original en la lista
        if(nombreHost==0){
            canalLibre00.push(mensaje);
        }else if(nombreHost==1){
            canalLibre01.push(mensaje);
        }

        //Deja esperando un mensaje para avisar cuando se liberará el canal
        scheduleAt(canalFinishTime, canalLibre);
    }
}

void enlace::initialize(){
    int error = par("error");
    int numTramas_env = par("numTramas_env");
    int numTramas_rec = par("numTramas_rec");
    int tamVentana = par("tamVentana");
    int tramas_libres = par("tramas_libres");
    int starter = par("starter");
    int direccion_host = par("direccion_host");

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

    if(tramas_libres > numTramas_env && (starter==direccion_host || starter==2)){
        opp_error("\nLa cantidad de tramas a esperar antes de un RR no puede ser mayor que el numero de tramas que pueden enviar los Hosts.\n");
        not_valid_enviroment = true;
    }

    if(not_valid_enviroment){
        endSimulation();
    }
}

void enlace::handleMessage(cMessage *msg){
    string name = msg->getName();
    int nombreHost = par("direccion_host");

    if(name != "FREE"){
        //Si el mensaje llega desde el otro Host
        if (msg->arrivedOn("desde_fisico") || (name[0]=='R' && name[1]=='E' && name[2]=='J')){
            //Procesarlo como si viniera desde abajo
            processMsgFromLowerLayer(msg);
        }
        //Sino, el mensaje viene desde intermedio
        else{
            //Procesarlo como si viniera desde arriba
            processMsgFromHigherLayer(msg);
        }

    //Corresponde enviar un mensaje en la cola
    }else{
        if(nombreHost==0){
            sending(canalLibre00.front(),"hacia_fisico");
            canalLibre00.pop();
        }else if(nombreHost==1){
            sending(canalLibre01.front(),"hacia_fisico");
            canalLibre01.pop();
        }
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
        int starter = par("starter");

        if(starter==nombreHost){
            if(nombreHost==0){
                while(!ventanaDeslizante00.empty()){
                    ventanaDeslizante00.pop();
                }

                while(!canalLibre00.empty()){
                    canalLibre00.pop();
                }
            }else if(nombreHost==1){
                while(!ventanaDeslizante01.empty()){
                    ventanaDeslizante01.pop();
                }

                while(!canalLibre01.empty()){
                    canalLibre01.pop();
                }
            }
        }

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

        sending(tramaComando,"hacia_fisico");
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
            sending(tramaComando,"hacia_fisico");
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
            sending(unnumberedFrame,"hacia_fisico");
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

            //Temporizador que espera un RR durante 10 segundos, si no, se auto envia un REJ para comenzar a enviar desde la trama no asentida
            DataFrame *esperandoRR = new DataFrame(FuncionesExtras::nombrando("REJ,",id_dato));

            esperandoRR->createFrame(nombreHost);

            scheduleAt(simTime()+10,esperandoRR);
            ev << "Host " << nombreHost << ": " << "Mandando un REJ," << id_dato << " a si mismo en caso que no llegue el RR," << id_dato << " en 5 segundos." << endl;

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
        if(nombreHost==0){
            ventanaDeslizante00.push(copiaParaVentana);
        }else if(nombreHost==1){
            ventanaDeslizante01.push(copiaParaVentana);
        }

        //Actualiza el valor de las tramas en la ventana
        cant_tramasVentana = par("cant_tramasVentana");
        cant_tramasVentana++;
        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

        string tipo_error;
        if (prob_error < error && error != 0){
            if(rand()%100 < 50){
                tipo_error = "BadSending";
                ev << "Host " << nombreHost << ": " << "Error en la informacion de la trama" << endl;
            }else{
                tipo_error = "Lost";
                ev << "Host " << nombreHost << ": " << "Error en el enviado de la trama" << endl;
            }            
        }else{
            tipo_error = "NONE";
            ev << "Host " << nombreHost << ": " << "no hay errores para esta trama:" <<  id_dato << endl;
        }           
        
        //Guarda el valor de las tramas enviadas y que no han recibido asentimiento RR
        int tramas_no_asentidas = par("tramas_no_asentidas");
        tramas_no_asentidas++;
        par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);        

        //En caso de ser menor a la probabilidad de error, la trama no se envia
        if (tipo_error == "NONE"){
            //Enviando trama por el medio fisico
            sending(tramaInformacion,"hacia_fisico");
            ev << "Host " << nombreHost << ": " << "Mandando trama de Información " << id_dato << " al host: " << address_dest;
        }else if(tipo_error == "Lost"){
            ev << "Host " << nombreHost << ": " << "La trama "<< nombre_dato.c_str() << " se ha perdido.";
        }else if(tipo_error == "BadSending")        {
            //Si no supera el umbral de error, la trama se setea como erronea.
            tramaInformacion->setName(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"ERROR,"));
            ev << "Host " << nombreHost << ": " << "La trama "<< nombre_dato.c_str() << " se ha enviado con error.";

            sending(tramaInformacion,"hacia_fisico");
            ev << "Host " << nombreHost << ": " << "Mandando trama de Información " << id_dato << " al host: " << address_dest;
        }

        //Cantidad de tramas maximas en la ventana
        unsigned int tamVentana = par("tamVentana");
        unsigned int tamActVentana;
        if(nombreHost==0){
            tamActVentana = ventanaDeslizante00.size();    
        }else if(nombreHost==1){
            tamActVentana = ventanaDeslizante01.size();    
        }        

        ev << "Tramas en la ventana: " << cant_tramasVentana << " - Ventana de: " << tamVentana << " - queue: " << tamActVentana << endl;
        //Si aún hay espacio en la ventana se manda un ACK para recibir más información
        if(tamActVentana != tamVentana){
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

        delete informacion;
    }
}

void enlace::processMsgFromLowerLayer(cMessage *packet){
    //Obtiene el estado del modulo
    string en_respuesta_a = par("en_respuesta_a");

    //Obtiene la direccion del host del modulo
    int nombreHost = par("direccion_host");

    int address_dest = par("direccion_dest");
    string packet_name = packet->getName();

    //Se transforma el mensaje que ingresa en tipo DataFrame
    DataFrame *dataframe = check_and_cast<DataFrame *>(packet);

    //Si el dataframe corresponde a una trama de informacion
    if(dataframe->getControl(0) == 0){

        //Si no se esta en estado de error
        if(en_respuesta_a != "ERROR"){
            //Si el paquete que ha llegado tiene error
            if (packet_name[0] == 'E' && packet_name[1] == 'R' && packet_name[2] == 'R' && packet_name[3] == 'O' && packet_name[4] == 'R'){
                int trama_con_error = FuncionesExtras::getValorId(packet_name.c_str());
                par("trama_con_error").setLongValue(trama_con_error);

                //Manda un REJ de error
                en_respuesta_a = "ERROR";
                par("en_respuesta_a").setStringValue(en_respuesta_a);

                //Usando dataframe para modificar la informacion
                dataframe->setName(FuncionesExtras::nombrando("REJ,",FuncionesExtras::getValorId(packet_name.c_str())));

                dataframe->createFrame(address_dest);

                sending(dataframe,"hacia_fisico");

            //Se recibe una trama correctamente
            }else{
                //Se envia la información recibida a aplicacion
                //Crea el packete de informacion para mandarlo a Aplicacion
                Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrandoTrama(packet_name.c_str(),"DATO,"));

                //Asignando valores correspondientes
                tramaComunicacion->createFrame(address_dest,dataframe->getInformation(),dataframe->getInformationArraySize());

                //Lo envia hacia aplicacion
                send(tramaComunicacion,"hacia_arriba");
                ev << "Host " << nombreHost << ": " << "Mandando mensaje recibido a aplicacion";

                //Asigna el valor de la última trama recibida
                int id_trama = FuncionesExtras::getValorId(packet_name.c_str());
                int ult_trama_recibida = par("ult_trama_recibida");

                //Si la trama que llegó corresponde a la secuencia
                if(ult_trama_recibida+1 == id_trama){
                    par("ult_trama_recibida").setLongValue(id_trama);

                    //Si se recibe el bit P/F en 1 se responde con un RR,N,1
                    if(dataframe->getControl(4) == 1){
                        //Se envia RR N al otro host

                        //Para asignar el valor_id
                        int valor_id = FuncionesExtras::getValorId(packet_name.c_str());
                        valor_id++;
                        dataframe->setName(FuncionesExtras::nombrando("RR,",valor_id));

                        //Actualiza el valor del ultimo RR enviado
                        par("ult_rr_enviado").setLongValue(valor_id);

                        //Asignando los valores necesesarios para RR
                        dataframe->createFrame(address_dest,0,NULL,0,1,0);

                        sending(dataframe,"hacia_fisico");
                        ev << "Host " << nombreHost << ": " << "Enviado Supervisory RR" << " a Host: " << address_dest;
                        par("tramas_no_asentidas").setLongValue(0);
                    }

                //Si no, significa que se perdio una trama en el camino
                }else{
                    int trama_con_error = ult_trama_recibida+1;
                    par("trama_con_error").setLongValue(trama_con_error);

                    //Manda un REJ de error
                    en_respuesta_a = "ERROR";
                    par("en_respuesta_a").setStringValue(en_respuesta_a);

                    //Usando dataframe para modificar la informacion
                    dataframe->setName(FuncionesExtras::nombrando("REJ,",trama_con_error));

                    dataframe->createFrame(address_dest);

                    sending(dataframe,"hacia_fisico");
                }
            }

        //Si ya se encontró un error en una trama y se espera que llegue correguida
        }else if(en_respuesta_a == "ERROR"){
            int valor_trama = FuncionesExtras::getValorId(packet_name.c_str());
            int trama_con_error = par("trama_con_error");

            //Si corresponde al error se anula el modo de respuesta y se pueden seguir recibiendo tramas tras registrar el error
            if(dataframe->getControl(4) == 1 && valor_trama == trama_con_error){
                en_respuesta_a = "NONE";
                par("en_respuesta_a").setStringValue(en_respuesta_a);

                //Se inicializa el metodo con el error arreglado
                processMsgFromLowerLayer(packet);

            //Si no las tramas se rechazan hasta que recibe el ERROR
            }else{
                delete packet;
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
                        DataFrame *primer_elemento;
                        if(nombreHost==0){
                            primer_elemento = ventanaDeslizante00.front();
                        }else if(nombreHost==1){
                            primer_elemento = ventanaDeslizante01.front();
                        }

                        string nombre_primer_elemento = primer_elemento->getName();

                        int id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                        //Recorre la Ventana removiendo los elementos asentidos
                        for(int i=0;id_tramaRR > id_tramaVD;i++){
                            if(id_tramaVD < id_tramaRR){
                                if(nombreHost==0){
                                    ventanaDeslizante00.pop();
                                }else if(nombreHost==1){
                                    ventanaDeslizante01.pop();
                                }
                                tramas_asentidas++;
                            }

                            if(nombreHost==0){
                                if(!ventanaDeslizante00.empty()){
                                    primer_elemento = ventanaDeslizante00.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(nombreHost==1){
                                if(!ventanaDeslizante01.empty()){
                                    primer_elemento = ventanaDeslizante01.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }
                        }

                        //Actualiza el valor de las tramas en la ventana
                        int cant_tramasVentana;
                        if(nombreHost==0){
                            cant_tramasVentana = ventanaDeslizante00.size();
                        }else if(nombreHost==1){
                            cant_tramasVentana = ventanaDeslizante01.size();
                        }
                        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

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
                        DataFrame *primer_elemento;
                        if(nombreHost==0){
                            primer_elemento = ventanaDeslizante00.front();
                        }else if(nombreHost==1){
                            primer_elemento = ventanaDeslizante01.front();                            
                        }
                        string nombre_primer_elemento = primer_elemento->getName();
                        int id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                        //Recorre la Ventana removiendo los elementos asentidos
                        for(int i=0;id_tramaRR > id_tramaVD;i++){
                            if(id_tramaVD < id_tramaRR){
                                if(nombreHost==0){
                                    ventanaDeslizante00.pop();
                                }else if(nombreHost==1){
                                    ventanaDeslizante01.pop();
                                }
                                tramas_asentidas++;
                            }

                            if(nombreHost==0){
                                if(!ventanaDeslizante00.empty()){
                                    primer_elemento = ventanaDeslizante00.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(nombreHost==1){
                                if(!ventanaDeslizante01.empty()){
                                    primer_elemento = ventanaDeslizante01.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }
                        }

                        //Actualiza el valor de las tramas en la ventana
                        int cant_tramasVentana;
                        bool vacio;
                        if(nombreHost==0){
                            cant_tramasVentana = ventanaDeslizante00.size();
                            vacio = ventanaDeslizante00.empty();
                        }else if(nombreHost==1){
                            cant_tramasVentana = ventanaDeslizante01.size();
                            vacio = ventanaDeslizante01.empty();
                        }
                        
                        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                        if(vacio){
                            //Envia una trama de comando DISC al otro host
                            en_respuesta_a = "DISC";
                            par("en_respuesta_a").setStringValue(en_respuesta_a);

                            //Trama commando DISC
                            dataframe->setName("DISC, Trama Comando");

                            //Fija los datos necesarios para la trama DISC
                            dataframe->createFrame(address_dest);

                            sending(dataframe,"hacia_fisico");
                            ev << "Host " << nombreHost << ": " << "Se envia trama DISC" << endl;
                        }
                    }
                }
            
            //Recibe un REJ
            }else if(M1 == 1){
                delete dataframe;
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
                if(nombreHost==0){
                    primer_elemento = ventanaDeslizante00.front();
                }else if(nombreHost==1){
                    primer_elemento = ventanaDeslizante01.front();
                }
                nombre_primer_elemento = primer_elemento->getName();
                id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                ev << "Host " << nombreHost << ": " << "Recibido REJ," << id_tramaREJ << " comenzando a analizar elementos, partiendo por: " << id_tramaVD << endl;

                //Recorre la Ventana removiendo los elementos asentidos previos al REJ
                for(int i=0;id_tramaREJ > id_tramaVD && ((nombreHost==0 && !ventanaDeslizante00.empty()) || (nombreHost==1 && !ventanaDeslizante01.empty()));i++){
                    if(id_tramaVD < id_tramaREJ){
                        if(nombreHost==0){
                            ventanaDeslizante00.pop();
                        }else if(nombreHost==1){
                            ventanaDeslizante01.pop();
                        }
                        tramas_asentidas++;
                    }

                    if(nombreHost==0 && !ventanaDeslizante00.empty()){
                        primer_elemento = ventanaDeslizante00.front();
                    }else if(nombreHost==1 && !ventanaDeslizante01.empty()){
                        primer_elemento = ventanaDeslizante01.front();
                    }
                    nombre_primer_elemento = primer_elemento->getName();

                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                }
                
                ev << "Host " << nombreHost << ": " << "Tramas asentidas: " << tramas_asentidas << endl;

                //Actualiza el valor de las tramas en la ventana
                int cant_tramasVentana;
                if(nombreHost==0){
                    cant_tramasVentana = ventanaDeslizante00.size();
                }else if(nombreHost==1){
                    cant_tramasVentana = ventanaDeslizante01.size();
                }
                par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                //Reinicia el valor de las tramas asentidas
                int tramas_no_asentidas = par("tramas_no_asentidas");
                tramas_no_asentidas -= tramas_asentidas;
                par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);

                //Re-envia la trama que tenía el error
                queue<DataFrame*> ventanaAuxiliar;
                if(nombreHost==0){
                    ventanaAuxiliar = ventanaDeslizante00;
                }else if(nombreHost==1){
                    ventanaAuxiliar = ventanaDeslizante01;
                }

                DataFrame *tramaAuxiliar = new DataFrame(ventanaAuxiliar.front()->getName());
                tramaAuxiliar->createFrame(address_dest,ventanaAuxiliar.front()->getInformationArraySize(),ventanaAuxiliar.front()->getInformation(),0,ventanaAuxiliar.front()->getControl(4),0);

                //Envia la trama que tenía el error
                sending(tramaAuxiliar,"hacia_fisico");
                ev << "Host " << nombreHost << ": " << "Reenviando: " << ventanaAuxiliar.front()->getName() << endl;

                //Retira el elemento enviado
                ventanaAuxiliar.pop();

                //Envia con delay cada una de las otras tramas faltantes
                vector<DataFrame*> vectorAuxiliar;
                vectorAuxiliar.resize(ventanaAuxiliar.size());
                for(int i=0;!ventanaAuxiliar.empty();i++){
                    vectorAuxiliar[i] = new DataFrame(ventanaAuxiliar.front()->getName());
                    vectorAuxiliar[i]->createFrame(address_dest,ventanaAuxiliar.front()->getInformationArraySize(),ventanaAuxiliar.front()->getInformation(),0,ventanaAuxiliar.front()->getControl(4),0);

                    //Envia todas las tramas con un retraso de 1 segundo acumulable
                    sendDelayed(vectorAuxiliar[i],simTime()+1*(1+i),"hacia_fisico");
                    
                    ventanaAuxiliar.pop();
                }

                //Si se asentio almenos una trama se solicita un ACK a aplicacion
                if(tramas_asentidas>0){
                    //Obtiene el valor del ultimo ACK pedido y pide el siguiente
                    id_tramaVD = par("ult_ack_enviado");
                    id_tramaVD++;

                    //Mandar un ACK,N al modulo de aplicación
                    cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",id_tramaVD));

                    //Actualiza el valor del ultimo ack pedido
                    int ult_ack_enviado = id_tramaVD;
                    par("ult_ack_enviado").setLongValue(ult_ack_enviado);

                    //Manda el mensaje a aplicacion
                    send(ack,"hacia_arriba");
                    ev << "Host " << nombreHost << ": " << "Mandando ACK al modulo de aplicación" << endl;
                }

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
                    sending(dataframe,"hacia_fisico");
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
                        sending(dataframe,"hacia_fisico");
                        ev << "Host " << nombreHost << ": " << "Enviado Supervisory RR" << " a Host: " << address_dest;
                    }else{
                        ev << "Host " << nombreHost << ": " << "UP no respondido debido a que ya se ha enviado un RR adecuado" << address_dest;
                    }

                //Se recibe un UA
                }else if(M2 == 6){
                    delete dataframe;

                    //Si la trama UA fue recibida por enviar un SABM
                    if(en_respuesta_a == "SABM"){    

                        en_respuesta_a = "NONE";
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

                        en_respuesta_a = "NONE";
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
                    sending(dataframe,"hacia_fisico");
                    ev << "Host " << nombreHost << ": " << "Enviado Unnumbered UA a Host: " << address_dest;
                }
            }
        }
    }
}
