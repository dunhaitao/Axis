// CSelectDlg.cpp: 实现文件
//

#include "pch.h"
#include "Axis.h"
#include "CSelectDlg.h"
#include "afxdialogex.h"


// CSelectDlg 对话框

IMPLEMENT_DYNAMIC(CSelectDlg, CDialogEx)

CSelectDlg::CSelectDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SELECT_STOCK, pParent)
	, m_i_CurSel(-1)
	, m_str_stockCode(_T(""))
{
}

CSelectDlg::~CSelectDlg()
{
}

void CSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_STOCK_CODE, m_str_stockCode);
	DDX_Control(pDX, IDC_COMBO_STOCK_MARKET, m_ctl_stockMarket);
}

BEGIN_MESSAGE_MAP(CSelectDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_STOCK_MARKET, &CSelectDlg::OnCbnSelchangeComboStockMarket)
END_MESSAGE_MAP()

// CSelectDlg 消息处理程序


void CSelectDlg::OnCbnSelchangeComboStockMarket()
{
	this->m_i_CurSel = this->m_ctl_stockMarket.GetCurSel();
}


BOOL CSelectDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	this->m_ctl_stockMarket.AddString(this->STOCK_MARKETES_NAME[0]);
	this->m_ctl_stockMarket.AddString(this->STOCK_MARKETES_NAME[1]);

	return TRUE;
}
