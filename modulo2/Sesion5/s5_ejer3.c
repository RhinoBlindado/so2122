// tarea11.c

#include <stdio.h>
#include <signal.h>

int main()
{
    sigset_t new_mask;

    // Se inicializa la máscara con todos los valores en 1.
    sigfillset(&new_mask);

    // Se quita el valor de SIGUSR1
    sigdelset(&new_mask, SIGUSR1);

    // Se desbloquea solamente al recibir SIGUSR1
    // porque es la única con el bit a 0.
    sigsuspend(&new_mask);

}