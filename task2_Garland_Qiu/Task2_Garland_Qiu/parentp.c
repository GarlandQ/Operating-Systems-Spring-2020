#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int main() {
	int a = 10, b = 25, fq = 0, fr = 0 ;
	fq = fork();
	if(fq == 0) {
		a = a + b;
		printf("(a = a + b)\na = %i\nb = %i\nprocess id = %i\n", a,b,getpid());
		fr = fork();
		if(fr != 0) {
			b = b + 20;
			printf("(b = b + 20)\na = %i\nb = %i\nprocess id = %i\n", a,b,getpid());
		}
		else {
			a = (a*b) + 30;
			printf("(a = (a*b) + 30)\na = %i\nb = %i\nprocess id = %i\n", a,b,getpid());
		}
	}
	else { 
		b = a + b - 5;
		printf("(b = a + b - 5)\na = %i\nb = %i\nprocess id = %i\n", a,b,getpid());
		wait(NULL);
	}
	return 0;
}