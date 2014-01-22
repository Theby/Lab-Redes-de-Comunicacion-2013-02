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
#include <llc.h>

#include <FuncionesExtras.h>

using namespace std;

//Define la clase para trabajar directamente con el modulo de LLC
Define_Module( llc );

//listas para el host 0
std::queue<DataFrame*> ventanaDeslizante00;
vector<DataFrame*> temporizadores00;
vector<string> tempoNames00;

//listas para el host 1
std::queue<DataFrame*> ventanaDeslizante01;
vector<DataFrame*> temporizadores01;
vector<string> tempoNames01;

//listas para el host 2
std::queue<DataFrame*> ventanaDeslizante02;
vector<DataFrame*> temporizadores02;
vector<string> tempoNames02;

//listas para el host 3
std::queue<DataFrame*> ventanaDeslizante03;
vector<DataFrame*> temporizadores03;
vector<string> tempoNames03;

//Valida las variables
void llc::initialize(){
    int error = par("error");
    int numTramas_env = par("numTramas_env");
    numTramas_env *= 3;
    par("numTramas_env").setLongValue(numTramas_env);    
    int numTramas_rec = par("numTramas_rec");
    numTramas_rec *= 3;
    par("numTramas_rec").setLongValue(numTramas_rec);
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

//Maneja el mensaje ingresado
void llc::handleMessage(cMessage *msg){
    string name = msg->getName();

    //Si el mensaje llega desde el otro Host
    if (msg->arrivedOn("desde_abajo")){
        //Procesarlo como si viniera desde abajo
        processMsgFromLowerLayer(msg);
    //Sino, el mensaje viene desde intermedio
    }else{
        if(name[0]=='R' && name[1]=='E' && name[2]=='J'){
            delete msg;
            int direccion_host = par("direccion_host");
            int direccion_dest = par("direccion_dest");

            //Si faltan tramas por ser asentidas por un RR se manda un UP para exigir respuesta
            DataFrame *unnumberedFrame = new DataFrame("UP,Trama Comando,");
            
            //Asignando valores necesarios para crear la trama
            unnumberedFrame->createFrame(direccion_dest);
            unnumberedFrame->setName(FuncionesExtras::nombrando(unnumberedFrame->getName(),direccion_dest));

            par("en_respuesta_a").setStringValue("UP");
            //Enviando a travez del canal fisico
            send(unnumberedFrame,"hacia_abajo");
            ev << "Host " << direccion_host << ": " << "Se envia UP para confirmar las ultimas tramas" << endl;
        }else{
            //Procesarlo como si viniera desde arriba
            processMsgFromHigherLayer(msg);
        }
    }
}

//Lo que llega desde aplicacion
void llc::processMsgFromHigherLayer(cMessage *dato){
    //Obtiene el estado del modulo
    string en_respuesta_a = par("en_respuesta_a");

    //Obtiene la direccion del host del modulo
    int direccion_host = par("direccion_host");

    int direccion_dest = par("direccion_dest");
    string nombre_dato = dato->getName();

    bool correguido = par("correguido");

    //Para guardar la cantidad de tramas de la ventana
    unsigned int cant_tramasVentana;

    //Si es una trama START
    if(nombre_dato == "START"){
        int starter = par("starter");

        //El host que comienza la transmisión inicializa todos los datos
        if(starter==direccion_host){
            while(!ventanaDeslizante00.empty()){
                ventanaDeslizante00.pop();
            }
            
            temporizadores00.clear();
            tempoNames00.clear();

            while(!ventanaDeslizante01.empty()){
                ventanaDeslizante01.pop();
            }

            temporizadores01.clear();
            tempoNames01.clear();

            while(!ventanaDeslizante02.empty()){
                ventanaDeslizante02.pop();
            }

            temporizadores02.clear();
            tempoNames02.clear();

            while(!ventanaDeslizante03.empty()){
                ventanaDeslizante03.pop();
            }

            temporizadores03.clear();
            tempoNames03.clear();
        }        

        //Para saber cual fue la última trama asentida y si es que faltan por asentir
        par("tramas_no_asentidas").setLongValue(0);
        par("ult_trama_recibida").setLongValue(-1);

        //Se conecta
        if (ev.isGUI()){
            getDisplayString().setTagArg("i",1,"");
            bubble("Conectado!");
        }

        //Manda el mensaje START a MAC
        send(dato,"hacia_abajo");
    //Si es una trama END
    }else if(nombre_dato == "END"){
     //Se dseconecta
     if (ev.isGUI()){
         getDisplayString().setTagArg("i",1,"black");
         bubble("Desconectado!");
     }

     //manda el mensaje END a MAC
     send(dato,"hacia_abajo");
    //Si es trama de informacion
    }else{
        //Se recibe un DATO,N por lo que se envia una trama de informacion al otro host
        srand(time(NULL));
        int prob_error = rand()%100;

        // Probabilidad de que se envie de forma erronea una trama
        int error = par("error");

        //Para el bit P/F
        int bit_pf;
        int tramas_libres = par("tramas_libres");

        //Pasar los datos de un paquete a otro (Informacion -> DataFrame)
        Informacion *informacion = check_and_cast<Informacion *>(dato);

        //Obtiene el valor id y lo compara con el último ACK enviado
        int id_dato = FuncionesExtras::getValorId(nombre_dato.c_str());
        int ult_ACK = par("ult_ack_enviado");

        if(id_dato == ult_ACK){
            //Para el bit P/F
            if(id_dato%tramas_libres == (tramas_libres-1)){
                bit_pf = 1;
            }else{
                bit_pf = 0;
            }

            //Temporizador que espera un RR durante 10 segundos, si no, se auto envia un REJ para comenzar a enviar desde la trama no asentida
            DataFrame *esperandoRR = new DataFrame(FuncionesExtras::nombrando("REJ,",id_dato));
            esperandoRR->createFrame(direccion_host);

            //Agrega el puntero del dataframe enviado
            string tempo_name;
            if(direccion_host==0){
                tempo_name = esperandoRR->getName();
                tempoNames00.push_back(tempo_name);
                temporizadores00.push_back(esperandoRR);
            }else if(direccion_host==1){
                tempo_name = esperandoRR->getName();
                tempoNames01.push_back(tempo_name);
                temporizadores01.push_back(esperandoRR);
            }else if(direccion_host==2){
                tempo_name = esperandoRR->getName();
                tempoNames02.push_back(tempo_name);
                temporizadores02.push_back(esperandoRR);
            }else if(direccion_host==3){
                tempo_name = esperandoRR->getName();
                tempoNames03.push_back(tempo_name);
                temporizadores03.push_back(esperandoRR);
            }           

            //Guardando el mensaje que se autoenviara
            int timer = par("timer");
            timer *= tramas_libres;
            scheduleAt(simTime()+timer,esperandoRR);
            ev << "Host " << direccion_host << ": " << "Mandando un REJ," << id_dato << " a si mismo en caso que no llegue el RR," << id_dato << " en " << timer << " segundos." << endl;
            ev << "Host " << direccion_host << ": " << "Tiempo esperado para mandar el REJ: " << simTime()+10 << endl;
            
            int direccion_actual = informacion->getDestino();
            par("direccion_dest").setLongValue(direccion_actual);
            
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
            tramaInformacion->createFrame(direccion_actual,informacion->getInformacionArraySize(),informacion->getInformacion(),0,bit_pf,0);
            copiaParaVentana->createFrame(direccion_actual,informacion->getInformacionArraySize(),informacion->getInformacion(),0,bit_pf,0);

            tramaInformacion->setName(FuncionesExtras::nombrando(tramaInformacion->getName(),',',direccion_actual));
            copiaParaVentana->setName(FuncionesExtras::nombrando(copiaParaVentana->getName(),',',direccion_actual));

            delete dato;

            //Se almacena la trama en la ventana
            if(direccion_host==0){
                ventanaDeslizante00.push(copiaParaVentana);
            }else if(direccion_host==1){
                ventanaDeslizante01.push(copiaParaVentana);
            }else if(direccion_host==2){
                ventanaDeslizante02.push(copiaParaVentana);
            }else if(direccion_host==3){
                ventanaDeslizante03.push(copiaParaVentana);
            }

            //Actualiza el valor de las tramas en la ventana
            cant_tramasVentana = par("cant_tramasVentana");
            cant_tramasVentana++;
            par("cant_tramasVentana").setLongValue(cant_tramasVentana);

            string tipo_error;
            correguido = true;
            if (prob_error < error && error != 0 && !correguido){
                if(rand()%100 < 100){
                    tipo_error = "BadSending";
                    ev << "Host " << direccion_host << ": " << "Error en la informacion de la trama" << endl;
                }else{
                    tipo_error = "Lost";
                    ev << "Host " << direccion_host << ": " << "Error en el enviado de la trama" << endl;
                }            
            }else{
                tipo_error = "NONE";
                ev << "Host " << direccion_host << ": " << "no hay errores para esta trama:" <<  id_dato << endl;
            }           
            
            //Guarda el valor de las tramas enviadas y que no han recibido asentimiento RR
            int tramas_no_asentidas = par("tramas_no_asentidas");
            tramas_no_asentidas++;
            par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);

            //Revisa bajo que condiciones se enviará la trama
            if (tipo_error == "NONE"){
                par("correguido").setBoolValue(false);
                send(tramaInformacion,"hacia_abajo");
                ev << "Host " << direccion_host << ": " << "Mandando trama de Información " << id_dato << " al host: " << direccion_dest << endl;
            }else if(tipo_error == "Lost"){
                tramaInformacion->setName(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"LOST,"));

                send(tramaInformacion,"hacia_abajo");
                ev << "Host " << direccion_host << ": " << "La trama "<< nombre_dato.c_str() << " se ha perdido." << endl;
            }else if(tipo_error == "BadSending")        {
                tramaInformacion->setName(FuncionesExtras::nombrandoTrama(nombre_dato.c_str(),"ERROR,"));
                tramaInformacion->setName(FuncionesExtras::nombrando(tramaInformacion->getName(),',',direccion_actual));
                ev << "Host " << direccion_host << ": " << "La trama "<< nombre_dato.c_str() << " se ha enviado con error." << endl;

                send(tramaInformacion,"hacia_abajo");
                ev << "Host " << direccion_host << ": " << "Mandando trama de Información " << id_dato << " a MAC";
            }
        }else{
            delete dato;
            ev << "Host " << direccion_host << ": " << "La trama \"DATO," << id_dato << "\" no corresponde a la ultima trama enviada: " << ult_ACK << " por lo que ha sido descartada" << endl;
        }
    }
}

//Maneja todas las tramas recibidas desde MAC
void llc::processMsgFromLowerLayer(cMessage *packet){
    //Obtiene el estado del modulo
    string en_respuesta_a = par("en_respuesta_a");

    //Obtiene la direccion del host del modulo
    int direccion_host = par("direccion_host");

    int direccion_dest = par("direccion_dest");
    string packet_name = packet->getName();

    int numTramas_env = par("numTramas_env");
    int tramasPorHost = numTramas_env/3;

    if(packet_name == "END"){
        delete packet;
        //Envia una trama DISC a MAC
        //Entra en estado de "esperando respuesta a DISC"
        en_respuesta_a = "DISC";
        par("en_respuesta_a").setStringValue(en_respuesta_a);

        //Trama commando DISC
        DataFrame *tramaComando = new DataFrame("DISC,Trama Comando,");

        direccion_dest = (direccion_dest+2)%4;

        par("direccion_dest").setLongValue(direccion_dest);

        //Crea los valores necesarios para una trama DISC
        tramaComando->createFrame(direccion_dest);
        tramaComando->setName(FuncionesExtras::nombrando(tramaComando->getName(),direccion_dest));

        //Envia la trama por el canal fisico
        send(tramaComando,"hacia_abajo");
        ev << "Host " << direccion_host << ": " << "Enviado Unnumbered DISC a Host: " << direccion_dest;
    }else if(packet_name == "START"){
        delete packet;
        //Envia el ACK,0 a aplicacion

        if (ev.isGUI()){
            getParentModule()->getDisplayString().setTagArg("i",1,"");
            getParentModule()->bubble("Conectado!");
            getDisplayString().setTagArg("i",1,"");
            bubble("Conectado!");
        }

        cMessage *ack = new cMessage("ACK,0");
        par("ult_ack_enviado").setLongValue(0);
        send(ack,"hacia_arriba");
        ev << "Host " << direccion_host << ": " << "Enviado ACK,0 hacia aplicacion" << endl;
    }else if(packet_name == "TESTIGO"){
        //Se lo devuelve a MAC
        send(packet,"hacia_abajo");
    }else{
        //Se transforma el mensaje que ingresa en tipo DataFrame
        DataFrame *dataframe = check_and_cast<DataFrame *>(packet);

        //Si el dataframe corresponde a una trama de informacion
        if(dataframe->getControl(0) == 0 && en_respuesta_a != "DISC"){
            int id_trama = FuncionesExtras::getValorId(packet_name.c_str());
            //Asigna el valor de la última trama recibida
            int ult_trama_recibida = par("ult_trama_recibida");

            if(en_respuesta_a == "ERROR"){
                    int trama_con_error = par("trama_con_error");

                    //Si corresponde al error se anula el modo de respuesta y se pueden seguir recibiendo tramas tras registrar el error
                    if(dataframe->getControl(4) == 1 && id_trama == trama_con_error){
                        en_respuesta_a = "NONE";
                        par("en_respuesta_a").setStringValue(en_respuesta_a);

                        //Se inicializa el metodo con el error arreglado
                        processMsgFromLowerLayer(dataframe);

                    //Si no las tramas se rechazan hasta que recibe el ERROR
                    }else{
                        delete packet;
                    }
            //Llego la trama esperada
            }else if((ult_trama_recibida+1)%tramasPorHost == id_trama){
                //Si el paquete que ha llegado tiene error
                if (packet_name[0] == 'E' && packet_name[1] == 'R' && packet_name[2] == 'R' && packet_name[3] == 'O' && packet_name[4] == 'R'){
                    int trama_con_error = id_trama;
                    par("trama_con_error").setLongValue(trama_con_error);

                    ev << "Host " << direccion_host << ": " << "La trama " << trama_con_error << " ha sido recibida con errores" << endl;

                    //Manda un REJ de error
                    en_respuesta_a = "ERROR";
                    par("en_respuesta_a").setStringValue(en_respuesta_a);

                    //Usando dataframe para modificar la informacion
                    dataframe->setName(FuncionesExtras::nombrando("REJ,",trama_con_error));

                    dataframe->createFrame(direccion_host);
                    dataframe->setName(FuncionesExtras::nombrando(dataframe->getName(),',',direccion_host));

                    send(dataframe,"hacia_abajo");
                    ev << "Host " << direccion_host << ": " << "Mandando REJ," << trama_con_error << endl;
                //Se recibe una trama correctamente
                }else{
                    //Se envia la información recibida a aplicacion
                    //Crea el packete de informacion para mandarlo a Aplicacion
                    Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrando("DATO,",id_trama));

                    //Asignando valores correspondientes
                    tramaComunicacion->createFrame(direccion_dest,dataframe->getInformation(),dataframe->getInformationArraySize());

                    //Lo envia hacia aplicacion
                    send(tramaComunicacion,"hacia_arriba");
                    ev << "Host " << direccion_host << ": " << "Mandando mensaje " << id_trama << " recibido a aplicacion" << endl;

                    //Si la trama que llegó corresponde a la secuencia
                    par("ult_trama_recibida").setLongValue(id_trama);

                    //Si se recibe el bit P/F en 1 se responde con un RR,N,1
                    if(dataframe->getControl(4) == 1){
                        //Para asignar el id_trama
                        id_trama++;
                        dataframe->setName(FuncionesExtras::nombrando("RR,",id_trama));

                        //Actualiza el valor del ultimo RR enviado
                        par("ult_rr_enviado").setLongValue(id_trama);

                        //Asignando los valores necesesarios para RR
                        dataframe->createFrame(direccion_host,0,NULL,0,1,0);
                        dataframe->setName(FuncionesExtras::nombrando(dataframe->getName(),',',direccion_host));

                        send(dataframe,"hacia_abajo");
                        ev << "Host " << direccion_host << ": " << "Enviado Supervisory RR," << id_trama << " a Host: " << direccion_dest << endl;
                    }else{  
                        //Cantidad de tramas actualmente en la ventana
                        int cant_tramasVentana = par("cant_tramasVentana");

                        //Cantidad de tramas maximas en la ventana
                        unsigned int tamVentana = par("tamVentana");
                        unsigned int tamActVentana;
                        if(direccion_host==0){
                            tamActVentana = ventanaDeslizante00.size();    
                        }else if(direccion_host==1){
                            tamActVentana = ventanaDeslizante01.size();    
                        }else if(direccion_host==2){
                            tamActVentana = ventanaDeslizante02.size();    
                        }else if(direccion_host==3){
                            tamActVentana = ventanaDeslizante03.size();    
                        }

                        //Si aún hay espacio en la ventana se manda un ACK para recibir más información
                        int valor_id = par("ult_ack_enviado");
                        valor_id++;

                        ev << "Tramas en la ventana: " << cant_tramasVentana << " - Ventana de: " << tamVentana << " - queue: " << tamActVentana << endl;

                        //Solo si hay espacio en la ventana y no se piden más datos de los que se deben enviar
                        if(tamActVentana != tamVentana && valor_id < numTramas_env && valor_id < tramasPorHost){
                            //Crea un ACK,N               
                            cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",valor_id));

                            //Actualiza el valor del último ACK pedido
                            par("ult_ack_enviado").setLongValue(valor_id%tramasPorHost);

                            //Envia ACK,N al modulo de aplicacion
                            send(ack,"hacia_arriba");
                            ev << "Host " << direccion_host << ": " << "Mandando ACK al modulo de aplicación" << endl;
                        //Envia testigo para que el token ring siga su funcionamiento
                        }else if(valor_id == tramasPorHost){
                            DataFrame *testigo = new DataFrame("TESTIGO");
                            send(testigo,"hacia_abajo");
                        }
                    }                    
                }
            //Si no, significa que se perdio una trama en el camino
            }else if((ult_trama_recibida+1)%tramasPorHost != id_trama){
                ev << "Host " << direccion_host << ": " << "No ha llegado la trama " << ult_trama_recibida+1 << " que corresponde. Llegó: " << id_trama << endl;

                int trama_con_error = ult_trama_recibida+1;
                par("trama_con_error").setLongValue(trama_con_error);

                //Manda un REJ de error
                en_respuesta_a = "ERROR";
                par("en_respuesta_a").setStringValue(en_respuesta_a);

                //Usando dataframe para modificar la informacion
                dataframe->setName(FuncionesExtras::nombrando("REJ,",trama_con_error));
                dataframe->createFrame(direccion_host);
                dataframe->setName(FuncionesExtras::nombrando(dataframe->getName(),',',direccion_host));

                send(dataframe,"hacia_abajo");
            }
        //Si no, puede ser de control o no-numerada
        }else{
            //Si el dataframe corresponde a una trama supervisora
            if(dataframe->getControl(1) == 0 && en_respuesta_a != "DISC"){
                int M1;

                M1 = pow(2,1)*dataframe->getControl(2) + pow(2,0)*dataframe->getControl(3);

                //Se recibe un RR
                if(M1 == 0){
                    //Revisa el valor del bit P/F
                    if(dataframe->getControl(4) == 1 && en_respuesta_a != "UP"){
                        delete dataframe;

                        //Trama a la que corresponde el RR
                        int id_tramaRR = FuncionesExtras::getValorId(packet_name.c_str());
                        
                        //Cancela los eventos hasta el RR recibido
                        cancelarEventosPendientes(id_tramaRR);

                        //Para saber cuantas tramas se asentieron
                        int tramas_asentidas = 0;

                        //Datos del primer elemento de la trama
                        DataFrame *primer_elemento;
                        if(direccion_host==0){
                            primer_elemento = ventanaDeslizante00.front();
                        }else if(direccion_host==1){
                            primer_elemento = ventanaDeslizante01.front();
                        }else if(direccion_host==2){
                            primer_elemento = ventanaDeslizante02.front();
                        }else if(direccion_host==3){
                            primer_elemento = ventanaDeslizante03.front();
                        }

                        string nombre_primer_elemento = primer_elemento->getName();

                        int id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                        //Recorre la Ventana removiendo los elementos asentidos
                        for(int i=0;id_tramaRR > id_tramaVD;i++){
                            if(id_tramaVD < id_tramaRR){
                                if(direccion_host==0){
                                    ventanaDeslizante00.pop();
                                }else if(direccion_host==1){
                                    ventanaDeslizante01.pop();
                                }else if(direccion_host==2){
                                    ventanaDeslizante02.pop();
                                }else if(direccion_host==3){
                                    ventanaDeslizante03.pop();
                                }
                                tramas_asentidas++;
                            }

                            if(direccion_host==0){
                                if(!ventanaDeslizante00.empty()){
                                    primer_elemento = ventanaDeslizante00.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(direccion_host==1){
                                if(!ventanaDeslizante01.empty()){
                                    primer_elemento = ventanaDeslizante01.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(direccion_host==2){
                                if(!ventanaDeslizante02.empty()){
                                    primer_elemento = ventanaDeslizante02.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(direccion_host==3){
                                if(!ventanaDeslizante03.empty()){
                                    primer_elemento = ventanaDeslizante03.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }
                        }

                        //Actualiza el valor de las tramas en la ventana
                        int cant_tramasVentana;
                        if(direccion_host==0){
                            cant_tramasVentana = ventanaDeslizante00.size();
                        }else if(direccion_host==1){
                            cant_tramasVentana = ventanaDeslizante01.size();
                        }else if(direccion_host==2){
                            cant_tramasVentana = ventanaDeslizante02.size();
                        }else if(direccion_host==3){
                            cant_tramasVentana = ventanaDeslizante03.size();
                        }
                        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                        //Obtiene el valor del ultimo ACK pedido y pide el siguiente
                        id_tramaVD = par("ult_ack_enviado");
                        id_tramaVD++;

                        cMessage *ack = new cMessage();                        
                        
                        if(id_tramaRR < numTramas_env){
                            if(id_tramaVD < numTramas_env){
                                //Mandar un ACK,N al modulo de aplicación
                                ack->setName(FuncionesExtras::nombrando("ACK,",id_tramaVD));

                                //Actualiza el valor del ultimo ack pedido
                                par("ult_ack_enviado").setLongValue(id_tramaVD%tramasPorHost);      
                            }                      
                        }else{
                            //Mandar un ACK,id_tramaRR al modulo de aplicación
                            ack->setName(FuncionesExtras::nombrando("ACK,",id_tramaRR));

                            //Actualiza el valor del ultimo ack pedido
                            par("ult_ack_enviado").setLongValue(id_tramaRR%tramasPorHost); 
                        }

                        //Manda el mensaje a aplicacion
                        send(ack,"hacia_arriba");
                        ev << "Host " << direccion_host << ": " << "Mandando ACK al modulo de aplicación" << endl;

                        //Reinicia el valor de las tramas asentidas
                        int tramas_no_asentidas = par("tramas_no_asentidas");
                        tramas_no_asentidas -= tramas_asentidas;
                        par("tramas_no_asentidas").setLongValue(tramas_no_asentidas);       

                    }else if(en_respuesta_a == "UP"){
                        //Cancela todos los eventos
                        cancelarEventosPendientes();

                        //Trama a la que corresponde el RR
                        int id_tramaRR = FuncionesExtras::getValorId(packet_name.c_str());

                        //Para saber cuantas tramas se asentieron
                        int tramas_asentidas = 0;

                        //Datos del primer elemento de la trama
                        DataFrame *primer_elemento;
                        if(direccion_host==0){
                            primer_elemento = ventanaDeslizante00.front();
                        }else if(direccion_host==1){
                            primer_elemento = ventanaDeslizante01.front();                            
                        }else if(direccion_host==2){
                            primer_elemento = ventanaDeslizante02.front();                            
                        }else if(direccion_host==3){
                            primer_elemento = ventanaDeslizante03.front();                            
                        }
                        string nombre_primer_elemento = primer_elemento->getName();
                        int id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                        //Recorre la Ventana removiendo los elementos asentidos
                        for(int i=0;id_tramaRR > id_tramaVD;i++){
                            if(id_tramaVD < id_tramaRR){
                                if(direccion_host==0){
                                    ventanaDeslizante00.pop();
                                }else if(direccion_host==1){
                                    ventanaDeslizante01.pop();
                                }else if(direccion_host==2){
                                    ventanaDeslizante02.pop();
                                }else if(direccion_host==3){
                                    ventanaDeslizante03.pop();
                                }
                                tramas_asentidas++;
                            }

                            if(direccion_host==0){
                                if(!ventanaDeslizante00.empty()){
                                    primer_elemento = ventanaDeslizante00.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(direccion_host==1){
                                if(!ventanaDeslizante01.empty()){
                                    primer_elemento = ventanaDeslizante01.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(direccion_host==2){
                                if(!ventanaDeslizante02.empty()){
                                    primer_elemento = ventanaDeslizante02.front();
                                    nombre_primer_elemento = primer_elemento->getName();

                                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                                }else{
                                    break;
                                }
                            }else if(direccion_host==3){
                                if(!ventanaDeslizante03.empty()){
                                    primer_elemento = ventanaDeslizante03.front();
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
                        if(direccion_host==0){
                            cant_tramasVentana = ventanaDeslizante00.size();
                            vacio = ventanaDeslizante00.empty();
                        }else if(direccion_host==1){
                            cant_tramasVentana = ventanaDeslizante01.size();
                            vacio = ventanaDeslizante01.empty();
                        }else if(direccion_host==2){
                            cant_tramasVentana = ventanaDeslizante02.size();
                            vacio = ventanaDeslizante02.empty();
                        }else if(direccion_host==3){
                            cant_tramasVentana = ventanaDeslizante03.size();
                            vacio = ventanaDeslizante03.empty();
                        }
                        
                        par("cant_tramasVentana").setLongValue(cant_tramasVentana);

                        //Si se logran asentir todas las tramas
                        if(vacio){
                            en_respuesta_a = "NONE";
                            par("en_respuesta_a").setStringValue(en_respuesta_a);

                            //Mandar un ACK,id_tramaRR al modulo de aplicación
                            cMessage *ack = new cMessage();
                            ack->setName(FuncionesExtras::nombrando("ACK,",id_tramaRR));

                            //Actualiza el valor del ultimo ack pedido
                            par("ult_ack_enviado").setLongValue(id_tramaRR%tramasPorHost); 

                            //Manda el mensaje a aplicacion
                            send(ack,"hacia_arriba");
                            ev << "Host " << direccion_host << ": " << "Mandando ACK al modulo de aplicación" << endl;

                            //Reinicia el valor de las tramas asentidas
                            par("tramas_no_asentidas").setLongValue(0);

                        //Si no vuelve a enviar un UP
                        }else{
                            //Si faltan tramas por ser asentidas por un RR se manda un UP para exigir respuesta
                            DataFrame *unnumberedFrame = new DataFrame("UP,Trama Comando,");
                            
                            //Asignando valores necesarios para crear la trama
                            unnumberedFrame->createFrame(direccion_dest);
                            unnumberedFrame->setName(FuncionesExtras::nombrando(unnumberedFrame->getName(),direccion_dest));

                            //Enviando a travez del canal fisico
                            send(unnumberedFrame,"hacia_abajo");
                            ev << "Host " << direccion_host << ": " << "Se envia UP para confirmar las ultimas tramas" << endl;
                        }
                    }                                    
                //Se recibe un REJ
                }else if(M1 == 1){
                    delete dataframe;
                    //Retransmite a partir de la trama deseada

                    //Obtiene el id de la trama REJ
                    int id_tramaREJ = FuncionesExtras::getValorId(packet_name.c_str());

                    //Cancela los eventos relacionados
                    cancelarEventosPendientes();

                    //Trama a la que corresponden los elementos de la Ventana Deslizante
                    int id_tramaVD = 0;

                    //Para ir guardando los datos de la ventana deslizante
                    DataFrame *primer_elemento;
                    string nombre_primer_elemento;

                    //Datos del primer elemento de la ventana
                    if(direccion_host==0){
                        primer_elemento = ventanaDeslizante00.front();
                    }else if(direccion_host==1){
                        primer_elemento = ventanaDeslizante01.front();
                    }else if(direccion_host==2){
                        primer_elemento = ventanaDeslizante02.front();
                    }else if(direccion_host==3){
                        primer_elemento = ventanaDeslizante03.front();
                    }
                    nombre_primer_elemento = primer_elemento->getName();
                    id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());

                    ev << "Host " << direccion_host << ": " << "Recibido REJ," << id_tramaREJ << " comenzando a analizar elementos, partiendo por: " << id_tramaVD << endl;

                    //Recorre la Ventana removiendo los elementos asentidos previos al REJ para buscar la trama con el error
                    for(int i=0;id_tramaREJ > id_tramaVD && ((direccion_host==0 && !ventanaDeslizante00.empty()) || (direccion_host==1 && !ventanaDeslizante01.empty()));i++){
                        if(id_tramaVD < id_tramaREJ){
                            if(direccion_host==0){
                                ventanaDeslizante00.pop();
                            }else if(direccion_host==1){
                                ventanaDeslizante01.pop();
                            }else if(direccion_host==2){
                                ventanaDeslizante02.pop();
                            }else if(direccion_host==3){
                                ventanaDeslizante03.pop();
                            }
                        }

                        if(direccion_host==0 && !ventanaDeslizante00.empty()){
                            primer_elemento = ventanaDeslizante00.front();
                        }else if(direccion_host==1 && !ventanaDeslizante01.empty()){
                            primer_elemento = ventanaDeslizante01.front();
                        }else if(direccion_host==2 && !ventanaDeslizante02.empty()){
                            primer_elemento = ventanaDeslizante02.front();
                        }else if(direccion_host==3 && !ventanaDeslizante03.empty()){
                            primer_elemento = ventanaDeslizante03.front();
                        }
                        nombre_primer_elemento = primer_elemento->getName();

                        id_tramaVD = FuncionesExtras::getValorId(nombre_primer_elemento.c_str());
                    }
                    
                    //Vacia las ventanas
                    if(direccion_host==0){
                        while(!ventanaDeslizante00.empty()){
                            ventanaDeslizante00.pop();
                        }
                    }else if(direccion_host==1){
                        while(!ventanaDeslizante01.empty()){
                            ventanaDeslizante01.pop();
                        }
                    }else if(direccion_host==2){
                        while(!ventanaDeslizante02.empty()){
                            ventanaDeslizante02.pop();
                        }
                    }else if(direccion_host==3){
                        while(!ventanaDeslizante03.empty()){
                            ventanaDeslizante03.pop();
                        }
                    }              

                    par("cant_tramasVentana").setLongValue(0);
                    par("tramas_no_asentidas").setLongValue(0);     

                    //Mandar un ACK,N al modulo de aplicación
                    cMessage *ack = new cMessage(FuncionesExtras::nombrando("ACK,",id_tramaVD));

                    par("correguido").setBoolValue(true);

                    //Actualiza el valor del ultimo ack pedido
                    int ult_ack_enviado = id_tramaVD;
                    par("ult_ack_enviado").setLongValue(ult_ack_enviado%tramasPorHost);

                    //Manda el mensaje a aplicacion
                    send(ack,"hacia_arriba");
                    ev << "Host " << direccion_host << ": " << "Mandando ACK al modulo de aplicación" << endl;
                }
            //Si el dataframe corresponde a una trama no-numerada (Unnumbered)
            }else{
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
                        dataframe->setName("UA,Trama Comando,");

                        //Seteando los valores necesarios para la trama UA
                        dataframe->createFrame(direccion_host);
                        dataframe->setName(FuncionesExtras::nombrando(dataframe->getName(),direccion_host));

                        //Enia el UA por el medio físico
                        send(dataframe,"hacia_abajo");
                        ev << "Host " << direccion_host << ": " << "Enviado Unnumbered UA a Host: " << direccion_dest << endl;
                    
                        en_respuesta_a = "NONE";
                        par("en_respuesta_a").setStringValue(en_respuesta_a);
                    //Se recibe un UP
                    }else if(M2 == 4){

                        int direccion = dataframe->getDestino();

                        if(direccion == direccion_host){
                            //Se responde con la última RR
                            int ult_trama_recibida = par("ult_trama_recibida");
                            ult_trama_recibida++;

                            dataframe->setName(FuncionesExtras::nombrando("RR,",ult_trama_recibida));

                            //Setea los valores necesarios para la trama RR
                            dataframe->createFrame(direccion_host);
                            dataframe->setName(FuncionesExtras::nombrando(dataframe->getName(),',',direccion_host));

                            //Actualiza el valor del ultimo RR enviado
                            par("ult_rr_enviado").setLongValue(ult_trama_recibida);

                            //Envia la trama por el medio fisico
                            send(dataframe,"hacia_abajo");
                            ev << "Host " << direccion_host << ": " << "Enviado Supervisory RR" << " a Host: " << direccion_dest << endl;
                        }else{
                            //Peloteo con MAC
                            //Se envia hacia abajo
                            send(dataframe,"hacia_abajo");
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
                            par("ult_ack_enviado").setLongValue(ult_ack_enviado%tramasPorHost);

                            send(ack,"hacia_arriba");
                            ev << "Host " << direccion_host << ": " << "Mandando ACK,0 al modulo de aplicación" << endl;

                        //Si la trama UA fue recibida por enviar DISC
                        }else if(en_respuesta_a == "DISC"){
                            en_respuesta_a = "NONE";
                            par("en_respuesta_a").setStringValue(en_respuesta_a);
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
                        dataframe->setName("UA,Trama Comando,");

                        //Fija los valores necesarios para la trama UA
                        dataframe->createFrame(direccion_host);
                        dataframe->setName(FuncionesExtras::nombrando(dataframe->getName(),direccion_host));

                        //Normal de conectado
                        if (ev.isGUI()){
                            getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->bubble("Conectado!");

                            int starter = par("starter");
                            int direccion_host = par("direccion_host");

                            //Si no estan ambos transmitiendo
                            if(starter != direccion_host && starter!= 2){
                                cMessage *connect = new cMessage("CONNECT");

                                send(connect,"hacia_arriba");
                            }
                        }

                        //Envia la trama por el canal físico
                        send(dataframe,"hacia_abajo");
                        ev << "Host " << direccion_host << ": " << "Enviado Unnumbered UA a Host: " << direccion_dest;
                    }
                }
            }
        }
    }          
}

//Cancela todos los eventos agendados
void llc::cancelarEventosPendientes(){
    int direccion_host = par("direccion_host");

    if(direccion_host==0){
        for(unsigned int i=0;i<temporizadores00.size();i++){
            cancelEvent(temporizadores00[i]); 
        }                        
        temporizadores00.clear();
    }else if(direccion_host==1){
        for(unsigned int i=0;i<temporizadores01.size();i++){
                cancelEvent(temporizadores01[i]);  
        }
        temporizadores01.clear();
    }else if(direccion_host==2){
        for(unsigned int i=0;i<temporizadores02.size();i++){
                cancelEvent(temporizadores02[i]);  
        }
        temporizadores02.clear();
    }else if(direccion_host==3){
        for(unsigned int i=0;i<temporizadores03.size();i++){
                cancelEvent(temporizadores03[i]);  
        }
        temporizadores03.clear();
    }
}

//Cancela hasta id_name
void llc::cancelarEventosPendientes(int id_name){
    int direccion_host = par("direccion_host");

    string tempo_name;
    if(direccion_host==0){
        for(unsigned int i=0;i<temporizadores00.size();i++){
            tempo_name = tempoNames00[i];
            if(FuncionesExtras::getValorId(tempo_name.c_str()) < id_name){
                cancelEvent(temporizadores00[i]);
            }    
        }                        
    }else if(direccion_host==1){
        for(unsigned int i=0;i<temporizadores01.size();i++){
            tempo_name = tempoNames01[i];
            if(FuncionesExtras::getValorId(tempo_name.c_str()) < id_name){
                cancelEvent(temporizadores01[i]);
            }    
        }
    }else if(direccion_host==2){
        for(unsigned int i=0;i<temporizadores02.size();i++){
            tempo_name = tempoNames02[i];
            if(FuncionesExtras::getValorId(tempo_name.c_str()) < id_name){
                cancelEvent(temporizadores02[i]);
            }    
        }
    }else if(direccion_host==3){
        for(unsigned int i=0;i<temporizadores03.size();i++){
            tempo_name = tempoNames03[i];
            if(FuncionesExtras::getValorId(tempo_name.c_str()) < id_name){
                cancelEvent(temporizadores03[i]);
            }    
        }
    }
}
