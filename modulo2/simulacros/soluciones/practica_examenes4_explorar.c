#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ftw.h>


#define NAME_LEN 7
#define PERMISOS 0700

#define S_ISREG_XGRPOTH_GTLEN(mode) S_ISREG(mode) && (((mode) & PERMISOS) == PERMISOS)


/*
 * explora todas las entradas del directorio de trabajo
 */


int visitar(const char* path, const struct stat* stat, int flags, struct FTW*ftw) {
    char *filename = strrchr(path, '/'); //obtiene la ultima ocurrencia de '/'
    char mensaje[100];


    if (filename != NULL) {
        filename = filename+1;

        if ( S_ISREG_XGRPOTH_GTLEN(stat->st_mode) ){
            char *lastname = strrchr(filename, '_');
            if(lastname && !strcmp(lastname+1, "watch.txt")) {
                sprintf(mensaje, "%s\n", path);
                write(STDOUT_FILENO, mensaje, strlen(mensaje));
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if(argc < 2 ){
        printf("Error. Falta parámetro directorio");
        exit(EXIT_FAILURE);
    }

    if (nftw(argv[1], visitar, 10, 0) != 0) {
        printf("interrupción de la busqueda de archivos");
    }

    return EXIT_SUCCESS;
}

