
// ChildView.cpp: CChildView 类的实现
//

#include "pch.h"
#include "framework.h"
#include "Axis.h"
#include "ChildFrm.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildView
IMPLEMENT_DYNAMIC(CChildView, CScrollView)
CChildView::CChildView()
{
}

CChildView::~CChildView()
{
	if (TRUE == m_b_inited_bitmap) {
		::ReleaseDC(0, m_h_dc);
		::DeleteDC(m_h_memDC);
		::DeleteObject(m_h_bitmap);
	}
}

BEGIN_MESSAGE_MAP(CChildView, CScrollView)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
    
	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnDraw(CDC* pDC)
{
	RECT clientRect;
	GetClientRect(&clientRect);
	SCROLLINFO scrollinfo_0;
	SCROLLINFO scrollinfo_1;
	scrollinfo_0.cbSize = sizeof(scrollinfo_0);
	scrollinfo_1.cbSize = sizeof(scrollinfo_1);
	scrollinfo_0.fMask = SIF_ALL;
	scrollinfo_1.fMask = SIF_ALL;
	GetScrollInfo(SB_HORZ, &scrollinfo_0);
	GetScrollInfo(SB_VERT, &scrollinfo_1);

	::BitBlt(pDC->m_hDC, scrollinfo_0.nPos, scrollinfo_1.nPos, clientRect.right, clientRect.bottom, m_h_memDC, scrollinfo_0.nPos, scrollinfo_1.nPos, SRCCOPY);
}

void CChildView::DrawBars(Gdiplus::Graphics& graphics, const LP_BAR_EXT p_bar_array, const size_t max_count, const int max_high) {
	Gdiplus::Color colorRed(248, 49, 49);
	Gdiplus::Pen redPen(colorRed, 1);
	Gdiplus::Color colorGreen(84, 255, 255);
	Gdiplus::Pen greenPen(colorGreen, 1);
	Gdiplus::SolidBrush greenBrush(colorGreen);
	//
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(0, 0, 0));
	//
	Gdiplus::Pen whitePen(Gdiplus::Color(255, 255, 255), 1);
	//
	for (size_t idx = 0; idx < max_count; idx++) {
		LP_BAR_EXT pCk = p_bar_array + idx;
		int px = 4 + idx * 8;
		Gdiplus::Rect kRect;
		kRect.Width = 6;
		kRect.X = px - 3;
		kRect.Height = abs(pCk->open - pCk->close);
		if (pCk->close > pCk->open) {
			kRect.Y = max_high - pCk->close;
			Gdiplus::Rect fRect;
			fRect.X = px - 2;
			fRect.Width = 5;
			fRect.Height = kRect.Height - 1;
			fRect.Y = kRect.Y + 1;
			graphics.DrawLine(&redPen, px, max_high - pCk->high, px, max_high - pCk->low);
			graphics.DrawRectangle(&redPen, kRect);
			graphics.FillRectangle(&blackBrush, fRect);
		}
		else if (pCk->open > pCk->close) {
			kRect.Y = max_high - pCk->open;
			kRect.Width = 7;
			graphics.DrawLine(&greenPen, px, max_high - pCk->high, px, max_high - pCk->low);
			graphics.FillRectangle(&greenBrush, kRect);
		}
		else {
			graphics.DrawLine(&whitePen, px, max_high - pCk->high, px, max_high - pCk->low);
			graphics.DrawLine(&whitePen, px - 3, max_high - pCk->open, px + 3, max_high - pCk->close);
		}
	}
}

void CChildView::DrawLines(Gdiplus::Graphics& graphics, Gdiplus::Color& color, const int pen_size, const LPP_BAR_EXT p_array, const size_t max_count, const int max_high) {
	int px1 = 0;
	int py1 = 0;
	Gdiplus::Pen linePen(color, pen_size);
	for (size_t idx = 0; idx < max_count; idx++) {
		LP_BAR_EXT pCk = *(p_array + idx);
		pCk->line_point = (pCk->line_peak > 0) ? pCk->high : pCk->low;
		if (0 == idx) {
			px1 = 4 + pCk->idx * 8;
			py1 = max_high - pCk->line_point;
		}
		else {
			int px2 = 4 + pCk->idx * 8;
			int py2 = max_high - pCk->line_point;
			graphics.DrawLine(&linePen, px1, py1, px2, py2);
			px1 = px2;
			py1 = py2;
		}
	}
}

void CChildView::DrawLines(Gdiplus::Graphics& graphics, Gdiplus::Color& color, const int pen_size, const LP_BAR_EXT p_array, const size_t max_count, const int max_high) {
	int px1 = 0;
	int py1 = 0;
	Gdiplus::Pen linePen(color, pen_size);
	for (size_t idx = 0; idx < max_count; idx++) {
		LP_BAR_EXT pCk = p_array + idx;
		pCk->line_point = (pCk->line_peak > 0) ? pCk->high : pCk->low;
		if (0 == idx) {
			px1 = 4 + pCk->idx * 8;
			py1 = max_high - pCk->line_point;
		}
		else {
			int px2 = 4 + pCk->idx * 8;
			int py2 = max_high - pCk->line_point;
			graphics.DrawLine(&linePen, px1, py1, px2, py2);
			px1 = px2;
			py1 = py2;
		}
	}
}

void DrawAxis(Gdiplus::Graphics& graphics, Gdiplus::Color& color, const LPP_AXIS p_array, const size_t max_count, const int max_high) {
	if ((NULL != p_array) && (max_count > 0)) {
		Gdiplus::SolidBrush brushAxis(color);
		for (size_t idx = 0; idx < max_count; idx++) {
			LP_AXIS pAxis = *(p_array + idx);
			Gdiplus::Rect objRect;
			objRect.X = 1 + pAxis->idx_left * 8;
			objRect.Y = max_high - pAxis->gg;
			objRect.Width = 8 * (pAxis->idx_right - pAxis->idx_left) - 2;
			objRect.Height = pAxis->gg - pAxis->dd;
			graphics.FillRectangle(&brushAxis, objRect);
		}
	}
}
void DrawAxis(Gdiplus::Graphics& graphics, Gdiplus::Color& color, const LP_AXIS p_array, const size_t max_count, const int max_high) {
	if ((NULL != p_array) && (max_count > 0)) {
		Gdiplus::SolidBrush brushAxis(color);
		for (size_t idx = 0; idx < max_count; idx++) {
			LP_AXIS pAxis = (p_array + idx);
			Gdiplus::Rect objRect;
			objRect.X = 1 + pAxis->idx_left * 8;
			objRect.Y = max_high - pAxis->gg;
			objRect.Width = 8 * (pAxis->idx_right - pAxis->idx_left) - 2;
			objRect.Height = pAxis->gg - pAxis->dd;
			graphics.FillRectangle(&brushAxis, objRect);
		}
	}
}

void CChildView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CWnd* pParent = GetParent();
	ASSERT_KINDOF(CChildFrame, pParent);
	CChildFrame* pFrame = (CChildFrame*)pParent;
	SIZE scrollSize;
	scrollSize.cx = pFrame->getAxisClientWidth();
	scrollSize.cy = pFrame->getAxisClientHeight() + 1;
	SetScrollSizes(MM_TEXT, scrollSize);

	m_h_dc = ::GetDC(0);
	m_h_bitmap = ::CreateCompatibleBitmap(m_h_dc, scrollSize.cx, scrollSize.cy);
	m_h_memDC = ::CreateCompatibleDC(m_h_dc);
	::SelectObject(m_h_memDC, m_h_bitmap);
	//
	Gdiplus::Graphics graphics(m_h_memDC);
	Gdiplus::Pen linePen(Gdiplus::Color(255, 255, 0), 1);
	Gdiplus::Pen segmentPen(Gdiplus::Color(216, 160, 223), 2);
	// linePen.SetDashStyle(Gdiplus::DashStyle::DashStyleDot);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(0, 0, 0));
	Gdiplus::Color colorRed(248, 49, 49);
	Gdiplus::Pen redPen(colorRed, 1);
	// Gdiplus::SolidBrush redBrush(Gdiplus::Color(255,255,100));
	Gdiplus::Color colorGreen(84, 255, 255);
	Gdiplus::Pen greenPen(colorGreen, 1);
	Gdiplus::SolidBrush greenBrush(colorGreen);
	Gdiplus::Pen whitePen(Gdiplus::Color(255, 255, 255), 1);
	Gdiplus::Color colorAxisA(140, 255, 255, 100);
	Gdiplus::Color colorAxisB(120, 0, 255, 255);
	Gdiplus::Color colorAxisC(140, 255, 174, 201);
	graphics.FillRectangle(&blackBrush, Gdiplus::Rect(0, 0, scrollSize.cx, scrollSize.cy));
	DrawBars(graphics, pFrame->m_p_axisdata->m_lday_array.bar_array, pFrame->m_p_axisdata->m_lday_array.bar_count, pFrame->m_p_axisdata->m_i_high);
	DrawLines(graphics, Gdiplus::Color(192, 255, 255, 0), 3, pFrame->m_p_axisdata->m_lday_array.line_array->pbar_array, pFrame->m_p_axisdata->m_lday_array.line_array->bar_count, pFrame->m_p_axisdata->m_i_high);
	DrawAxis(graphics, colorAxisC, pFrame->m_p_axisdata->m_lday_array.axis_array, pFrame->m_p_axisdata->m_lday_array.axis_count, pFrame->m_p_axisdata->m_i_high);
	DrawAxis(graphics, colorAxisB, pFrame->m_p_axisdata->m_1hour_array.axis_of_lday_array, pFrame->m_p_axisdata->m_1hour_array.axis_count, pFrame->m_p_axisdata->m_i_high);
	DrawAxis(graphics, colorAxisA, pFrame->m_p_axisdata->m_half_array.axis_of_lday_array, pFrame->m_p_axisdata->m_half_array.axis_count, pFrame->m_p_axisdata->m_i_high);
	graphics.ReleaseHDC(m_h_memDC);
	m_b_inited_bitmap = TRUE;
}

void CChildView::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	// CScrollView::PostNcDestroy();
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// return CScrollView::OnEraseBkgnd(pDC);
	RECT clientRect;
	GetClientRect(&clientRect);
	CBrush blackBrush;
	blackBrush.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(&clientRect, &blackBrush);
	return TRUE;
}
