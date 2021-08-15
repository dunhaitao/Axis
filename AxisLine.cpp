#include "pch.h"
#include "AxisAPI.h"

void line_make_line(LP_BAR_ARRAY in_array);
int line_if_contain(LP_BAR_EXT p1, LP_BAR_EXT p2);
int line_dir(LP_BAR_EXT p1, LP_BAR_EXT p2);
void line_if_peak(LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LPLineCalculateResult lpResult);
void line_peak_cut(const size_t idx, const LP_BAR_EXT p1, const LP_BAR_EXT p2, const LP_BAR_EXT p3, LP_BAR_ARRAY in_array, LP_BAR_LIST p_peak_list);
void line_make_it(LPP_BAR_EXT peak_array, const size_t max_count, LP_BAR_ARRAY in_array);
void line_is_line_ver2(LP_BAR_EXT p0, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LPLineCalculateResult lpResult);
int line_is_pre_line(LP_BAR_EXT p1, LP_BAR_EXT p2);
void line_is_line(LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LPLineCalculateResult lpResult);

void make_line(LP_BAR_ARRAY in_array)
{
    line_make_line(in_array);
}

void line_make_line(LP_BAR_ARRAY in_array)
{
    int i_ready = 0;
    LP_BAR_EXT p1 = NULL;
    LP_BAR_EXT p2 = NULL;
    LP_BAR_EXT p3 = NULL;
    size_t idx = 3;
    if (in_array->bar_count > 3) {
        int i_contain = 0;
        int i_dir = 0;
        p1 = in_array->bar_array + 0;
        p2 = in_array->bar_array + 1;
        p3 = in_array->bar_array + 2;
        LP_BAR_EXT p4 = NULL;
        // 判断出现方向
        while (idx < in_array->bar_count) {
            p4 = (in_array->bar_array + idx);
            i_contain = line_if_contain(p1, p2);
            if (0 == i_contain) {
                break;
            }
            else {
                p1 = p2;
                p2 = p3;
                p3 = p4;
            }
            idx++;
        }
        while (idx < in_array->bar_count) {
            p4 = (in_array->bar_array + idx);
            i_contain = line_if_contain(p2, p3);
            if (0 != i_contain) {
                i_dir = line_dir(p1, p2);
                if (i_contain > 0) {
                    if (i_dir > 0) {
                        p2->r_low = p3->r_low;
                    }
                    else if (i_dir < 0) {
                        p2->r_high = p3->r_high;
                    }
                    else {
                        std::cout << "  -==== 警告:B1:应该有方向的 ====-\n";
                        break;
                    }
                }
                else if (i_contain < 0) {
                    if (i_dir > 0) {
                        p3->r_low = p2->r_low;
                    }
                    else if (i_dir < 0) {
                        p3->r_high = p2->r_high;
                    }
                    else {
                        std::cout << "  -==== 警告:B2:应该有方向的 ====-\n";
                        break;
                    }
                    p2 = p3;
                }
                else {
                    std::cout << "  -==== 警告:A:应该有方向的 ====-\n";
                    break;
                }
                p3 = p4;
            }
            else {
                i_ready = 1;
                break;
            }
            idx++;
        }
    }
    if (i_ready > 0) {
        BAR_LIST peak_list;
        line_peak_cut(idx, p1, p2, p3, in_array, &peak_list);
        size_t peak_size = peak_list.size();
        LPP_BAR_EXT peak_array = new LP_BAR_EXT[peak_size];
        BAR_LIST::iterator iter;
        size_t peak_idx = 0;
        for (iter = peak_list.begin(); iter != peak_list.end(); iter++, peak_idx++) {
            *(peak_array + peak_idx) = *iter;
        }
        line_make_it(peak_array, peak_size, in_array);
        delete[] peak_array;
    }
}

int line_if_contain(LP_BAR_EXT p1, LP_BAR_EXT p2)
{
    int result = 0;
    if (p1->r_high <= p2->r_high && p1->r_low >= p2->r_low) {
        result = -1;
    }
    else if (p1->r_high >= p2->r_high && p1->r_low <= p2->r_low) {
        result = 1;
    }
    return result;
}

int line_dir(LP_BAR_EXT p1, LP_BAR_EXT p2)
{
    int result = 0;
    if (p1->r_high > p2->r_high && p1->r_low > p2->r_low) {
        result = -1;
    }
    else if (p1->r_high < p2->r_high && p1->r_low < p2->r_low) {
        result = 1;
    }
    return result;
}

void line_if_peak(LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->point = 0;
    if (p1->r_high > p2->r_high && p3->r_high > p2->r_high && p1->r_low > p2->r_low && p3->r_low > p2->r_low) {
        lpResult->code = -1;
        lpResult->point = p2->r_low;
    }
    else if (p1->r_high < p2->r_high && p3->r_high < p2->r_high && p1->r_low < p2->r_low && p3->r_low < p2->r_low) {
        lpResult->code = 1;
        lpResult->point = p2->r_high;
    }
}

void line_peak_cut(const size_t tidx, const LP_BAR_EXT p1, const LP_BAR_EXT p2, const LP_BAR_EXT p3, LP_BAR_ARRAY in_array, LP_BAR_LIST p_peak_list)
{
    int ridx = 0;
    int i_contain = 0;
    int i_dir = 0;
    LP_BAR_EXT tmp_p1 = p1;
    LP_BAR_EXT tmp_p2 = p2;
    LP_BAR_EXT tmp_p3 = p3;
    size_t idx = tidx;
    while (idx < in_array->bar_count) {
        LP_BAR_EXT tmp_p4 = (in_array->bar_array + idx);
        i_contain = line_if_contain(tmp_p3, tmp_p4);
        if (0 != i_contain) {
            i_dir = line_dir(tmp_p2, tmp_p3);
            if (i_contain > 0) {
                if (i_dir > 0) {
                    tmp_p3->r_low = tmp_p4->r_low;
                }
                else if (i_dir < 0) {
                    tmp_p3->r_high = tmp_p4->r_high;
                }
                else {
                    std::cout << "  -==== 警告:C1:应该有方向的 ====-\n";
                    break;
                }
            }
            else if (i_contain < 0) {
                if (i_dir > 0) {
                    tmp_p4->r_low = tmp_p3->r_low;
                }
                else if (i_dir < 0) {
                    tmp_p4->r_high = tmp_p3->r_high;
                }
                else {
                    std::cout << "  -==== 警告:C2:应该有方向的 ====-\n";
                    break;
                }
                tmp_p3 = tmp_p4;
            }
            else {
                std::cout << "  -==== 警告:D:应该有方向的 ====-\n";
                break;
            }
        }
        else {
            LineCalculateResult lineCalculateResult;
            line_if_peak(tmp_p1, tmp_p2, tmp_p3, &lineCalculateResult);
            if (0 != lineCalculateResult.code) {
                tmp_p2->line_point = lineCalculateResult.point;
                tmp_p2->line_idx = ridx;
                tmp_p2->line_peak = lineCalculateResult.code;
                p_peak_list->push_back(tmp_p2);
                ridx++;
            }
            tmp_p1 = tmp_p2;
            tmp_p2 = tmp_p3;
            tmp_p3 = tmp_p4;
        }
        idx++;
    }
}

void line_make_it(LPP_BAR_EXT peak_array, const size_t max_peak, LP_BAR_ARRAY inx_array)
{
    BAR_LIST line_dot_list;
    size_t idx = 2;
    if (max_peak > 2) {
        LP_BAR_EXT p0 = NULL;
        LP_BAR_EXT p1 = *(peak_array + 0);
        LP_BAR_EXT p2 = *(peak_array + 1);
        LP_BAR_EXT p3 = NULL;
        int i_line = 0;
        while (idx < max_peak) {
            p3 = *(peak_array + idx);
            i_line = line_is_pre_line(p1, p2);
            if (0 == i_line) {
                p2 = p3;
            }
            else {
                break;
            }
            idx++;
        }
        while (idx < max_peak) {
            p3 = *(peak_array + idx);
            LineCalculateResult lineCalculateResult;
            line_is_line(p1, p2, p3, &lineCalculateResult);
            if (0 != lineCalculateResult.code) {
                if (0 != lineCalculateResult.over) {
                    p2->line_peak = 0;
                }
                else {
                    line_dot_list.push_back(p1);
                }
                p1 = lineCalculateResult.p1;
                p2 = lineCalculateResult.p2;
            }
            else {
                size_t line_size = line_dot_list.size();
                if (line_size > 0) {
                    p0 = line_dot_list.back();
                    line_is_line_ver2(p0, p1, p2, p3, &lineCalculateResult);
                    if (0 != lineCalculateResult.code) {
                        if (abs(lineCalculateResult.over) > 1) {
                            p1->line_peak = 0;
                            p2->line_peak = 0;
                            p1 = line_dot_list.back();
                            line_dot_list.pop_back();
                            p2 = lineCalculateResult.p2;
                        }
                    }
                }
            }
            idx++;
        }
        line_dot_list.push_back(p1);
        line_dot_list.push_back(p2);
    }
    //
    size_t line_dot_size = line_dot_list.size();
    if (line_dot_size > 0) {
        inx_array->line_array = new PBAR_ARRAY;
        inx_array->line_array->bar_count = line_dot_size;
        inx_array->line_array->pbar_array = new LP_BAR_EXT[line_dot_size];
        BAR_LIST::iterator iter;
        size_t line_bar_idx = 0;
        for (iter = line_dot_list.begin(); iter != line_dot_list.end(); iter++, line_bar_idx++) {
            LP_BAR_EXT p = *iter;
            p->line_idx = line_bar_idx;
            // TRACE(_T("date:%d, peak:%d, high:%d, low:%d\n"), p->date, p->line_peak, p->high, p->low);
            *(inx_array->line_array->pbar_array + line_bar_idx) = p;
        }
    }
}

void line_is_line_ver2(LP_BAR_EXT p0, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->over = 0;
    if ((p2->line_peak > 0 && p3->line_peak < 0) && (p2->r_high > p3->r_high && p2->r_low > p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = -1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
        else {
            if ((p3->r_low < p1->r_low) && (p2->r_high < p0->r_high)) {
                lpResult->code = -1;
                lpResult->p1 = p0;
                lpResult->p2 = p3;
                lpResult->over = -2;
            }
        }
    }
    else if ((p2->line_peak < 0 && p3->line_peak > 0) && (p2->r_high < p3->r_high && p2->r_low < p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = 1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
        else {
            if ((p3->r_high > p1->r_high) && (p2->r_low > p0->r_low)) {
                lpResult->code = 1;
                lpResult->p1 = p0;
                lpResult->p2 = p3;
                lpResult->over = 2;
            }
        }
    }
    else if ((p2->line_peak > 0 && p3->line_peak > 0) && (p2->r_high < p3->r_high)) {
        lpResult->code = 1;
        lpResult->over = 1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
    else if ((p2->line_peak < 0 && p3->line_peak < 0) && (p2->r_low > p3->r_low)) {
        lpResult->code = -1;
        lpResult->over = -1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
}

int line_is_pre_line(LP_BAR_EXT p1, LP_BAR_EXT p2)
{
    int result = 0;
    if (p2->line_peak > 0 && p1->line_peak  < 0 && p2->r_high > p1->r_high && p2->r_low > p1->r_low) {
        if ((p2->idx - p1->idx) > 3) {
            result = 1;
        }
    }
    else if (p2->line_peak < 0 && p1->line_peak > 0 && p2->r_high < p1->r_high && p2->r_low < p1->r_low) {
        if ((p2->idx - p1->idx) > 3) {
            result = -1;
        }
    }
    return result;
}

void line_is_line(LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->over = 0;
    if ((p2->line_peak > 0 && p3->line_peak < 0) && (p2->r_high > p3->r_high && p2->r_low > p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = -1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
    }
    else if ((p2->line_peak < 0 && p3->line_peak > 0) && (p2->r_high < p3->r_high && p2->r_low < p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = 1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
    }
    else if ((p2->line_peak > 0 && p3->line_peak > 0) && (p2->r_high < p3->r_high)) {
        lpResult->code = 1;
        lpResult->over = 1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
    else if ((p2->line_peak < 0 && p3->line_peak < 0) && (p2->r_low > p3->r_low)) {
        lpResult->code = -1;
        lpResult->over = -1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
}
