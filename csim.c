CS2011 Cachelab by Gavin Hayes (gahayes) and Benjamin Sarkis (bgsarkis)
#include "cachelab.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    int i, v;
    v = 0;    
    for(i = 0; i < argc; i++)
    {
        if(strcmp(argv[i], "-h") == 0)
            printf("Usage is:\n"); 

         if(strcmp(argv[i], "-v") == 0)
         {
            v = 1;
            printf("Verbose %d\n", v);           
         }
    }
    printSummary(0, 0, 0);
    return 0;
}
