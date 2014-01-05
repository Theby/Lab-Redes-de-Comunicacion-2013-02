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

std::vector<Informacion*> buffer00;
std::vector<Informacion*> buffer01;

//Define la clase para trabajar directamente con el modulo de aplicacion
Define_Module( aplicacion );

//Valida los atributos
void aplicacion::initialize(){
    int tamTrama = par("tamTrama");
    int direccion_dest = par("direccion_dest");
    int direccion_host = par("direccion_host");
    int numTramas_env = par("numTramas_env");
    int numTramas_rec = par("numTramas_rec");
    int starter = par("starter");

    bool not_valid_enviroment = false;

    if(tamTrama < 0){
        opp_error("\nLa el tamaño no puede ser negativo.\n");
        not_valid_enviroment = true;
    }

    if(direccion_dest < 0){
        opp_error("\nLa direccion de destino no puede ser negativa.\n");
        not_valid_enviroment = true;        
    }

    if(direccion_host < 0){
        opp_error("\nLa direccion del host no puede ser negativo.\n");
        not_valid_enviroment = true;        
    }

    if(starter==direccion_host){
        if(numTramas_env < 0){
            opp_error("\nLa cantidad de tramas no puede ser negativa.\n");
            not_valid_enviroment = true;        
        }
        
        if(numTramas_rec != 0){
            opp_error("\nLa cantidad de tramas debe ser 0 si solo el host 0 envia datos.\n");
            not_valid_enviroment = true;
        }
    }else if(starter==2){
        if(numTramas_env < 0){
            opp_error("\nLa cantidad de tramas no puede ser negativa.\n");
            not_valid_enviroment = true;        
        }
        
        if(numTramas_rec < 0){
            opp_error("\nLa cantidad de tramas no puede ser negativa.\n");
            not_valid_enviroment = true;
        }
    }
    
    if(starter < 0 || starter > 2){
        opp_error("\nEl selector de hosts no puede ser negativo o mayor a 2.\n");
        not_valid_enviroment = true;        
    }

    if(not_valid_enviroment){
        endSimulation();
    }else{
        postInitialize();
    }
}

//Inicializa el sistema
void aplicacion::postInitialize(){
    //obtiene el valor de master para saber quien parte
    int master = par("starter");

    //obtiene el valor del host en el que se ubica
    int address_host = par("direccion_host");

    if(address_host==0){
        buffer00.clear();
    }else if(address_host==1){
        buffer01.clear();
    }

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

//Maneja el mensaje recibido
void aplicacion::handleMessage(cMessage* msg){
    //Obtiene la direccion del host del modulo
    int nombreHost = par("direccion_host");
    string msg_name = msg->getName();    

    //Si el mensaje ha llegado desde intermedio
    if (msg->arrivedOn("desde_abajo")){

        //Es ACK
        if(msg_name[0] == 'A' && msg_name[1] == 'C' && msg_name[2] == 'K'){
            delete msg;
            ev << "Host " << nombreHost << ": " << "El mensaje: " << msg_name << " fue correctamente recibido." << endl;            

            string estado = par("estado");

            if(estado != "END"){
                //Normal de activo
                if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

                //Mensaje de conectado
                if(msg_name[4] == '0'){
                    if (ev.isGUI()) bubble("Conectado!");
                }

                //valor N del ACK,N recibido
                int valor_id = FuncionesExtras::getValorId(msg_name.c_str());

                //modo de transmisión
                int starter = par("starter");

                //Numero maximo de tramas a enviar
                int numTramas_env = par("numTramas_env");

                //Obtiene el numero maximo de tramas a recibir
                int numTramas_rec = par("numTramas_rec");

                //Numero de tramas enviadas
                int contTramas_env = par("contTramas_env");

                //Numero de tramas recibidas
                int contTramas_rec = par("contTramas_rec");

                //Solo si ambos host están transmitiendo
                if(starter == 2){
                    /*
                     * Si se ha recibido el último dato y recibe un pedido superior a las tramas que tiene, se desconecta
                     * Analogo a si ya no se pueden enviar más tramas y se ha recibido el último dato
                     * Siempre se genera solo uno de estos casos (controlado por @estado)
                     */ 
                    if(numTramas_env == contTramas_env && numTramas_rec == contTramas_rec){
                        //Negro de desconectado
                        if (ev.isGUI()){
                            getDisplayString().setTagArg("i",1,"black");
                            bubble("Desconectado!");
                        }

                        //creando un mensaje END
                        cMessage *end = new cMessage("END");

                        par("estado").setStringValue("END");

                        //Enviando el mensaje a Enlace
                        send(end, "hacia_abajo");
                        ev << "Host " << nombreHost << ": " << "Enviando mensaje END a Enlace" << endl;
                    }else if(contTramas_env < numTramas_env){
                        //Si el DATO,valor_id no ha sido enviado aún
                        if(!isInBuffer(valor_id)){
                            //Comienza a generar informacion
                            generaInfo(FuncionesExtras::getValorId(msg_name.c_str()));

                        //Si ya fue enviado con anterioridad                    
                        }else{
                            //Se envia una trama "nueva" con la información ya guardada con anterioridad
                            int address_dest = par("direccion_dest");

                            Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrando("DATO,",valor_id));

                            //Asignando valores correspondientes
                            if(nombreHost==0){
                                tramaComunicacion->createFrame(address_dest,buffer00[valor_id]->getInformacion(),buffer00[valor_id]->getInformacionArraySize());
                            }else if(nombreHost==1){
                                tramaComunicacion->createFrame(address_dest,buffer01[valor_id]->getInformacion(),buffer01[valor_id]->getInformacionArraySize());
                            }

                            //Se envia el mensaje a intermedio
                            send(tramaComunicacion, "hacia_abajo");
                        }
                    }

                //Si solo un host esta transmitiendo
                }else{
                    //Esta condición ocurre cuando se recibe un ACK,numTramas_env+1, luego del último RR del otro host
                    if(numTramas_env == contTramas_env && valor_id == numTramas_env){
                        //Se responde con un END

                        //Negro de desconectado
                        if (ev.isGUI()){
                            getDisplayString().setTagArg("i",1,"black");
                            bubble("Desconectado!");
                        }

                        //creando un mensaje END
                        cMessage *end = new cMessage("END");

                        par("estado").setStringValue("END");

                        //Enviando el mensaje a Enlace
                        send(end, "hacia_abajo");
                        ev << "Host " << nombreHost << ": " << "Enviando mensaje END a Enlace" << endl;
                    }else{
                        //Si el DATO,valor_id no ha sido enviado aún
                        if(!isInBuffer(valor_id)){
                            //Comienza a generar informacion
                            generaInfo(FuncionesExtras::getValorId(msg_name.c_str()));

                        //Si ya fue enviado con anterioridad                    
                        }else{
                            //Se envia una trama "nueva" con la información ya guardada con anterioridad
                            int address_dest = par("direccion_dest");

                            Informacion *tramaComunicacion = new Informacion(FuncionesExtras::nombrando("DATO,",valor_id));

                            //Asignando valores correspondientes
                            if(nombreHost==0){
                                tramaComunicacion->createFrame(address_dest,buffer00[valor_id]->getInformacion(),buffer00[valor_id]->getInformacionArraySize());
                            }else if(nombreHost==1){
                                tramaComunicacion->createFrame(address_dest,buffer01[valor_id]->getInformacion(),buffer01[valor_id]->getInformacionArraySize());
                            }

                            //Se envia el mensaje a intermedio
                            send(tramaComunicacion, "hacia_abajo");
                        }
                    }
                }
            }

        //Es DATO
        }else if(msg_name[0] == 'D' && msg_name[1] == 'A' && msg_name[2] == 'T' && msg_name[3] == 'O'){
            delete msg;
            string estado = par("estado");

            if(estado != "END"){
                //Normal de activo
                if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

                //Actualiza el valor de tramas recibidas
                int contTramas_rec = par("contTramas_rec");
                contTramas_rec++;
                par("contTramas_rec").setLongValue(contTramas_rec);

                ev << "Host " << nombreHost << ": " << "El dato: " << msg_name << " fue correctamente recibido." << endl;

                //modo de transmisión
                int starter = par("starter");

                //Solo si ambos host están transmitiendo
                if(starter == 2){
                    //Numero maximo de tramas a enviar
                    int numTramas_env = par("numTramas_env");

                    //Obtiene el numero maximo de tramas a recibir
                    int numTramas_rec = par("numTramas_rec");

                    //Numero de tramas enviadas
                    int contTramas_env = par("contTramas_env");

                    //Numero de tramas recibidas
                    int contTramas_rec = par("contTramas_rec");

                    /*
                     * Si ya no se pueden enviar más tramas y se ha recibido el último dato, se desconecta
                     * Analogo a si se ha recibido el último dato y recibe un pedido superior a las tramas que tiene
                     * Siempre se genera solo uno de estos casos (controlado por @estado)
                     */ 
                    if(numTramas_env == contTramas_env && numTramas_rec == contTramas_rec && numTramas_env != numTramas_rec){
                        //Negro de desconectado
                        if (ev.isGUI()){
                            getDisplayString().setTagArg("i",1,"black");
                            bubble("Desconectado!");
                        }

                        //creando un mensaje END
                        cMessage *end = new cMessage("END");

                        par("estado").setStringValue("END");

                        //Enviando el mensaje a Enlace
                        send(end, "hacia_abajo");
                        ev << "Host " << nombreHost << ": " << "Enviando mensaje END a Enlace" << endl;
                    }

                }
            }

        //Es Connect
        }else if(msg_name == "CONNECT"){
            delete msg;

            //Normal de activo
            if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

            //Mensaje de conectado
            if (ev.isGUI()) bubble("Conectado!");            
       
        //Es Disconnect
        }else if(msg_name == "DISCONNECT"){
            delete msg;

            //Normal de activo
            if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

            if (ev.isGUI()){
                //Negro de desconectado
                getDisplayString().setTagArg("i",1,"black");
                bubble("Desconectado!");
            }            
        
        //Es Fin
        }else if(msg_name == "FIN"){
            delete msg;

            //Negro de desconectado
            if (ev.isGUI()){
                getDisplayString().setTagArg("i",1,"black");
                bubble("Desconectado!");
            }

            //creando un mensaje END
            cMessage *end = new cMessage("END");

            par("estado").setStringValue("END");

            //Enviando el mensaje a Enlace
            send(end, "hacia_abajo");
            ev << "Host " << nombreHost << ": " << "Enviando mensaje END a Enlace" << endl;

        //Desconocido
        }else{
            delete msg;

            //Normal de activo
            if (ev.isGUI()) getDisplayString().setTagArg("i",1,"");

            ev << "Host " << nombreHost << ": " << "El mensaje: " << msg_name << " no es valido y ha sido eliminado" << endl;
            
        }
    }else{
        delete msg;

        //Normal de activo
        if (ev.isGUI()) getDisplayString().setTagArg("i",1,""); 

        ev << "Host " << nombreHost << ": " << "Mensaje llegado desde un lugar desconocido" << endl;
        
    }
}

//Genera tramas de Informacion
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
        //Para guardar las tramas que se envian y reciben, así como el modo de transmisión
        //Numero maximo de tramas a enviar
        int numTramas_env = par("numTramas_env");

        //Obtiene el numero maximo de tramas a recibir
        int numTramas_rec = par("numTramas_rec");

        //Numero de tramas enviadas
        int contTramas_env = par("contTramas_env");

        //Numero de tramas recibidas
        int contTramas_rec = par("contTramas_rec");

        ev << "Host " << nombreHost << ": " << "Tramas recibidas: " << contTramas_rec << " de " << numTramas_rec << endl;
        if(contTramas_env < numTramas_env){
            ev << "Host " << nombreHost << ": " << "Tramas enviadas: " << contTramas_env << " de " << numTramas_env << endl;
        }else{
            ev << "Host " << nombreHost << ": " << "Enviadas todas las tramas" << endl;
        }
        
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
        Informacion *copiaBuffer = new Informacion(FuncionesExtras::nombrando("DATO,",trama_id));

        //Asignando valores correspondientes
        tramaComunicacion->createFrame(address_dest,informacionRand,tamTrama);
        copiaBuffer->createFrame(address_dest,informacionRand,tamTrama);

        if(nombreHost==0){
            buffer00.push_back(copiaBuffer);
        }else if(nombreHost==1){
            buffer01.push_back(copiaBuffer);
        }

        //Actualiza el valor de tramas enviadas
        contTramas_env = par("contTramas_env");
        contTramas_env ++;
        par("contTramas_env").setLongValue(contTramas_env);

        //Se envia el mensaje a intermedio
        send(tramaComunicacion, "hacia_abajo");

        //Informando al Usuario el dato enviado
        ev << "Host " << nombreHost << ": " << "Enviado trama " << trama_id << " al modulo de enlace " << address_dest;
    }
}

//Revisa si el ACK llegado corresponde a una trama ya enviada
bool aplicacion::isInBuffer(int valor_id){
    int nombreHost = par("direccion_host");
    string nombre;

    if(nombreHost==0){
        for(unsigned int i=0;i<buffer00.size();i++){
            nombre = buffer00[i]->getName();
            if(valor_id == FuncionesExtras::getValorId(nombre.c_str())){
                return true;
            }
        }
    }else if(nombreHost==1){
        for(unsigned int i=0;i<buffer01.size();i++){
            nombre = buffer01[i]->getName();
            if(valor_id == FuncionesExtras::getValorId(nombre.c_str())){
                return true;
            }
        }
    }

    return false;
}

