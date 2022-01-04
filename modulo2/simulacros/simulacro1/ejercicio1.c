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
                if(S_ISREG(fileStat.st_mode))
                {
                    /**
                     * - Todos los permisos para usuario
                     * - Lectura para otros
                     * - Ninguno para grupo
                     */
                    if(((fileStat.st_mode & ~S_IFTM) == 0702 && (strlen(actFileInfo->d_name) > 7))
                    {
                        printf("%s|%o|%ld\n", actFileInfo->d_name, fileStat.st_mode &~S_IFMT, fileStat.st_size);

                    }
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

  //  printf("Los i-nodos son: \n");
    checkDir(dirPointer, pathName, &regFileCount, &byteCount);

 //   printf("Existen %d archivos regulares con permiso x para grupo y otros.\n", regFileCount);
 //   printf("El tamaño total ocupado por dichos archivos es de %d bytes.\n", byteCount);

    if (closedir(dirPointer) != 0)
    {
        printf("Error en el cierre del directorio\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
} 
