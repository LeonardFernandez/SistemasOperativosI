#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>	//mode constants
#include <fcntl.h>		//O_ constants

void main(){
	const int SIZE = 4096;	//tamanio de mem compartida
	int shm_fd;				//clave
	const char *name ="memoria compartida";
	const char *ruta ="/usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt";
	void *ptr;
	
	//Abrir-acceder a segmento de mem compartida
	shm_fd=shm_open(name, O_RDONLY,0666);
	if(shm_fd==-1){
		printf("error al acceder a segmento mem compartida \n");
		exit(-1);
	}
	//
	ptr=mmap(0,SIZE,PROT_READ,MAP_SHARED,shm_fd,0);
	if(ptr == MAP_FAILED){
		printf("Map failed \n");
		exit(-1);
	}
	//Mostramos contenido de memoria compartida
	printf("%s" ,(char *)ptr);
	//Eliminamos el segmento de mem compartida
	if(shm_unlink(name) == -1){
		printf("Error al eliminar segmento mem compartida\n");
		exit(-1);
	}
}
