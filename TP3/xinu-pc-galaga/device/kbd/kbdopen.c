/* kbdopen.c  -  kbdopen */

#include <xinu.h>
#include<keyboard.h>
/*------------------------------------------------------------------------
 * kbdopen  -  Open the ps/2 keyboard device
 *------------------------------------------------------------------------
 */

devcall	kbdopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a kbd */
	 char	*mode			/* Unused for a kbd */
	)
{
	//Al acceder al teclado, proceso toma el semaforoTeclado
	wait(semaforoTeclado);
	pidTeclado=getpid();
}
