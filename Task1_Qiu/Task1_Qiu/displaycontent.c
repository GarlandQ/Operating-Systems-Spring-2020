#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	char *filepath = argv[1];
	char buffer[2048];
	//Check if file exists
	int exist = access(filepath, F_OK);
	if(exist == 0)
	{
		int readfile = open(filepath, O_RDONLY);
		if(readfile == -1)
			printf("\n %s is not readable \n", filepath);
		else
		{
			while(read(readfile, buffer, sizeof(buffer)) > 0) {
				printf("%s", buffer);
			}
			printf("\n");
		}
		close(readfile);
	}
	else
	{
		printf("\n file does not exist or can't be accessed \n");
	}
	return 0;
}