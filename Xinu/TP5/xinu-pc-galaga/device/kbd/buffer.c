#include<keyboard.h>

buffer_t bufferChar;

//Arreglo ciruclar


void iniciarBuffer(buffer_t* a){
    a->frente=0;
    a->fin=0;
    a->tamaño=10;
}

int estaLleno(buffer_t* a){
    return  ( ( (a->fin+1)%(a->tamaño) ) == (a->frente) );
}

int estaVacio(buffer_t*a){
    return (a->fin==a->frente);
}

int insertar(buffer_t* a, char elem){
    //Inserte en posicion fin
    int lleno=estaLleno(a);
    if(!lleno){
        a->arreglo[a->fin]=elem;
    }
    return !lleno;
}

int eliminar(buffer_t*a){
    int vacio=estaVacio(a);
    if(!vacio){
        a->arreglo[a->frente]=0;
        a->frente=( (a->frente+1)%a->tamaño );   
    }
    return !vacio;
}
