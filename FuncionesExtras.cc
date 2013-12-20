/* * FuncionesExtras.cc
 *
 *  Created on: 10-12-2013
 *      Author: Esteban Gaete
 */

#include "FuncionesExtras.h"

#include <string.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int FuncionesExtras::getValorId(const char* input_complete_name){
    string nombre_completo = input_complete_name;

    //Para guardar el tamaño del resto del mensaje
    int rest_tam_msg=0;

    for(int i=0;nombre_completo[i]!= ',';i++){
        rest_tam_msg++;
    }
    rest_tam_msg++;

    string nombre;
    if(nombre_completo[0] == 'A' || nombre_completo[0] == 'D'){
        //Obtiene el ID y lo guarda en "nombre"
        for(unsigned int i=rest_tam_msg;i<nombre_completo.length();i++){
            nombre.push_back(nombre_completo[i]);
        }
    }else{
        for(unsigned int i=rest_tam_msg;nombre_completo[i]!=',';i++){
            //nombre[i-rest_tam_msg] = nombre_completo[i];
            nombre.push_back(nombre_completo[i]);
        }
    }

    //Lo transforma a entero
    return atoi(nombre.c_str());
}

int FuncionesExtras::getValorPF(const char* input_complete_name){
    string nombre_completo = input_complete_name;

    //Para guardar el tamaño del resto del mensaje
    int rest_tam_msg=0;

    int contador;
    for(contador=0;nombre_completo[contador]!= ',';contador++){
        rest_tam_msg++;
    }
    rest_tam_msg++;
    contador++;

    //lee hasta la siguiente coma
    for(int i=contador;nombre_completo[i]!= ',';i++){
        rest_tam_msg++;
    }
    rest_tam_msg++;

    string nombre;
    if(nombre_completo[0] == 'A' || nombre_completo[0] == 'D'){
        //Obtiene el ID y lo guarda en "nombre"
        for(unsigned int i=rest_tam_msg;i<nombre_completo.length();i++){
            nombre.push_back(nombre_completo[i]);
        }
    }else{
        for(unsigned int i=rest_tam_msg;nombre_completo[i]!=',';i++){
            nombre.push_back(nombre_completo[i]);
        }
    }

    //Lo transforma a entero
    return atoi(nombre.c_str());
}

const char* FuncionesExtras::getNombreTrama(const char* input_complete_name){
    string nombre_original = input_complete_name;
    string nombre;

    for(int i=0;nombre_original[i]!=',';i++){
        nombre.push_back(nombre_original[i]);
    }

    return nombre.c_str();
}

const char* FuncionesExtras::nombrandoTrama(const char* input_original_name,const char* input_new_name){
    string nombre_original = input_original_name;
    string nuevo_nombre = input_new_name;

    //ID del mensaje obtenido del nombre_original
    int msg_id;

    //Obtiene el valor del id
    msg_id = getValorId(nombre_original.c_str());

    string name_msg_id;

    name_msg_id = intToString(msg_id);

    //Para el nuevo nombre
    string buffer;
    buffer = nuevo_nombre + name_msg_id;

    //retorna el nuevo nombre con el ID
    return buffer.c_str();
}

const char* FuncionesExtras::nombrando(const char* input_original_name, int valor){
    string nombre_original = input_original_name;
    string nombre_valor;

    nombre_valor = intToString(valor);

    //Para el nombre de la trama
    string buffer;
    buffer = nombre_original + nombre_valor;

    return buffer.c_str();
}

const char* FuncionesExtras::intToString(int numero){
    //String con el numero invertido
    string inv_numero;

    //Numero real
    string real_numero;

    if(numero !=0 ){
        //caracter auxiliar para calcular cada digito
        char digito;
        for(int i=0;numero!=0;i++){
            digito = (numero%10) + 48;
            inv_numero.push_back(digito);
            numero /= 10;
        }

        int index = inv_numero.length()-1;
        for(unsigned int i=0;i<inv_numero.length();i++){
            real_numero.push_back(inv_numero[index-i]);
        }

    }else{
        real_numero.push_back(48);
    }

    return real_numero.c_str();
}
