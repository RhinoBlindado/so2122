
// Tipos de dato primitivos e info de ficheros en directorio
#include<sys/types.h>

// Constantes.
#include<unistd.h>

// Referente a ficheros y permisos.
#include<sys/stat.h>

// Utilizar el wait() para los procesos.
#include<sys/wait.h>

// Para el open(), fcntl() y banderas.
#include<fcntl.h>

// E/S Estándar.
#include<stdio.h>

// Errores.
#include<errno.h>

// Biblioteca estándar.
#include<stdlib.h>

// Manipulación de cadenas.
#include<string.h>

// Abrir directorios
#include <dirent.h>

// Recorrer árbol de directorios automáticamente:
//  - Evita que aparezca un warning en la compilación
#define __USE_XOPEN_EXTENDED 1
//  - La biblioteca como tal.
#include <ftw.h>


#define PERM 0702
#define CHECK_FILE(mode, len, block) ((S_ISREG(mode) || S_ISDIR(mode)) && ((mode & ~S_IFMT) == PERM) && (len < 8) && ((8 <= block) && (block <= 12) ))
//#define CHECK_TYPE_FILE(mode) (S_ISREG(mode)) && (((mode) & PERM) == PERM)


void checkDir(DIR *dirP, char* pathName, int *regCount, int *byteCount)
{
    DIR *dirPointer;
    char fullPath[256];
    struct dirent *actFileInfo;
    struct stat fileStat;

    while ( (actFileInfo = readdir(dirP)) != NULL )
    {
        strcpy(fullPath, pathName);
        if((strcmp(actFileInfo->d_name, ".") != 0) && (strcmp(actFileInfo->d_name, "..") != 0))
        {
            strcat(fullPath, actFileInfo->d_name);
            if (stat(fullPath, &fileStat) < 0)
            {
                printf("Error en leer atributo de archivo %s\n", fullPath);
                exit(EXIT_FAILURE);
            }
            else
            {
                if(CHECK_FILE(fileStat.st_mode, strlen(actFileInfo->d_name), fileStat.st_blocks))
                {
                    printf("%s|%ld\n", actFileInfo->d_name, fileStat.st_ino);
                }
            }
        }
    }
}


int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        printf("Error en número de parámetros\n");
        exit(EXIT_FAILURE);
    }

    umask(0);

    char *pathName;
    DIR *dirPointer;
    int regFileCount = 0, byteCount = 0;

    pathName = argv[1];
    
    if ( (dirPointer = opendir(pathName)) == NULL)
    {
        printf("Error al abrir directorio\n");
        exit(EXIT_FAILURE);
    }

    checkDir(dirPointer, pathName, &regFileCount, &byteCount);

    if (closedir(dirPointer) != 0)
    {
        printf("Error en el cierre del directorio\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
} 
 
 
