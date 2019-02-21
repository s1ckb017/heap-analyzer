
#ifndef HEAPANALYZER_H
#define HEAPANALYZER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define FREE  		0
#define INUSE 		1
#define TOPCH  		2

typedef uint8_t free_c_t;

typedef struct{
	uint64_t prev_size;
	uint64_t size;
	uint8_t mainArena;
	uint8_t mmapped;
	uint8_t prev_inUse;
	uint64_t nextFreeChunk;
	uint64_t prevFreeChunk;
	free_c_t free;
}__attribute__((packed)) chunk;



int heapAnalysis(uint64_t *, uint64_t);
void printChunk(chunk *);

#endif
