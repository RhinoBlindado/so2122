#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>


void main(int argc, char *argv[])
{
    int numProcs = 5, aliveChildren=1, status;
    pid_t pid, pidReturn;

    for(int i=0; i< numProcs; i++)
    {
        pid = fork();
        if(!pid)
        {
            printf("Soy el hijo (%d) %d\n",i+1, getpid());
            sleep(1);
            break;
        }
    }

    while(aliveChildren && pid)
    {
       pidReturn = wait(&status);
       printf("Acaba de finalizar el hijo %d\n", pidReturn);
       numProcs--;
       printf("Quedan %d hijos vivos\n", numProcs);
       if(numProcs == 0)
       {
           aliveChildren = 0;
       }
    }

}
 
 
