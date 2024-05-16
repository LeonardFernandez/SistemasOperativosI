/* mut.c - mut, operar, incrementar */
#include <xinu.h>
void operar(void), incrementar(void);
unsigned char numX = 0;
sid32 myMutex;
/*------------------------------------------------------------------------
* mut -- programa con regiones criticas
*------------------------------------------------------------------------
*/
void mutMutex(void){
	int i;
	myMutex=semcreate(1);//mutex_init(&myMutex);
	resume( create(operar, 4096, 20, "process 1", 0) );
	resume( create(incrementar, 4096, 20, "process 2", 0) );
	sleep(10);
}
/*------------------------------------------------------------------------
* operar numX e y
*------------------------------------------------------------------------
*/
void operarMutex(void){
	wait(myMutex);//mutex_lock(&myMutex);
	int y = 0;
	//signal(myMutex);//mutex_unlock(&myMutex);
	printf("Si no existen mensajes de ERROR entonces todo va OK! \n");
	while (1) {
		//wait(myMutex);//mutex_lock(&myMutex);
		/* si numX es multiplo de 10 */
		if ((numX% 10) == 0) {
			y = numX* 2; /* como y es el doble de numX entonces
			* y es multiplo de 10 tambien
			*/
			/* si y no es multiplo de 10 entonces hubo un error */
		if ((y % 10) != 0)
			printf("\r ERROR!! y=%d, numX=%d \r", y, numX);
		}
		
	}
	signal(myMutex);//mutex_unlock(&myMutex);
}
/*------------------------------------------------------------------------
* incrementar numX
*------------------------------------------------------------------------
*/
void incrementarMutex(void){
	wait(myMutex);//mutex_lock(&myMutex);
	while (1) {
		numX= numX+ 1;
	}
	signal(myMutex);//mutex_unlock(&myMutex);
}
