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
	
	shm_fd=shm_open(name, O_CREAT | O_RDWR, 0666); //se crea segmento mem compartida
	ftruncate(shm_fd, SIZE);	//configura tamanio segmento mem compartida
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd,0);
	if(ptr == MAP_FAILED){
		printf("Map failed \n");
		exit(1);
	}
	int file_fd = open(ruta, O_RDONLY);	//Abrimos archivo de txt en modo lectura
	if(file_fd == -1){
		printf("Error al abrir archivo\n");
		exit(1);
	}
	
	ssize_t bytes_read=read(file_fd, ptr,SIZE);	//Lee file_fd y guarda en ptr
	if(bytes_read==-1){
		printf("Error al leer y almacenar archivo\n");
		exit(1);
	}
}
