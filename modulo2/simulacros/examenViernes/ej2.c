
// Tipos de dato primitivos e info de ficheros en directorio
#include<sys/types.h>

// Constantes.
#include<unistd.h>

// Referente a ficheros y permisos.
#include<sys/stat.h>

// Utilizar el wait() para los procesos.
#include<sys/wait.h>

// Para el open(), fcntl() y banderas.
#include<fcntl.h>

// E/S Estándar.
#include<stdio.h>

// Errores.
#include<errno.h>

// Biblioteca estándar.
#include<stdlib.h>

// Manipulación de cadenas.
#include<string.h>

// Abrir directorios
#include <dirent.h>

#define PERM 0700
#define THE_GOOD_CHECK(mode) (S_ISREG(mode)) &&  ((mode & ~S_IFMT) == PERM)
#define CHECK_TYPE_FILE(mode) (S_ISREG(mode)) && (((mode) & PERM) == PERM)

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

        execlp("ls", "ls", "-i", NULL);

    }
    // Padre
    else 
    { 
        // Cerrando salida del cause.
        close(cause1[1]);
    	dup2(cause1[0],STDIN_FILENO);    

        char buffer[MAX_SIZE];
        
        char *bufferConcat = malloc(256 * sizeof(char*));
        char *iNodeInfo = malloc(256 * sizeof(char*));
        char *fileInfo = malloc(256 * sizeof(char*));
        char *output = malloc(256 * sizeof(char*));

        strcpy(bufferConcat, "");
        strcpy(iNodeInfo, "");
        strcpy(fileInfo, "");
        strcpy(output, "");

        int count = 0;
        int fileCount = 0;
        int numBytes;
        int fileDcrp;

        int biggestInode = -1;
        int actInode;

        while (numBytes = read(cause1[0], buffer, MAX_SIZE))
        {

            if(strcmp(buffer, "\n") == 0)
            {
                count = 0;
                sprintf(output,"/tmp/%s", fileInfo);

                if( (fileDcrp = open(output, O_CREAT | O_WRONLY | O_TRUNC, 0771)) < 0 )
                {
                    printf("Error en open()\n");
                    exit(EXIT_FAILURE);
                }

                if( (write(fileDcrp, iNodeInfo, strlen(iNodeInfo))) < 0 )
                {
                    printf("Error en write()\n");
                    exit(EXIT_FAILURE);
                }
                

                actInode = atoi(iNodeInfo);
                if(biggestInode < actInode)
                {
                    biggestInode = actInode;
                }

                fileCount++;

                strcpy(iNodeInfo, "");
                strcpy(fileInfo, "");

            }
            else
            {
                if(strcmp(buffer, " ") == 0)
                {
                    count++;
                }
                else
                {
                    if (count == 0)
                    {
                        strcat(iNodeInfo, buffer);
                    }
                    else
                    {
                        strcat(fileInfo, buffer);
                    }
                }
            }
        }

        int status;
        wait(&status);

        if( (fileDcrp = open("./resultados.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660)) < 0)
        {
            printf("Error en el open()\n");
            exit(EXIT_FAILURE);
        }



        sprintf(output, "Resultados:\ni-nodo mas alto: %d, numero de archivos: %d\n", biggestInode, fileCount);

        if( (write(fileDcrp, output, strlen(output))) < 0)
        {
            printf("Error en el write()\n");
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
} 
