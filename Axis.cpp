
// Axis.cpp: 定义应用程序的类行为。
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Axis.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "CSetupDlg.h"
#include "CSelectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAxisApp

BEGIN_MESSAGE_MAP(CAxisApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CAxisApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CAxisApp::OnFileNew)
	ON_COMMAND(ID_MENU_ITEM_SETUP_TDX_HOME, &CAxisApp::OnMenuItemSetupTdxHome)
END_MESSAGE_MAP()


// CAxisApp 构造

CAxisApp::CAxisApp() noexcept :
	m_hMDIAccel(NULL),
	m_hMDIMenu(NULL)
{
	m_bHiColorIcons = TRUE;


	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Axis.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	Gdiplus::GdiplusStartup(&GdiplusToken, &GdiplusStartupInput, NULL);
}

CAxisApp::~CAxisApp()
{
	Gdiplus::GdiplusShutdown(GdiplusToken);
}

// 唯一的 CAxisApp 对象

LPCTSTR SV_5MIN_FILE_TEMPLATE = _T("%s\\vipdoc\\%s\\fzline\\%s%s.lc5");
LPCTSTR SV_LDAY_FILE_TEMPLATE = _T("%s\\vipdoc\\%s\\lday\\%s%s.day");
LPCTSTR iniFile = _T("\\Stock.ini");
LPCTSTR appFile = _T("TDX_APP_HOME_DIR");
LPCTSTR keyFile = _T("tdx_home");
CAxisApp theApp;


// CAxisApp 初始化

BOOL CAxisApp::InitInstance()
{
	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMDIFrameWnd* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建主 MDI 框架窗口
	if (!pFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	// 试图加载共享 MDI 菜单和快捷键表
	//TODO: 添加附加成员变量，并加载对
	//	应用程序可能需要的附加菜单类型的调用
	HINSTANCE hInst = AfxGetResourceHandle();
	m_hMDIMenu  = ::LoadMenu(hInst, MAKEINTRESOURCE(IDR_AxisTYPE));
	m_hMDIAccel = ::LoadAccelerators(hInst, MAKEINTRESOURCE(IDR_AxisTYPE));

	// 主窗口已初始化，因此显示它并对其进行更新
	pFrame->ShowWindow(m_nCmdShow);
	pFrame->UpdateWindow();
	this->LoadIniFile();
	return TRUE;
}

int CAxisApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	if (m_hMDIMenu != nullptr)
		FreeResource(m_hMDIMenu);
	if (m_hMDIAccel != nullptr)
		FreeResource(m_hMDIAccel);

	return CWinAppEx::ExitInstance();
}

// CAxisApp 消息处理程序

void CAxisApp::OnFileNew()
{
	CSelectDlg dlg;
	INT_PTR nRes = dlg.DoModal();
	if (IDCANCEL == nRes) {
		return;
	}
	if (dlg.m_i_CurSel < 0 || (dlg.m_str_stockCode.IsEmpty())) {
		return;
	}
	if (this->FindStockDataFile(dlg.STOCK_MARKETES_CODE[dlg.m_i_CurSel], dlg.m_str_stockCode)) {
		CAxisData* pAxisdata = new CAxisData;
		CString str_5min_filepath;
		CString str_lday_filepath;
		CString strTitle;
		strTitle.AppendFormat(_T("市场：〔%s〕，代码〔%s〕"), dlg.STOCK_MARKETES_NAME[dlg.m_i_CurSel], (LPCTSTR)dlg.m_str_stockCode);
		str_5min_filepath.AppendFormat(SV_5MIN_FILE_TEMPLATE, this->m_str_tdxHome.GetString(), dlg.STOCK_MARKETES_CODE[dlg.m_i_CurSel], dlg.STOCK_MARKETES_CODE[dlg.m_i_CurSel], dlg.m_str_stockCode);
		str_lday_filepath.AppendFormat(SV_LDAY_FILE_TEMPLATE, this->m_str_tdxHome.GetString(), dlg.STOCK_MARKETES_CODE[dlg.m_i_CurSel], dlg.STOCK_MARKETES_CODE[dlg.m_i_CurSel], dlg.m_str_stockCode);
		pAxisdata->Init(dlg.STOCK_MARKETES_CODE[dlg.m_i_CurSel], dlg.m_str_stockCode, strTitle.GetString(), str_5min_filepath.GetString(), str_lday_filepath.GetString());
		CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
		// 创建新的 MDI 子窗口
		pFrame->MyCreateNewChild(RUNTIME_CLASS(CChildFrame), IDR_AxisTYPE, pAxisdata, m_hMDIMenu, m_hMDIAccel);
	}
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CAxisApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CAxisApp 自定义加载/保存方法

void CAxisApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CAxisApp::LoadCustomState()
{
}

void CAxisApp::SaveCustomState()
{
}

// CAxisApp 消息处理程序
void CAxisApp::OnMenuItemSetupTdxHome()
{
	CSetupDlg dlg;
	dlg.m_str_tdxHome.SetString(this->m_str_tdxHome);
	INT_PTR nRes = dlg.DoModal();
	if (IDCANCEL == nRes)
		return;
	if (dlg.m_str_tdxHome.IsEmpty()) {
		return;
	}
	LPTSTR lptstrUserProfileDir;
	LPTSTR lptstrFullIniFile;
	SHGetKnownFolderPath(FOLDERID_Profile, 0, 0, &lptstrUserProfileDir);
	size_t fullIniFileLen = _tcslen(lptstrUserProfileDir) + _tcslen(iniFile) + 2;
	lptstrFullIniFile = new TCHAR[fullIniFileLen];
	memset(lptstrFullIniFile, 0, sizeof(TCHAR) * fullIniFileLen);
	_stprintf_s(lptstrFullIniFile, fullIniFileLen, _T("%s%s"), lptstrUserProfileDir, iniFile);
	CoTaskMemFree(lptstrUserProfileDir);
	CFileFind finder;
	BOOL ifFind = finder.FindFile(lptstrFullIniFile);
	if (!ifFind)
	{
		::WritePrivateProfileString(appFile, keyFile, dlg.m_str_tdxHome, lptstrFullIniFile);
		this->m_str_tdxHome.SetString(dlg.m_str_tdxHome);
	}
	else {
		::WritePrivateProfileString(appFile, keyFile, dlg.m_str_tdxHome, lptstrFullIniFile);
		this->m_str_tdxHome.SetString(dlg.m_str_tdxHome);
	}
	delete[] lptstrFullIniFile;
}


BOOL CAxisApp::LoadIniFile()
{
	BOOL result = FALSE;
	TCHAR str_tmp_define[1024] = { 0 };
	LPTSTR lptstrUserProfileDir;
	LPTSTR lptstrFullIniFile;
	SHGetKnownFolderPath(FOLDERID_Profile, 0, 0, &lptstrUserProfileDir);
	size_t fullIniFileLen = _tcslen(lptstrUserProfileDir) + _tcslen(iniFile) + 2;
	lptstrFullIniFile = new TCHAR[fullIniFileLen];
	memset(lptstrFullIniFile, 0, sizeof(TCHAR) * fullIniFileLen);
	_stprintf_s(lptstrFullIniFile, fullIniFileLen, _T("%s%s"), lptstrUserProfileDir, iniFile);
	CoTaskMemFree(lptstrUserProfileDir);
	LPCTSTR defaultValue = _T("C:\\new_tdx");
	CFileFind finder;
	BOOL ifFind = finder.FindFile(lptstrFullIniFile);
	if (!ifFind)
	{
		::WritePrivateProfileString(appFile, keyFile, defaultValue, lptstrFullIniFile);
		this->m_str_tdxHome.SetString(defaultValue);
	}
	else {
		::GetPrivateProfileString(appFile, keyFile, defaultValue, str_tmp_define, 1024, lptstrFullIniFile);
		this->m_str_tdxHome.SetString(str_tmp_define);
	}
	delete[] lptstrFullIniFile;
	return result;
}

BOOL CAxisApp::FindStockDataFile(LPCTSTR lpszMarket, LPCTSTR lpszCode)
{
	BOOL result = FALSE;
	CString filepath;
	filepath.AppendFormat(SV_5MIN_FILE_TEMPLATE, this->m_str_tdxHome.GetString(), lpszMarket, lpszMarket, lpszCode);
	CFileFind filefind;
	result = filefind.FindFile(filepath, 0);
	if (FALSE != result) {
		filepath.Empty();
		filepath.AppendFormat(SV_LDAY_FILE_TEMPLATE, this->m_str_tdxHome.GetString(), lpszMarket, lpszMarket, lpszCode);
		result = filefind.FindFile(filepath, 0);
		if (FALSE == result) {
			AfxMessageBox(_T("没有找该股票的日线数据文件！"), MB_OK | MB_ICONEXCLAMATION);
		}
	}
	else {
		AfxMessageBox(_T("没有找该股票的5分钟数据文件！"), MB_OK | MB_ICONEXCLAMATION);
	}
	filefind.Close();

	return result;
}
