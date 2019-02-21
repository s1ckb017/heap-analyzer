#include "tracepid.h"


/**
 * routine to get start address of heap and its length, start address is saved in startHeap
 *
 * @param startHeap 
 * @param pid
 *
 * @return length of heap or -1 on error
 */
uint64_t getHeapAddr(uint64_t * startHeap, const pid_t pid){
	FILE * fp = NULL;
	char line[1024], *tmp;
	char addr[18] = {0};
	uint64_t ret = 0, endAddr = 0;
	char path[256];

	BUILDMAPSPATH(path, 256, pid)

	fp = fopen(path, "r");
	if(fp == NULL){
		fprintf(stderr, "Fopen Error: %s", strerror(errno));
		return -1;
	}
	while(fgets(line, 1024, fp) != NULL ){
		if(strstr(line, "[heap]") != NULL){
			if((tmp = strchr(line, '-')) != NULL){
				strcpy(addr, "0x");
				strncat(addr, line, tmp - line);
				*startHeap = strtol(addr, NULL, 16);
			}		
			if((tmp = strchr(line, ' '))!= NULL){
				strcpy(addr, "0x");
				strncpy(addr,strchr(line, '-') + 1, tmp - strchr(line, '-'));
				endAddr = strtol(addr, NULL, 16);
				ret = endAddr - *startHeap;	
				printf("[+]\tHeapLen:%ld\tstartHeap:%p\tendHeap:%p\n", ret, (void *) *startHeap, (void *) endAddr);
				break;
			}

		} else continue;
	
	}
	fclose(fp);
	return ret;
}




/**
 * routine to attach through ptrace on process identified by pid
 *
 * @param pid
 *
 * @return 0 or -1 on error
 */
int ptrace_attach(const pid_t pid){
	int r = 0;
	errno = 0;
	ptrace(PTRACE_ATTACH, pid, NULL, NULL);
	if(errno == 0){
		return 0;
		waitpid(pid, &r, 0);
	}
	else {
		fprintf(stderr, "ptrace_attach: %s\n", strerror(errno));
		return -1;
	}
}



/**
 * routine to detach from pid
 *
 * @param pid
 *
 * @return 0 or -1 on error
 */
int ptrace_detach(const pid_t pid){
	errno = 0;
	ptrace(PTRACE_DETACH, pid, NULL, NULL);
	if(errno == 0) return 0;
	else{
		fprintf(stderr, "ptrace_detach: %s\n", strerror(errno));
		return -1;
	}
}



/**
 * routine to save all heap memory in vector
 *
 * @param startHeap 	start address of heap
 * @param len   		length od heap
 * @param pid 			pid to analyze
 * @param mem 			memory where is saved all heap of pid
 *
 * @return 0 or -1 on error 
 */
int getHeapMem(const uint64_t  startHeap, const uint64_t len, const pid_t pid, uint64_t * mem){
	uint64_t i = 0, j = 0;
	
	while(i < (len/8)){
		errno = 0;
		mem[j] = ptrace(PTRACE_PEEKTEXT, pid, (void *)startHeap+i, 0);
	    if(errno != 0){
			continue;
		}
		//printf("mem: %p \n", mem[j]);
		j++;
		i+=8;
	}
	return 0;
}

