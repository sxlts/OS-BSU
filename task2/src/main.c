#include <stdio.h>
#include <windows.h>


int* array;
unsigned int Size;
int MINi = 0, MAXi = 0;
double AVG = 0;

DWORD WINAPI MIN_MAX(CONST LPVOID lpParam) {
	for(int i = 0 ; i < Size ; ++i){
		if(array[i] < array[MINi]) MINi = i;
		Sleep(7);
		if(array[i] > array[MAXi]) MAXi = i;
		Sleep(7);
	}

	printf("MIN: %d\nMAX: %d\n", array[MINi], array[MAXi]);
	ExitThread(0);
}

DWORD WINAPI AVERAGE(CONST LPVOID lpParam) {
	long long int sum = 0;
	for(int i = 0 ; i < Size ; ++i){
		sum+=array[i];
		Sleep(12);
	}
	AVG = (double)sum / (double)Size;

	printf("AVERAGE: %.2lf\n", AVG);
	ExitThread(0);
}

int main(int argc, char** argv){
	printf("give array size : ");
	Size = 200000;

	while(Size > 100000 || Size == 0)
		scanf("%u", &Size);

	array = malloc(Size * sizeof(int));
	printf("give %u array elements\n", Size);

	for(int i = 0 ; i < Size ; ++i)	scanf("%d", &array[i]);

	HANDLE hMIN_MAX = CreateThread(
		NULL,
		0,
		&AVERAGE,
		NULL,
		0,
		NULL
	);
	HANDLE hAVERAGE = CreateThread(
		NULL,
		0,
		&MIN_MAX,
		NULL,
		0,
		NULL
	);

	if(hMIN_MAX == NULL) printf("Error with %s!\n", "MIN_MAX thread");
	else {
		WaitForSingleObject(hMIN_MAX, INFINITE);
		CloseHandle(hMIN_MAX);
	}
	if(hAVERAGE == NULL) printf("Error with %s!\n", "AVERAGE thread");
	else {
		WaitForSingleObject(hAVERAGE, INFINITE);
		CloseHandle(hAVERAGE);
	}

	free(array);
	printf("Finished!\n");
	return 0;
}
