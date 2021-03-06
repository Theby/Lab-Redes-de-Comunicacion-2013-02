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

//
// TODO auto-generated type
//
module enlace
{
    //Parámetros del nivel de Enlace
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
        volatile int timer = default(10);

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

        @display("i=block/ifcard,black;is=n;bgb=130,246;bgi=background/terrain");
    gates:
        //Con el nivel inferior
        input desde_fisico;
        output hacia_fisico;

        //con el nivel superior
        input desde_arriba;
        output hacia_arriba;

    submodules:
        //Llamado al nivel de aplicación
        Modulo_LLC: llc {
            //parámetros del nivel
            parameters:
                tamVentana = tamVentana;
                direccion_host = direccion_host;
                direccion_dest = direccion_dest;
                tramas_libres = tramas_libres;
                starter = starter;
                numTramas_env = numTramas_env;
                numTramas_rec = numTramas_rec;
                error = error;
                @display("p=67,48");
        }

        //Llamado al nivel intermedio
        Modulo_MAC: mac {
            //parámetros del nivel
            parameters:
                direccion_host = direccion_host;
                direccion_dest = direccion_dest;
                @display("p=67,155");
        }

    connections:
        //Desde LLC hacia MAC
        Modulo_LLC.hacia_abajo --> Modulo_MAC.desde_arriba;
        //Desde LLC hacia intermedio
        Modulo_LLC.hacia_arriba --> hacia_arriba;
        //Desde MAC hacia el otro host
        Modulo_MAC.hacia_fisico --> hacia_fisico;
        //Desde MAC hacia LLC
        Modulo_MAC.hacia_arriba --> Modulo_LLC.desde_abajo;
        //Desde intermedio a LLC
        desde_arriba --> Modulo_LLC.desde_arriba;
        //Desde el otro host a MAC
        desde_fisico --> Modulo_MAC.desde_fisico;

}
