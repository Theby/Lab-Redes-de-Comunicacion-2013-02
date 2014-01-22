#ifndef FUNCIONESEXTRAS_H_
#define FUNCIONESEXTRAS_H_

#include <vector>
#include <math.h>
#include <string.h>
#include <cstdlib>
#include <iostream>
#include <stdio.h>

class FuncionesExtras {
    public:
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
