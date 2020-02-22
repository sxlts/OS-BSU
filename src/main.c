#include "header.h"

int main(int argc, char** argv){
	char BinaryFileName[50];
	char FileName[50];
	char payment[8];
	char LineNumber[8];

	printf("Enter binary file name: ");
	scanf("%49s", BinaryFileName);
	printf("Enter line number: ");
	scanf("%7s", LineNumber);
	
	char cmd[100];
	sprintf(cmd, "creator.exe %s %s", BinaryFileName, LineNumber);
	STARTUPINFO info = {sizeof(info)};
	PROCESS_INFORMATION processInfo;
	CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo);
	WaitForSingleObject(processInfo.hProcess, INFINITE);

	printf("Enter report file name: ");
	scanf("%49s", FileName);
	printf("Enter payment value: ");
	scanf("%7s", payment);

	sprintf(cmd, "reporter.exe %s %s %s", BinaryFileName, FileName, payment);
	CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo);
	WaitForSingleObject(processInfo.hProcess, INFINITE);

	printf("Finished!\n");
	return 0;
}
