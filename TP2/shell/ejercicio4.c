/* ex2.c - main, sndA, sndB */

#include <xinu.h>

void	sndA(void), sndB(void);

/*------------------------------------------------------------------------
 * main  --  example of creating processes in Xinu
 *------------------------------------------------------------------------
 */
void	ejercicio4()
{
	int pidA=create(sndA, 1024, 20, "process 1", 0);
	int pidB= create(sndB, 1024, 20, "process 2", 0) ;
    resume(pidA);
    resume(pidB); 
    sleepms(10000);
    kill(pidA);
    kill(pidB);
    
}

/*------------------------------------------------------------------------
 * sndA  --  repeatedly emit 'A' on the console without terminating
 *------------------------------------------------------------------------
 */
void	sndA(void)
{
	while( 1 )
		putc(CONSOLE, 'A');
}

/*------------------------------------------------------------------------
 * sndB  --  repeatedly emit 'B' on the console without terminating
 *------------------------------------------------------------------------
 */
void	sndB(void)
{
	while( 1 )
		putc(CONSOLE, 'B');
}