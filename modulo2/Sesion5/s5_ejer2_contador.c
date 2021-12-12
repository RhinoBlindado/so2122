#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

static int signalCounter[32] = { 0 };

static void sig_USR_hdlr(int sigNum)
{
    signalCounter[sigNum]++;
    printf("Señal %d recibida %d veces.\n", sigNum, signalCounter[sigNum]);
}

int main(int argc, char *argv[])
{
    struct sigaction sig_USR_nact;
    if(setvbuf(stdout,NULL,_IONBF,0)) 
    {    
        perror("\nError en setvbuf");
    }

    sig_USR_nact.sa_handler = sig_USR_hdlr;

    sigfillset (&sig_USR_nact.sa_mask);
    sig_USR_nact.sa_flags = 0;

    for(int i = 1; i < 32; i++)
    {
        if(i != 9 && i != 19)
        {
            if( sigaction(i ,&sig_USR_nact,NULL) <0) 
            {
                perror("\nError al intentar establecer el manejador de senal para");
                exit(EXIT_FAILURE);
            }
        }
    }

    for(;;){}

}