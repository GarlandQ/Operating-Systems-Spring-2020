#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char* argv[]) {

	char* filepath = argv[1];
	int returnval;
	// Check file existence
	returnval = access (filepath, F_OK);
	if (returnval == 0)
		printf ("\n %s exists\n", filepath);
	else
	{
		if (errno == ENOENT)
			printf ("%s does not exist\n", filepath);
		else if (errno == EACCES)
			printf ("%s is not accessible\n", filepath);
		return 0;
	}
	// Check read access ...
	int returnread;
	returnread = access (filepath, R_OK);
	if (returnread == -1)
		printf ("\n %s is NOT readable\n", filepath);
	else
		printf("\n %s is readable\n", filepath);
	
	// Check write access ...
	int returnwrite;
	returnwrite = access(filepath, W_OK);
	if(returnwrite == -1)
		printf("\n %s is NOT writeable\n", filepath);
	else
		printf("\n %s is writeable\n", filepath);

	return 0;
}
