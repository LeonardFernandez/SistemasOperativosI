/* mut.c - mut, operar, incrementar */
#include <mutex.h>
void operarMutex(void), incrementarMutex(void);
unsigned char numX = 0;
struct mutex myMutex;
/*------------------------------------------------------------------------
* mut -- programa con regiones criticas
*------------------------------------------------------------------------
*/
void mutMutex(void){
	int i;
	mutex_init(&myMutex);
	resume( create(operarMutex, 4096, 20, "process 1", 0) );
	resume( create(incrementarMutex, 4096, 20, "process 2", 0) );
	sleep(10);
}
/*------------------------------------------------------------------------
* operar numX e y
*------------------------------------------------------------------------
*/
void operarMutex(void){
	mutex_lock(&myMutex);
	int y = 0;
	mutex_unlock(&myMutex);
	printf("Si no existen mensajes de ERROR entonces todo va OK! \n");
	while (1) {
		mutex_lock(&myMutex);
		/* si numX es multiplo de 10 */
		if ((numX% 10) == 0) {
			y = numX* 2; /* como y es el doble de numX entonces
			* y es multiplo de 10 tambien
			*/
			/* si y no es multiplo de 10 entonces hubo un error */
		if ((y % 10) != 0)
			printf("\r ERROR!! y=%d, numX=%d \r", y, numX);
		}
        mutex_unlock(&myMutex);
	}
}
/*------------------------------------------------------------------------
* incrementar numX
*------------------------------------------------------------------------
*/
void incrementarMutex(void){
	mutex_lock(&myMutex);
	while (1) {
		numX= numX+ 1;
	}
	mutex_unlock(&myMutex);
}