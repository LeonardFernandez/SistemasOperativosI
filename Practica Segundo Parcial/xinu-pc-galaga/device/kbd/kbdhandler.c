/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* kbdhandler.c - kbdhandler */
#include <xinu.h>
#include <keyboard.h>

//unsigned char tecla_actual;
unsigned char get_scancode()
{
    unsigned char inputdata;
    inputdata = inportb(KEYBOARD_DATAREG);
    return inputdata;
}


/*------------------------------------------------------------------------
 *  kbdhandler  -  Handle an interrupt for the keyboard device
 *------------------------------------------------------------------------
 */
void kbdhandler(void)
{
	char t[80];
	unsigned char scancode; 
	unsigned int shift_key = 0;
	int i = 10;
	//insertar(bufferChar,0);
	scancode = get_scancode();

	
	sprintf(t, "kbd: 0x%x     ", scancode);
	print_text_on_vga(10, 300, t);
	//Lee un byte, almacena en buffer y libera permiso buffer para getc
	//Verificar lleno
	//insertar(&bufferChar,scancode);
	send(pidTeclado,scancode);
	/*if(!estaLleno(&bufferChar)){
		insertar(&bufferChar,scancode);
		send(pidGetC,1);
	}*/
	

	
}
