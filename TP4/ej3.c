#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

//Struct de cada thread que incluye su informacion para la conversion
typedef struct {
    int thread_id;
    int filaInicio;
	int input;
} ThreadData;

unsigned char nueva_imagen [2000*2000*3];
int in_fd;
int out_fd;
BMPHeader h;
BMPInfoHeader infoh;

void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

void *convertir(void *arg) 
{
	ThreadData *data = (ThreadData *)arg;
	int width = infoh.width;
	int height = infoh.height;
	unsigned char gray;
	int filaIni= data->filaInicio;
	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	unsigned char pixel[3];
	//Me posiciono justo despues de la cabecera en el archivo entrada
	lseek(data->input, sizeof(BMPHeader) + sizeof(BMPInfoHeader)+(filaIni*width*3), SEEK_CUR);

	for (int y = filaIni; y < filaIni+height; y++) {
		for (int x = 0; x < width; x++) {
			read(data->input, pixel, 3);
			gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
			nueva_imagen[y*(width*3+padding) + x*3]= gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 1] = gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 2] = gray;
		}
		lseek(data->input, padding, SEEK_CUR); // Skipping padding
	}
	pthread_exit(NULL);
}

void main(){
    pthread_t threads[3];   //tres threads
    ThreadData data[3];		//Argumentos, datos para los threads
    
    //Inicio Chequeo
    in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}	
	read(in_fd, &h, sizeof(BMPHeader));
	if (h.magic[0] != 'B' || h.magic[1] != 'M') {
		something_wrong(in_fd, "No es BMP");
	}
	read(in_fd, &infoh, sizeof(BMPInfoHeader));
	if (infoh.bpp != 24 || infoh.compression != 0) {
		something_wrong(in_fd, "No 24-bit BMP");
	}
    out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0) {
		something_wrong(in_fd, "error en output");
	}
    //Fin Chequeo
	//Division de filas para cada thread
	int height = infoh.height;
	int rows_per_thread = height / 3;
	//Archivos lectura input
	int inFD0=open(BMP_FILE, O_RDONLY);
	int inFD1=open(BMP_FILE, O_RDONLY);
	int inFD2=open(BMP_FILE, O_RDONLY);
	data[0].input=inFD0;
	data[1].input=inFD1;
	data[2].input=inFD2;
	for(int i=0; i<3;i++){
		data[i].filaInicio=i*rows_per_thread;
	}

	//Padre escribe la cabecera
	write(out_fd, &h, sizeof(BMPHeader));
	write(out_fd, &infoh, sizeof(BMPInfoHeader));
	

    //Threads se encargan de la conversion
    pthread_create(&threads[0],NULL,convertir,(void *) &data[0]);
    pthread_create(&threads[1],NULL,convertir,(void *)&data[1]);
    pthread_create(&threads[2],NULL,convertir,(void *)&data[2]);
    //Esperamos a que terminen los threads
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    //Main escribe la imagen
	write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);

}


