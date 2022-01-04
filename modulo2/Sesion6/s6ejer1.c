#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

/**
 * @brief Verifica si se ha abierto correctamente el archivo
 * 
 * @param retVal Valor de retorno de open()
 */
void isOpenErr(int retVal)
{
    if(retVal < 0)
    {
        printf("\nError %d en open\n",errno);
        perror("\nError en open\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int inputFile, outputFile, pipeType;
    char *command, *fileName;

    // Si no es el numero de argumentos pedidos, salir.
    if(argc != 4)
    {
        perror("Numero incorrecto de argumentos\n");
        exit(EXIT_FAILURE);
    }

    command = argv[1];
    fileName = argv[3];

    if (strcmp(argv[2], ">") == 0)
    {
        pipeType = STDOUT_FILENO;
    }
    else if (strcmp(argv[2], "<") == 0)
    {
        pipeType = STDIN_FILENO;
    }
    else
    {
        perror("Segundo argumento incorrecto.\n");
        exit(EXIT_FAILURE);
    }    


    /**
     * 1 Indica cambiar entrada estandar
     * 0 Indica cambiar salida estandar
     */
    if(pipeType)
    {

        outputFile = open(fileName, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
        isOpenErr(outputFile);
        close(STDOUT_FILENO);
    }
    else
    {
        outputFile = open(fileName, O_CREAT | O_RDONLY, S_IRUSR | S_IWUSR);
        isOpenErr(outputFile);
        close(STDIN_FILENO);
    }

    if(dup2(outputFile, pipeType) == -1)
    {
        printf("Error en fcntl\n");
        exit(EXIT_FAILURE);
    }

    // Se ejecuta la orden
    execlp(command, command, NULL);

    close(outputFile);

    return 0;
}