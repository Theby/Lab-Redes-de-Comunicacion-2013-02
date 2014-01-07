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
// Definición del módulo
//
module sistema
{
    //Parámetros
    parameters:
        volatile int starter = default(0);
        volatile int numTramas;
        volatile int tamVentana;
        volatile int tramas_libres;
        volatile int error;
        volatile int tamTrama = default(4);

        @display("i=old/ring;is=n");
    //Submódulos que lo componen
    submodules:
        //Definición del host 1
        Host_0: host {
            //Parámetros
            parameters:
                tamTrama = tamTrama;
                numTramas_env = numTramas;
                numTramas_rec = numTramas;
                tamVentana = tamVentana;
                starter = starter;
                tramas_libres = tramas_libres;
                error = error;
                //Se suministra la dirección que le corresponde al host y al destino
                direccion_host = 0;
                direccion_dest = 1;
                @display("p=70,50");
        }
        //Definición del host 2
        Host_1: host {
            //Parámetros
            parameters:
                tamTrama = tamTrama;
                numTramas_env = numTramas;
                numTramas_rec = numTramas;
                tamVentana = tamVentana;
                starter = starter;
                tramas_libres = tramas_libres;
                error = error;
                //Se suministra la dirección que le corresponde al host y al destino
                direccion_host = 1;
                direccion_dest = 2;
                @display("p=170,50");
        }
        //Definición del host 3
        Host_2: host {
            //Parámetros
            parameters:
                tamTrama = tamTrama;
                numTramas_env = numTramas;
                numTramas_rec = numTramas;
                tamVentana = tamVentana;
                starter = starter;
                tramas_libres = tramas_libres;
                error = error;
                //Se suministra la dirección que le corresponde al host y al destino
                direccion_host = 2;
                direccion_dest = 3;
                @display("p=170,150");
        }
        //Definición del host 4
        Host_3: host {
            //Parámetros
            parameters:
                tamTrama = tamTrama;
                numTramas_env = numTramas;
                numTramas_rec = numTramas;
                tamVentana = tamVentana;
                starter = starter;
                tramas_libres = tramas_libres;
                error = error;
                //Se suministra la dirección que le corresponde al host y al destino
                direccion_host = 3;
                direccion_dest = 0;
                @display("p=70,150");
        }
    //Conexiones de la red
    connections:
        //Desde el Host 1 hacia el Host 2
        Host_0.salida --> canal --> Host_1.entrada;
        //Desde el Host 2 hacia el Host 3
        Host_1.salida --> canal --> Host_2.entrada;
        //Desde el Host 3 hacia el Host 4
        Host_2.salida --> canal --> Host_3.entrada;
        //Desde el Host 4 hacia el Host 0
        Host_3.salida --> canal --> Host_0.entrada;
}

//Definición del canal de transmisión
channel canal extends ned.DatarateChannel
{
    //Retardo de propagación
    delay = 100ms;
    //Velocidad de datos
    datarate = 128000bps;
}

//
// Instanciación de la red
//
network red extends sistema
{
    //Parámetros
    parameters:
        numTramas @prompt("Ingrese el máximo número de tramas que pueden enviar los hosts") = default(1);
        tamVentana @prompt("Tamaño de las ventanas deslizantes: ") = default(1);
        tramas_libres @prompt("Número de tramas a enviar/recibir antes de recibir un Asentimiento RR") = default(1);
        error @prompt("Ingrese cual es la probabilidad de que la trama posea un error (de 0 a 100)") = default(0);
        @display("bgi=background/green,t");
}
