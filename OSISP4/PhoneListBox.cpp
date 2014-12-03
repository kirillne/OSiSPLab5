// PhoneListBox.cpp : implementation file
//

#include "stdafx.h"
#include "OSISP4.h"
#include "PhoneListBox.h"



void PhoneListBox::OnItemClickFillEdit(int index, HWND hDlg)
{
	TCHAR buffer[20];
	Record temp = arrayOfRecords[index];
	::SetDlgItemText(hDlg, IDC_SURNAMEEDIT, temp.Surname.c_str());
	::SetDlgItemText(hDlg, IDC_NAMEEDIT, temp.Name.c_str());
	::SetDlgItemText(hDlg, IDC_SECNAMEEDIT, temp.SecName.c_str());
	::SetDlgItemText(hDlg, IDC_STREETEDIT, temp.Streat.c_str());
	_itow(temp.PhoneNumber, buffer, 10);
	::SetDlgItemText(hDlg, IDC_PHONEEDIT, buffer);
	_itow(temp.House, buffer,  10);
	::SetDlgItemText(hDlg, IDC_HOUSEEDIT, buffer);
	_itow(temp.Building, buffer, 10);
	::SetDlgItemText(hDlg, IDC_BUILDINGEDIT, buffer);
	_itow(temp.Flat, buffer,  10);
	::SetDlgItemText(hDlg, IDC_FLATEDIT, buffer);
}

// PhoneListBox

IMPLEMENT_DYNAMIC(PhoneListBox, CVSListBox)

PhoneListBox::PhoneListBox()
{
	arrayOfRecords = GetRecordArray();
}

PhoneListBox::~PhoneListBox()
{
}

void PhoneListBox::OnSelectionChanged()
{
	CString str;
	int index = this->GetSelItem();
	if (index != -1)
	{
		OnItemClickFillEdit(index, hDlg);
	}
}

BEGIN_MESSAGE_MAP(PhoneListBox, CVSListBox)
END_MESSAGE_MAP()



// PhoneListBox message handlers


