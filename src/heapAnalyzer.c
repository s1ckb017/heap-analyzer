#include "heapAnalyzer.h"


int heapAnalysis(uint64_t * mem, uint64_t len){
	uint64_t i = 0, j = 0;
	chunk * current_chunk = NULL;
	while(i < len/8){
		current_chunk = (chunk*)realloc(current_chunk, sizeof(chunk)*(j+1));
		if(current_chunk == NULL) return -1;
		
		memcpy(&(current_chunk[j].prev_size), mem+i, sizeof(uint64_t));	
		memcpy(&(current_chunk[j].size), mem+i+1, sizeof(uint64_t));

		
		current_chunk[j].mainArena = current_chunk[j].size & 4;
		current_chunk[j].mmapped = current_chunk[j].size & 2;
		current_chunk[j].prev_inUse = current_chunk[j].size & 1;
		
		current_chunk[j].size = current_chunk[j].size            \
								- current_chunk[j].mainArena     \
								- current_chunk[j].mmapped       \
								- current_chunk[j].prev_inUse;
		
		memcpy(&(current_chunk[j].nextFreeChunk), mem+i+2, sizeof(uint64_t));
		memcpy(&(current_chunk[j].prevFreeChunk), mem+i+3, sizeof(uint64_t));
		
		i = i  + (current_chunk[j].size)/8;
		
		if(i*sizeof(uint64_t*) + current_chunk[j].size >= len)
				current_chunk[j].free = TOPCH;
		
		if(j != 0){	
			if(current_chunk[j].prev_inUse == 1)
				current_chunk[j-1].free = INUSE;	
			else 
				current_chunk[j-1].free = FREE;
		}
		j++;
	}
	for (i = 0; i < j; i++ ){
		printChunk(&current_chunk[i]);
	}
	return 0;
}


void printChunk(chunk * c){
	
	if (c->free == TOPCH)
		printf("\n[*]\tTop Chunk\n");
	else if(c->free == FREE)
		printf("\n[-]\tFreed Chunk\n");
	else if (c->free == INUSE)
		printf("\n[+]\tInUse Chunk\n");
	
	printf("\t\tPrevious Size        : %u\n", c->prev_size);
	printf("\t\tChunk Size           : %u\n", c->size);
	if(c->free == FREE){
		printf("\t\tNext Free Chunk      : %p\n", (void *)c->nextFreeChunk);
		printf("\t\tPrevious Free Chunk  : %p\n", (void *)c->prevFreeChunk);
	}	
	printf("\t\tMainArena[%d] MMapped[%d]\n", c->mainArena, c->mmapped);
	return;
}
