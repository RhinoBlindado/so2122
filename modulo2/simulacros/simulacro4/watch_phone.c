#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<sys/wait.h>

#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

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
    int cause1[2];
    pid_t pid;

    if(argc > 2)
    {
        printf("Error en numero de parametros\n");
        exit(-1);
    }


    umask(0);
    pipe(cause1); // Llamada al sistema para crear un cauce sin nombre

    pid = absFork();

    // Hijos
    if (pid == 0) 
    {
        // Cerrando lectura del cause.
        close(cause1[0]);

        // Cerrando salida estandar, redigiriendo al cause1.
        dup2(cause1[1],STDOUT_FILENO);

        execlp("./explorar_datos_EXE", "./explorar_datos_EXE", "./watch_phone/", NULL);

    }
    // Padre
    else 
    { 
        // Cerrando salida del cause.
        close(cause1[1]);
    	dup2(cause1[0],STDIN_FILENO);    

        char buffer[MAX_SIZE];
        
        char *bufferConcat = malloc(256 * sizeof(char*));
        char *output = malloc(256 * sizeof(char*));

        strcpy(bufferConcat, "");

        int count = 0;
        int numBytes;
        char **pathList = malloc(32 * sizeof(char*));

        while (numBytes = read(cause1[0], buffer, MAX_SIZE))
        {
            //buffer[numBytes] = '\0';

            if(strcmp(buffer, "\n") != 0)
            {
                strcat(bufferConcat, buffer);
            }
            else
            {
                pathList[count] = malloc((strlen(bufferConcat) + 1) * sizeof(char));
                strcpy(pathList[count], bufferConcat);
                strcpy(bufferConcat, "");
                count++;
            }
        }

        int cause2[2];
        
        for (int i = 0; i < count; i++)
        {

            pipe(cause2);
            pid = absFork();

            if(pid == 0)
            {
                // Cerrando el cause de lectura.
                close(cause2[0]);

                // Duplicando el cause de escritura.
                dup2(cause2[1],STDOUT_FILENO);

                execlp("cut", "cut", "-d", ",", "-f", argv[1], pathList[i], NULL);
                printf("Error en exec2\n");
                exit(-1);
            }
            else
            {
                close(cause2[1]);
                dup2(cause2[0], STDIN_FILENO);

                sprintf(output, "Fichero: %s\n", pathList[i]);
                write(STDOUT_FILENO, output, strlen(output));

                while (numBytes = read(cause2[0], buffer, MAX_SIZE))
                {
                    write(STDOUT_FILENO, buffer, MAX_SIZE);
                }
                sprintf(output, "\n");
                write(STDOUT_FILENO, output, strlen(output));
            }
        }
    }

    return EXIT_SUCCESS;
} 
