#include<sys/types.h>	//Primitive system data types for abstraction of implementation-dependent data types.
						//POSIX Standard: 2.6 Primitive System Data Types <sys/types.h>
#include<unistd.h>		//POSIX Standard: 2.10 Symbolic Constants         <unistd.h>
#include<sys/stat.h>
#include<fcntl.h>		//Needed for open
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>

// Abrir directorios
#include <dirent.h>

// Información sobre ficheros en directorio
#include <sys/types.h>

// Manipulacion de strings
#include <string.h>

// Recorrer árbol de directorios automáticamente:
//  - Evita que aparezca un warning en la compilación
#define __USE_XOPEN_EXTENDED 1
//  - La biblioteca como tal.
#include <ftw.h>

int regCount = 0;
int byteCount = 0;

int checkFile(const char* path, const struct stat* stat, int flags, struct FTW* ftw)
{
    char *algo, *otro;
    printf("base=%d\n", ftw->base);
    printf("blocks=%ld\n", stat->st_blocks);
    printf("string=%s\n", path);

    if(S_ISREG(stat->st_mode))
    {  
        if((stat->st_mode & S_IXOTH) && (stat->st_mode & S_IXGRP))
        {
            printf("%s %ld\n", path, stat->st_ino);
            regCount++;
            byteCount += stat->st_size;
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Error en número de parámetros\n");
        exit(EXIT_FAILURE);
    }

    char *pathName;
    DIR *dirPointer;

    pathName = argv[1];
    
    printf("Los i-nodos son: \n");

    if( nftw(pathName, checkFile, 10, 0) != 0 )
    {
        printf("Error en función 'nftw'\n");
    }

    printf("Existen %d archivos regulares con permiso x para grupo y otros.\n", regCount);
    printf("El tamaño total ocupado por dichos archivos es de %d bytes.\n", byteCount);

    return EXIT_SUCCESS;
}
 
 
 
