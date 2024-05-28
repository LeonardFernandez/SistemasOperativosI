// Ejercicio 2 a y b
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


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

void convertir(int in_fd, int out_fd, BMPHeader h, BMPInfoHeader infoh, int num) 
{

	int width = infoh.width;
	int height = infoh.height;
	unsigned char gray;

	int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
	unsigned char pixel[3];
	
	lseek(in_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader), SEEK_CUR);
	lseek(in_fd, num * width * height, SEEK_CUR);
	
	lseek(out_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader) + num * width * height , SEEK_CUR);

	for (int y = 0; y < (height/3) ; y++) {
		for (int x = 0; x < width; x++) {
			read(in_fd, pixel, 3);
			gray = ( pixel[2] + pixel[1] + pixel[0]) / 3;
			nueva_imagen[y*(width*3+padding) + x*3]= gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 1] = gray;
			nueva_imagen[y*(width*3+padding) + x*3 + 2] = gray;
		}
		lseek(in_fd, padding, SEEK_CUR); // Skipping padding
	}
	
	write(out_fd, &nueva_imagen[0], width * height);
}

void something_wrong(int fd, const char *m)
{
	if (fd > 0)
		close(fd);
	printf("%s\n", m),
	exit(1);
}

int main() 
{       
    int pid_hijo1, pid_hijo2, pid_hijo3, status;
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
	
    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
	
	pid_hijo1 = fork(); 
	if(pid_hijo1 == 0){
	        int in_fd1 = open(BMP_FILE, O_RDONLY);
	        int out_fd1 = open(GRAYSCALE_FILE, O_WRONLY, 0644);
	        convertir(in_fd1, out_fd1, h, infoh, 2);
	        if (out_fd1 < 0) {
		      something_wrong(in_fd1, "error en output");
	        }
	        //lseek(out_fd1, infoh.width * infoh.height * 2, SEEK_SET);
	        //write(out_fd1, &nueva_imagen[0], infoh.width * infoh.height);
	        close(in_fd1);
	        close(out_fd1);
	}else{  // Padre
	        pid_hijo2 = fork(); 
	        if(pid_hijo2 == 0){
	                int in_fd2 = open(BMP_FILE, O_RDONLY);
	                int out_fd2 = open(GRAYSCALE_FILE, O_WRONLY, 0644);
	                convertir(in_fd2, out_fd2, h, infoh, 1);
	                if (out_fd2 < 0) {
		              something_wrong(in_fd2, "error en output");
	                }
	                //lseek(out_fd2, infoh.width * infoh.height, SEEK_SET);
	                //write(out_fd2, &nueva_imagen[0], infoh.width * infoh.height);
	                close(in_fd2);
	                close(out_fd2);
	        }else{  // Padre
	                pid_hijo3 = fork(); 
	                if(pid_hijo3 == 0){
	                      int in_fd3 = open(BMP_FILE, O_RDONLY);
	                      int out_fd3 = open(GRAYSCALE_FILE, O_WRONLY, 0644);
	                      convertir(in_fd3, out_fd3, h, infoh, 0);
	                      if (out_fd3 < 0) {
		                      something_wrong(in_fd3, "error en output");
	                      }
	                      //lseek(out_fd3, 0, SEEK_SET);
	                      //write(out_fd3, &nueva_imagen[0], infoh.width * infoh.height);
	                      close(in_fd3);
	                      close(out_fd3);
	                }else{
	                      waitpid(pid_hijo1,&status,0);
	                      waitpid(pid_hijo2,&status,0);
	                      waitpid(pid_hijo3,&status,0);
	                      
	                      
	                      close(in_fd);
	                      close(out_fd);
	                      
	                      kill(pid_hijo1, SIGTERM);
	                      kill(pid_hijo2, SIGTERM);
	                      kill(pid_hijo3, SIGTERM);

	                      printf("Imagen en gris generada. %s\n", GRAYSCALE_FILE);
	                      exit(EXIT_SUCCESS);
	                }
	          
	        }
	 
	 }
}



