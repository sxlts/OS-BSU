#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <stdbool.h>

#define LIST_START 3*sizeof(int)
#define emptyMsg "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"

HANDLE hfile;
int FileHead = 0, FileTail = 0, FileSize = 0;

char message[21] = "";

void updHead(){
    	FileHead+= 20;
    	if(FileHead == FileSize) FileHead = LIST_START;

    	SetFilePointer(hfile, 0 * sizeof(int), NULL, FILE_BEGIN);
	WriteFile(hfile, &FileHead, sizeof(int), NULL, NULL);
}
void updTail(){
	FileTail+= 20;
    	if(FileTail == FileSize) FileTail = LIST_START;

    	SetFilePointer(hfile, 1 * sizeof(int), NULL, FILE_BEGIN);
	WriteFile(hfile, &FileTail, sizeof(int), NULL, NULL);
}
void updSize(){
    	SetFilePointer(hfile, 2 * sizeof(int), NULL, FILE_BEGIN);
	WriteFile(hfile, &FileSize, sizeof(int), NULL, NULL);
}
void getHead(){
    	SetFilePointer(hfile, 0 * sizeof(int), NULL, FILE_BEGIN);
	ReadFile(hfile, &FileHead, sizeof(int), NULL, NULL);
}
void getTail(){
    	SetFilePointer(hfile, 1 * sizeof(int), NULL, FILE_BEGIN);
	ReadFile(hfile, &FileTail, sizeof(int), NULL, NULL);
}
void getSize(){
    	SetFilePointer(hfile, 2 * sizeof(int), NULL, FILE_BEGIN);
	ReadFile(hfile, &FileSize, sizeof(int), NULL, NULL);
}
bool FreeToWrite(){
    	getHead();

    	char tempMsg[21];
    	SetFilePointer(hfile, FileHead, NULL, FILE_BEGIN);
	ReadFile(hfile, tempMsg, 20 * sizeof(char), NULL, NULL);

	Sleep(5);

    	return (strcmp(tempMsg, emptyMsg) == 0);
}
bool FreeToRead(){
    	getTail();

    	char tempMsg[21];
    	SetFilePointer(hfile, FileTail, NULL, FILE_BEGIN);
	ReadFile(hfile, tempMsg, 20 * sizeof(char), NULL, NULL);
    
	Sleep(5);

    	return (strcmp(tempMsg, emptyMsg) != 0);
}
bool WriteMsg(){
	if(FreeToWrite() == false) return false;

    	SetFilePointer(hfile, FileHead, NULL, FILE_BEGIN);
	WriteFile(hfile, message, 20 * sizeof(char), NULL, NULL);
    	updHead();
    
    	return true;
}
bool ReadMsg(){
    	if(FreeToRead() == false) return false;

    	SetFilePointer(hfile, FileTail, NULL, FILE_BEGIN);
	ReadFile(hfile, message, 20 * sizeof(char), NULL, NULL);

	SetFilePointer(hfile, FileTail, NULL, FILE_BEGIN);
	WriteFile(hfile, emptyMsg, 20 * sizeof(char), NULL, NULL);

    	updTail();

    	return true;
}
