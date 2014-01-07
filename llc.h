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

#ifndef LLC_H_
#define LLC_H_

class llc : public cSimpleModule{
    protected:
    	//Inicializar módulo
        virtual void initialize();

        //Procesador de mensaje desde Intermedio
        virtual void processMsgFromHigherLayer(cMessage *dato);

        //Procesador de Mensaje desde el otro host
        virtual void processMsgFromLowerLayer(cMessage *packet);

        //Maneja los mensajes llegados al modulo
        virtual void handleMessage(cMessage *msg);        

        //Cancela todos los eventos pendientes dados por scheduleAt
        virtual void cancelarEventosPendientes();

        //Cancela los eventos pendientes dados por scheduleAt hasta id_name
        virtual void cancelarEventosPendientes(int id_name);
};


#endif /* LLC_H_ */





