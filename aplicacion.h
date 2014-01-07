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

class aplicacion : public cSimpleModule{
    protected:
        //Inicializar módulo
        virtual void initialize();

        //Maneja los menasjes recibidos "desde_abajo"
        virtual void handleMessage(cMessage *msg);

        //Para despues de verificar las variables
        virtual void postInitialize();

        //Generador de la información
        virtual void generaInfo(int trama_id);

        //Retornar verdadero si el DATO dado por valor_id pertenece al buffer
        virtual bool isInBuffer(int valor_id);
};


#endif /* APLICACION_H_ */
