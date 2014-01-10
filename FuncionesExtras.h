/*
 * FuncionesExtras.h
 *
 *  Created on: 10-12-2013
 *      Author: Esteban Gaete
 */

#ifndef FUNCIONESEXTRAS_H_
#define FUNCIONESEXTRAS_H_

#include <vector>

class FuncionesExtras {
public:
    /*
     * Obtiene el valor del nombre, es decir:
     * Si es "DATO,N" obtiene N
     * Si es "ACK,N" obtiene N
     * Si es "I,0,0" obtiene el primero 0
     * Si es "RR,0,0" obtiene el primero 0
     */
    static int getValorId(const char* input_complete_name);

    /*
     * Obtiene el valor del bit P/F
     */
    static int getValorPF(const char* input_complete_name);

    /*
     * Del nombre original obtiene el Id y lo concatena con el new name
     */
    static const char* nombrandoTrama(const char* input_original_name,const char* input_new_name);

   /*
    * Del nombre original obtiene el nombre sin sus atributos de informacion de trama
    */
    static const char* getNombreTrama(const char* input_complete_name);

    /*
     * Concatena original name con valor
     */
    static const char* nombrando(const char* input_original_name, int valor);

    /*
     * Concatena original name con coma y con valor
     */
    static const char* nombrando(const char* input_original_name, char coma, int valor);

    /*
     * Transforma de entero a string
     */
    static const char* intToString(int numero);

    /*
     * Transforma un entero en un arreglo de int* donde cada posicion es un número binario
     */
    static int* intToBitArray(int numero, int tamano);

    /*
     * Transforma un entero en un arreglo de int* donde cada posicion es un número binario
     */
    static int* intToBitArray(int numero);

    /*
     * Para saber que tamaño tendrá el arreglo, se deben usar en conjunto con intToBitArray();
     */
    static int tamBitArray(int numero);

    /*
     * Transforma un arreglo de numeros binario en un numero decimal
     */
    static int bitArrayToInt(int* numero_binario, int size);
};

#endif /* FUNCIONESEXTRAS_H_ */
