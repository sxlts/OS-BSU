#include "header.h"

int main(int argc, char **argv){
	if(argc < 2) exit(1);
	
	FILE *InputFile = fopen(argv[1], "w+b");
	int LineNumber = atoi(argv[2]);

	printf("%-10s%-10s%-10s\n", "ID:", "Name:", "Hours:");
	for(int i = 0 ; i < LineNumber ; ++i){
		emp temp;

		scanf("%d%49s%lf", &temp.num, temp.name, &temp.hours);
		printf("%-10d%-10s%-8.1lf\n", temp.num, temp.name, temp.hours);

		fwrite(&temp, sizeof(emp), 1, InputFile);
	}

	fclose(InputFile);
	return 0;
}
