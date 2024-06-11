/* kbdclose.c  -  kbdclose */

#include <xinu.h>
#include<keyboard.h>
/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */
devcall	kbdclose (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	//Al cerrar teclado el proceso libera semaforoTeclado
	signal(semaforoTeclado);
}
