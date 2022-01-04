#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ftw.h>


#define NAME_LEN 7
#define PERMISOS 0702

#define S_ISREG_XGRPOTH_GTLEN(mode, len) S_ISREG(mode) && len > NAME_LEN && (((mode) & PERMISOS) == PERMISOS)

/*
 * explora todas las entradas del directorio de trabajo
 */


int visitar(const char* path, const struct stat* stat, int flags, struct FTW*ftw) {
    char *filename = strrchr(path, '/'); //obtiene la ultima ocurrencia de '/'
    char mensaje[100]="";

    if (filename != NULL) {
        filename = filename+1;

        if ( S_ISREG_XGRPOTH_GTLEN(stat->st_mode, strlen(filename))) {
//            printf("Level: %d Path: <%s>: <%o> \n", ftw->level, path, stat->st_mode);
            sprintf(mensaje, "%s|%o|%d\n", filename, stat->st_mode &~S_IFMT, (int) stat->st_size);

//            printf("%s\n", mensaje);
            write(STDOUT_FILENO, mensaje, strlen(mensaje));
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    if (nftw("ficheros", visitar, 10, 0) != 0) {
        printf("interrupción de la busqueda de archivos");
    }
    return EXIT_SUCCESS;
}

