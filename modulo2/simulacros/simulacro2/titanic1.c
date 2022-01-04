#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>


// Limite de los tipos de datos
#include <limits.h>

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

        execlp("cut", "cut", "-d" ,";", "-f", "6" , "titanic.csv", NULL);

    }
    // Padre
    else 
    { 
        close(cause[1]);
    	dup2(cause[0],STDIN_FILENO);    

        char buffer[MAX_SIZE];
        
        char *concatBuffer = malloc(256 * sizeof(char *));
        char *output = malloc(256 * sizeof(char *));

        strcpy(concatBuffer, "");

        int actAge;
        int counter = 0;
        float avgAge;
        int youngest = INT_MAX;
        int oldest = -1;
        int countLess5yrs = 0;

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
                actAge = atoi(concatBuffer);

                avgAge += (float) actAge;

                counter++;

                if(actAge > oldest)
                {
                    oldest = actAge;
                }

                if(actAge < youngest)
                {
                    youngest = actAge;
                }

                if(actAge < 5)
                {
                    countLess5yrs++;
                }
                strcpy(concatBuffer, "");
            }
        }

        avgAge /= counter;

        if( (fileDecrp = open("./resultado_titanic.txt", O_CREAT | O_WRONLY | O_TRUNC, 0660)) < 0)
        {
            printf("Error en Open\n");
            exit(-1);
        }

        sprintf(output, "Total personas: %d\nEdad media: %0.2f\nMin. Edad: %d\nMax. Edad: %d\nMenores de 5: %d", counter, avgAge, youngest, oldest, countLess5yrs);
        write(fileDecrp, output, strlen(output));

    }
    return EXIT_SUCCESS;
}



 
 
 
