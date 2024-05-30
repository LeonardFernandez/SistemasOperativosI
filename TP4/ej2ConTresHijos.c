
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"

#pragma pack(push, 1)
typedef struct {
	unsigned char magic[2];
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset;
} BMPHeader;

typedef struct {
	unsigned int headerSize;
	int width;
	int height;
	unsigned short planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int imageSize;
	int xPixelsPerM;
	int yPixelsPerM;
	unsigned int colorsUsed;
	unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

unsigned char nueva_imagen [2000*2000*3];

void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

char convertir(int in_fd, int out_fd, BMPHeader h, BMPInfoHeader infoh,int inicio, int fin) 
{
	int width = infoh.width;
	int height = infoh.height;
	unsigned char gray;

	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	unsigned char pixel[3];
	//Me posiciono justo despues de la cabecera en ambos archivos
	lseek(in_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader)+inicio, SEEK_CUR);
	lseek(out_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader)+inicio, SEEK_CUR);
	
	for (int y = 0; y < height/3; y++) {
		for (int x = 0; x < width; x++) {
			read(in_fd, pixel, 3);
			gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
			nueva_imagen[y*(width*3+padding) + x*3]= gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 1] = gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 2] = gray;
		}
		lseek(in_fd, padding, SEEK_CUR); // Skipping padding
	}
	//Escribo en el archivo salida
	write(out_fd, &nueva_imagen[0], width * height);
}

int main() 
{
	int in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}

	BMPHeader h;
	read(in_fd, &h, sizeof(BMPHeader));

	if (h.magic[0] != 'B' || h.magic[1] != 'M') {
		something_wrong(in_fd, "No es BMP");
	}

	BMPInfoHeader infoh;
	read(in_fd, &infoh, sizeof(BMPInfoHeader));

	if (infoh.bpp != 24 || infoh.compression != 0) {
		something_wrong(in_fd, "No 24-bit BMP");
	}

	int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}
	//Copio la cabecera al archivo de salida
	write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
    int pid1,pid2,pid3;
    pid1 = fork();
    if(pid1==0){
        int inicio=0;
        int fin =354;
		//Abro la imagen a convertir
		int in_fd1 = open(BMP_FILE, O_RDONLY);
		//Abro de nuevo el archivo de salida BMP
	    int out_fd1 = open(GRAYSCALE_FILE, O_WRONLY, 0644); 
        //convertir(in_fd1,out_fd1,h,infoh,inicio,fin);
		close(in_fd1);
		close(out_fd1);
    }else{//Ejecuta padre
		pid2=fork();
        if(pid2==0){
            int inicio=354;
            int fin =708;
            //Abro la imagen a convertir
            int in_fd2 = open(BMP_FILE, O_RDONLY);
            //Abro de nuevo el archivo de salida BMP
            int out_fd2 = open(GRAYSCALE_FILE, O_WRONLY, 0644); 
            convertir(in_fd2,out_fd2,h,infoh,inicio,fin);
            close(in_fd2);
            close(out_fd2);
        }else{//Ejecuta padre
            pid3=fork();
            if(pid3==0){
                int inicio=708;
                int fin =1063;
                //Abro la imagen a convertir
                int in_fd1 = open(BMP_FILE, O_RDONLY);
                //Abro de nuevo el archivo de salida BMP
                int out_fd1 = open(GRAYSCALE_FILE, O_WRONLY, 0644); 
                //convertir(in_fd1,out_fd1,h,infoh,inicio,fin);
                close(in_fd1);
                close(out_fd1);
            }else{//Ejecuta ṕadre
                wait(NULL);
                wait(NULL);
                wait(NULL);
                close(in_fd);
                close(out_fd);
                printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
            }
        }
	}

    /*Ejecuta padre
        wait(NULL);
		close(in_fd);
		close(out_fd);
        printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
		exit(0);*/
}