#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

// Manipulacion de strings
#include <string.h>


void main(int argc, char *argv[])
{

    if(argc < 2)
    {
        printf("Error en parámetros\n");
        exit(EXIT_FAILURE);
    }

    char *progName;
    int isBackground = 0;

    progName = argv[1];

    if(strcmp(argv[argc - 1], "bg") == 0)
    {
        isBackground = 1;
    }

    char **progParams;
    int sizeList = argc - 1 - isBackground;
    progParams = malloc(sizeList * sizeof(char*));

    for (int i = 1; i < argc - isBackground; i++)
    {
        progParams[i-1] = malloc((strlen(argv[i]) + 1) * sizeof(char));
        strcpy(progParams[i-1] ,argv[i]);
    }
    
    pid_t pid;

    if((pid = fork()) < 0)
    {
        printf("Error en el fork.\n");
        exit(EXIT_FAILURE);
    }

    // Hijo
    if(pid == 0)
    {
        execvp(progName, progParams);

    }
    // Padre
    else
    {
        int status;
	    wait(&status);
    }

    for (int i = 0; i < sizeList; i++) free(progParams[i]);
    free(progParams);

}
 
 
