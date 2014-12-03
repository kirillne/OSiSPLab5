#include "stdafx.h"
#include "HashTable.h"

#define TABLESIZE 255 

HashTable::HashTable()
{
	headers = (std::vector<ListElement>**)calloc(sizeof(ListElement*), TABLESIZE);
	for (int i = 0; i < TABLESIZE; i++)
	{
		headers[i] = new std::vector<ListElement>();
	}
}

void HashTable::AddElement(std::wstring value, int index)
{
	int hash = GetHash(value);
	HashTable::ListElement element;
	element.Index = index;
	element.Value = value;
	headers[hash]->push_back(element);
}

int HashTable::GetHash(std::wstring value)
{
	int hash = 0;
	for (int i = 0; i < (value.size() > 3 ? 3 : value.size()); i++)
	{
		hash += (int)(value[i]);
	}
	hash %= TABLESIZE;
	return hash > -1 ? hash : -hash;
}

void  HashTable::GetIndex(std::wstring value, int* buf)
{
	int hash = GetHash(value);
	int foundedCount = 0;
	for (std::vector<HashTable::ListElement>::const_iterator it = headers[hash]->begin(); 
		it != headers[hash]->end(); ++it)
	{
		bool result = true;
		const TCHAR* elem = it->Value.c_str();
		const TCHAR* cvalue = value.c_str();
		for (int i = 0; i < value.size();i++)
		{
			if (elem[i] != cvalue[i]) result = false;
		}
		if (result)
		{
			buf[foundedCount++] = it->Index;
		}
	}
	buf[foundedCount] = -1;
}

HashTable::~HashTable()
{
	for (int i = 0; i < TABLESIZE; i++)
	{
		delete headers[i];
	}
	free(headers);
}
