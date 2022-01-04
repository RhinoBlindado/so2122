#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>

/**
 * @brief Verifica si se ha abierto correctamente el archivo
 * @param retVal Valor devuelto por open()
 */
void myOpen(int retVal)
{
    if(retVal < 0)
    {
        printf("\nError %d en open\n",errno);
        perror("\nError en open\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    int fd1, fd2;
    pid_t pid;

    // Archivos a crearse
    fd1 = open("ejer3_ARCHIVO_A.txt", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);
    fd2 = open("ejer3_ARCHIVO_B.txt", O_CREAT|O_TRUNC|O_WRONLY, S_IRUSR|S_IWUSR);

    struct flock lock1, lock2;
    pid = fork();


    if(pid)
    {
        printf("Proceso %d hijo", pid);

        lock1.l_type = F_WRLCK;
        lock1.l_whence = SEEK_SET;
        lock1.l_start = 0;
        lock1.l_len = 0;

        if (fcntl (fd1, F_SETLKW, &lock1) == -1) 
        {
            perror("Error al intentar bloquear archivo A\n");
	    }

        printf("Archivo A bloqueado");
        sleep(10);
        lock2.l_type = F_WRLCK;
        lock2.l_whence = SEEK_SET;
        lock2.l_start = 0;
        lock2.l_len = 0;

        if (fcntl (fd2, F_SETLKW, &lock2) == -1) 
        {
            perror("Error al intentar bloquear archivo B\n");
	    } 
    }
    else
    {
        printf("Proceso padre");

        lock1.l_type = F_WRLCK;
        lock1.l_whence = SEEK_SET;
        lock1.l_start = 0;
        lock1.l_len = 0;

        if (fcntl (fd2, F_SETLKW, &lock1) == -1) 
        {
            perror("Error al intentar bloquear B\n");
	    }

        printf("Archivo B bloqueado");
        sleep(10);
        lock2.l_type = F_WRLCK;
        lock2.l_whence = SEEK_SET;
        lock2.l_start = 0;
        lock2.l_len = 0;

        if (fcntl (fd1, F_SETLKW, &lock2) == -1) 
        {
            perror("Error al intentar bloquear A\n");
	    } 
    }



    return 0;
} 
