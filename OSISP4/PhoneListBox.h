#pragma once
#include "ItemRecord.h"



// PhoneListBox


class PhoneListBox : public CVSListBox
{
	DECLARE_DYNAMIC(PhoneListBox)

public:
	PhoneListBox();
	virtual ~PhoneListBox();
	void OnSelectionChanged();
	HWND hDlg;

protected:
	DECLARE_MESSAGE_MAP()
private:
	void OnItemClickFillEdit(int index, HWND hDlg);
	Record* arrayOfRecords;
};


