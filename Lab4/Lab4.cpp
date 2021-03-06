
// Lab4.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Lab4.h"
#include "HashTable.h"

#define BLOCKSIZE 0x100000

bool databaseOpened = false;

void CloseDatabase();

void OpenDatabase();

HashTable* surNameHashTable;

HashTable* streatHashTable;

HashTable* phoneNumberHashTable;

CRITICAL_SECTION criticalSection;

HANDLE hMap;

LARGE_INTEGER	fileSize;


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


long long  ReadElement(TCHAR *element, TCHAR* string, long long start)
{
	long long  i = start;
	int elementIndex = 0;
	while (string[i] != ';' && string[i] != '\r' && string[i] != '\0')
	{
		element[elementIndex++] = string[i++];
	}
	element[elementIndex] = 0;
	return i + 1;
}

Record ConvertStringToRecord(TCHAR* string, long long* elementStart)
{
	TCHAR element[255];
	Record record;
	if (string[*elementStart] == 65279) (*elementStart)++;

	*elementStart = ReadElement(element, string, *elementStart);
	record.PhoneNumber = _wtoi(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.Surname = std::wstring(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.Name = std::wstring(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.SecName = std::wstring(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.Streat = std::wstring(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.House = _wtoi(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.Building = _wtoi(element);

	*elementStart = ReadElement(element, string, *elementStart);
	record.Flat = _wtoi(element);
	if (string[*elementStart] == '\r') (*elementStart)++;
	if (string[*elementStart] == '\n') (*elementStart)++;
	return record;
}


void OpenDatabase()
{
	InitializeCriticalSection(&criticalSection);

	HANDLE hFile;
	hFile = CreateFile(TEXT("E:\\telbase.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	
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

	/*TCHAR* string;
	int stringLenght = 0;
	long long offset = 0;
	int blockNumber = 0;
	long long   totalOffset = 0;
	long long   maxOffset = 0;
	while (totalOffset < fileSize.QuadPart)
	{
		long long lenght = BLOCKSIZE*sizeof(TCHAR)* 2;
		maxOffset = BLOCKSIZE;
		if ((blockNumber + 2)*sizeof(TCHAR)*BLOCKSIZE > fileSize.QuadPart)
		{
			lenght = fileSize.QuadPart - blockNumber*BLOCKSIZE*sizeof(TCHAR);
			maxOffset = lenght / sizeof(TCHAR);
		}
		string = (TCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, blockNumber*BLOCKSIZE*sizeof(TCHAR), lenght);
		while (offset < maxOffset)
		{
			long long prevOffset = offset;
			Record record = ConvertStringToRecord(string, &offset);
			surNameHashTable->AddElement(record.Surname, prevOffset, blockNumber);
			streatHashTable->AddElement(record.Streat, offset, blockNumber);
			phoneNumberHashTable->AddElement(std::to_wstring(record.PhoneNumber), offset, blockNumber);
			totalOffset += (offset - prevOffset)*sizeof(TCHAR);
		}
		blockNumber++;
		offset -= BLOCKSIZE;
	}*/
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
	long long lenght = BLOCKSIZE*sizeof(TCHAR)* 2;
	if ((element.BlockOffset + 2)*sizeof(TCHAR)*BLOCKSIZE > fileSize.QuadPart)
	{
		lenght = fileSize.QuadPart - element.BlockOffset*BLOCKSIZE*sizeof(TCHAR);
	}
	TCHAR* string = (TCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, element.BlockOffset*BLOCKSIZE*sizeof(TCHAR), lenght);
	string += element.Offset;
	long long readOffset = 0;
	Record result = ConvertStringToRecord(string, &readOffset);
	UnmapViewOfFile(string);
	
	return result;
}


std::wstring GetSurName(Record record)
{
	return record.Surname;
}

std::wstring GetStreat(Record record)
{
	return record.Streat;
}


std::wstring GetNumber(Record record)
{
	return std::to_wstring(record.PhoneNumber);
}


int Search(const TCHAR* surname, Record* buf, HashTable* hashTable,Operation operation)
{

	/*ListElement elemnts[255];
	int count = hashTable->GetIndex(std::wstring(surname), elemnts);
	for (int i = 0; i < count; i++)
	{
		buf[i] = GetElement(elemnts[i]);
	}
	return count;*/

	int surnameLenght = _tcslen(surname) ;
	int count = 0;
	TCHAR* string;
	int stringLenght = 0;
	long long offset = 0;
	int blockNumber = 0;
	long long   totalOffset = offset;
	long long   maxOffset = 0;
	while (totalOffset < fileSize.QuadPart)
	{
		long long lenght = BLOCKSIZE*sizeof(TCHAR)* 2;
		maxOffset = BLOCKSIZE;
		if ((blockNumber + 2)*sizeof(TCHAR)*BLOCKSIZE > fileSize.QuadPart)
		{
			lenght = fileSize.QuadPart - blockNumber*BLOCKSIZE*sizeof(TCHAR);
			maxOffset = lenght / sizeof(TCHAR);
		}
		string = (TCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, blockNumber*BLOCKSIZE*sizeof(TCHAR), lenght);
		while (offset < maxOffset)
		{
			long long prevOffset = offset;
			if (string[offset] == '\r')
			{
				offset += 2;
				continue;
			}
			Record record = ConvertStringToRecord(string, &offset);
			bool result = true;
			std::wstring elem = operation(record).c_str();
			int compareLenght = elem.size() > surnameLenght ? surnameLenght : elem.size();
			for (int i = 0; i < compareLenght; i++)
			{
				if (elem[i] != surname[i])
				{
					result = false;
					break;
				}
			}
			if (result)
			{
				buf[count++] = record;
			}
			totalOffset += (offset - prevOffset)*sizeof(TCHAR);
		}
		blockNumber++;
		offset -= BLOCKSIZE;
	}
	return count;
}


LAB4_API int SearchSurname(const TCHAR* surname, Record* buf)
{
	if (databaseOpened)
	{
		return Search(surname, buf, surNameHashTable, GetSurName);
	}
	return 0;
}

LAB4_API int SearchStreat(const TCHAR* streat, Record* buf)
{
	if (databaseOpened)
	{
		return Search(streat, buf, streatHashTable, GetStreat);
	}
	return 0;
}


LAB4_API int SearchPhoneNumber(int number, Record* buf)
{
	if (databaseOpened)
	{
		return Search(std::to_wstring(number).c_str(), buf, phoneNumberHashTable, GetNumber);
	}
	return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   