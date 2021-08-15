// CAxisData.cpp: 实现文件
//

#include "pch.h"
#include "Axis.h"
#include "CAxisData.h"

// CAxisData

CAxisData::CAxisData()
{
}

CAxisData::~CAxisData()
{
	if (TRUE == m_bool_5min_inited) {
		if (NULL != m_5min_array.line_array) {
			delete m_5min_array.line_array->pbar_array;
			delete m_5min_array.line_array;
			m_5min_array.line_array = NULL;
		}
		delete[] m_5min_array.bar_array;
	}
	if (TRUE == m_bool_half_inited) {
		if (NULL != m_half_array.line_array) {
			delete m_half_array.line_array->pbar_array;
			delete m_half_array.line_array;
			m_half_array.line_array = NULL;
		}
		if (NULL != m_half_array.axis_array) {
			for (size_t idx = 0; idx < m_half_array.axis_count; idx++) {
				delete* (m_half_array.axis_array + idx);
			}
			delete[] m_half_array.axis_array;
			m_half_array.axis_array = NULL;
		}
		if (NULL != m_half_array.axis_of_lday_array) {
			delete[] m_half_array.axis_of_lday_array;
			m_half_array.axis_of_lday_array = NULL;
		}
		delete[] m_half_array.bar_array;
	}
	if (TRUE == m_bool_half_inited) {
		if (NULL != m_1hour_array.line_array) {
			delete m_1hour_array.line_array->pbar_array;
			delete m_1hour_array.line_array;
			m_1hour_array.line_array = NULL;
		}
		if (NULL != m_1hour_array.axis_array) {
			for (size_t idx = 0; idx < m_1hour_array.axis_count; idx++) {
				delete* (m_1hour_array.axis_array + idx);
			}
			delete[] m_1hour_array.axis_array;
			m_1hour_array.axis_array = NULL;
		}
		if (NULL != m_1hour_array.axis_of_lday_array) {
			delete[] m_1hour_array.axis_of_lday_array;
			m_1hour_array.axis_of_lday_array = NULL;
		}
		delete[] m_1hour_array.bar_array;
	}
	if (TRUE == m_bool_lday_inited) {
		if (NULL != m_lday_array.line_array) {
			delete m_lday_array.line_array->pbar_array;
			delete m_lday_array.line_array;
			m_lday_array.line_array = NULL;
		}
		if (NULL != m_lday_array.axis_array) {
			for (size_t idx = 0; idx < m_lday_array.axis_count; idx++) {
				delete *(m_lday_array.axis_array + idx);
			}
			delete[] m_lday_array.axis_array;
			m_lday_array.axis_array = NULL;
		}
		delete[] m_lday_array.bar_array;
	}
}

void CAxisData::SetDefine(LPCTSTR lpszMarket, LPCTSTR lpszCode, LPCTSTR lpszTitle)
{
	m_str_market.SetString(lpszMarket);
	m_str_code.SetString(lpszCode);
	m_str_title.SetString(lpszTitle);
}

// CAxisData 成员函数
BOOL CAxisData::Init(LPCTSTR lpszMarket, LPCTSTR lpszCode, LPCTSTR lpszTitle, LPCTSTR lpsz5minFilepath, LPCTSTR lpszLdayFilepath)
{
	BOOL result = FALSE;
	SetDefine(lpszMarket, lpszCode, lpszTitle);
	CFile dataFile;
	if (dataFile.Open(lpsz5minFilepath, CFile::modeRead | CFile::typeBinary)) {
		ULONGLONG fileLength = dataFile.GetLength();
		m_5min_array.bar_count = fileLength / sizeof(MINU_BAR);
		if (m_5min_array.bar_count > 0) {
			m_5min_array.bar_array = new BAR_EXT[m_5min_array.bar_count];
			memset(m_5min_array.bar_array, 0, sizeof(BAR_EXT) * m_5min_array.bar_count);
			m_bool_5min_inited = TRUE;
			LP_MINU_BAR p_min_bar = new MINU_BAR[256];
			UINT i_read_count = 0;
			UINT total_idx = 0;
			do {
				UINT tmp_count = dataFile.Read(p_min_bar, sizeof(MINU_BAR) * 256);
				i_read_count = tmp_count / sizeof(MINU_BAR);
				for (UINT tmp_idx = 0; tmp_idx < i_read_count; tmp_idx++, total_idx++) {
					(m_5min_array.bar_array + total_idx)->idx = total_idx;
					(m_5min_array.bar_array + total_idx)->f_open = (p_min_bar + tmp_idx)->f_open;
					(m_5min_array.bar_array + total_idx)->f_high = (p_min_bar + tmp_idx)->f_high;
					(m_5min_array.bar_array + total_idx)->f_low = (p_min_bar + tmp_idx)->f_low;
					(m_5min_array.bar_array + total_idx)->f_close = (p_min_bar + tmp_idx)->f_close;
					(m_5min_array.bar_array + total_idx)->open = (int)roundf(100 * (p_min_bar + tmp_idx)->f_open);
					(m_5min_array.bar_array + total_idx)->high = (int)roundf(100 * (p_min_bar + tmp_idx)->f_high);
					(m_5min_array.bar_array + total_idx)->low = (int)roundf(100 * (p_min_bar + tmp_idx)->f_low);
					(m_5min_array.bar_array + total_idx)->close = (int)roundf(100 * (p_min_bar + tmp_idx)->f_close);
					(m_5min_array.bar_array + total_idx)->r_high = (m_5min_array.bar_array + total_idx)->high;
					(m_5min_array.bar_array + total_idx)->r_low = (m_5min_array.bar_array + total_idx)->low;
					(m_5min_array.bar_array + total_idx)->i_seq = (p_min_bar + tmp_idx)->i_seq;
					(m_5min_array.bar_array + total_idx)->f_amount = (p_min_bar + tmp_idx)->f_amount;
					(m_5min_array.bar_array + total_idx)->vol = (p_min_bar + tmp_idx)->vol;
					//
					float s_date = (float)(p_min_bar + tmp_idx)->i_date;
					unsigned int year = floor((float)(s_date / 2048)) + 2004;
					unsigned int month = floor((p_min_bar + tmp_idx)->i_date % 2048 / 100);
					unsigned int day = (p_min_bar + tmp_idx)->i_date % 2048 % 100;
					(m_5min_array.bar_array + total_idx)->date = year * 10000 + month * 100 + day;
					if (0 == total_idx) {
						m_i_high = (m_5min_array.bar_array + total_idx)->high;
						m_i_low = (m_5min_array.bar_array + total_idx)->low;
					}
					else {
						if ((m_5min_array.bar_array + total_idx)->high > m_i_high) {
							m_i_high = (m_5min_array.bar_array + total_idx)->high;
						}
						if ((m_5min_array.bar_array + total_idx)->low < m_i_low) {
							m_i_low = (m_5min_array.bar_array + total_idx)->low;
						}
					}
				}
			} while (256 == i_read_count);
			TRACE(_T("m_ull_5min_size=%d, total_idx=%d, max_high=%d, min_low=%d\n"), m_5min_array.bar_count, total_idx, m_i_high, m_i_low);
			delete[] p_min_bar;

			int i_rate = (m_i_high - m_i_low) / 50;
			int minuts = 15;
			::translate_minu_2_half(&m_5min_array, &m_half_array, i_rate, &m_i_high, &m_i_low, minuts);
			minuts = 60;
			::translate_minu_2_half(&m_5min_array, &m_1hour_array, i_rate, &m_i_high, &m_i_low, minuts);
			if (m_half_array.bar_count > 0) {
				::make_line(&m_half_array);
				m_bool_half_inited = TRUE;
			}
			if (m_1hour_array.bar_count > 0) {
				::make_line(&m_1hour_array);
				m_bool_half_inited = TRUE;
			}
			::translate_half_2_lday(&m_half_array, &m_lday_array);
			if (m_lday_array.bar_count > 0) {
				::make_line(&m_lday_array);
				::make_axis_of_lday(&m_half_array);
				::make_axis_of_lday(&m_1hour_array);
				::translate_axis_minus_2_lday(&m_half_array, &m_lday_array);
				::translate_axis_minus_2_lday(&m_1hour_array, &m_lday_array);
				::make_axis_of_lday(&m_lday_array);
				m_bool_lday_inited = TRUE;
			}
		}
	}
	dataFile.Close();
	return result;
}

LPCTSTR CAxisData::GetTitle()
{
	return m_str_title.GetString();
}