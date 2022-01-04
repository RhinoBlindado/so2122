#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ftw.h>

#define TAM 100


char** obtener_datos(int total_datos, char* buf){
    char *datos[total_datos];
    char *t;
    int i=0;
    t = strtok(buf, "|");
    while( t != NULL ) {
        datos[i] = t;
        t = strtok(NULL, "|");
        i++;
    }
    return datos;
}

int main(int argc, char *argv[]) {
    int fd[2], numBytes, fdtemp;
    char buf[TAM];
    pid_t PID;

    //(1) crear el cauce
    pipe(fd);


//    setvbuf(stdout,NULL,_IONBF,0);

    //(2) Ejecución concurrente Procesos Padre-Hijo
    if ( (PID= fork())<0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (PID == 0) { // Hijo llama a ex1
        close(fd[0]);
        if (dup2(fd[1],STDOUT_FILENO) <0){
            perror("Error en el write \n");
            exit(EXIT_FAILURE);
        }
        execlp("./practica_examenes1", "practica_examenes1", NULL);
        perror("Error en ejecutando practica_examenes1\n");
    }
    else { // Padre
        close(fd[1]);
        while ((numBytes=read(fd[0], buf, TAM) >0)) {
            char **datos=obtener_datos(3, buf);
            char* nombre = datos[0];
            char* permisos = datos[1];
            char* tamanio = datos[2];
            char filepath[100];
            char contenido[100];
            sprintf(filepath, "/tmp/%s", nombre);
            sprintf(contenido, "%s", tamanio);
//            printf("arhivo %s. contenido:\n%s\n", nombre, contenido);

            if ( (fdtemp=open(filepath, O_CREAT|O_TRUNC|O_WRONLY, 0770))<0){
                perror("Error en el open \n");
                exit(EXIT_FAILURE);
            }
            else{
                if(write(fdtemp, contenido, strlen(contenido))<0){
                    perror("Error en el write \n");
                    exit(EXIT_FAILURE);
                }
                close(fdtemp);
            }

        }

        if (numBytes <0){
            perror("Error en el write \n");
            exit(EXIT_FAILURE);
        }
    }


    return EXIT_SUCCESS;
}

