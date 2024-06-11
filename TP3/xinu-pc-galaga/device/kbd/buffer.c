#include<xinu.h>
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
    //int lleno=estaLleno(a);
    a->arreglo[a->fin]=elem;
    a->fin=(a->fin+1)%(a->tamaño);
    /*a->arreglo[0]='h';
    a->arreglo[1]='k';
    a->arreglo[2]='l';*/
    return 0;
}

char sacarFrente(buffer_t* a){
    int vacio=estaVacio(a);
    char c;
    if(!vacio){
        c=a->arreglo[a->frente];
        a->arreglo[a->frente]=0;
        a->frente=( (a->frente+1)%a->tamaño );   
    }
    return c;
}

void imprimir(buffer_t*a){
    char cadena[30];
    for(int i=0;i<9;i++){
        //sprintf(cadena, "%c \n",a->arreglo[i]);
        printf("%c \n", a->arreglo[i]);
    }
    //print_text_on_vga(300, 40, cadena);
}
