#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

CRITICAL_SECTION cs;
HANDLE hEventStart;
HANDLE* hEventWait;
HANDLE* hEventContinue;

int* Array;
unsigned int ArraySize = 0;
char status;
int* Numbers;

DWORD WINAPI marker(const LPVOID lpParam){
	WaitForSingleObject(hEventStart, INFINITE);

	int n = *(int*)lpParam;

	srand(n);
	int index;
	unsigned int count = 0;
	while(status == 1){
		while(Array[index = rand()%ArraySize] == 0){
			Sleep(5);
			EnterCriticalSection( &cs );
			Array[index] = n;
			count++;
			LeaveCriticalSection( &cs );
			Sleep(5);
		}

		printf("thread:%d count:%d index:%d\n", n, count, index);

		Sleep(15);
		SetEvent(hEventWait[n - 1]);
		WaitForSingleObject(hEventContinue[n - 1], INFINITE);
		ResetEvent(hEventWait[n - 1]);
		ResetEvent(hEventContinue[n - 1]);
	}
	for(int i = 0 ; i < ArraySize ; ++i) if(Array[i] == n) Array[i] = 0;
	SetEvent(hEventWait[n - 1]);
	ExitThread(0);
}

int main(){
	hEventStart = CreateEvent(NULL, TRUE, FALSE, NULL);
	printf("give array size\n");
	scanf("%d", &ArraySize);
	Array = malloc(ArraySize * sizeof(int));
	for(int i = 0 ; i < ArraySize ; ++i){
		Array[i] = 0;
	}

	int ThredCount;
	printf("give thread number\n");
	scanf("%d" , &ThredCount);
	HANDLE* THREAD = malloc(ThredCount * sizeof(HANDLE*));
	hEventWait = malloc(ThredCount * sizeof(HANDLE*));
	hEventContinue = malloc(ThredCount * sizeof(HANDLE*));
	Numbers = malloc(ThredCount * sizeof(HANDLE*));

	for(int i = 0 ; i < ThredCount ; ++i) {
		Numbers[i] = i + 1;
		THREAD[i] = CreateThread(NULL, 0, &marker,  (LPVOID)(&Numbers[i]), 0, NULL);
		hEventWait[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		hEventContinue[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
	}
	
	status = 1;
	InitializeCriticalSection( &cs );
	SetEvent(hEventStart);

	for(int i = 0 ; i < ThredCount ; ++i){
		WaitForMultipleObjects(ThredCount, hEventWait, TRUE, INFINITE);

		for(int i = 0 ; i < ArraySize ; ++i) printf("%d ", Array[i]);
		printf("\n");

		printf("give thread index to close\n");
		unsigned int CloseThreadIndex;
		scanf("%u", &CloseThreadIndex);

		status = 0;
		SetEvent(hEventContinue[(CloseThreadIndex - 1)% ThredCount]);
		WaitForSingleObject(THREAD[(CloseThreadIndex - 1)% ThredCount], INFINITE);	

		for(int i = 0 ; i < ArraySize ; ++i) printf("%d ", Array[i]);
		printf("\n");

		status = 1;

		for(int j = 0 ; j < ThredCount ; ++j) SetEvent(hEventContinue[j]);
	}

	CloseHandle(hEventStart);
	for(int i = 0 ; i < ThredCount ; ++i) {
		CloseHandle(THREAD[i]);
		CloseHandle(hEventWait[i]);
		CloseHandle(hEventContinue[i]);
	}
	free(Array);

	return 0;
}

