#include <xinu.h>
#define ASCII_ESC 27

int vel_a = 0;
int vel_b = 0;
int distancia=0;
sid32 semaforo;
int estadoA=-1;
int estadoB=-1;

corredor_a(){
	while(1) {
		vel_a++;
		wait(semaforo);
		distancia++;
		signal(semaforo);
	}
}

corredor_b(){
	while(1) {
		vel_b++;
		wait(semaforo);
		distancia++;
		signal(semaforo);
	}
}


muestra_carrera() {
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC );
	while(1) {
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC );
    		printf( "Velocidad corredor A: %d \n", vel_a);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[6;10f", ASCII_ESC );
    		printf( "Velocidad corredor B: %d \n", vel_b);

		sleepms(50);
	}
}
	
cambiarEstado(int unPid){
	wait(semaforo);
	int prioridadProceso=getprio(unPid);
	int prioridadActual=getprio(getpid());
	//Si prioridad actual es mayor entonces proceso suspendido
	if(prioridadActual>prioridadProceso){
		chprio(unPid,prioridadActual);
	}else{ //Sino proceso esta en listos
		suspend(unPid);
	}
	signal(semaforo);
}

imprimirDistancia(){
	wait(semaforo);
	printf("Distancia: %d \n",distancia);
	signal(semaforo);
}

control_race(){
	semaforo=semcreate(1);
	int pidA=create(corredor_a,2048,20,"procA",0);
	int pidB=create(corredor_b,2048,20,"procB",0);
	int pidMuestra=create(muestra_carrera,2048,20,"procM",0);
    resume(pidMuestra);
	resume(pidA);
	resume(pidB);
	
	/* Decirle al sistema que el modo input es RAW */
	control(CONSOLE, TC_MODER,0,0);
	int c;
	while(1){
		c=getchar();
		switch(c){
			case '1' :cambiarEstado(pidA);	break;
			case '5' :kill(pidA); 	break;
			case '2' :cambiarEstado(pidB);	break;
			case '6' :kill(pidB);	break;
			case 'p' :imprimirDistancia();	break;
			default:	break;
		}
	}
	control(CONSOLE,TC_MODEC,0,0); //Consola tty modo cooked
}