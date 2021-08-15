#pragma once


// CSelectDlg 对话框

class CSelectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectDlg)

public:
	CSelectDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSelectDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SELECT_STOCK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned int m_i_CurSel;
	CString m_str_stockCode;
	CComboBox m_ctl_stockMarket;
	afx_msg void OnCbnSelchangeComboStockMarket();
public:
	LPCTSTR STOCK_MARKETES_NAME[2] = { _T("上海"),_T("深圳") };
	LPCTSTR STOCK_MARKETES_CODE[2] = { _T("sh"),_T("sz") };
	virtual BOOL OnInitDialog();
};
