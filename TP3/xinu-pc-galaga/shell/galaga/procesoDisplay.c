//Mantiene la vida y puntaje en pantalla
#include<xinu.h>
int vidas,puntaje;
int display(){
    vidas=3;
    puntaje=0;
    char cadena[30];
    int msg;
    while(1){
        sprintf(cadena, "Vidas  --> %d \n Puntaje --> %d",vidas,puntaje);
        print_text_on_vga(300, 40, cadena);
        msg=receive();
        switch(msg){
            case 1: vidas--; break; //Pierde una vida
            case 2: puntaje++; break;//Gana puntaje
            case 3: vidas=3;puntaje=0;
                sprintf(cadena, "");
                print_text_on_vga(300, 40, cadena); break; //Restaura puntaje y vidas
            default: break;
        }
    }
}