#include <sys/mman.h>
#include "heapAnalyzer.h"
#include "tracepid.h"

int usage(char *);

/*
 * Renderlo indipendente dalle piattaforme /uint64_t/void  /8/sizeof(void *)/
 * migliorare il print del chunk
 *
 *
 *
*/


int main(int argc, char **argv){
	pid_t pid = 0;
	uint64_t heap_start_addr = 0;
	uint64_t heap_length = 0;
	uint64_t * mem = 0;
	
	if(argc < 2){
		usage(argv[0]);
		return 0;
	}
	pid = atoi(argv[1]);
	heap_length = getHeapAddr(&heap_start_addr, pid);
	if(heap_length == -1 ) return 1;
	mem = (uint64_t *) mmap(NULL, heap_length, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); 
	if(ptrace_attach(pid) == -1) return 1;
	if(getHeapMem(heap_start_addr, heap_length, pid, mem) == -1){
		ptrace_detach(pid);
		return 1;
	}
	heapAnalysis(mem, heap_length);
	ptrace_detach(pid);
	return 0;
}


int usage(char * name){
	printf("Usage: %s <pid>\n", name);
	return 0;
}
