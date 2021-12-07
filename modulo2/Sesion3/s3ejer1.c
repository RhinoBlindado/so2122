#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Error en número de parámetros\n");
        exit(EXIT_FAILURE);
    }

    int num = atoi(argv[1]);
    pid_t pid;

    if((pid = fork()) < 0)
    {
        printf("Error en el fork.\n");
        exit(EXIT_FAILURE);
    }

    if(pid == 0)
    {
        if(num % 2 == 0)
        {
            printf("[Hijo] El número es par\n");
        }
        else
        {
            printf("[Hijo] El número es impar\n");
        }
    }
    else
    {
        if(num % 4 == 0)
        {
            printf("[Padre] El número es divisible entre 4\n");
        }
        else
        {
            printf("[Padre] El número no es divisible entre 4\n");
        }

    }

}
 
