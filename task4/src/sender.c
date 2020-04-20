#include "util.h"

int main(int argc, char** argv){
    	char EventName[50];
	hfile = CreateFile(
		argv[1],
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_HIDDEN,
		NULL
	);	

	sprintf(EventName, "Sender%s", argv[2]);

	HANDLE SenderStart = OpenEvent(EVENT_ALL_ACCESS, TRUE, EventName);
	HANDLE hSender = OpenEvent(EVENT_ALL_ACCESS, TRUE, "Sender");
	HANDLE * hReady = (HANDLE*) malloc(2 * sizeof(HANDLE*));
	hReady[0] = OpenEvent(EVENT_ALL_ACCESS, TRUE, "Receiver");
	hReady[1] = OpenEvent(EVENT_ALL_ACCESS, TRUE, "CloseProcess");

    	getSize();
	getHead();
	getTail();

	HANDLE hmutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, "Global\\FILEUSE");

	SetEvent(SenderStart);

	while(true){
		WaitForSingleObject(hmutex, INFINITE);

		printf("(sender %s) Give message (exit! to close process)\n", argv[2]);
		scanf("%s", message);

		if(strcmp("exit!", message) == 0) {
			ReleaseMutex(hmutex);
			break;
		}
        
		if(!WriteMsg()){
			printf("(sender %s) [buffer is full!]\n", argv[2]);
			ResetEvent(hReady[0]);
			ReleaseMutex(hmutex);
			WaitForMultipleObjects(2, hReady, FALSE, INFINITE);
		}else{
			SetEvent(hSender);
			ReleaseMutex(hmutex);
		}

	}
	CloseHandle(SenderStart);
	CloseHandle(hSender);
	CloseHandle(hReady[0]);
	CloseHandle(hReady[1]);
	free(hReady);
	CloseHandle(hmutex);
	CloseHandle(hfile);

	return 0;
}
