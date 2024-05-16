#include <stdio.h>
void main(){
    int pid=fork();
    if(pid==0){
        for(int i=0;i<=2000;i++){
            if(esBisiesto(i)!=-1){
                printf("Proceso hijo 1, año bisiesto: %i \n",i);
            }
        }
    }else{
        int pid2=fork();
        if(pid2==0){
            for(int i=2001;i<=4000;i++){
                if(esBisiesto(i)!=-1){
                    printf("Proceso hijo 2, año bisiesto: %i \n",i);
                }
            }
        }
    }
}


int esBisiesto(int año){  
    int retorno=-1;
    if(año%4==0){
        if(año%100!=0){
            retorno=año;//Es bisiesto
        }
    }else{
        if(año%100==0){
            if(año%400==0){
                retorno=año;//Es bisiesto
            }
        }
    }
    return retorno;
}



