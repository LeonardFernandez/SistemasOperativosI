#include <xinu.h>
void productor (void), consumidor (void);
int32 n = 0;	/*Global variables are shared by all processes*/
sid32 semProd, semCons;
/*---------------------------------------------------------------------
*	main - Example of unsynchronized producer and consumer processes
*----------------------------------------------------------------------
*/
void mainProdCons(void){
	semProd=semcreate(1);	//Semaforo productor 1 permiso
	semCons=semcreate(0);			//Semaforo consumidor 0 permisos
	resume( create(consumidor, 4096, 20, "consu", 0)); 
	resume( create(productor, 4096, 20, "produ", 0));
}
/*---------------------------------------------------------------------
*	produce - Increment n 2000 times and exit
*---------------------------------------------------------------------
*/
void productor (void){
	wait(semProd);	//Produce
	int32 i;
	for(i=1;i<=1000; i++)
		n++;
	signal(semCons);//Avisa que hay producto
}
/*---------------------------------------------------------------------
*	consume - Print n 2000 times and exit
*---------------------------------------------------------------------
*/
void consumidor (void){
	wait(semCons); //Consume
	int32 i;
	for(i=1;i<=1000; i++)
		printf("The value of n is %d \n", n);
	signal(semProd);	//Avisa al productor
}
