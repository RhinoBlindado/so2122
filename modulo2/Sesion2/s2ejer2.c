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

int main(int argc, char *argv[])
{

    if(argc != 3)
    {
        printf("Error en número de parámetros\n");
        exit(EXIT_FAILURE);
    }

    char *pathName;
    long newPermissions;

    pathName = argv[1];
    newPermissions = strtol(argv[2], NULL, 8);

    DIR *dirPointer;
    struct dirent *actFileInfo;
    
    struct stat filePermissions;

    if ( (dirPointer = opendir(pathName)) == NULL)
    {
        printf("Error al abrir directorio\n");
        exit(EXIT_FAILURE);
    }

    char fullPath[256];
    int err;
    while( (actFileInfo = readdir(dirPointer)) != NULL )
    {
        strcpy(fullPath, pathName);
        strcat(fullPath,actFileInfo->d_name);
        if (stat(fullPath, &filePermissions) < 0)
        {
            printf("Error en leer atributo de archivo %s\n", fullPath);
            exit(EXIT_FAILURE);
        }

        if((err = chmod(fullPath, newPermissions)) < 0) 
        {
            printf("%s: Permisos [Viejos: %o, Error: %d\n",actFileInfo->d_name, filePermissions.st_mode, err);
        }
        else
        {
            printf("%s: Permisos [Viejos: %o, Nuevos: %lo]\n", actFileInfo->d_name, filePermissions.st_mode, newPermissions);
        }

    }

    if (closedir(dirPointer) != 0)
    {
        printf("Error en el cierre del directorio\n");
        exit(EXIT_FAILURE);
    }

    /*
int fd1,fd2;
struct stat atributos;

//CREACION DE ARCHIVOS
if( (fd1=open("archivo1",O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0) {
	printf("\nError %d en open(archivo1,...)",errno);
	perror("\nError en open");
	exit(EXIT_FAILURE);
}

umask(0);
if( (fd2=open("archivo2",O_CREAT|O_TRUNC|O_WRONLY,S_IRGRP|S_IWGRP|S_IXGRP))<0) {
	printf("\nError %d en open(archivo2,...)",errno);
	perror("\nError en open");
	exit(EXIT_FAILURE);
}

//CAMBIO DE PERMISOS
if(stat("archivo1",&atributos) < 0) {
	printf("\nError al intentar acceder a los atributos de archivo1");
	perror("\nError en lstat");
	exit(EXIT_FAILURE);
}
if(chmod("archivo1", (atributos.st_mode & ~S_IXGRP) | S_ISGID) < 0) {
	perror("\nError en chmod para archivo1");
	exit(EXIT_FAILURE);
}
if(chmod("archivo2",S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH) < 0) {
	perror("\nError en chmod para archivo2");
	exit(EXIT_FAILURE);
}
*/
return EXIT_SUCCESS;
}
 
