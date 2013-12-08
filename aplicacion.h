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

#ifndef APLICACION_H_
#define APLICACION_H_

class aplicacion : public ::cSimpleModule
{
    public:
        //Inicializar módulo
        virtual void initialize();

        //Maneja los menasjes recibidos "desde_abajo"
        virtual void handleMessage(cMessage *msg);

        //Generador de la información
        virtual void generaInfo();
};


#endif /* APLICACION_H_ */
