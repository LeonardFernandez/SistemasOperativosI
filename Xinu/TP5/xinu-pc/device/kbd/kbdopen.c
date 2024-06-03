/* kbdopen.c  -  kbdopen */

#include <xinu.h>
extern sid32 semKeyboard;
extern pid32 pidKeyboard;
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
	//Tomamos un permiso
	wait(semKeyboard);
	//Si toma permiso entonces guardamos pid
	pidKeyboard=getpid();
	/* No action -- just return the device descriptor */

	return devptr->dvnum;
}
