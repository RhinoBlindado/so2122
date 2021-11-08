#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
    int num = atoi(argv[1]);
    pid_t pid;

    if(fork() == 0)
    {
        if(num % 2 == 0)
        {
            printf("[Hijo] El numero es par\n");
        }
        else
        {
            printf("[Hijo] El numero es impar\n");
        }
    }
    else
    {

        if(num % 4 == 0)
        {
            printf("[Padre] El numero es divisible entre 4\n");
        }
        else
        {
            printf("[Padre] El numero no es divisible entre 4\n");
        }

    }

}
 
