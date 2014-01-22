#include "FuncionesExtras.h"

//using namespace std;

const char* FuncionesExtras::intToString(int numero){
    //String con el numero invertido
    std::string inv_numero;

    //Numero real
    std::string real_numero;

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

int* FuncionesExtras::intToBitArray(int numero, int tamano){
    int* bitArray;
    bitArray = (int*)malloc(sizeof(int)*tamano);

    if(numero != 0){
        std::vector<int> numero_binario;

        //Transforma numero a un número binario
        for(int i=0;numero>0;i++){
            numero_binario.push_back(numero%2);
            numero /= 2;
        }

        int offset = tamano - numero_binario.size();

        for(int i=0;i < offset;i++){
            bitArray[i] = 0;
        }
        std::cout << "---" << std::endl;
        for(int i=offset;i < tamano;i++){
            bitArray[i] = numero_binario.back();
            numero_binario.pop_back();
            std::cout << bitArray[i] << std::endl;
        }
        std::cout << "---" << std::endl;
    }else{
        for(int i=0;i<tamano;i++){
            bitArray[i] = 0;    
        }        
    }

    return bitArray;
}

int* FuncionesExtras::intToBitArray(int numero){
    int* bitArray;
    bitArray = (int*)malloc(sizeof(int)*8);

    for(int i=0;i<8;i++){
        bitArray[i]=0;
    }

    //Pasa los numeros uno a uno
    if(numero==1){
        bitArray[7]=1;
    }else if(numero==2){
        bitArray[6]=1;
    }else if(numero==3){
        bitArray[6]=1;
        bitArray[7]=1;
    }

    return bitArray;
}

int FuncionesExtras::tamBitArray(int numero){
    if(numero != 0){
        int tam=1;

        //Calcula el tamaño que tendrá el array
        for(tam=0;numero>1;tam++){
            numero /= 2;
        }

        return tam;
    }else{
        return 1;
    }
}

int FuncionesExtras::bitArrayToInt(int* numero_binario, int size){
    int numero=0;
    std::vector<int> vector_binario; 
    int* direccion_binaria = numero_binario;

    for(int i=0;i<size;i++){
        vector_binario.push_back(direccion_binaria[i]);
    }

    for(int i=0;i<size;i++){
        numero += pow(2,i)*vector_binario.back();
        vector_binario.pop_back();
    }

    return numero;
}
