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
    int inputFile, outputFile, com2tocom1[2];
    char *command1, *command2;

    // Si no es el numero de argumentos pedidos, salir.
    if(argc != 4)
    {
        perror("Numero incorrecto de argumentos\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[2], "|") != 0)
    {
        perror("Segundo argumento incorrecto.\n");
        exit(EXIT_FAILURE);
    }    

    command1 = argv[1];
    command2 = argv[3];

    pid_t pid;

    if((pid = fork()) < 0)
    {
        printf("Error en fork\n");
        exit(EXIT_FAILURE);
    }
    
    if(pipe(com2tocom1) < 0)
    {
        printf("Error en pipe\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0) // Hijo
    {
        
        close(STDIN_FILENO);
        fcntl(com2tocom1[STDOUT_FILENO], )
    }
    else // Padre
    {

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