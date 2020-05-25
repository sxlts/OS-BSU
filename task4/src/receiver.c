#include "util.h"

char cmd[100];
unsigned int MessageNumber;
char BinaryFileName[50];
char EventName[50];
int SenderNumber;
HANDLE * hProcessInfo;
HANDLE * hReady;
HANDLE hmutex;

HANDLE CALL(char *cmd){
	STARTUPINFO info = {sizeof(info)};
	PROCESS_INFORMATION processInfo;

	if(
			CreateProcess(
				NULL,
				cmd,
				NULL,
				NULL,
				TRUE,
				CREATE_NEW_CONSOLE,
				NULL,
				NULL,
				&info,
				&processInfo
			)
	) return processInfo.hProcess;
	else return NULL;
}

//thread to watch for closing senders
DWORD WINAPI SendersClose(CONST LPVOID lpParam) {
	WaitForMultipleObjects(SenderNumber, hProcessInfo, TRUE, INFINITE);
	SetEvent(hReady[0]);

	WaitForSingleObject(hmutex, INFINITE);
	printf("AllSendersClosed!\n");
	ReleaseMutex(hmutex);

	ExitThread(0);
}

int main(int argc, char** argv){
	
	printf("(receiver) give file name : ");
	scanf("%49s", BinaryFileName);
	printf("(receiver) give messages number : ");
	scanf("%u", &MessageNumber);

	hmutex = CreateMutex(NULL, FALSE, "Global\\FILEUSE");	

	hfile = CreateFile(
		BinaryFileName,
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		CREATE_NEW,
		FILE_ATTRIBUTE_HIDDEN,
		NULL
	);

	FileSize = 20 * MessageNumber + LIST_START;
	FileHead = LIST_START - 20;
	FileTail = LIST_START - 20;
    	updHead();
    	updTail();
    	updSize();
	
	printf("(receiver) give senders number : ");
	scanf("%d", &SenderNumber);
	
	HANDLE * hSenderStart = malloc(sizeof(HANDLE*) * SenderNumber);

	hReady = (HANDLE*) malloc(2 * sizeof(HANDLE*));
	hReady[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	hReady[1] = CreateEvent(NULL, TRUE, FALSE, "Sender");

	hProcessInfo = malloc(sizeof(HANDLE*) * SenderNumber);
	HANDLE hReceiver = CreateEvent(NULL, TRUE, FALSE, "Receiver");
	HANDLE hClosed = CreateEvent(NULL, TRUE, FALSE, "CloseProcess");

	//create start events for senders
	for(int i = 0 ; i < SenderNumber ; ++i) {
		sprintf(EventName, "%s%d", "Sender", i);
		hSenderStart[i] = CreateEvent(NULL, TRUE, FALSE, EventName);
	}

	//start senders
	for(unsigned int i = 0 ; i < SenderNumber ; ++i){
		sprintf(cmd, "sender.exe %s %d", BinaryFileName, i);
		hProcessInfo[i] = CALL(cmd);
		if(hProcessInfo[i] == NULL) return 1;
	} 

	//wait for all senders
	WaitForMultipleObjects(SenderNumber, hSenderStart, TRUE, INFINITE);
	//"supervisor" thread
	HANDLE hSendersClose = CreateThread(NULL, 0, &SendersClose, NULL, 0, NULL);

	//main loop
	while(true){
		WaitForSingleObject(hmutex, INFINITE);

		printf("type anything to read message ([exit!] to close process)\n");
		char c[10];
		scanf("%10s", c);
		if(strcmp("exit!", c) == 0){
			SetEvent(hClosed);
			printf("Receiver closed!\n");
			ReleaseMutex(hmutex);
			break;
		}

        	if(!ReadMsg()){
			printf("[buffer is empty!]\n");
			ResetEvent(hReady[1]);
			ReleaseMutex(hmutex);
			WaitForMultipleObjects(2, hReady, FALSE, INFINITE);
        	}else {
			printf("(receiver) %s\n", message);
			SetEvent(hReceiver);
			ReleaseMutex(hmutex);
		}
	}

	//clear
	WaitForMultipleObjects(SenderNumber, hProcessInfo, TRUE, INFINITE);
	for(int i = 0 ; i < SenderNumber ; ++i) CloseHandle(hProcessInfo[i]), CloseHandle(hSenderStart[i]);
	free(hProcessInfo);
	free(hSenderStart);
	CloseHandle(hReady[0]);
	CloseHandle(hReady[1]);
	free(hReady);
	CloseHandle(hClosed);
	CloseHandle(hmutex);
	CloseHandle(hfile);
	CloseHandle(hReceiver);
	CloseHandle(hSendersClose);
	DeleteFile(BinaryFileName);

	return 0;
}
