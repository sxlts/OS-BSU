#include "header.h"

int main(int argc, char **argv){
	if(argc < 3) exit(1);
	
	FILE *OutputFile = fopen(argv[1], "a+b");
	if(!OutputFile) exit(1);
	int LineNumber = atoi(argv[2]);

	printf("give %d lines:\n", LineNumber);

	for(int i = 0 ; i < LineNumber ; ++i){
		EMPLOYEE temp;
		scanf("%d%49s%lf", &temp.num, temp.name, &temp.hours);
		fwrite(&temp, sizeof(EMPLOYEE), 1, OutputFile);
	}
	printf("Report added to %s !\n", argv[1]);

	fclose(OutputFile);
	return 0;
}
