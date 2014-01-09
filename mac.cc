//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <stdlib.h>
#include <string.h>
#include <queue>
#include <omnetpp.h>

#include <DataFrame_m.h>
#include <Informacion_m.h>
#include <mac.h>

#include <FuncionesExtras.h>

using namespace std;

//Define la clase para trabajar directamente con el modulo de MAC
Define_Module( mac );

//Colas para las tramas que han llegado cuando no se es holder
std::queue<DataFrame*> colaDeTramas00;
std::queue<DataFrame*> colaDeTramas01;
std::queue<DataFrame*> colaDeTramas02;
std::queue<DataFrame*> colaDeTramas03;

std::queue<DataFrame*> canalLibre00;
std::queue<DataFrame*> canalLibre01;
std::queue<DataFrame*> canalLibre02;
std::queue<DataFrame*> canalLibre03;

//Solo usado para cuando se mandan mensajes atravez del canal físico
void mac::sending(cMessage* msg, const char* ngate){
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
        }else if(nombreHost==2){
            canalLibre02.push(mensaje);
        }else if(nombreHost==3){
            canalLibre03.push(mensaje);
        }

        //Deja esperando un mensaje para avisar cuando se liberará el canal
        scheduleAt(canalFinishTime, canalLibre);
    }
}

//Valida las variables
void mac::initialize(){
    bool not_valid_enviroment = false;

    while(!colaDeTramas00.empty()){
        colaDeTramas00.pop();
    }

    while(!colaDeTramas01.empty()){
        colaDeTramas01.pop();
    }

    while(!colaDeTramas02.empty()){
        colaDeTramas02.pop();
    }

    while(!colaDeTramas03.empty()){
        colaDeTramas03.pop();
    }

    while(!canalLibre00.empty()){
        canalLibre00.pop();
    }

	while(!canalLibre01.empty()){
        canalLibre01.pop();
    }

	while(!canalLibre02.empty()){
        canalLibre02.pop();
    }

	while(!canalLibre03.empty()){
        canalLibre03.pop();
    }

    if(not_valid_enviroment){
        endSimulation();
    }
}

//Maneja el mensaje ingresado
void mac::handleMessage(cMessage *msg){
	string name = msg->getName();
    bool holder = par("holder");

	if(name != "FREE"){
	    //Si el mensaje llega desde el otro Host
	    if (msg->arrivedOn("desde_fisico")){
	        //Procesarlo como si viniera desde el otro host
	        processMsgFromLowerLayer(msg);

	    //Sino, el mensaje viene desde LLC    
	    }else{
	    	if(name[0]=='U' && name[1]=='P' && !holder){
	    		//Procesarlo como si viniera desde el otro host
	        	processMsgFromLowerLayer(msg);
	    	}else{
		        //Procesarlo como si viniera desde LLC
		        processMsgFromHigherLayer(msg);
		    }
	    }
	//Corresponde enviar un mensaje de la cola
    }else{
        int direccion_host = par("direccion_host");

        if(direccion_host==0){
            sending(canalLibre00.front(),"hacia_fisico");
            canalLibre00.pop();
        }else if(direccion_host==1){
            sending(canalLibre01.front(),"hacia_fisico");
            canalLibre01.pop();
        }else if(direccion_host==2){
            sending(canalLibre02.front(),"hacia_fisico");
            canalLibre02.pop();
        }else if(direccion_host==3){
            sending(canalLibre03.front(),"hacia_fisico");
            canalLibre03.pop();
        }
    }
}

void mac::processMsgFromHigherLayer(cMessage *dato){
	string packet_name = dato->getName();
    bool holder = par("holder");

	//Si es un mensaje "START"
	if(packet_name == "START"){
		delete dato;

		//Crea el testigo como un cMessage
		DataFrame *testigo = new DataFrame("TESTIGO");

		//Mensajes de recibido
		if (ev.isGUI()){
			getParentModule()->getParentModule()->bubble("Conectado y Testigo enviado");
			getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"");
			getParentModule()->getDisplayString().setTagArg("i",1,"");
			getParentModule()->bubble("Conectado!");
			getDisplayString().setTagArg("i",1,"");
			bubble("Conectado!");
		}

		//lo envia al anillo
		sending(testigo,"hacia_fisico");
        par("holder").setBoolValue(false);
	//Si recibe cualquier otro tipo de trama
	}else if(packet_name == "TESTIGO"){
        if(holder){
    		//Envia el testigo peloteado
    		bool conectado = par("conectado");
    		if(conectado){
    			if(ev.isGUI()){
    				getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"");
    			 	getParentModule()->getParentModule()->bubble("Testigo enviado");
    			} 
    		}
    		sending(dato,"hacia_fisico");
            par("holder").setBoolValue(false);
        }
	//Peloteo de ACK
	}else if(packet_name[0]=='A' && packet_name[1]=='C' && packet_name[2]=='K'){
		//Se lo pelotea de vuelta para mantener una visual más amigable
		send(dato,"hacia_arriba");
	}else if(packet_name == "END"){
		DataFrame *testigo = new DataFrame("TESTIGO");
		send(testigo,"hacia_arriba");
	//UA de desconexión
	}else if(packet_name[0]=='U' && packet_name[1]=='A'){
		//Se desconecta, deja cant_holder en 3, manda la trama UA, manda el testigo y si lo recibe denuevo lo destruye
		if(ev.isGUI()){
			getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"black");
            getParentModule()->getParentModule()->bubble("Desconectado");
            getParentModule()->getDisplayString().setTagArg("i",1,"black");
            getParentModule()->bubble("Desconectado");
            getDisplayString().setTagArg("i",1,"black");
            bubble("Desconectado");
        }

        par("conectado").setBoolValue(false);

        sending(dato,"hacia_fisico");
        DataFrame *testigo = new DataFrame("TESTIGO");
		send(testigo,"hacia_arriba");
		par("cant_holder").setLongValue(3);
	//Sí es una trama LOST
	}else if(packet_name[0]=='L'){
		//Significa que es una trama con el error de "perdido", por lo que se suelta el testigo sin enviar la trama
		delete dato;

		DataFrame *testigo = new DataFrame("TESTIGO");		
		//Pelota el testigo consigo mismo para crear un interfaz más agradable y que el testigo no salga junto a la trama enviada
		send(testigo,"hacia_arriba");
	}else{

        if(packet_name[0] == 'I'){
            DataFrame *dataframe = check_and_cast<DataFrame *>(dato);
            int direccion = FuncionesExtras::bitArrayToInt(dataframe->getAddress(),8);

            par("direccion_dest").setLongValue(direccion);
        }

		//Lo envia hacia fisico y se pelotea testigo con LLC
		sending(dato,"hacia_fisico");

		DataFrame *testigo = new DataFrame("TESTIGO");		
		//Pelota el testigo consigo mismo para crear un interfaz más agradable y que el testigo no salga junto a la trama enviada
		send(testigo,"hacia_arriba");
	}
}

void mac::processMsgFromLowerLayer(cMessage *dato){
	string packet_name = dato->getName();
	int direccion_host = par("direccion_host");
	int direccion_dest = par("direccion_dest");
	int cant_holder = par("cant_holder");
	bool desconectar = par("desconectar");

	//Si recibe "TESTIGO" desde el medio fisico
	if(packet_name == "TESTIGO" && (cant_holder < 3 || !desconectar)){
		delete dato;
		par("holder").setBoolValue(true);
		cant_holder++;
		par("cant_holder").setLongValue(cant_holder);

		//Da le mensaje que corresponde al recibir el testigo
		if(ev.isGUI()){
            getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"gold");
            getParentModule()->getParentModule()->bubble("Testigo recibido!");
        }

        bool conectado = par("conectado");
        if(!conectado){
            getDisplayString().setTagArg("i",1,"");
        	bubble("Conectado!");
        	par("conectado").setBoolValue(true);

        	//Envia "START" para iniciar comunicaciones en este host
        	cMessage *start = new cMessage("START");
        	send(start,"hacia_arriba");
        }else{
        	//Hace uso de las tramas que le llegaron mientras no era holder
        	//Manda solo una trama hacia arriba y la saca de la cola
        	if(direccion_host==0){
        		if(!colaDeTramas00.empty()){
                    send(colaDeTramas00.front(),"hacia_arriba");	                
	                colaDeTramas00.pop();
            	}else if(cant_holder == 2 && desconectar){
            		if(!desconectar){
            			DataFrame *testigo = new DataFrame("TESTIGO");
            			send(testigo,"hacia_arriba");
            		}else{
	            		//Manda un "END" a LLC para terminar comunicacines
	            		cMessage *end = new cMessage("END");
	            		send(end,"hacia_arriba");
	            	}
            	}else{
            		//Suelta el testigo
                    bool conectado = par("conectado");
                    if(conectado){
                        if(ev.isGUI()){
                            getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->getParentModule()->bubble("Testigo enviado");
                        } 
                    }
                    DataFrame *testigo = new DataFrame("TESTIGO");
                    sending(testigo,"hacia_fisico");
                    par("holder").setBoolValue(false);
            	}
            }else if(direccion_host==1){
                if(!colaDeTramas01.empty()){
                    send(colaDeTramas01.front(),"hacia_arriba");
	                colaDeTramas01.pop();
            	}else if(cant_holder == 2 && desconectar){
            		if(!desconectar){
            			DataFrame *testigo = new DataFrame("TESTIGO");
            			send(testigo,"hacia_arriba");
            		}else{
	            		//Manda un "END" a LLC para terminar comunicacines
	            		cMessage *end = new cMessage("END");
	            		send(end,"hacia_arriba");
	            	}
            	}else{
            		//Suelta el testigo
                    bool conectado = par("conectado");
                    if(conectado){
                        if(ev.isGUI()){
                            getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->getParentModule()->bubble("Testigo enviado");
                        } 
                    }
                    DataFrame *testigo = new DataFrame("TESTIGO");
                    sending(testigo,"hacia_fisico");
                    par("holder").setBoolValue(false);
            	}
            }else if(direccion_host==2){
                if(!colaDeTramas02.empty()){
                    send(colaDeTramas02.front(),"hacia_arriba");
                    colaDeTramas02.pop();
            	}else if(cant_holder == 2 && desconectar){
            		if(!desconectar){
            			DataFrame *testigo = new DataFrame("TESTIGO");
            			send(testigo,"hacia_arriba");
            		}else{
	            		//Manda un "END" a LLC para terminar comunicacines
	            		cMessage *end = new cMessage("END");
	            		send(end,"hacia_arriba");
	            	}
            	}else{
            		//Suelta el testigo
                    bool conectado = par("conectado");
                    if(conectado){
                        if(ev.isGUI()){
                            getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->getParentModule()->bubble("Testigo enviado");
                        } 
                    }
                    DataFrame *testigo = new DataFrame("TESTIGO");
                    sending(testigo,"hacia_fisico");
                    par("holder").setBoolValue(false);
            	}
            }else if(direccion_host==3){
                if(!colaDeTramas03.empty()){
                    send(colaDeTramas03.front(),"hacia_arriba");
	                colaDeTramas03.pop();
            	}else if(cant_holder == 2 && desconectar){
            		if(!desconectar){
            			DataFrame *testigo = new DataFrame("TESTIGO");
            			send(testigo,"hacia_arriba");
            		}else{
	            		//Manda un "END" a LLC para terminar comunicacines
	            		cMessage *end = new cMessage("END");
	            		send(end,"hacia_arriba");
	            	}
            	}else{
            		//Suelta el testigo
                    bool conectado = par("conectado");
                    if(conectado){
                        if(ev.isGUI()){
                            getParentModule()->getParentModule()->getDisplayString().setTagArg("i",1,"");
                            getParentModule()->getParentModule()->bubble("Testigo enviado");
                        } 
                    }
                    DataFrame *testigo = new DataFrame("TESTIGO");
                    sending(testigo,"hacia_fisico");
                    par("holder").setBoolValue(false);
            	}
            }
        }
	}else if(cant_holder == 3 && desconectar){
		//Borra el "TESTIGO"
		delete dato;
	}else{
		//Le ha llegado cualquier otra trama por lo que debe guardarse si es suya o redirigirla si es para otro
		DataFrame *dataframe = check_and_cast<DataFrame *>(dato);

		int direccion = FuncionesExtras::bitArrayToInt(dataframe->getAddress(),8);
		if(packet_name[0]!='U' && desconectar){
			par("cant_holder").setLongValue(0);
		}

		//Si corresponde lo guarda para esperar al token
		if(direccion == direccion_host || (direccion == direccion_dest && dataframe->getControl(0) == 1)){
			//Se almacena la trama en la cola
            if(direccion_host==0){
                colaDeTramas00.push(dataframe);
            }else if(direccion_host==1){
                colaDeTramas01.push(dataframe);
            }else if(direccion_host==2){
                colaDeTramas02.push(dataframe);
            }else if(direccion_host==3){
                colaDeTramas03.push(dataframe);
            }

        //Si no corresponde al host lo vuelve a poner en el canal
		}else{
			sending(dataframe,"hacia_fisico");
		}
	}
}
