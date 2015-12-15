//CS2011 Cachelab by Gavin Hayes and Benjamin Sarkis gahayes-bgsarkis
#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>

//Symbolic constants for Misses, hits, and evictions
#define HIT 1
#define MISS 2
#define MISS_HIT 4
#define MISS_EVICTION 8
#define MISS_EVICTION_HIT 16

//Struct for line of a cache
struct line{
    int validBit; 
    int tagBits; 
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

//function prototypes
void makeCache(struct cache *aCache, int set, int lines);
int mostRecent(struct cache *aCache, int whichSet, int); 
void use();
int lookAtTraces(struct cache *aCache, char *aBuf, int set, int line, int bloc);

int main(int argc, char **argv)
{ 
    //For the loop that reads the command line
    int opt;

    //E, b, and s respectively
    int lines, block, set;

    //For verbose command
    int flag = 0;

    //Hit, Miss, or Eviction
    int result;

    struct cache aCache;

    //The file of interest
    FILE *someFile;

    //What he command line will take directly
    char fileName[200];

    //The results printed to the screen, used in the fgets function for printing results
    char outBuf[200];

   // Stats of interest
    lines = 0;
    block = 0;
    set = 0;    
    
    //This number makes sure the user input the correct number of commands
    int args = 0; 

    //This loop is what reads from the command line
    while((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1){
	switch(opt){
	case 'h':
		use();
		break;
        case 'v':
		flag = 1;
                break;
	case 's':
		args++;
		set = atoi(optarg);
		break;
	case 'E':
		args++;
		lines = atoi(optarg);
		break;
	case 'b':
		args++;
		block = atoi(optarg);
		break;
	case 't':
		args++;
		strcpy(fileName, optarg);
                break;
	default:
		printf("./csim: Missing required command line argument\n");
		use();
		exit(-1);
		break;	
	}
    }
    //Make sure the four crucial arguments were entered
    if(args < 4){
	use();
	exit(-1);
    }

    //Create the cache
    makeCache(&aCache, set, lines);
    
    //Open a file for reading only
    someFile = fopen(fileName, "r");
    
    
    
    //If the file is empty, print an error message
    if(!someFile){
	printf("What kind of file is %s? I quit\n", fileName);
	return -1;
    }
    
    //Now the actual recording of results begins (until the file is NULL)
    //We are only interested in operators that have a space before them, (i.e. we ignore 'I')
    while (NULL != fgets(outBuf, 200, someFile)){
	if(' ' == outBuf[0]){
		//Make sure to add the null terminator! (Because of how strlen works)
		outBuf[strlen(outBuf)-1] = '\0';
		result = lookAtTraces(&aCache, outBuf, set, lines, block);
                //if we use the -v verbose flag
		if(1 == flag){
			switch (result){
			case MISS_EVICTION_HIT:
				printf("%s miss eviction hit\n", outBuf+1);
				break;
			case MISS_HIT:
				printf("%s miss hit\n", outBuf+1);
				break;
			case MISS_EVICTION:
				printf("%s miss eviction\n", outBuf+1);
				break;
			case MISS:
				printf("%s miss\n", outBuf+1);
				break;
			case HIT:
				printf("%s miss eviction hit\n", outBuf+1);
				break;
			default:
				break;
			}//end of switch statement
		 }//end of verbode flag statement
	   }//end of determining if operator is not 'I'
    }//end of while loop
    fclose(someFile);
    printSummary(hits, misses, evictions);
    return 0;
}


/** Describes how ./csim is meant to be used
*/

void use(){
	printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
	printf("Options:\n");
	printf("-h		Print this optional help message.\n");
	printf("-v		Optional verbose flag.\n");
	printf("-s <num>	Number of set index bits.\n");
	printf("-E <num>	Number of lines per set.\n");
	printf("-b <num>	Number of block offset bits.\n");
	printf("-t <file>	Trace file.\n\n");
	printf("Examples:\n");
	printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
	printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n"); 
}



/**Updates where the most recently interacted portion of data was and makes sure no other
   line has a greater access time than the most recently interacted data. 
   @param aCache The cache of interest
   @param whichSet The set accessed
   @param whichLine The line accessed
   @return 0 for success
*/

int mostRecent(struct cache *aCache, int whichSet, int whichLine){
  int counter; //loop counter cycling through lines
  for(counter = 0; counter < aCache -> linesSet; counter++){
	//if
	//1.  The valid bit is 1 (meaning data is already in that part of the cache) and...
        //2.  Any line accessed has a time greater than the time of the line you are accessing
        //Then lower the access time of any valid line traversed (since it has been accessed)
	if ( (aCache -> listSet[whichSet].listLines[counter].validBit == 1) 
	   && (aCache -> listSet[whichSet].listLines[counter].access_time > 
		aCache -> listSet[whichSet].listLines[whichLine].access_time)){
	 		--aCache -> listSet[whichSet].listLines[counter].access_time;
		}
	}
	//What ever happens, set the access time of the interested line to the number of lines in a set
	aCache -> listSet[whichSet].listLines[whichLine].access_time = aCache -> linesSet;
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
                aCache->listSet[i].listLines[j].validBit = 0;
            }
        }
}
/** Scans all traces to a cache and returns a hit, miss, eviction, etc. value appropriately
    @param aCache pointer to a cache structure
    @param aBuf pointer to the beginning of a buffer
    @param set exponent part of the set
    @param line the line of interest
    @param bloc actual data within 
    @return the result of an operation (hit, miss, eviction, or any relevant combination) 
*/
int lookAtTraces(struct cache *aCache, char *aBuf, int set, int line, int bloc){
	int address;
	int mask = 0x7fffffff;  //Used for set and tag bits
	int num; //loop counter
	char optField; //M, I, L, or S
	int whichSet;  //Bits of set in questions
	int whichTag;  //Ditto for tag
	//The space between the conversion specifiers helps with finding the right operations
	sscanf(aBuf, " %c %x", &optField, &address);
        //Find the set and tag by "masking out" thr appropriate bits via shifting
        whichSet = (mask >> (31 - set)) & (address >> bloc);
	whichTag = (mask >> (31 - set - bloc)) & (address >> (set + bloc));
	
	//Now we cycle through the lines of a given set to determine evictions, hits, and misses
	for(num = 0; num < aCache -> linesSet; num++){
		//if the valid bit is one AND the tag is equivalent, we hit
		if(1 == aCache->  listSet[whichSet].listLines[num].validBit && whichTag == aCache -> listSet[whichSet].listLines[num].tagBits){
			if(optField == 'M'){
				//Because M is a data load and a data store, this is a double hit
				hits += 2;
			}
			else{
				//Just hit once otherwise
				++hits;
			}
			mostRecent(aCache, whichSet, num);
			//It either hit once or twice but either way it did hit, so return hit.
			return HIT;
		}
			
		misses++; //<--If it skipped over the hit statement, we can assume it misses
		//if the valid bit is zero, we miss
		if(0 == aCache -> listSet[whichSet].listLines[num].validBit){
			//If we miss, we then set the whole line to one and set the tag to the tag 				//of the address of interest
			aCache-> listSet[whichSet].listLines[num].validBit = 1;
			aCache-> listSet[whichSet].listLines[num].tagBits = whichTag;
			//			
			//if we have the data load and store, it is both a miss and a hit
			if(optField == 'M'){
				hits++;
				return MISS_HIT;
			}
			//Otherwise it is just a plain miss
			else{
				return MISS;
			}
		}

		//Since we have been keeping track of when things were accessed, if it matches
		//a time of a place previously accessed, then we evict
		evictions++;  //<-- If it didn't immediately miss or hit, then we have to evict a 
		//portion of a cache
		if(1 == aCache -> listSet[whichSet].listLines[num].access_time){
			aCache -> listSet[whichSet].listLines[num].tagBits = whichTag;
			aCache -> listSet[whichSet].listLines[num].validBit = 1;
			mostRecent(aCache, whichSet, num);
			//Normally we evict then miss.  But since this is a data load AND a data 
			//store, we hit as well
			if('M' == optField){
				hits++;
				return MISS_EVICTION_HIT;
			}
			else{
				return MISS_EVICTION;
			}
		}//end if statement	
	}//end loop
	return 0;  //<-- Just in case
}
	
	



