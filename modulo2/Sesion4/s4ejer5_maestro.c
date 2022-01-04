#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include <math.h>


#define TRUE 1
#define FALSE 0

pid_t absFork()
{
    pid_t pid;

    if ( (pid= fork())<0 ) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return pid;
}

int main(int argc, char *argv[])
{
    int slave2master1[2], slave2master2[2], numBytes, minVal, maxVal, midVal, count = 0;
    pid_t pid;
    char buffer[10];

    if(argc < 3)
    {
        printf("Error: Faltan argumentos");
        exit(-1);
    }

    minVal = atoi(argv[1]);
    maxVal = atoi(argv[2]);

    midVal = floor((minVal + maxVal) / 2);
 
    pipe(slave2master1); // Llamada al sistema para crear un cauce sin nombre
    pipe(slave2master2);

    for(int i=0; i< 2; i++)
    {
        pid = absFork();
        if(!pid)
        {
            count = i;
            break;
        }
    }

    // Hijos
    if (pid == 0) 
    {
        char n[3];

        char lo[4];
        char hi[4];

        if (count == 0)
        {
            close(slave2master1[0]);
            dup2(slave2master1[1],STDOUT_FILENO);

            sprintf(lo, "%d", minVal);
            sprintf(hi, "%d", midVal);
            execlp("./s4ejer5_esclavo_EXE", "s4ejer5_esclavo_EXE", lo, hi, NULL);
        }
        else
        {
            close(slave2master2[0]);
            dup2(slave2master2[1],STDOUT_FILENO);

            sprintf(lo, "%d", midVal + 1);
            sprintf(hi, "%d", maxVal);
            execlp("./s4ejer5_esclavo_EXE", "s4ejer5_esclavo_EXE", lo, hi, NULL);

        }
        exit(EXIT_SUCCESS);
    }
    // Padre
    else 
    { 
        close(slave2master1[1]);
    	dup2(slave2master1[0],STDIN_FILENO);

        numBytes = read(slave2master1[0], buffer, 4);

        while (numBytes)
        {
            write(STDOUT_FILENO, buffer, 4);
            numBytes = read(slave2master1[0], buffer, numBytes);
        }

        close(slave2master2[1]);
    	dup2(slave2master2[0],STDIN_FILENO);

        numBytes = read(slave2master2[0], buffer, 4);

        while (numBytes)
        {
            write(STDOUT_FILENO, buffer, 4);
            numBytes = read(slave2master2[0], buffer, numBytes);
        }
    }
    return EXIT_SUCCESS;
}



 
