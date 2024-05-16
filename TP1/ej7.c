#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<signal.h>

int esPrimo(int num) {
	int esPrimo=1;
    if (num <= 1) {
        esPrimo= 0; // No es primo
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            esPrimo= 0; // No es primo
        }
    }
    return esPrimo; // Es primo
}

void main(void){
	int pid;
	pid=fork();
	
	if(pid==0){ //Si id es cero entonces hijo esta ejecutando
		for(int i=1000; i<=5000; i++){
			if(esPrimo(i)){
				printf("%i \n", i);
			}
		}
	}else{
		for(int i=0; i<=1000; i++){
			if(esPrimo(i)){
				printf("%i \n", i);
			}
		}
		kill(pid, SIGKILL);
		exit(0);
	}
}
