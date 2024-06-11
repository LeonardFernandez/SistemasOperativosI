#include<xinu.h>
#include<keyboard.h>
buffer_t test;
void testArreglo(){
    iniciarBuffer(&test);
    insertar(&test,'h');
    insertar(&test,'j');
    insertar(&test,'k');
    sacarFrente(&test);
    sacarFrente(&test);
    insertar(&test, 'O');
    imprimir(&test);
    char c=sacarFrente(&test);
    printf("%c  ",c);
}