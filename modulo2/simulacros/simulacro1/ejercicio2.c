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
        
        char *name = malloc(256 * sizeof(char*));
        char *perm = malloc(256 * sizeof(char*));
        char *size = malloc(256 * sizeof(char*));
        char *output = malloc(256 * sizeof(char *));

        strcpy(name, "");
        strcpy(perm, "");
        strcpy(size, "");

        int column = 0;
        int numBytes;
        int fileDecrp; 

        while (numBytes = read(cause[0], buffer, MAX_SIZE))
        {
            //buffer[numBytes] = '\0';
            if(strcmp(buffer, "|") != 0)
            {
                switch (column)
                {
                    case 0:
                        strcat(name, buffer);
                    break;
                    
                    case 1:
                        strcat(perm, buffer);
                    break;

                    case 2:
                        strcat(size, buffer);
                    break;
                }
            }
            else
            {
                column++;
            }

            if(strcmp(buffer, "\n") == 0)
            {
                strcpy(output, "/tmp/");
                strcat(output, name);
                
                if ((fileDecrp = open(output, O_CREAT|O_TRUNC|O_WRONLY, strtol(perm, NULL, 8))) < 0)
                {
                    printf("error en open\n");
                    exit(-1);
                }
                
                if (write(fileDecrp, size, strlen(size)) < 0)
                {
                    printf("error en write\n");
                    exit(-1);
                }
                close(fileDecrp);

                column = 0;
                stpcpy(name, "");
                stpcpy(perm, "");
                stpcpy(size, "");
            }
        }

    }
    return EXIT_SUCCESS;
}



 
 
