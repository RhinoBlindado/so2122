#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
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

char** obtener_datos(int total_datos, char* buf){
    char **datos = malloc(256 * sizeof(char*));;
    char *t;
    int i=0;
    t = strtok(buf, "|");
    while( t != NULL ) {
        datos[i] = t;
        t = strtok(NULL, "|");
        i++;
    }
    return datos;
}

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

        execlp("./ejercicio1_EXE", "./ejercicio1_EXE", "./ficheros/", NULL);

    }
    // Padre
    else 
    { 
        close(cause[1]);
    	dup2(cause[0],STDIN_FILENO);    

        char buffer[MAX_SIZE];
        
        char *accBuffer = malloc(256 * sizeof(char*));
        char *output = malloc(256 * sizeof(char *));

        strcpy(accBuffer, "");

        int numBytes;
        int fileDecrp; 
        while (numBytes = read(cause[0], buffer, MAX_SIZE))
        {
            if(strcmp(buffer, "\n") == 0)
            {
                char **datos=obtener_datos(3, accBuffer);
                char* nombre = datos[0];
                char* permisos = datos[1];
                char* tamanio = datos[2];

                strcpy(output, "/tmp/");
                strcat(output, nombre);
                
                if ((fileDecrp = open(output, O_CREAT|O_TRUNC|O_WRONLY, strtol(permisos, NULL, 8))) < 0)
                {
                    printf("error en open\n");
                    exit(-1);
                }
                
                if (write(fileDecrp, tamanio, strlen(tamanio)) < 0)
                {
                    printf("error en write\n");
                    exit(-1);
                }
                close(fileDecrp);
                
                strcpy(accBuffer, "");
            }
            else
            {
                strcat(accBuffer, buffer);
            }
        }

    }
    return EXIT_SUCCESS;
}



 
 
