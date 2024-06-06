
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
extern  buffer_t bufferChar;	//Buffer tamaño 10, guarda 10 caracteres