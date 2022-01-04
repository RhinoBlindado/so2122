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

#define __USE_XOPEN_EXTENDED 1

#include <ftw.h>


#define PERM 0702
#define CHECK_TYPE_FILE(len, mode) (S_ISREG(mode)) && (len > 7) && (((mode) & PERM) == PERM)

int checkFile(const char* path, const struct stat* stat, int flags, struct FTW* ftw)
{
    char *filename = strrchr(path, '/');


    if(filename != NULL)
    {
        filename += 1;
        if(CHECK_TYPE_FILE(strlen(filename), stat->st_mode))
        {
            printf("%s|%o|%d\n", filename, stat->st_mode & ~S_IFMT, (int)stat->st_size);
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

    umask(0);

    char *pathName;
    pathName = argv[1];
    
    if( nftw(pathName, checkFile, 10, 0) != 0 )
    {
        printf("Error en función 'nftw'\n");
    }

    return EXIT_SUCCESS;
} 
