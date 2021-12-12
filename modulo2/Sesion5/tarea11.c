// tarea11.c

#include <stdio.h>
#include <signal.h>

int main()
{
    sigset_t new_mask;

    // Todas las señales con bit a 0
    sigemptyset(&new_mask);

    // Se pone el bit a 1 de SIGUSR1
    sigaddset(&new_mask, SIGUSR1);

    // Se desbloquea con cualquier señal que
    // tenga el bit a 0.
    sigsuspend(&new_mask);

}