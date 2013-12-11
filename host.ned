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

// Definición del Host
module host
{
    //parámetros
    parameters:
        //dirección del host
        int direccion_host;
        
        //direccion del destino
        int direccion_dest;
        
        //tamaño de trama
        int tamTrama;
        
        //número de tramas
        int numTramas;

		//tamańo máximo de ventana
        int tamVentana;

        //define cual host(0 o 1) comienza la ejecucion
        int starter;
        
        @display("bgl=10;i=device/server2;is=n;bgb=130,335");
    //Puertas de enlace
    gates:
        //puerta de entrada
        input entrada;
        //puerta de salida
        output salida;
    //Llamado a Submódulos        
    submodules:
        //Llamado al nivel de aplicación
        varAplicacion: aplicacion {
            //parámetros del nivel
            parameters:
                tamTrama = tamTrama;
                direccion_host = direccion_host;
                direccion_dest = direccion_dest;
                starter = starter;
                numTramas = numTramas;
                @display("p=67,50");
        }
        //Llamado al nivel intermedio
        varIntermedio: intermedio {
            //parámetros del nivel
            parameters:
                @display("p=67,158");
        }
        //Llamado al nivel de enlace
        Modulo_Enlace: enlace {
            //parámetros del nivel
            parameters:
                tamTrama = tamTrama;
                tamVentana = tamVentana;
                direccion_host = direccion_host;
                direccion_dest = direccion_dest;
                tramas_libres = tramas_libres;                
                numTramas = numTramas;
                @display("p=67,266");
        }
    connections:
        //Desde aplicacion hacia intermedio
        varAplicacion.hacia_abajo --> { } --> varIntermedio.desde_arriba;
        //Desde intermedio a aplicación
        varIntermedio.hacia_arriba --> { } --> varAplicacion.desde_abajo;
        //Desde intermedio a enlace
        varIntermedio.hacia_abajo --> { } --> varEnlace.desde_arriba;
        //Desde enlace a intermedio
        varEnlace.hacia_arriba --> { } --> varIntermedio.desde_abajo;
        //Desde enlace a la salida del host
        varEnlace.hacia_fisico --> { } --> salida;
        //Desde la salida del host hacia enlace
        entrada --> { } --> varEnlace.desde_fisico;
}