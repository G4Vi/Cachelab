//CS2011 Cachelab by Gavin Hayes and Benjamin Sarkis gahayes-bgsarkis
#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

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
	 if(strcmp(argv[i], "-s") == 0){
	   printf("Set index bits: %d ", atoi(argv[i])^2);
	}
	if(strcmp(argv[i], "-E") == 0){
	
	}
	if(strcmp(argv[i], "-b") == 0){

	}
	if(strcmp(argv[i], "-t") == 0){

	}
    }
    printSummary(0, 0, 0);
    return 0;
}
