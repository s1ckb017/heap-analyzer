#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(){
	char *a,*b,*c;
	printf("pid: %d\n", getpid());

	a = malloc(200);
	b = malloc(300);
	c = malloc(400);
	
	strcpy(a, "ciao comba");
	strcpy(b, "blablablablablablablablablablablabla");
	strcpy(c, "lyapunovlyapunovlyapunovlyapunovlyapunov");
	
	free(a);
	free(b);
	free(malloc(24));
	free(c);

	sleep(1000);
	return 0;
}
