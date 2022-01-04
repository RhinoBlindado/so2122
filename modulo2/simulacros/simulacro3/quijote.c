#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

#include<sys/wait.h>

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


#define MAX_SIZE 1

int main(int argc, char *argv[])
{
    int cause[2];
    pid_t pid;

    umask(0);
    pipe(cause); // Llamada al sistema para crear un cauce sin nombre

    pid = absFork();

    // Hijos
    if (pid == 0) 
    {
 
        close(cause[0]);
        dup2(cause[1],STDOUT_FILENO);

        execlp("./tam_quijote_EXE", "./tam_quijote_EXE", "./ficheros/", NULL);
        printf("Error de Execlp\n");
        exit(-1);
    }
    // Padre
    else 
    { 
        close(cause[1]);
    	dup2(cause[0],STDIN_FILENO);    

        char buffer[MAX_SIZE];
        
        char *concatBuffer = malloc(256 * sizeof(char*));
        char *output = malloc(256 * sizeof(char*));

        strcpy(concatBuffer, "");
        strcpy(output, "");

        int column = 0;
        int numBytes;
        int fileDecrp; 

        while (numBytes = read(cause[0], buffer, MAX_SIZE))
        {
            buffer[MAX_SIZE] = '\0';

            if(strcmp(buffer, "\n") != 0)
            {
                strcat(concatBuffer, buffer);
            }
            else
            {
                break;
            }
        }

        int status;
        wait(&status);

        if((fileDecrp = open("./resultado.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660)) < 0 )
        {
            printf("Error en open\n");
            exit(-1);
        }

        sprintf(output,"Tamaño quijote.txt: %s\n", concatBuffer);

        if(write(fileDecrp, output, strlen(output)) < 0)
        {
            printf("Error en write\n");
            exit(-1);
        }

        pid = absFork();
        int cause_wc[2];

        pipe(cause_wc);
        // Hijos
        if (pid == 0) 
        {
    
            close(cause_wc[0]);
            dup2(cause_wc[1], STDOUT_FILENO);

            execlp("wc", "wc", "-w", "./ficheros/quijote.txt" ,NULL);
            printf("Error de Execlp\n");
            exit(-1);
        }
        // Padre
        else 
        {
            close(cause_wc[1]);
    	    dup2(cause_wc[0], STDIN_FILENO);   

            while (numBytes = read(cause_wc[0], buffer, MAX_SIZE))
            {
                buffer[MAX_SIZE] = '\0';

                if(strcmp(buffer, " ") != 0)
                {
                    strcat(concatBuffer, buffer);
                }
                else
                {
                    break;
                }
            }

            sprintf(output, "\n\nPalabras: %s", concatBuffer);
            if(write(fileDecrp, output, strlen(output)) < 0)
            {
                printf("Error en write2\n");
                exit(-1);
            }

            wait(&status);
            close(fileDecrp);

        }

    }

    return EXIT_SUCCESS;
}



 
 
 
