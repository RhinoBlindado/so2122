#include<sys/types.h>	
#include<sys/wait.h>	
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include <stdlib.h>


void main(int argc, char *argv[])
{
    int numProcs = 5, aliveChildren=1, status;
    pid_t pid, pidReturn, pidList[5];

    for(int i=0; i< numProcs; i++)
    {
        pid = fork();
        if(!pid)
        {
            printf("Soy el hijo (%d) %d\n",i+1, getpid());
            sleep(1);
            break;
        }
        else
        {
            pidList[i] = pid;
        }
    }

    if(pid)
    {        
        for(int i = 0; i <= 4; i+=2)
        {
            pidReturn = waitpid(pidList[i], &status, 0);
            printf("Acaba de finalizar el hijo (%d) %d\n", i, pidReturn);
            numProcs--;
        }

        while(aliveChildren)
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

}
 
 
