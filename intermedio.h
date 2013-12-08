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

#ifndef INTERMEDIO_H_
#define INTERMEDIO_H_

class intermedio : public cSimpleModule
{
    protected:
        //Procesador de mensajes desde aplicacion
        virtual void processMsgFromHigherLayer(cMessage *packet);

        //Procesador de mensajes hacia enlace
        virtual void processMsgFromLowerLayer(cMessage *packet);

        virtual void handleMessage(cMessage* msg);
};


#endif /* INTERMEDIO_H_ */
