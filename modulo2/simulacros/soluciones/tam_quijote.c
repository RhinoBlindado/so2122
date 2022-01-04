//
// Created by Fran on 14/11/2019.
//
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h> //isdigit()
#include <limits.h> //limites como INT_MAX

#define PERMISOS 0702
#define S_ISREG_XGRPOTH(mode) S_ISREG(mode) && (((mode) & PERMISOS) == PERMISOS)

int visitar(const char* path, const struct stat* stat, int flags, struct FTW*ftw) {
    char *filename = strrchr(path, '/'); //obtiene la ultima ocurrencia de '/'

    if (filename != NULL) {
        filename = filename+1;

        if ( S_ISREG_XGRPOTH(stat->st_mode)) {
            printf("Level: %d Path: <%s>: <%o> \n", ftw->level, path, stat->st_mode);
        }
        if (!strcmp(filename,"quijote.txt")) {
            char mensaje[100];
            sprintf(mensaje, "%d\n", stat->st_size);
            write(STDIN_FILENO,mensaje,strlen(mensaje));
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (nftw("./quijote", visitar, 1, 0) != 0) {
        printf("interrupción de la busqueda de archivos");
    }

    return(0);
}
