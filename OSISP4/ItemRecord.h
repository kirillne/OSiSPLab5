#pragma once
#include "stdafx.h"

struct Record
{
	std::wstring Surname;
	std::wstring Name;
	std::wstring SecName;
	int PhoneNumber;
	std::wstring Streat;
	int House;
	int Building;
	int Flat;
};

Record* GetRecordArray();

