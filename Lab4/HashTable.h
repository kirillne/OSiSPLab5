#pragma once
class HashTable
{
public:
	struct ListElement
	{
		int Offset;
		std::wstring Value;
	};
	HashTable();
	~HashTable();
	void AddElement(std::wstring value, int offset);
	void GetIndex(std::wstring value, int* buf);
private:
	std::vector<ListElement>** headers;
	int GetHash(std::wstring value);
};

