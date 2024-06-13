#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h> 
//Tamaño imagen
void main(){
    char SHM_NAME[] = "pgm_invert";
    unsigned char *mapeoImagen;
    //Abro archivo imagen
    int in_fd = open("cat.pgm", O_RDONLY);
    if(in_fd<0){
        printf("Error open");
    }else{
        int pid=getpid();
        printf("Exito open %d\n",pid);
    }
    size_t tamanioBytes = lseek(in_fd, 0, SEEK_END);
    printf("%d pid: %d \n",tamanioBytes,getpid());
    //Vuelvo al principio el puntero de archivo
    lseek(in_fd,0,SEEK_SET);
    int shmPadre = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shmPadre,tamanioBytes);
    mapeoImagen = mmap(0,tamanioBytes, PROT_READ | PROT_WRITE, MAP_SHARED, shmPadre, 0);
    
    
    //Leo imagen y la pongo en memoria compartida
    read(in_fd,mapeoImagen,tamanioBytes);
    int pid=fork();
    if(pid==0){
        //Ejecuta hijo
        int in_fd = open("cat.pgm", O_RDONLY);
        size_t tamanioBytes = lseek(in_fd, 0, SEEK_END);
        //Vuelvo al principio el puntero de archivo
        lseek(in_fd,0,SEEK_SET);
        unsigned char *mapeoImagen;
        char SHM_NAME[] = "pgm_invert";
        int shmHijo = shm_open(SHM_NAME, O_RDONLY, 0666);
        //Leo imagen despositada en memoria compartida
        mapeoImagen = mmap(0,tamanioBytes, PROT_READ, MAP_SHARED, shmHijo, 0);

        //Creo archivo salida
        int out_fd = open("catOut.pgm", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        write(out_fd,mapeoImagen,tamanioBytes);
        close(out_fd);
        close(in_fd);
    }else{
        //Ejecuta padre
        //Espero que termine hijo
        wait(NULL);
        shm_unlink(SHM_NAME);
        close(in_fd);
    }
    //shm_unlink(SHM_NAME);
    //close(in_fd);
}