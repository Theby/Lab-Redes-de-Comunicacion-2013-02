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
        volatile int tamTrama;
        volatile int numTramas;
        volatile int tamVentana;
        volatile int starter;       
        volatile int tramas_libres;
        volatile int error;

        @display("i=device/lan;is=n");
    //Submódulos que lo componen
    submodules:
        //Definición del host 1
        Host_0: host {
            //Parámetros
            parameters:
                tamTrama = tamTrama;
                numTramas = numTramas;
                tamVentana = tamVentana;
                starter = starter;                
                tramas_libres = tramas_libres;
                error = error;
                //Se suministra la dirección que le corresponde al host y al destino
                direccion_host = 00000000;
                direccion_dest = 00000001;
                @display("p=70,50");
        }
        //Definición del host 2
        Host_1: host {
            //Parámetros
            parameters:
                tamTrama = tamTrama;
                numTramas = numTramas;
                tamVentana = tamVentana;
                starter = starter;
                tramas_libres = tramas_libres;
                error = error;
                //Se suministra la dirección que le corresponde al host y al destino
                direccion_host = 00000001;
                direccion_dest = 00000000;
                @display("p=170,50");
        }
    //Conexiones de la red
    connections:
        //Desde el Host 1 hacia el Host 2
        Host_0.salida --> canal --> Host_1.entrada;
        //Desde el Host 2 hacia el Host 1
        Host_1.salida --> canal --> Host_0.entrada;
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
        numTramas @prompt("Ingrese el máximo número de tramas que puede enviar un host") = default(1);
        tamTrama @prompt("Ingrese el largo en bits que tendrá la información enviada por cada Trama") = default(4);
        tramas_libres @prompt("Ingrese el número de tramas que se enviarán antes de recibir un Asentimiento RR") = default(1);
        tamVentana @prompt("Ingrese el número máximo de tramas por ventana") = default(5);
        starter @prompt("Ingrese cual host comenzará la transmisión: 0 o 1; si selecciona 2, comenzarán los dos al mismo tiempo.") = default(0);
        error @prompt("Ingrese cual es la probabilidad de que la trama posea un error (de 0 a 99)") = default(7);        
        @display("bgi=background/green,t");
}