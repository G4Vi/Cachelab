//CS2011 Cachelab by Gavin Hayes and Benjamin Sarkis gahayes-bgsarkis
#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>


//Struct for line of a cache
struct line{
    int valid; 
    int tag; 
    int access_time; 
};

//Struct for the set of a cache
struct actualSet{
    struct line *listLines;
};

//Struct for cache itself
struct cache{
        int sets;    
        int linesSet;         
        struct actualSet *listSet;
};


//Relevant Data
int hits = 0;
int misses = 0;
int evictions = 0;

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
    //This loop is what reads from the command line
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
/** Makes a simulated cache
    @param aCache pointer to a cache struct
    @param set the exponent part of the number of sets in the cache
    @param lines number of lines within each set
*/
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
/** Scans all traces to a cache and returns a hit, miss or eviction value appropriately
    @param aCache pointer to a cache structure
    @param aBuf pointer to the beginning of a buffer
    @param set exponent part of the set
    @param line the line of interest
    @param bloc actual data within 
    @param flagges 
*/
int lookAtTraces(struct cache *aCache, char *aBuf, int set, int line, int bloc, int flagged){
	int address;
	int mask = 0x7fffffff;  //Used for set and tag bits
	int i; //loop counter
	char optField; //M, I, L, or S
	int whichSet;  //Bits of set in questions
	int whichTag;  //Ditto for tag
	//The space between the conversion specifiers helps with finding the right operations
	sscanf(aBuf, " %c %x", &optField, &address);
        //Find the set and tag by "masking out" thr appropriate bits via shifting
        whichSet = (mask >> (31 - set)) & (address >> bloc);
	whichTag = (mask >> (31 - set - bloc)) & (address >> (set + bloc));
	
	//Now we cycle through the lines of a given set to determine evictions, hits, and misses
	for(i = 0; i < aCache -> linesSet; i++){
		//if the valid bit is one, we hit
		if(1 == aCache->  listSet[whichSet].listLines[i].valid && whichTag == aCache -> listSet[whichSet].listLines[i].tag){
			if(optField == 'M'){
				//Because M is a data load and a data store, this is a double hit
				hits += 2;
			}
			else{
				//Just hit once otherwise
				++hits;
			}
			//updateAccessTime(aCache, whichSet, i);
			return 1;  //<--A dummy value for now, make a HIT constant
		}
	}
}

//Still need to add code for evictions and misses
	
	
	



