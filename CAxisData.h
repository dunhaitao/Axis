#pragma once
#include "AxisAPI.h"
// CAxisData 命令目标


class CAxisData : public CObject
{
public:
	CAxisData();
	virtual ~CAxisData();
protected:
	BOOL m_bool_5min_inited = FALSE;
	BOOL m_bool_half_inited = FALSE;
	BOOL m_bool_lday_inited = FALSE;
	CString m_str_market;
	CString m_str_code;
	CString m_str_title;
public:
	int m_i_high = 0;
	int m_i_low = 0;
	BAR_ARRAY m_5min_array;
	BAR_ARRAY m_half_array;
	BAR_ARRAY m_1hour_array;
	BAR_ARRAY m_lday_array;
protected:
	void SetDefine(LPCTSTR lpszMarket, LPCTSTR lpszCode, LPCTSTR lpszTitle);
public:
	LPCTSTR GetTitle();
	BOOL Init(LPCTSTR lpszMarket, LPCTSTR lpszCode, LPCTSTR lpszTitle, LPCTSTR lpsz5minFilepath, LPCTSTR lpszLdayFilepath);
};
