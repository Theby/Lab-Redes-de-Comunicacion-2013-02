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

#ifndef ENLACE_H_
#define ENLACE_H_

class enlace : public cSimpleModule
{
    protected:
    	//Inicializar módulo
        virtual void initialize();

        //Procesador de mensaje desde Intermedio
        virtual void processMsgFromHigherLayer(cMessage *dato);

        //Procesador de Mensaje desde el otro host
        virtual void processMsgFromLowerLayer(cMessage *packet);

        virtual void handleMessage(cMessage *msg);

        virtual void sending(cMessage* msg, const char* ngate);

};


#endif /* ENLACE_H_ */





