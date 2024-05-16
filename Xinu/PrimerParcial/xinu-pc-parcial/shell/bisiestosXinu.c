/* bisiestos.c en XINU */

#include <mutex.h>
#define ASCII_ESC 27
long int calculo[2];
int estadoPA;//estadoPA indica estado del proceso a, si es -1 suspendido,caso contrario 1
int estadoPB;//idem estadoPA pero para el proceso b
struct mutex myMutex;

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

/* completar el codigo del programa bisiesto */
bisiesto(int base, int limite, int n){
	int i;

	/* base, limite y n vienen como argumentos */
	for (i=base; i<=limite; i++) {
		if (esBisiesto(i)!=-1){ 
            mutex_lock(&myMutex); //Seccion critica toma lock
			calculo[n] = i;	
            mutex_unlock(&myMutex);
			sleepms(300);
        }  
	}

}

muestra_bisiestos() {
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC );

	while(1) {
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC );
            mutex_lock(&myMutex);
    		printf( "proceso a: %d \n", calculo[0]);
            mutex_unlock(&myMutex);
		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[7;10f", ASCII_ESC );
            mutex_lock(&myMutex);
    		printf( "proceso b: %d \n", calculo[1]);
            mutex_unlock(&myMutex);
		sleepms(50);
	}
}

cambiarA(int pid){
    if(estadoPA==1){
        printf( "%c[9;10f", ASCII_ESC );
        printf("Proceso A SUSPENDIDO \n");
        suspend(pid);
        estadoPA=-1;
    }else{
        printf( "%c[9;10f", ASCII_ESC );
        printf("Proceso A REANUDA \n");
        resume(pid);
        estadoPA=1;
    }
}
cambiarB(int pid){
    if(estadoPB==1){
        printf( "%c[9;10f", ASCII_ESC );
        printf("Proceso B SUSPENDIDO \n");
        suspend(pid);
        estadoPB=-1;
    }else{
        printf( "%c[9;10f", ASCII_ESC );
        printf("Proceso B REANUDA \n");
        resume(pid);
        estadoPB=1;
    }
}

/* completar el gestor de procesos bisiestos */
gestor_bisiestos(){
    mutex_init(&myMutex);
    int pidA, pidB, pidMuestra;
    pidA=create(bisiesto,2048,20,"procA",3,0,2000,0);
    pidB=create(bisiesto,2048,20,"procB",3,2001,4000,1);
    pidMuestra=create(muestra_bisiestos,2048,20,"procMuestra",0);
    resume(pidA);
    resume(pidB);
    resume(pidMuestra);
    estadoPA=1;
    estadoPB=1;
	while(1) {
		/* gestionar las pulsaciones del teclado, procesar, informar al usuario */
        /* Decirle al sistema que el modo input es RAW */
	    control(CONSOLE, TC_MODER,0,0);
	    int c;
	    while(1){
		    c=getchar();
		    switch(c){
			    case '1' :cambiarA(pidA);	                        break;//Proceso a --> suspende/reanuda
			    case '3' :kill(pidA);                    	        break;//Proceso a --> finalizar
			    case '7' :cambiarB(pidB);	                        break;//Proceso b --> suspende/reanuda
			    case '9' :kill(pidB);                   	        break;//Proceso b --> finalizar
                case 'q' :kill(pidA);kill(pidB);kill(pidMuestra);   break;//Finalizar todo, volver a shell
			    default:	break;
		    }
            
	    }
	    control(CONSOLE,TC_MODEC,0,0); //Consola tty modo cooked
		sleepms(5);
	}

}
