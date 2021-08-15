#include "pch.h"
#include "AxisAPI.h"

void translate_minu_2_half(const LP_BAR_ARRAY in_array, LP_BAR_ARRAY out_array, int i_rate, int* p_high, int* p_low, int& minuts)
{
    BAR_LIST half_list;
    LP_BAR_EXT p_half = NULL;
    size_t idx = 0;
    int last_date = 0;
    short last_min = 0;
    for (idx = 0; idx < in_array->bar_count; idx++) {
        (in_array->bar_array + idx)->open = (int)roundf((1000 * (in_array->bar_array + idx)->f_open) / i_rate);
        (in_array->bar_array + idx)->high = (int)roundf((1000 * (in_array->bar_array + idx)->f_high) / i_rate);
        (in_array->bar_array + idx)->low = (int)roundf((1000 * (in_array->bar_array + idx)->f_low) / i_rate);
        (in_array->bar_array + idx)->close = (int)roundf((1000 * (in_array->bar_array + idx)->f_close) / i_rate);
        if (0 == idx) {
            *p_low = (in_array->bar_array + idx)->low;
            *p_high = 0;
        }
        else {
            if ((in_array->bar_array + idx)->high > *p_high) {
                *p_high = (in_array->bar_array + idx)->high;
            }
            if ((in_array->bar_array + idx)->low < *p_low) {
                *p_low = (in_array->bar_array + idx)->low;
            }
        }
        if (((in_array->bar_array + idx)->date != last_date) || (((in_array->bar_array + idx)->i_seq - last_min) > minuts)) {
            p_half = new BAR_EXT;
            memset(p_half, 0, sizeof(BAR_EXT));
            p_half->date = (in_array->bar_array + idx)->date;
            p_half->f_open = (in_array->bar_array + idx)->f_open;
            p_half->open = (in_array->bar_array + idx)->open;
            p_half->low = (in_array->bar_array + idx)->low;
            p_half->r_low = (in_array->bar_array + idx)->low;
            p_half->idx = idx;
            half_list.push_back(p_half);
            last_min = (in_array->bar_array + idx)->i_seq - 5;
            last_date = (in_array->bar_array + idx)->date;
        }
        if (NULL != p_half) {
            p_half->f_close = (in_array->bar_array + idx)->f_close;
            p_half->close = (in_array->bar_array + idx)->close;
            p_half->i_seq = (in_array->bar_array + idx)->i_seq;
            if ((in_array->bar_array + idx)->high > p_half->high) {
                p_half->f_high = (in_array->bar_array + idx)->f_high;
                p_half->high = (in_array->bar_array + idx)->high;
                p_half->r_high = (in_array->bar_array + idx)->high;
            }
            if ((in_array->bar_array + idx)->low < p_half->low) {
                p_half->f_low = (in_array->bar_array + idx)->f_low;
                p_half->low = (in_array->bar_array + idx)->low;
                p_half->r_low = (in_array->bar_array + idx)->low;
            }
        }
    }
    size_t half_count = half_list.size();
    if (half_count > 0) {
        out_array->bar_array = new BAR_EXT[half_count];
        out_array->bar_count = half_count;
        BAR_LIST::iterator iter;
        size_t idx = 0;
        for (iter = half_list.begin(); iter != half_list.end(); iter++, idx++) {
            (out_array->bar_array + idx)->idx = idx;
            (out_array->bar_array + idx)->date = (*iter)->date;
            (out_array->bar_array + idx)->i_seq = (*iter)->i_seq;
            (out_array->bar_array + idx)->f_open = (*iter)->f_open;
            (out_array->bar_array + idx)->f_high = (*iter)->f_high;
            (out_array->bar_array + idx)->f_low = (*iter)->f_low;
            (out_array->bar_array + idx)->f_close = (*iter)->f_close;
            (out_array->bar_array + idx)->open = (*iter)->open;
            (out_array->bar_array + idx)->high = (*iter)->high;
            (out_array->bar_array + idx)->low = (*iter)->low;
            (out_array->bar_array + idx)->close = (*iter)->close;
            (out_array->bar_array + idx)->r_high = (*iter)->high;
            (out_array->bar_array + idx)->r_low = (*iter)->low;
            // TRACE(_T("'%d','%d',%d,%d,%d,%d,%f,%d\n"), (*iter)->date, (*iter)->i_seq, (*iter)->open, (*iter)->high, (*iter)->low, (*iter)->close, 0.1, 0);
            delete (*iter);
        }
    }
    TRACE(_T("转换分钟至半小时%d个记录\n"), half_count);
}

void translate_half_2_lday(const LP_BAR_ARRAY in_array, LP_BAR_ARRAY out_array)
{
    BAR_LIST lday_list;
    LP_BAR_EXT p_lday = NULL;
    size_t idx = 0;
    int last_date = 0;
    for (idx = 0; idx < in_array->bar_count; idx++) {
        if ((in_array->bar_array + idx)->date != last_date) {
            p_lday = new BAR_EXT;
            memset(p_lday, 0, sizeof(BAR_EXT));
            p_lday->date = (in_array->bar_array + idx)->date;
            p_lday->open = (in_array->bar_array + idx)->open;
            p_lday->low = (in_array->bar_array + idx)->low;
            p_lday->r_low = (in_array->bar_array + idx)->low;
            p_lday->idx = idx;
            lday_list.push_back(p_lday);
            last_date = (in_array->bar_array + idx)->date;
        }
        if (NULL != p_lday) {
            p_lday->close = (in_array->bar_array + idx)->close;
            p_lday->i_seq = (in_array->bar_array + idx)->i_seq;
            if ((in_array->bar_array + idx)->high > p_lday->high) {
                p_lday->high = (in_array->bar_array + idx)->high;
                p_lday->r_high = (in_array->bar_array + idx)->high;
            }
            if ((in_array->bar_array + idx)->low < p_lday->low) {
                p_lday->low = (in_array->bar_array + idx)->low;
                p_lday->r_low = (in_array->bar_array + idx)->low;
            }
        }
    }
    size_t lday_count = lday_list.size();
    if (lday_count > 0) {
        out_array->bar_array = new BAR_EXT[lday_count];
        out_array->bar_count = lday_count;
        BAR_LIST::iterator iter;
        size_t idx = 0;
        for (iter = lday_list.begin(); iter != lday_list.end(); iter++, idx++) {
            (out_array->bar_array + idx)->idx = idx;
            (out_array->bar_array + idx)->date = (*iter)->date;
            (out_array->bar_array + idx)->i_seq = (*iter)->i_seq;
            (out_array->bar_array + idx)->open = (*iter)->open;
            (out_array->bar_array + idx)->high = (*iter)->high;
            (out_array->bar_array + idx)->low = (*iter)->low;
            (out_array->bar_array + idx)->close = (*iter)->close;
            (out_array->bar_array + idx)->r_high = (*iter)->high;
            (out_array->bar_array + idx)->r_low = (*iter)->low;
            delete (*iter);
        }
    }
    TRACE(_T("转换半小时至天%d个记录\n"), lday_count);
}


void translate_axis_minus_2_lday(LP_BAR_ARRAY p_bar_array, LP_BAR_ARRAY p_lday_array) {
    size_t idx = 0;
    size_t didx = 0;
    LPP_AXIS in_array = p_bar_array->axis_array;
    size_t in_count = p_bar_array->axis_count;
    if (NULL != in_array) {
        if (in_count > 0) {
            int b_bool_success = 0;
            p_bar_array->axis_of_lday_array = new AXIS[in_count];
            for (idx = 0; idx < in_count; idx++) {
                LP_AXIS p = *(in_array + idx);
                b_bool_success = 0;
                for (size_t day_idx = didx; day_idx < p_lday_array->bar_count; day_idx++) {
                    LP_BAR_EXT d = p_lday_array->bar_array + day_idx;
                    if (p->date_left == d->date) {
                        didx = day_idx - 1;
                        p_bar_array->axis_of_lday_array[idx].idx_left = d->idx;
                        p_bar_array->axis_of_lday_array[idx].date_left = d->date;
                        p_bar_array->axis_of_lday_array[idx].dd = p->dd;
                        p_bar_array->axis_of_lday_array[idx].gg = p->gg;
                        p_bar_array->axis_of_lday_array[idx].zd = p->zd;
                        p_bar_array->axis_of_lday_array[idx].zg = p->zg;
                        b_bool_success = 1;
                        break;
                    }
                }
                if (0 == b_bool_success) {
                    TRACE(_T("转换分钟线到日线时出现异常！ date:%d\n"), p->date_left);
                }
                if (1 == b_bool_success) {
                    for (size_t day_idx = didx; day_idx < p_lday_array->bar_count; day_idx++) {
                        LP_BAR_EXT d = p_lday_array->bar_array + day_idx;
                        if (p->date_right == d->date) {
                            didx = day_idx - 1;
                            p_bar_array->axis_of_lday_array[idx].idx_right = d->idx;
                            p_bar_array->axis_of_lday_array[idx].date_right = d->date;
                            b_bool_success = 1;
                            break;
                        }
                    }
                    if (0 == b_bool_success) {
                        TRACE(_T("转换分钟线到日线时出现异常！ date:%d\n"), p->date_right);
                    }
                }
            }
        }
    }
}
