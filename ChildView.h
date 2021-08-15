
// ChildView.h: CChildView 类的接口
//


#pragma once
#include "CAxisData.h"


// CChildView 窗口

class CChildView : public CScrollView
{
	DECLARE_DYNAMIC(CChildView)
// 构造
public:
	CChildView();

// 特性
public:
// 操作
public:

// 重写
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HDC m_h_memDC = 0;
	HDC m_h_dc = 0;
	HBITMAP m_h_bitmap = 0;
	BOOL m_b_inited_bitmap = FALSE;

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnDraw(CDC* /*pDC*/);
	void DrawBars(Gdiplus::Graphics& graphics, const LP_BAR_EXT p_bar_array, const size_t max_count, const int max_high);
	void DrawLines(Gdiplus::Graphics& graphics, Gdiplus::Color& color, const int pen_size, const LPP_BAR_EXT p_array, const size_t max_count, const int max_high);
	void DrawLines(Gdiplus::Graphics& graphics, Gdiplus::Color& color, const int pen_size, const LP_BAR_EXT p_array, const size_t max_count, const int max_high);
public:
	virtual void OnInitialUpdate();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

