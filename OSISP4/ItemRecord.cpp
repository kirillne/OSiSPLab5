#include "stdafx.h"
#include "ItemRecord.h"


static Record arrayOfRecords[0x10000];

Record* GetRecordArray()
{
	return arrayOfRecords;
}