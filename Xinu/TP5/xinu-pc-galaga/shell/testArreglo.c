#include<xinu.h>
#include<keyboard.h>
void testArreglo(){
    iniciarBuffer(bufferChar);
    insertar(bufferChar,'h');
    imprimir(bufferChar);
}