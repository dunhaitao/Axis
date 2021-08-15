
// Axis.h: Axis 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号


// CAxisApp:
// 有关此类的实现，请参阅 Axis.cpp
//

class CAxisApp : public CWinAppEx
{
public:
	CAxisApp() noexcept;
	~CAxisApp();

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
protected:
	HMENU  m_hMDIMenu;
	HACCEL m_hMDIAccel;
	CString m_str_tdxHome;
	ULONG_PTR GdiplusToken;
	Gdiplus::GdiplusStartupInput GdiplusStartupInput;

public:
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMenuItemSetupTdxHome();
private:
	BOOL LoadIniFile();
	BOOL FindStockDataFile(LPCTSTR lpszMarket, LPCTSTR lpszCode);
};

extern CAxisApp theApp;
