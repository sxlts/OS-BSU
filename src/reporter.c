#include "header.h"

int main(int argc, char** argv){
	if(argc < 4) exit (1);
	
	FILE *InputFile = fopen(argv[1], "rb");
	FILE *OutputFile = fopen(argv[2], "w+a");
	if(!InputFile) exit(1);
	if(!OutputFile) exit(1);

	double pay = atoi(argv[3]);
	EMPLOYEE temp;

	printf("%-10s%-10s%-10s\n", "ID:", "Name:", "Salary:");
	while(fread(&temp, sizeof(EMPLOYEE), 1, InputFile)){
		printf("%-10d%-10s%-8.1lf\n", temp.num, temp.name, temp.hours * pay);
		fprintf(OutputFile, "%-10d%-10s%-8.1lf\n", temp.num, temp.name, temp.hours * pay);
	}

	fclose(InputFile);
	fclose(OutputFile);

	return 0;
}
