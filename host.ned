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
// Definición del Host
//
module host
{
    //parámetros
    parameters:
        //dirección del host
        int direccion_host;

        //direccion del destino
        int direccion_dest;

        //tamańo de trama
        int tamTrama;

        //número de tramas maxima que envia
        int numTramas_env;
        
        //número de tramas maxima que recibe
        int numTramas_rec;

		//tamaño máximo de ventana
        int tamVentana;

        //define cual host(0 o 1) comienza la ejecucion
        int starter;
        
        //Numero de tramas libres
        int tramas_libres;

		//define cual es la probabilidad de error
        int error;

        @display("bgl=10;i=device/server2,black;is=n;bgb=130,335;bgi=background/terrain");
    //Puertas de enlace
    gates:
        //puerta de entrada
        input entrada;
        //puerta de salida
        output salida;
    //Llamado a Submódulos        
    submodules:
        //Llamado al nivel de aplicación
        Modulo_Aplicacion: aplicacion {
            //parámetros del nivel
            parameters:
                tamTrama = tamTrama;
                direccion_host = direccion_host;
                direccion_dest = direccion_dest;
                starter = starter;
                numTramas_env = numTramas_env;
                numTramas_rec = numTramas_rec;
                @display("p=67,50");
        }
        //Llamado al nivel intermedio
        Modulo_Intermedio: intermedio {
            //parámetros del nivel
            parameters:
                @display("p=67,158");
        }
        //Llamado al nivel de enlace
        Modulo_Enlace: enlace {
            //parámetros del nivel
            parameters:
                tamVentana = tamVentana;
                direccion_host = direccion_host;
                direccion_dest = direccion_dest;
                tramas_libres = tramas_libres;
                starter = starter;                
                numTramas_env = numTramas_env;
                numTramas_rec = numTramas_rec;
                error=error;
                @display("p=67,266");
        }
    connections:
        //Desde aplicacion hacia intermedio
        Modulo_Aplicacion.hacia_abajo --> Modulo_Intermedio.desde_arriba;
        //Desde intermedio a aplicación
        Modulo_Intermedio.hacia_arriba --> Modulo_Aplicacion.desde_abajo;
        //Desde intermedio a enlace
        Modulo_Intermedio.hacia_abajo --> Modulo_Enlace.desde_arriba;
        //Desde enlace a intermedio
        Modulo_Enlace.hacia_arriba --> Modulo_Intermedio.desde_abajo;
        //Modulo_Enlace enlace a la salida del host
        Modulo_Enlace.hacia_fisico --> salida;
        //Desde la salida del host hacia enlace
        entrada --> Modulo_Enlace.desde_fisico;
}