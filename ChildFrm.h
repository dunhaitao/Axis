
// ChildFrm.h: CChildFrame 类的接口
//

#pragma once
#include "ChildView.h"
#include "CAxisData.h"

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// 特性
protected:
	CSplitterWnd m_wndSplitter;
public:
	CAxisData* m_p_axisdata = NULL;

// 操作
public:
	long getAxisClientWidth() {
		long result = 0;
		if (NULL != m_p_axisdata) {
			result = m_p_axisdata->m_lday_array.bar_count * 8 + 1;
		}
		return result;
	}
	long getAxisClientHeight() {
		long result = 0;
		if (NULL != m_p_axisdata)
			result = (m_p_axisdata->m_i_high - m_p_axisdata->m_i_low);
		return result;
	}
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 实现
public:
	// 框架工作区的视图。
	CChildView m_wndView;
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 生成的消息映射函数
protected:
	HICON m_hIcon;
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};
