//Ejercicio 1 2doParcial
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
//Aca guardo la imagen salida

//Primeros 15 bytes de imagen es la cabecera, lo almaceno en arreglo cabecera
unsigned char cabecera[15];
int in_fd;

void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

int calculoTamaño(){
    char c;
    int i=1;
    int tamaño=0;
    //Si read devuelve 0(i==0) entonces llegamos al final del archivo leido
    while(i!=0){
        //read lee un byte y avanza un byte
        i=read(in_fd,&c,1);
        tamaño=tamaño+1;
    }
    return tamaño;
}

void main(){
    //Abro archivo
    in_fd=open("cat.pgm", O_RDONLY);
    if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}
    int out_fd=open("cat2.pgm", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}
    //Leo la cabecera del archivo original (15 bytes)
    read(in_fd,&cabecera[0],15);
    //Escribo la cabecera en el archivo de salida
    write(out_fd,&cabecera[0],15);
    //Obtengo tamaño de la imagen
    int tamañoImagen=calculoTamaño();
    unsigned char nueva_imagen[tamañoImagen+1];
    //Me posiciono despues de la cabecera en el archivo original
    lseek(in_fd,15,SEEK_SET);

    for(int i=tamañoImagen; i>=0; i--){
        read(in_fd,&nueva_imagen[i],1);
    }
    //Escribo en archivo salida
    write(out_fd,&nueva_imagen[0],tamañoImagen);
    close(in_fd);
    close(out_fd);
}