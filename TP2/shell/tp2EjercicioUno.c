#include<xinu.h>
void sumar(char nombreProceso){
    int suma;
    for(suma=0; suma<=5000; suma++){
        //if(suma%10==0)
        printf("%c incrementa %d \n",nombreProceso, suma);
    }
}

void ejercicioUno(){
    int pidA,pidB,pidC,msA,msB,msC;
    pidA=create(sumar,2048,2,"procesoA",1,'A');
    pidB=create(sumar,2048,2,"procesoB",1,'B');
    pidC=create(sumar,2048,2,"procesoC",1,'C');
    resume(pidA);
    resume(pidB);
    resume(pidC);
    msA=120;
    msB=60;
    msC=20;
    high_level_scheduler(pidA,msA,pidB,msB,pidC,msC);
}