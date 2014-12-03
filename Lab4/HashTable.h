#pragma once
struct ListElement
{
	long long Offset;
	int BlockOffset;
	std::wstring Value;
};

class HashTable
{
public:
	
	HashTable();
	~HashTable();
	void AddElement(std::wstring value, long long offset, int blockOffset);
	int GetIndex(std::wstring value, ListElement* buf);
private:
	std::vector<ListElement>** headers;
	int GetHash(std::wstring value);
};

