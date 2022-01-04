#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ftw.h>

#define NUM_COLS 10
#define MAX_COL_NAME 30

int main(int argc, char *argv[]) {
    char col_names[NUM_COLS][MAX_COL_NAME] = {"inodo", "permisos", "links", "usuario", "grupo", "size", "mes", "dia", "hora", "fichero"};
    int fd[2], numBytes, fdtemp;
    char buf[2];
    pid_t PID;

    //reseteo mascaras
    umask(0);

    //(1) crear el cauce
    pipe(fd);


    //(2) Ejecución concurrente Procesos Padre-Hijo
    if ( (PID= fork())<0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (PID == 0) { // Hijo
        close(fd[0]);
        if (dup2(fd[1],STDOUT_FILENO) <0){
            perror("Error en el write \n");
            exit(EXIT_FAILURE);
        }

        char command[50];
        sprintf(command, "ls -ldSrhiF *");
        execl("/bin/sh", "/bin/sh", "-c", command, NULL);
        perror("Error en execl");
    }
    else { // Padre
        close(fd[1]);

        char dato[100]="";
        long int min_inodo=INTMAX_MAX;
        int nuevo_min_inodo=0;
        char min_inodo_name[100]="";
        char min_inodo_size[100]="";


        //*****
        //0:leo i-nodo; 1: leo permisos; 2: leo contador enlaces duros; 3: leo usuario; 4: leo grupo; 5:leo size archivo
        //6: leo fecha mes; 7: leo fecha día; 8: leo fecha hora; 9: leo nombre archivo
        int col_index=0;
        //*****

        while (read(fd[0], buf, 1)) {
            //concatenamos dato leido de cada columna
            if(strcmp(buf, " ") && strcmp(buf, "\n")){ // buf != " " && buf != "\n"
                strcat(dato, buf);
            }
            //fin de lectura de cada columna. Aquí ya tenemos el dato completo, por ejemplo la columna permisos
            else if(strlen(dato)>0){
                if(col_index==0){ //columna i-nodo
                    long int inodo = atoi(dato);
                    nuevo_min_inodo = inodo < min_inodo;
                    if(nuevo_min_inodo){
                        min_inodo = inodo;
                    }
                }
                else if(col_index==9 && !strcmp(dato, "netflix")){
//                    printf("*****NETFLIX*****"); //GUARDAR SERIE
                }


                if(nuevo_min_inodo){
                    if(col_index==5){
                        strcpy(min_inodo_size, dato);
                    }
                    if(col_index == 9){
                        strcpy(min_inodo_name, dato);
                    }
                }

                printf("%s: %s\n", col_names[col_index], dato);
                strcpy(dato, "");
                col_index=(col_index+1)%NUM_COLS; //aumentamos contador de columna
                if(col_index==0){
                    printf("\n");
                }
            }
        }

        printf("i-nodo máximo: %ld. Archivo: %s. Size: %s", min_inodo, min_inodo_name, min_inodo_size);
    }


    return EXIT_SUCCESS;
}

