#include "header.h"

int main(int argc, char **argv){
	if(argc < 3) exit(1);
	
	FILE *OutputFile = fopen(argv[1], "w+b");
	if(!OutputFile) exit(1);
	int LineNumber = atoi(argv[2]);

	for(int i = 0 ; i < LineNumber ; ++i){
		EMPLOYEE temp;
		printf("give line: ");
		scanf("%d%49s%lf", &temp.num, temp.name, &temp.hours);
		fwrite(&temp, sizeof(EMPLOYEE), 1, OutputFile);
	}

	fclose(OutputFile);
	return 0;
}
