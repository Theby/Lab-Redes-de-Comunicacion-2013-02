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

simple mac
{
    //Parámetros del nivel de MAC
    parameters:
        //Dirección de Host
        int direccion_host;

		//Dirección de Destino
        int direccion_dest;
        
        //Para saber si esta conectado o no
        volatile bool conectado = default(false);
        
        //Para saber si tiene el testigo o no
        volatile bool holder = default(false);
        
        //Para saber cuantas veces ha llegado consecutivamente y sin interrumpciones el Testigo al host
        volatile int cant_holder = default(0);
        
        //Para saber si los hosts se desconectan o no tras mandar todas sus tramas
        volatile bool desconectar = default(false);
        
    	@display("i=block/rxtx,black;is=n");
    	
    gates:
        //Con el nivel inferior
        input desde_fisico;
        output hacia_fisico;

        //con el nivel superior
        input desde_arriba;
        output hacia_arriba;
}
