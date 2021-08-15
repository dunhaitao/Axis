// CSetupDlg.cpp: 实现文件
//

#include "pch.h"
#include "Axis.h"
#include "CSetupDlg.h"
#include "afxdialogex.h"


// CSetupDlg 对话框

IMPLEMENT_DYNAMIC(CSetupDlg, CDialogEx)

CSetupDlg::CSetupDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETUP_TDX_HOME, pParent)
	, m_str_tdxHome(_T(""))
{

}

CSetupDlg::~CSetupDlg()
{
}

void CSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TDX_HOME, m_str_tdxHome);
}


BEGIN_MESSAGE_MAP(CSetupDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LOOKUP_TDX_HOME, &CSetupDlg::OnBnClickedButtonLookupTdxHome)
END_MESSAGE_MAP()

// CSetupDlg 消息处理程序

void CSetupDlg::OnBnClickedButtonLookupTdxHome()
{
	TCHAR lpszTdxHome[1024] = { 0 };
	BROWSEINFO bInfo = { 0 };
	bInfo.hwndOwner = this->m_hWnd;
	bInfo.lpszTitle = _T("选择通达信目录");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NONEWFOLDERBUTTON;
	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (nullptr == lpDlist) {
		return;
	}
	SHGetPathFromIDList(lpDlist, lpszTdxHome);
	this->m_str_tdxHome.SetString(lpszTdxHome);
	UpdateData(FALSE);
}
