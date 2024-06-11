#include<xinu.h>
#include<keyboard.h>

char kbdgetc(){
    char c;
    if(getpid()==pidTeclado){
        wait(semaforoBuffer);
        c=sacarFrente(&bufferChar);
    }
    return c;
}