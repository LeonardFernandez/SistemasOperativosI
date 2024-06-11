
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)


extern unsigned char kblayout [128];  // { ... } Fill your layout yourself 

typedef struct buffer
{
    unsigned char arreglo[10];
    int frente;
    int fin;
    int tamaño;
    /* data */
}buffer_t;
//Todos pueden ver bufferChar
extern  buffer_t bufferChar;	//Buffer tamaño 10, guarda 10 caracteres
extern pid32 pidTeclado; //pidProceso que accede al teclado
extern sid32 semaforoTeclado; //semaforo acceso teclado, solo un proceso accede teclado
extern sid32 semaforoBuffer; //Semaforo teclado, getc puede quitar char si hay permisos
