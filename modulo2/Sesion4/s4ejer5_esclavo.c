#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

int checkPrime(int n)
{
    int range = floor(sqrt(n));
    int isPrime = 1;

    for (int i = 2; i <= range; i++)
    {
        if(n % i == 0)
        {
            isPrime = 0;
            break;
        }
    }
    
    return isPrime;
}


int main(int argc, char *argv[])
{

    int minVal, maxVal;
    char n[4];


    minVal = atoi(argv[1]);
    maxVal = atoi(argv[2]);

    for (int i = minVal; i <= maxVal; i++)
    {
        if(checkPrime(i))
        {
            printf("%d\n", i);
        }
    }

    return EXIT_SUCCESS;
}



 
