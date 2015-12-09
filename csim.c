//CS2011 Cachelab by Gavin Hayes and Benjamin Sarkis gahayes-bgsarkis
#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int opt, flags;
    int nsecs, tfnd;
   
    tfnd = 0;
    flags = 0;
    nsecs = 0;    
    while((opt = getopt(args, argv, "hvs:E:b:t:") != -1){
	switch(opt){
	case 'h':
		printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
		printf("Options:\n");
		printf("-h		Print this help message.\n");
		printf("-v		Optional verbose flag.\n");
		printf("-s <num>	Number of set index bits.\n");
		printf("-E <num>	Number of lines per set.\n");
		printf("-b <num>	Number of block offset bits.\n");
		printf("-t <file>	Trace file.\n\n");
		printf("Examples:\n");
		printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
		printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
	case 'v':
	case 's':
	case 'E':
	case 'b':
	case 't':
	default:
		printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
		printf("Options:\n");
		printf("-h		Print this help message.\n");
		printf("-v		Optional verbose flag.\n");
		printf("-s <num>	Number of set index bits.\n");
		printf("-E <num>	Number of lines per set.\n");
		printf("-b <num>	Number of block offset bits.\n");
		printf("-t <file>	Trace file.\n\n");
		printf("Examples:\n");
		printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
		printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n"); 	
	}
    }
    printSummary(0, 0, 0);
    return 0;
}
