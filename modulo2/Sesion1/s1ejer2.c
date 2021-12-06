#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

/**
 * @brief Verifica si se ha abierto correctamente el archivo, 
 * si el valor de retorno de open() es negativo, se genera un error y finaliza el programa.
 * @param retVal Valor devuelto por open().
 */
void checkOpen(int retVal)
{
    if(retVal < 0)
    {
        printf("\nError %d en open",errno);
        perror("\nError en open");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[])
{
    int inputFile, outputFile;
    char* buffer[80];

    // Si se pasa un parametro al programa...
    if(argc > 1)
    {
        //... se abre en modo lectura.
        checkOpen( (inputFile = open(argv[1], O_RDONLY)) );
    }
    // Sino, se utiliza la entrada estandar.
    else
    {
        inputFile = STDIN_FILENO;
    }

    // Se abre/crea el fichero de salida en modo de escritura, se trunca si tiene algo.
    checkOpen( (outputFile = open("salida.txt", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR)) );

    int counter = 1;
    char n[3];
    int bytesRead;

    bytesRead = read(inputFile, buffer, 80);
    lseek(outputFile, 80, SEEK_SET);
    while(bytesRead)
    {
        write(outputFile, "Bloque ", strlen("Bloque "));
        sprintf(n, "%d", counter);
        write(outputFile, n, strlen(n));
        write(outputFile, ":\n", strlen(":\n"));
        write(outputFile, buffer, bytesRead);
        write(outputFile, "\n", strlen("\n"));

        counter++;
        bytesRead = read(inputFile, buffer, 80);
    }

    lseek(outputFile, 0, SEEK_SET);
    write(outputFile, "El numero de bloques es ", strlen("El numero de bloques es "));
    write(outputFile, n, strlen(n));
    write(outputFile, "\n", strlen("\n"));

    close(inputFile);
    close(outputFile);
}