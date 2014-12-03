// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the LAB4_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// LAB4_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef LAB4_EXPORTS
#define LAB4_API  __declspec(dllexport)
#else
#define LAB4_API  __declspec(dllimport)
#endif

#include <string>
#include <tchar.h>

struct LAB4_API  Record
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

LAB4_API int SearchSurname(const TCHAR* surname, Record* buf);
 
LAB4_API int SearchStreat(const TCHAR* surname, Record* buf);

LAB4_API int SearchPhoneNumber(int number, Record* buf);

LAB4_API void Change(Record oldRecord, Record newRecord);

LAB4_API void Add(Record record);

LAB4_API void Delete(Record record);


