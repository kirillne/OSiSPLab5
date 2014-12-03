
// OSISP4Dlg.h : header file
//

#pragma once
#include "afxvslistbox.h"
#include "PhoneListBox.h"
#include "afxeditbrowsectrl.h"
#include "afxbutton.h"
#include "afxwin.h"


// COSISP4Dlg dialog
class COSISP4Dlg : public CDialogEx
{
// Construction
public:
	COSISP4Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OSISP4_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileExit();
	PhoneListBox Item;
	void RefreshItem(int,int);
	CMFCEditBrowseCtrl PhoneEdit;
	CStatic PhoneLabel;
	CMFCEditBrowseCtrl SurnameEdit;
	CStatic SurnameLabel;
	CMFCEditBrowseCtrl BuildingEdit;
	CMFCEditBrowseCtrl FlatEdit;
	CMFCEditBrowseCtrl HouseEdit;
	CMFCEditBrowseCtrl NameEdit;
	CMFCButton SearchPhoneButton;
	CMFCButton SearchStreetButton;
	CMFCButton SearchSurnameButton;
	CMFCEditBrowseCtrl SecnameEdit;
	CMFCEditBrowseCtrl StreetEdit;
	afx_msg void OnBnClickedSearchphonebutton();
	afx_msg void OnBnClickedSearchsurnamebutton();
	afx_msg void OnBnClickedSearchstreetbutton();
private:
	Record* arrayOfRecords;
};