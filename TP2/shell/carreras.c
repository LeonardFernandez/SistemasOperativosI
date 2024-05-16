#include <xinu.h>

#define ASCII_ESC_SEBA 27

int vel_a_seba = 0;
int vel_b_seba = 0;
int distancia_seba = 0;
sid32 mutex_seba;

corredor_a_seba()
{
	while(1) {
		vel_a_seba++;
		wait(mutex_seba);
		distancia_seba++;
		signal(mutex_seba);
	}
}

corredor_b_seba()
{

	while(1) {
		vel_b_seba++;
		wait(mutex_seba);
		distancia_seba++;
		signal(mutex_seba);
	}
}


muestra_carrera_seba() 
{
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC_SEBA );

	while(1) {

		printf( "%c[3;10f", ASCII_ESC_SEBA);
		printf( "Distancia: %d \n", distancia_seba);

		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC_SEBA );
    		printf( "Velocidad corredor A: %d \n", vel_a_seba);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[6;10f", ASCII_ESC_SEBA );
    		printf( "Velocidad corredor B: %d \n", vel_b_seba);

		sleepms(50);
	}
}

int proceso[3];
uint32 prflags = 0;
//No le den bola a esto, se puede hacer teniendo variables, me complique al pedo
void process_switch(int my_pr)
{
	if( (prflags >> my_pr) & 1U )
	{
		wait(mutex_seba);
		suspend(proceso[my_pr]);
		signal(mutex_seba);
		prflags  ^= 1U << my_pr;
	}
	else
	{
		resume(proceso[my_pr]);
		prflags ^= 1U << my_pr;
	}
}

shellcmd control_race_seba() {


	mutex_seba = semcreate(1);
	prflags = 0;
	distancia_seba = 0;

	chprio(currpid, 20);

	proceso[0] = create(corredor_a_seba, 1024, 20, "Corredor A", 0);
	proceso[1] = create(corredor_b_seba, 1024, 20, "Corredor B", 0);
	proceso[2] = create(muestra_carrera_seba, 1024, 20, "Mostrar Carrara", 0);

	for(int i=0; i<3; i++) 
		process_switch(i);

	control(CONSOLE, TC_MODER, NULL, NULL);
	int c=1;
	while(c!='0')
	{
		c = getchar();
		switch(c) {
		case '1': process_switch(0); break;
		case '2': kill(proceso[0]); break;
		case '5': process_switch(1); break;
		case '6': kill(proceso[1]); break;
		default: break;
		}
	}
	control(CONSOLE, TC_MODEC, NULL, NULL);

}