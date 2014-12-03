
// Lab4.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Lab4.h"
#include "HashTable.h"

#define BLOCKSIZE 0x10000

bool databaseOpened = false;

void CloseDatabase();

void OpenDatabase();

HashTable* surNameHashTable;

HashTable* streatHashTable;

HashTable* phoneNumberHashTable;

CRITICAL_SECTION criticalSection;

HANDLE hMap;


BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	setlocale(LC_ALL, ".1251");
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (!databaseOpened)
			OpenDatabase();

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		if (databaseOpened)
			CloseDatabase();
		break;
	}
	return TRUE;
}

BOOL ReadLine(HANDLE hFile, TCHAR *pszBuffer, DWORD dwSize, int* readed)
{
	
	DWORD i, dwRead;
	
	if (!ReadFile(hFile, pszBuffer, dwSize, &dwRead, NULL) || (dwRead == 0))
	{
		SetLastError(ERROR_HANDLE_EOF);
		return FALSE;
	}

	for (i = 0; i < dwRead; i++)
	{
		if (pszBuffer[i] == '\r')
		{
			pszBuffer[i] = 0;
			if (i + 1 < dwRead && pszBuffer[i + 1] == '\n')
			{
				i++;
			}
			break;
		}
		else if (pszBuffer[i] == '\n')
		{
			pszBuffer[i] = 0;
			break;
		}
	}
	*readed = i + 1;

	if (i >= dwRead)
	{
		pszBuffer[i] = 0;
		
	}
	else
	{
		i++;
	}

	SetFilePointer(hFile, i*2 - dwRead, NULL, FILE_CURRENT);

	return TRUE;
}


int ReadElement(TCHAR *element, TCHAR* string, int start)
{
	int i = start;
	int elementIndex = 0;
	while (string[i] != ';')
	{
		element[elementIndex++] = string[i++];
	}
	element[elementIndex] = 0;
	return i + 1;
}

Record ConvertStringToRecord(TCHAR* string)
{
	TCHAR element[255];
	int elementStart = 0;
	Record record;

	elementStart = ReadElement(element, string, elementStart);
	record.PhoneNumber = _wtoi(element);

	elementStart = ReadElement(element, string, elementStart);
	record.Surname = std::wstring(element);

	elementStart = ReadElement(element, string, elementStart);
	record.Name = std::wstring(element);

	elementStart = ReadElement(element, string, elementStart);
	record.SecName = std::wstring(element);

	elementStart = ReadElement(element, string, elementStart);
	record.Streat = std::wstring(element);

	elementStart = ReadElement(element, string, elementStart);
	record.House = _wtoi(element);

	elementStart = ReadElement(element, string, elementStart);
	record.Building = _wtoi(element);

	elementStart = ReadElement(element, string, elementStart);
	record.Flat = _wtoi(element);
	return record;
}


void OpenDatabase()
{
	InitializeCriticalSection(&criticalSection);

	HANDLE hFile;
	hFile = CreateFile(TEXT("E:\\telbase.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	LARGE_INTEGER	fileSize;
	if (GetFileSizeEx(hFile, &fileSize) == FALSE) {
		CloseHandle(hFile);
		return;
	}

	hMap = CreateFileMapping(
		hFile,
		NULL,
		PAGE_READONLY,
		fileSize.HighPart, fileSize.LowPart, L"name");

	if (hFile == INVALID_HANDLE_VALUE)
	{
		int i = GetLastError();
		SetLastError(ERROR_FILE_INVALID);
		return;
	}
	
	surNameHashTable = new HashTable();
	streatHashTable = new HashTable();
	phoneNumberHashTable = new HashTable();

	TCHAR string[255];
	int size = 255;
	int stringLenght = 0;
	long long offset = 0;
	int blockNumber = 0;
	while (ReadLine(hFile, string, size, &stringLenght))
	{
		if (string[0] == 0) continue;
		Record record = ConvertStringToRecord(string);

		surNameHashTable->AddElement(record.Surname, offset,blockNumber);
		streatHashTable->AddElement(record.Streat, offset, blockNumber);
		phoneNumberHashTable->AddElement(std::to_wstring(record.PhoneNumber), offset, blockNumber);
		offset += stringLenght;
		if (offset >= BLOCKSIZE)
		{
			blockNumber++;
			offset -= BLOCKSIZE;
		}
	}
	databaseOpened = true;
	CloseHandle(hFile);
}

void CloseDatabase()
{
	delete surNameHashTable;
	delete streatHashTable;
	delete phoneNumberHashTable;
	CloseHandle(hMap);
	databaseOpened = false;
}

Record GetElement(ListElement element)
{
	TCHAR* string = (TCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, element.BlockOffset*BLOCKSIZE*sizeof(TCHAR), BLOCKSIZE);
	int a = GetLastError();
	string += element.Offset;
	Record result =  ConvertStringToRecord(string);
	UnmapViewOfFile(string);
	
	return result;
}

int Search(const TCHAR* surname, Record* buf, HashTable* hashTable)
{

	ListElement elemnts[255];
	int count = hashTable->GetIndex(std::wstring(surname), elemnts);
	for (int i = 0; i < count; i++)
	{
		buf[i] = GetElement(elemnts[i]);
	}
	return count;
}


LAB4_API int SearchSurname(const TCHAR* surname, Record* buf)
{
	if (databaseOpened)
	{
		return Search(surname, buf, surNameHashTable);
	}
	return 0;
}

LAB4_API int SearchStreat(const TCHAR* streat, Record* buf)
{
	if (databaseOpened)
	{
		return Search(streat, buf, streatHashTable);
	}
	return 0;
}


LAB4_API int SearchPhoneNumber(int number, Record* buf)
{
	if (databaseOpened)
	{
		return Search(std::to_wstring(number).c_str(), buf, phoneNumberHashTable);
	}
	return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   