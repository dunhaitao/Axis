#pragma once


// CSetupDlg 对话框

class CSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupDlg)

public:
	CSetupDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSetupDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETUP_TDX_HOME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 通达信目录
	CString m_str_tdxHome;
	afx_msg void OnBnClickedButtonLookupTdxHome();
};
