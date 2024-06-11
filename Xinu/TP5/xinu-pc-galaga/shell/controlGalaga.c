#include<xinu.h>
unsigned char tecla_actual
void controlGalaga(){
    open(KEYBOARD,NULL, NULL);
    while(1){
        //tecla actual lee un byte
        read(KEYBOARD,&tecla_actual,1);
    }
}