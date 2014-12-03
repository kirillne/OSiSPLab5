// AddRecord.cpp : implementation file
//

#include "stdafx.h"
#include "OSISP4.h"
#include "AddRecord.h"
#include "afxdialogex.h"

#define TEXT_SIZE 20


// AddRecord dialog

IMPLEMENT_DYNAMIC(AddRecord, CDialogEx)

AddRecord::AddRecord(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddRecord::IDD, pParent)
{

}

AddRecord::~AddRecord()
{
}

void AddRecord::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, CCancelButton);
	DDX_Control(pDX, IDOK, COkButton);
	DDX_Control(pDX, IDC_ADDRECORDBUILDING, AddRecordBuildingEdit);
	DDX_Control(pDX, IDC_ADDRECORDFLAT, AddRecordFlatEdit);
	DDX_Control(pDX, IDC_ADDRECORDHOUSE, AddRecordHouseEdit);
	DDX_Control(pDX, IDC_ADDRECORDNAME, AddRecordNameEdit);
	DDX_Control(pDX, IDC_ADDRECORDPHONE, AddRecordPhoneEdit);
	DDX_Control(pDX, IDC_ADDRECORDSECNAME, AddRecordSecNameEdit);
	DDX_Control(pDX, IDC_ADDRECORDSTREET, AddRecordStreetEdit);
	DDX_Control(pDX, IDC_ADDRECORDSURNAME, AddRecordSurnameEdit);
}


BEGIN_MESSAGE_MAP(AddRecord, CDialogEx)
	ON_BN_CLICKED(IDOK, &AddRecord::OnBnClickedAddrecordok)
	ON_BN_CLICKED(IDCANCEL, &AddRecord::OnBnClickedAddrecordcancel)
END_MESSAGE_MAP()


// AddRecord message handlers


void AddRecord::OnBnClickedAddrecordok()
{
	Record item;
	TCHAR textItem[TEXT_SIZE];
	try
	{
		CString *temp = new CString("1234567891");
		CWnd *DlgItem = new CWnd();
		//AddRecordSurnameEdit.Attach(GetDlgItem(IDC_ADDRECORDSURNAME)));
		/*item.Surname = (LPCSTR) temp.GetString();
		GetDlgItemTextW(IDC_ADDRECORDNAME, temp);
		item.Name = (LPCSTR) temp.GetString();
		GetDlgItemTextW(IDC_ADDRECORDSECNAME, temp);
		item.SecName = (LPCSTR) temp.GetString();
		GetDlgItemTextW(IDC_ADDRECORDPHONE, temp);
		item.PhoneNumber = _wtoi(temp);
		GetDlgItemTextW(IDC_ADDRECORDSTREET, temp);
		item.Streat = (LPCSTR) temp.GetString();
		GetDlgItemTextW(IDC_ADDRECORDHOUSE, temp);
		item.House = _wtoi(temp);*/
	}
	catch(...)
	{
		MessageBox(L"Fail");
	}
}


void AddRecord::OnBnClickedAddrecordcancel()
{
	// TODO: Add your control notification handler code here
}


void AddRecord::CallAddRecord(void)
{
	CDialog AddRecordDlg(IDD_ADDRECORDDIALOG);
	int result = AddRecordDlg.DoModal();
	if(result == IDCANCEL)
		AddRecordDlg.EndDialog(0);
	else if (result == IDOK)
	{
		AddRecord::OnBnClickedAddrecordok();
	}

}
