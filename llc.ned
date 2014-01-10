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

package laboratorio_01_02;

simple llc
{
    //Parámetros del nivel de LLC
    parameters:
        //Dirección de Host
        int direccion_host;

		//Dirección de Destino
        int direccion_dest;

        //número de tramas que envia
        int numTramas_env;

        //número de tramas que recibe
        int numTramas_rec;

        //Tamaño de ventana
        int tamVentana;

        //Cantidad de tramas en la ventana
        volatile int cant_tramasVentana = default(0);

        //Numero de tramas libres
        int tramas_libres;

        //define cual es la probabilidad de error
        int error;

        //Define cual host(0 o 1) comienza la comunicación
        int starter;

        //Timer para re-enviarse mensajes con scheduleAt();
        volatile int timer = default(3);

        //numero de tramas no asentidas
        volatile int tramas_no_asentidas = default(0);

        //ultima trama recibida
        volatile int ult_trama_recibida = default(-1);

        //Para guardar el valor del ultimo ACK enviado
        volatile int ult_ack_enviado = default(0);

        //Para guardar el valor del ultimo ACK enviado
        volatile int ult_rr_enviado = default(0);

        //Para saber el estado del nivel de enlace
        volatile string en_respuesta_a = default("NONE");

        //Numero de la trama enviada con error
        volatile int trama_con_error = default(0);
        
        //Para saber si esta conectado o no
        volatile bool conectado = default(false);
        
        //Para fijar la direccion actual tras recibir un ACK de aplicacion
        volatile int direccion_actual = default(0);
        
        //Para saber si es una trama correguida o una nueva
        volatile bool correguido = default(false);

        @display("i=block/uparrow,black;is=n");

    gates:
        //Con el nivel inferior
        input desde_abajo;
        output hacia_abajo;

        //con el nivel superior
        input desde_arriba;
        output hacia_arriba;
}
