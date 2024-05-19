#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"
#define NUM_HIJOS 3

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

unsigned char nueva_imagen[2000 * 2000 * 3];

void convertir(int in_fd, int out_fd, BMPHeader h, BMPInfoHeader infoh, int inicio_fila, int fin_fila) {

    int width = infoh.width;
    int height = infoh.height;
    unsigned char gray;

    int padding = (4 - (width * 3) % 4) % 4; // Calculating padding size
    unsigned char pixel[3];

    lseek(in_fd, h.offset, SEEK_SET); // Mover el cursor al inicio de los datos de la imagen

    // Saltar filas hasta el inicio de la sección asignada a este hijo
    for (int i = 0; i < inicio_fila; i++) {
        lseek(in_fd, (width * 3 + padding), SEEK_CUR);
    }

    for (int y = inicio_fila; y <= fin_fila; y++) {
        for (int x = 0; x < width; x++) {
            read(in_fd, pixel, 3);
            gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
            nueva_imagen[y * (width * 3 + padding) + x * 3] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 1] = gray;
            nueva_imagen[y * (width * 3 + padding) + x * 3 + 2] = gray;
        }
        lseek(in_fd, padding, SEEK_CUR); // Skipping padding
    }
}

int main() {
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0) {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }

    BMPHeader h;
    read(in_fd, &h, sizeof(BMPHeader));

    if (h.magic[0] != 'B' || h.magic[1] != 'M') {
        printf("El archivo no es un BMP\n");
        exit(EXIT_FAILURE);
    }

    BMPInfoHeader infoh;
    read(in_fd, &infoh, sizeof(BMPInfoHeader));

    if (infoh.bpp != 24 || infoh.compression != 0) {
        printf("El archivo BMP no es de 24 bits o no está sin comprimir\n");
        exit(EXIT_FAILURE);
    }

    // Creamos tres hijos
    for (int i = 0; i < NUM_HIJOS; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Proceso hijo
            int inicio_fila = i * (infoh.height / NUM_HIJOS);
            int fin_fila = (i + 1) * (infoh.height / NUM_HIJOS) - 1;
            if (i == NUM_HIJOS - 1) {
                // Para el último hijo, ajustamos la cantidad de filas si no es divisible
                fin_fila = infoh.height - 1;
            }

            int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (out_fd < 0) {
                perror("Error al abrir el archivo de salida");
                exit(EXIT_FAILURE);
            }

            convertir(in_fd, out_fd, h, infoh, inicio_fila, fin_fila);

            close(in_fd);
            close(out_fd);
            exit(EXIT_SUCCESS);
        }
    }

    // Esperamos a que todos los hijos terminen
    for (int i = 0; i < NUM_HIJOS; i++) {
        wait(NULL);
    }

    // Escribimos la nueva imagen en escala de grises
    int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        perror("Error al abrir el archivo de salida");
        exit(EXIT_FAILURE);
    }

    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));
    write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);

    close(in_fd);
    close(out_fd);

    printf("Imagen en gris generada: %s\n", GRAYSCALE_FILE);

    return 0;
}
