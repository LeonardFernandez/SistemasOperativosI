#include <xinu.h>
#define ASCII_ESC_O 27

int vel_ao = 0;
int vel_bo = 0;

corredor_aOriginal(){
	while(1) {
		vel_ao++;
	}
}

corredor_bOriginal(){
	while(1) {
		vel_bo++;
	}
}


muestra_carreraOriginal() {
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC_O );
	while(1) {
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC_O );
    		printf( "Velocidad corredor A: %d \n", vel_ao);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[6;10f", ASCII_ESC_O );
    		printf( "Velocidad corredor B: %d \n", vel_bo);

		sleepms(50);
	}


}

control_raceOriginal(){
	//chprio(currpid, 20);
	int pidA=create(corredor_aOriginal,1024,20,"procA",0);
	int pidB=create(corredor_bOriginal,1024,20,"procB",0);
	int pidMuestra=create(muestra_carreraOriginal,1024,20,"procM",0);
    resume(pidMuestra);
	resume(pidA);
	resume(pidB);
	
}