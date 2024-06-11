/* kbdinit.c  -  kbdinit */

#include <xinu.h>
#include <keyboard.h>

unsigned char kblayout [128];  // { ... } Fill your layout yourself 
pid32 pidTeclado; //pidProceso que accede al teclado
sid32 semaforoTeclado; //semaforo acceso teclado, solo un proceso accede teclado
sid32 semaforoBuffer; //Semaforo teclado, getc puede quitar char si hay permisos

void keyboard_wait(byte a_type) //unsigned char
{
  int _time_out=100000; //unsigned int
  if(a_type==0)
  {
    while(_time_out--) //Data
    {
      if((inportb(0x64) & 1)==1)
      {
        return;
      }
    }
    return;
  }
  else
  {
    while(_time_out--) //Signal
    {
      if((inportb(0x64) & 2)==0)
      {
        return;
      }
    }
    return;
  }
}




/*------------------------------------------------------------------------
 *  kbdinit  -  Initialize the ps/2 keyboard
 *------------------------------------------------------------------------
 */
devcall	kbdinit (
	  struct dentry	*devptr		/* Entry in device switch table	*/
	)
{
	int i;
	for (i=0; i<128; i++)
		kblayout[i] = i;
	//keyboard_restart();

	//Iniciamos buffer
	iniciarBuffer(&bufferChar);
	//Creamos semaforo acceso teclado con un permiso
	semaforoTeclado=semcreate(1);
	//Creamos semaforo acceso buffer,0 permisos(handler libera permisos)
	semaforoBuffer=semcreate(0);

	byte _status;  //unsigned char

	//Enable the keyboard device   old: auxiliary mouse device
	keyboard_wait(1);
	outportb(0x64, 0xAE);
 
	//Enable the interrupts
	keyboard_wait(1);
	outportb(0x64, 0x20);
	keyboard_wait(0);
	_status=(inportb(0x60) | 1);
	keyboard_wait(1);
	outportb(0x64, 0x60);
	keyboard_wait(1);
	outportb(0x60, _status);
 
	//Tell the keyboard to use default settings
	/* while((inportb(0x64)&2)!=0){};*/
	// outportb(0x64, 0xF6);
	//keyboard_read();  //Acknowledge
 
	//Enable the keyboard
	while ((inportb(0x64) & 2) != 0) {
		inportb(0x60);
	}; 


	set_evec(1 + IRQBASE, (uint32)kbdhandlerirq);
	//outportb(0x64, 0xF4);
	//keyboard_read();  //Acknowledge

	return OK;
}
