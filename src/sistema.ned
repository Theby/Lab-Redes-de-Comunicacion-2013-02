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

package laboratorio_redes_01_02;

//Importación del host
import
"host";

//Definición del módulo
module sistema
//Parámetros
parameters:
tamTrama: numeric;
//Submódulos que lo componen
submodules:
//Definición del host 1
Host_1: host;
//Parámetros
parameters:
tamTrama = tamTrama,
//Se suministra la dirección que le corresponde al host
direccion = 1;
//Aspecto gráfico del host
display: "p=71,79;i=old/server1";
//Definición del host 2
Host_2: host;
//Parámetros
parameters:
tamTrama = tamTrama,
//Se suministra la dirección que le corresponde al host
direccion = 2;
//Aspecto gráfico del host
display: "p=190,79;i=old/server1";
//Conexiones de la red
connections:
//Desde el Host 1 hacia el Host 2
Host_1.salida --> canal --> Host_2.entrada;
//Desde el Host 2 hacia el Host 1
Host_2.salida --> canal --> Host_1.entrada;
//Aspecto gráfico de la red
display: "o=,,;b=274,179,yellow,,,";
//Fin del módulo
endmodule
//Definición del canal de transmisión
channel canal
//Retardo de propagación
delay 100ms;
//Velocidad de datos
datarate 128000bps;
endchannel


//Instanciación de la red
network red : sistema
//Parámetros
parameters:
tamTrama = input(4, "Ingrese el largo de la palabra, entero mayor a cero:");
//Fin de la definición de la red
endnetwork