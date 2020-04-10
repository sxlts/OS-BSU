#include "util.h"

bool CALL(char *cmd){
	STARTUPINFO info = {sizeof(info)};
	PROCESS_INFORMATION processInfo;

	if(CreateProcess(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
	WaitForSingleObject(processInfo.hProcess, INFINITE);
	else return false;

	return true;
}

int main(int argc, char** argv){
	char BinaryFileName[50];
	unsigned int MessageNumber;
	scanf("%49s", BinaryFileName);
	scanf("%u", &MessageNumber);
	
	HANDLE hfile = CreateFile(
		BinaryFileName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_WRITE,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_HIDDEN,
		NULL
	);

	
	unsigned int SenderNumber;
	scanf("%u", &SenderNumber);
	
	hSenderStart = malloc(sizeof(HANDLE) * SenderNumber);
	for(int i = 0 ; i < SenderNumber ; ++i) hSenderStart[i] = CreateEvent(NULL, TRUE, FALSE, NULL);

	char cmd[100];
	sprintf(cmd, "sender.exe %s %u", BinaryFileName, MessageNumber);

	for(int i = 0 ; i < SenderNumber ; ++i) if(!CALL(cmd)) return 1;

	WaitForMultipleObjects(SenderNumber, hSenderStart, TRUE, INFINITE);

	while(true){
		char c;
		scanf("%c", &c);

		if(c == 'E' || c == 'e'){
			CloseHandle(hfile);
			DeleteFileA(BinaryFileName);
			return 0;
		}else{
			
		}
	}
}
