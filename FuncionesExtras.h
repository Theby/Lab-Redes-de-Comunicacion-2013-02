/*
 * FuncionesExtras.h
 *
 *  Created on: 10-12-2013
 *      Author: Esteban Gaete
 */

#ifndef FUNCIONESEXTRAS_H_
#define FUNCIONESEXTRAS_H_

class FuncionesExtras {
public:
    FuncionesExtras();
    static int getValorId(const char* input_complete_name);
    static const char* nombrandoTrama(const char* input_original_name,const char* input_new_name);
    static const char* nombrando(const char* input_original_name, int valor);
    static const char* intToString(int numero);
};

#endif /* FUNCIONESEXTRAS_H_ */
