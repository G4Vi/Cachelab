//CS2011 Cachelab by Gavin Hayes and Benjamin Sarkis gahayes-bgsarkis
#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

struct line{
    int valid; 
    int tag; 
    int access_time; 
};

struct actualSet{
    struct line *listLines;
};

struct cache{
        int sets;    
        int linesSet;         
        struct actualSet *listSet;
};

void makeCache(struct cache *aCache, int set, int lines);

int main(int argc, char **argv)
{
    int opt;
    int lines, block, set;

    struct cache aCache;

    char* file = NULL;
    lines = 0;
    block = 0;
    set = 0;    
    while((opt = getopt(argc, argv, "hs:E:b:t:") != -1)){
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
		break;
	case 's':
		set = atoi(optarg);
		break;
	case 'E':
		lines = atoi(optarg);
		break;
	case 'b':
		block = atoi(optarg);
		break;
	case 't':
		file = optarg;
	default:
		printf("./csim: Missing required command line argument\n");
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
		exit(EXIT_FAILURE);	
	}
    }

    makeCache(&aCache, set, lines);
    printSummary(0, 0, 0);
    //printSummary(hit_count, miss_count, eviction_count);
    return 0;
}

void makeCache(struct cache *aCache, int set, int lines){
    int i, j;
	aCache->sets = (2 << set); //number of sets = 2^s
	aCache->linesSet = lines;

        aCache->listSet = calloc(aCache->sets, sizeof(struct actualSet));
       
        if(!aCache->listSet)
	    exit(-1);        

	for(i = 0; i < aCache->sets; i++)
        {
            aCache->listSet[i].listLines = calloc(aCache->linesSet, sizeof(struct line));

            if(!aCache->listSet[i].listLines)
	        exit(-1);

            for(j = 0; j < aCache->linesSet; j++)
            {
                aCache->listSet[i].listLines[j].valid = 0;
            }
        }
}

