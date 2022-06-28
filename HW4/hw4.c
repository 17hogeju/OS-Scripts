#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	const int PAGE_SIZE = 4096;
	unsigned int virtual_a, page_num, offset;

	if(argc != 2)
	{
		fprintf(stderr, "Useage: <virtaual address (in decimal)>\n");
		return -1;
	}

	virtual_a = atoi(argv[1]);

	page_num = virtual_a / PAGE_SIZE;
	offset = virtual_a % PAGE_SIZE;

	printf("The address %d contains: \n", virtual_a);
	printf("page number = %d\n", page_num);
	printf("offset = %d\n", offset);


	return 0;
}

