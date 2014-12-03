
// Lab4.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Lab4.h"
#include "HashTable.h"

bool databaseOpened = false;

void CloseDatabase();

void OpenDatabase();

std::vector<Record>* dataBase;

HashTable* surNameHashTable;

HashTable* streatHashTable;

HashTable* phoneNumberHashTable;

CRITICAL_SECTION criticalSection;


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

BOOL ReadLine(HANDLE hFile, TCHAR *pszBuffer, DWORD dwSize)
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

void OpenDatabase()
{
	InitializeCriticalSection(&criticalSection);
	HANDLE hFile;
	hFile = CreateFile(TEXT("E:\\telbase.txt"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		int i = GetLastError();
		SetLastError(ERROR_FILE_INVALID);
		return;
	}
	
	surNameHashTable = new HashTable();
	streatHashTable = new HashTable();
	phoneNumberHashTable = new HashTable();
	dataBase = new std::vector<Record>();

	TCHAR string[255];
	int size = 255;
	while (ReadLine(hFile, string, size))
	{
		if (string[0] == 0) continue;
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

		int index = dataBase->size();
		dataBase->push_back(record);
		surNameHashTable->AddElement(record.Surname, index);
		streatHashTable->AddElement(record.Streat, index);
		phoneNumberHashTable->AddElement(std::to_wstring(record.PhoneNumber), index);

	}
	databaseOpened = true;
	CloseHandle(hFile);
}



void CloseDatabase()
{
	delete surNameHashTable;
	delete streatHashTable;
	delete phoneNumberHashTable;
	delete dataBase;
	databaseOpened = false;
}

int Search(const TCHAR* surname, Record* buf, HashTable* hashTable)
{

	int indexes[255];
	hashTable->GetIndex(std::wstring(surname), indexes);
	int i = 0;
	while (indexes[i] != -1)
	{
		buf[i] = dataBase->at(indexes[i]);
		i++;
	}
	return i;
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


void RewriteDatabase()
{
	if (databaseOpened)
	{
		HANDLE hFile;
		hFile = CreateFile(TEXT("E:\\telbase.txt"), GENERIC_WRITE, 0, NULL,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			int i = GetLastError();
			SetLastError(ERROR_FILE_INVALID);
			return;
		}

		for (std::vector<Record>::const_iterator it = dataBase->begin();
			it != dataBase->end(); ++it)
		{
			std::wstring text = std::to_wstring(it->PhoneNumber) + L";" + it->Surname + L";" + it->Name + L";"
				+ it->SecName + L";" + it->Streat + L";" + std::to_wstring(it->House) + L";" +
				std::to_wstring(it->Building) + L";" + std::to_wstring(it->Flat) + L"\r\n";
 			const TCHAR* buf = text.c_str();
			WriteFile(hFile, buf, text.size()*sizeof(TCHAR), NULL, NULL);
		}
		CloseHandle(hFile);
	}
}

LAB4_API void Change(Record oldRecord, Record newRecord)
{
	EnterCriticalSection(&criticalSection);
	if (databaseOpened)
	{
		int buf[255];
		phoneNumberHashTable->GetIndex(std::to_wstring(oldRecord.PhoneNumber), buf);
		int index = buf[0];
		dataBase->operator[](index) = newRecord;
		RewriteDatabase();
		OpenDatabase();
	}
	LeaveCriticalSection(&criticalSection);
}



LAB4_API void Add( Record record)
{
	EnterCriticalSection(&criticalSection);
	if (databaseOpened)
	{
		dataBase->push_back(record);
		RewriteDatabase();
		OpenDatabase();
	}
	LeaveCriticalSection(&criticalSection);
}

LAB4_API void Delete(Record record)
{
	EnterCriticalSection(&criticalSection);
	if (databaseOpened)
	{
		for (std::vector<Record>::iterator it = dataBase->begin(); it != dataBase->end(); it++)
		{
			if (it->PhoneNumber == record.PhoneNumber)
			{
				dataBase->erase(it);
				break;  
			}
		}
		RewriteDatabase();
		OpenDatabase();
	}
	LeaveCriticalSection(&criticalSection);
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    