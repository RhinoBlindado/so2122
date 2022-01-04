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


#define PERM 0700
#define THE_GOOD_CHECK(mode) (S_ISREG(mode)) &&  ((mode & ~S_IFMT) == PERM)
#define CHECK_TYPE_FILE(mode) (S_ISREG(mode)) && (((mode) & PERM) == PERM)


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
                if(THE_GOOD_CHECK(fileStat.st_mode))
                {
                    printf("%s\n", fullPath);
                }
                else if(S_ISDIR(fileStat.st_mode))
                {
                    if ( (dirPointer = opendir(fullPath)) == NULL)
                    {
                        printf("Error al abrir directorio\n");
                        exit(EXIT_FAILURE);
                    }
                    strcat(fullPath, "/");
                    checkDir(dirPointer, fullPath, regCount, byteCount);
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
 
 
