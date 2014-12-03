
// OSISP4Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OSISP4.h"
#include "OSISP4Dlg.h"
#include "afxdialogex.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



typedef  int (*SearchSurnameType)(const TCHAR* surname, Record* buf);

typedef  int(*SearchPhoneType)(int number, Record* buf);

typedef  int(*SearchStreetType)(const TCHAR* street, Record* buf);



SearchStreetType SearchStreet;
SearchSurnameType SearchSurname;
SearchPhoneType SearchPhone;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COSISP4Dlg dialog



COSISP4Dlg::COSISP4Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COSISP4Dlg::IDD, pParent)
{
	arrayOfRecords = GetRecordArray();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COSISP4Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PHONELISTBOXCONTROL, Item);
	DDX_Control(pDX, IDC_PHONEEDIT, PhoneEdit);
	DDX_Control(pDX, IDC_PHONELABEL, PhoneLabel);
	DDX_Control(pDX, IDC_SURNAMEEDIT, SurnameEdit);
	DDX_Control(pDX, IDC_SURNAMELABEL, SurnameLabel);
	DDX_Control(pDX, IDC_BUILDINGEDIT, BuildingEdit);
	DDX_Control(pDX, IDC_FLATEDIT, FlatEdit);
	DDX_Control(pDX, IDC_HOUSEEDIT, HouseEdit);
	DDX_Control(pDX, IDC_NAMEEDIT, NameEdit);
	DDX_Control(pDX, IDC_SEARCHPHONEBUTTON, SearchPhoneButton);
	DDX_Control(pDX, IDC_SEARCHSTREETBUTTON, SearchStreetButton);
	DDX_Control(pDX, IDC_SEARCHSURNAMEBUTTON, SearchSurnameButton);
	DDX_Control(pDX, IDC_SECNAMEEDIT, SecnameEdit);
	DDX_Control(pDX, IDC_STREETEDIT, StreetEdit);
}

BEGIN_MESSAGE_MAP(COSISP4Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_EXIT, &COSISP4Dlg::OnFileExit)
	ON_BN_CLICKED(IDC_SEARCHPHONEBUTTON, &COSISP4Dlg::OnBnClickedSearchphonebutton)
	ON_BN_CLICKED(IDC_SEARCHSURNAMEBUTTON, &COSISP4Dlg::OnBnClickedSearchsurnamebutton)
	ON_BN_CLICKED(IDC_SEARCHSTREETBUTTON, &COSISP4Dlg::OnBnClickedSearchstreetbutton)
END_MESSAGE_MAP()


// COSISP4Dlg message handlers

BOOL COSISP4Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//SetDllDirectory(NULL);
	HMODULE hLibrary;
	hLibrary = LoadLibrary(L"Lab4.dll");
	DWORD error = GetLastError();
	if (hLibrary == NULL)
		return FALSE;
	else
	{
		SearchSurname = (SearchSurnameType)GetProcAddress(hLibrary, "?SearchSurname@@YAHPB_WPAURecord@@@Z");
		SearchPhone = (SearchPhoneType)GetProcAddress(hLibrary, "?SearchPhoneNumber@@YAHHPAURecord@@@Z");
		SearchStreet = (SearchStreetType)GetProcAddress(hLibrary, "?SearchStreat@@YAHPB_WPAURecord@@@Z");
	}
	Item.hDlg = GetSafeHwnd();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COSISP4Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COSISP4Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COSISP4Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void COSISP4Dlg::OnFileExit()
{
	DestroyWindow();
}

void COSISP4Dlg::OnBnClickedSearchphonebutton()
{
	int length = Item.GetCount();
	TCHAR temp[20];
	int phoneNumber;
	GetDlgItemText(IDC_PHONEEDIT, temp, 20);
	phoneNumber = _wtoi(temp);
	int number = SearchPhone(phoneNumber, arrayOfRecords);
	RefreshItem(number, length);
}


void COSISP4Dlg::OnBnClickedSearchsurnamebutton()
{
	int length = Item.GetCount();
	TCHAR temp[20];
	GetDlgItemText(IDC_SURNAMEEDIT, temp, 20);
	std::wstring surname(temp);
	int number = SearchSurname(surname.c_str(), arrayOfRecords);
	RefreshItem(number, length);
}


void COSISP4Dlg::OnBnClickedSearchstreetbutton()
{
	int length = Item.GetCount();
	TCHAR temp[20];
	GetDlgItemText(IDC_STREETEDIT, temp, 20);
	std::wstring street(temp);
	int number = SearchStreet(street.c_str(), arrayOfRecords);
	RefreshItem(number, length);
}


void COSISP4Dlg::RefreshItem(int resultLength,int deleteLength )
{
	for (int i = 0; i < deleteLength; i++)
		Item.RemoveItem(0);
	for (int i = 0; i < resultLength; i++)
	{
		std::wstring superStro4ka;
		superStro4ka += std::to_wstring(arrayOfRecords[i].PhoneNumber);
		superStro4ka += (L" " + arrayOfRecords[i].Surname);
		superStro4ka += (L" " + arrayOfRecords[i].Name);
		superStro4ka += (L" " + arrayOfRecords[i].SecName);
		superStro4ka += (L" " + arrayOfRecords[i].Streat);
		superStro4ka += (L" " + std::to_wstring(arrayOfRecords[i].House));
		superStro4ka += (L" " + std::to_wstring(arrayOfRecords[i].Building));
		superStro4ka += (L" " + std::to_wstring(arrayOfRecords[i].Flat));
		Item.AddItem(superStro4ka.c_str());
	}

}

//MessageBox(L"Delete done");