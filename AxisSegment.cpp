#include "pch.h"
#include "AxisAPI.h"

void segment_if_segment(int layer, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LPLineCalculateResult lpResult);
int segment_is_contain(int layer, int i_dir, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4);
int segment_cut(int layer, size_t tidx, size_t max_line_points, int i_segment, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LPP_BAR_EXT line_points, LP_BAR_LIST p_parent_points);
int segment_wait_check(int layer, int i_dir, LP_BAR_EXT p1, LP_BAR_EXT p3, LP_BAR_EXT p5, LP_BAR_EXT p6);
int segment_wait_check_2(int layer, int i_dir, LP_BAR_EXT p3, LP_BAR_EXT p4, LP_BAR_EXT p5, LP_BAR_EXT p6);
int segment_is_peak(int layer, int i_dir, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LP_BAR_EXT p5, LP_BAR_EXT p6);

void make_segment(const LP_BAR_ARRAY in_array)
{
    return;
    if (NULL == in_array->line_array) {
        make_line(in_array);
    }
    size_t idx = 0;
    idx = 3;
    BAR_LIST segment_bar_list;
    if ((in_array->line_array->bar_count - idx) > 0) {
        LP_BAR_EXT p1 = *(in_array->line_array->pbar_array + 0);
        LP_BAR_EXT p2 = *(in_array->line_array->pbar_array + 1);
        LP_BAR_EXT p3 = *(in_array->line_array->pbar_array + 2);
        LP_BAR_EXT p4 = *(in_array->line_array->pbar_array + 3);
        LineCalculateResult lineCalculateResult;
        memset(&lineCalculateResult, 0, sizeof(LineCalculateResult));
        while (idx < in_array->line_array->bar_count) {
            p4 = *(in_array->line_array->pbar_array + idx);
            segment_if_segment(0, p1, p2, p3, p4, &lineCalculateResult);
            if (0 != lineCalculateResult.code) {
                break;
            }
            p1 = p2;
            p2 = p3;
            p3 = p4;
            idx += 1;
        }
        // p0 = p1 # �߶ε����
        p1 = p2;
        p2 = p3;
        p3 = p4;
        idx += 1;
        int i_need_jump = 0;
        while (idx < in_array->line_array->bar_count) {
            p4 = *(in_array->line_array->pbar_array + idx);
            if (i_need_jump > 0) {
                p3 = p4;
            }
            else {
                int i_contain = segment_is_contain(0, lineCalculateResult.code, p1, p2, p3, p4);
                if (i_contain > 0) {
                    p2 = p4;
                    i_need_jump = 2;
                }
                else {
                    break;
                }
            }
            idx += 1;
            if (i_need_jump > 0) {
                i_need_jump -= 1;
            }
        }
        std::cout << "    -==== �߶ο�ʼ ====-\n";
        int i_sgement_count = segment_cut(0, idx, in_array->line_array->bar_count, lineCalculateResult.code, p1, p2, p3, p4, in_array->line_array->pbar_array, &segment_bar_list);
        //
        size_t segment_dot_size = segment_bar_list.size();
        TRACE2("    -==== �߶ν���:�߶�����:%d:%d ====-\n", i_sgement_count, segment_dot_size);
        /*
        if (segment_dot_size > 0) {
            in_array->segment_array = new PBAR_ARRAY;
            in_array->segment_array->bar_count = segment_dot_size;
            in_array->segment_array->pbar_array = new LP_BAR_EXT[segment_dot_size];
            BAR_LIST::iterator iter;
            size_t seg_bar_idx = 0;
            for (iter = segment_bar_list.begin(); iter != segment_bar_list.end(); iter++, seg_bar_idx++) {
                LP_BAR_EXT p = *iter;
                p->seg_idx = seg_bar_idx;
                // TRACE(_T("date:%d, peak:%d, high:%d, low:%d\n"), p->date, p->line_peak, p->high, p->low);
                *(in_array->segment_array->pbar_array + seg_bar_idx) = p;
            }
        }
        */
    }
}

void segment_if_segment(int layer, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->over = 0;
    if (p1->line_peak < 0) {
        if (p1->line_point < p2->line_point && p2->line_point > p3->line_point && p3->line_point > p1->line_point && p2->line_point < p4->line_point && p3->line_point < p4->line_point) {
            lpResult->code = 1;
        }
    }
    else if (p1->line_peak > 0) {
        if (p1->line_point > p2->line_point && p2->line_point < p3->line_point && p3->line_point < p1->line_point && p2->line_point > p4->line_point && p3->line_point > p4->line_point) {
            lpResult->code = -1;
        }
    }
    std::cout << "    -==== LEVEL:[" << layer << "], �ж��Ƿ���߶�:����[" << ((lpResult->code > 0) ? "�����߶�" : (lpResult->code < 0 ? "�����߶�" : "�����߶�")) << "] p1:idx=[" << p1->idx << "],date=[" << p1->date << "],peak=[" << p1->line_peak << "],line=[" << p1->line_point << "] | p2:idx=[" << p2->idx << "],date=[" << p2->date << "],peak=[" << p2->line_peak << "],line=[" << p2->line_point << "] | p3:idx=[" << p3->idx << "],date=[" << p3->date << "],peak=[" << p3->line_peak << "],line=[" << p3->line_point << "] | p4:idx=[" << p4->idx << "],date=[" << p4->date << "],peak=[" << p4->line_peak << "],line=[" << p4->line_point << "] ====" << std::endl;
    if (0 == lpResult->code) {
        if (p1->line_peak < 0) {
            if (p3->line_point < p1->line_point) {
                lpResult->over = -2;
                std::cout << "    -==== ���ּ������:A ====-" << std::endl;
            }
        }
        else if (p1->line_peak > 0) {
            if (p3->line_point > p1->line_point) {
                lpResult->over = 2;
                std::cout << "    -==== ���ּ������:B ====-" << std::endl;
            }
        }
    }
}

int segment_is_contain(int layer, int i_dir, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4)
{
    int result = 0;
    if (p1->line_peak > 0) {
        if (p1->line_point >= p3->line_point && p2->line_point <= p4->line_point) {
            result = 1;
        }
        else if (p1->line_point <= p3->line_point && p2->line_point >= p4->line_point) {
            result = -1;
        }
    }
    else if (p1->line_peak < 0) {
        if (p1->line_point >= p3->line_point && p2->line_point <= p4->line_point) {
            result = -1;
        }
        else if (p1->line_point <= p3->line_point && p2->line_point >= p4->line_point) {
            result = 1;
        }
    }
    std::cout << "    -==== LEVEL:[" << layer << "], �ж��Ƿ���Ҫ�ϲ�:�߶η���:[" << ((i_dir > 0) ? "����" : (i_dir < 0 ? "����" : "û���򣨳��������쳣��")) << "]:����[" << ((result > 0) ? "����Һϲ�" : (result < 0 ? "�Ұ���ϲ�" : "����ϲ�")) << "] p1:idx=[" << p1->idx << "],date=[" << p1->date << "],peak=[" << p1->line_peak << "],line=[" << p1->line_point << "] | p2:idx=[" << p2->idx << "],date=[" << p2->date << "],peak=[" << p2->line_peak << "],line=[" << p2->line_point << "] | p3:idx=[" << p3->idx << "],date=[" << p3->date << "],peak=[" << p3->line_peak << "],line=[" << p3->line_point << "] | p4:idx=[" << p4->idx << "],date=[" << p4->date << "],peak=[" << p4->line_peak << "],line=[" << p4->line_point << "] ====" << std::endl;
    return result;
}

int segment_cut(int layer, size_t tidx, size_t max_line_points, int i_segment, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LPP_BAR_EXT line_points, LP_BAR_LIST p_parent_points)
{
    LP_BAR_EXT last_peak = NULL;
    LP_BAR_EXT last_append = NULL;
    size_t idx = tidx;
    int result_count = 0;
    if ((max_line_points - (idx + 2)) > 0) {
        LP_BAR_EXT p5 = line_points[idx + 1];
        idx += 2;
        int now_status = 0;
        int i_need_jump = 0;
        int i_peak_type_2 = 0;
        LP_BAR_EXT p1_wait_check = NULL;
        LP_BAR_EXT p2_wait_check = NULL;
        LP_BAR_EXT p3_wait_check = NULL;
        LP_BAR_EXT p4_wait_check = NULL;
        while (idx < max_line_points) {
            std::cout << "   -==== *** start now_status:[" << now_status << "]====-" << std::endl;
            LP_BAR_EXT p6 = line_points[idx];
            if (1 == now_status) {
                if (i_need_jump > 0) {
                    p1 = p2;
                    p2 = p3;
                    p3 = p4;
                    p4 = p5;
                    p5 = p6;
                    std::cout << "    -==== segment_is_contain:A ====-" << std::endl;
                    // int i_contain_b = segment_is_contain(layer, i_segment, p3, p4, p5, p6);
                    // if (i_contain_b > 0) {
                    //     p4 = p6;
                    //     i_need_jump += 2;
                    // }
                }
                else {
                    // ���ͳɹ����ж��Ƿ�͸���Ƿ�����ߣ���ڶ������
                    int i_wait_check = segment_wait_check(layer, i_segment, p1_wait_check, p3_wait_check, p5, p6);
                    if (abs(i_wait_check) > 1) {
                        p1 = p2;
                        p2 = p3;
                        p3 = p4;
                        p4 = p5;
                        p5 = p6;
                        now_status = 0;
                        std::cout << "    -==== �ж������Ƿ񱻴�͸: ����:��͸:A ====-" << std::endl;
                    }
                    else if (abs(i_wait_check) > 0) {
                        i_segment = 0 - i_segment;
                        p1 = p3;
                        p2 = p4;
                        p3 = p5;
                        p4 = p6;
                        i_need_jump = 2;
                        now_status = 0;
                        p_parent_points->push_back(p3_wait_check);
                        result_count++;
                        last_append = p3_wait_check;
                        std::cout << "    -==== �������߶� ====-" << std::endl;
                        std::cout << "    -==== �ɾ�����:A idx:[" << p3_wait_check->idx << "],date:[" << p3_wait_check->date << "],line:[" << p3_wait_check->line_point << "] ====-" << std::endl;
                    }
                    else {
                        std::cout << "    -==== û�����ߣ���û�д�͸:A�� ====-" << std::endl;
                        i_peak_type_2 += 1;
                        if (i_peak_type_2 > 1) {
                            BAR_LIST tmp_k_list;
                            int tmp_points_count = segment_cut(layer + 1, idx - 2, max_line_points, 0 - i_segment, p1, p2, p3, p4, line_points, &tmp_k_list);
                            if (tmp_points_count > 0) {
                                now_status = 0;
                                i_segment = 0 - i_segment;
                                p_parent_points->push_back(p3_wait_check);
                                result_count++;
                                last_append = p3_wait_check;
                                idx -= 1;
                            }
                        }
                        else {
                            p1 = p2;
                            p2 = p3;
                            p3 = p4;
                            p4 = p5;
                            p5 = p6;
                            now_status = 1;
                            i_need_jump = 2;
                        }
                    }
                }
            }
            else if (2 == now_status) {
                // ǰ�����Ұ�����û����������
                if (i_need_jump > 0) {
                    p1 = p2;
                    p2 = p3;
                    p3 = p4;
                    p4 = p5;
                    p5 = p6;
                }
                else {
                    int i_wait_check_2 = segment_wait_check_2(layer, i_segment, p3_wait_check, p4_wait_check, p5, p6);
                    if (abs(i_wait_check_2) > 0) {
                        if (abs(i_wait_check_2) > 1) {
                            i_need_jump = 2;
                            now_status = 0;
                            p1 = p3;
                            p2 = p4;
                            p3 = p5;
                            p4 = p6;
                            std::cout << "    -==== ǰ�����Ұ���������͸ ====-" << std::endl;
                        }
                        else if (abs(i_wait_check_2) > 0) {
                            now_status = 1;
                            p1 = p2_wait_check;
                            p2 = p3_wait_check;
                            p3 = p4_wait_check;
                            p4 = p5;
                            p5 = p6;
                            std::cout << "    -==== ǰ�����Ұ����������������� ====-" << std::endl;
                        }
                    }
                    else {
                        i_need_jump = 2;
                    }
                }
            }
            else if (3 == now_status) {
                // ǰ�������Ұ���������������
                int i_wait_check_2 = segment_wait_check(layer, i_segment, p1_wait_check, p3_wait_check, p5, p6);
                if (abs(i_wait_check_2) > 1) {
                    p1 = p2;
                    p2 = p3;
                    p3 = p4;
                    p4 = p5;
                    p5 = p6;
                    now_status = 0;
                    std::cout << "    -==== �ж������Ƿ񱻴�͸: ����:��͸:B ====-" << std::endl;
                }
                else {
                    std::cout << "    -==== û�д�͸:B���������ߡ�idx:[" << p3_wait_check->idx << "],date:[" << p3_wait_check->date << "],line:[" << p3_wait_check->line_point << "] ====-" << std::endl;
                    i_segment = 0 - i_segment;
                    p1 = p3;
                    p2 = p4;
                    p3 = p5;
                    p4 = p6;
                    i_need_jump = 2;
                    now_status = 0;
                    p_parent_points->push_back(p3_wait_check);
                    result_count++;
                    last_append = p3_wait_check;
                    std::cout << "    -==== �������߶� ====-" << std::endl;
                }
            }
            else {
                if (i_need_jump > 0) {
                    p5 = p6;
                }
                else {
                    std::cout << "    -==== segment_is_contain:B ====-" << std::endl;
                    int i_contain_a = segment_is_contain(layer, i_segment, p1, p2, p3, p4);
                    if (i_contain_a < 0) {
                        std::cout << "    -==== ǰ�����������Ұ�����Ҫ�ж������������߶� ====-" << std::endl;
                        LineCalculateResult i_tmp_segment;
                        segment_if_segment(layer, p3, p4, p5, p6, &i_tmp_segment);
                        if (abs(i_segment - i_tmp_segment.code) > 1) {
                            if (layer > 0) {
                                last_peak = p3;
                                std::cout << "   -==== �˳�ѭ��:A ====-" << std::endl;
                                break;
                            }
                            std::cout << "    -==== ǰ�����������Ұ������������ߣ�����һ�ʣ��ж��Ƿ�͸" << std::endl;
                            now_status = 3;
                            p1_wait_check = p1;
                            p2_wait_check = p2;
                            p3_wait_check = p3;
                            p4_wait_check = p4;
                            p1 = p2;
                            p2 = p3;
                            p3 = p4;
                            p4 = p5;
                            p5 = p6;
                        }
                        else {
                            std::cout << "    -==== ǰ�����������Ұ�����û�������ߣ����¶��ʣ��ж��Ƿ���������[" << i_tmp_segment.over << "] ====-" << std::endl;
                            if (abs(i_tmp_segment.over) > 1) {
                                p1 = p3;
                                p4->line_point = p2->line_point;
                                p2 = p4;
                                p3 = p5;
                                p4 = p6;
                                i_need_jump = 2;
                            }
                            else {
                                now_status = 2;
                                p1_wait_check = p1;
                                p2_wait_check = p2;
                                p3_wait_check = p3;
                                p4_wait_check = p4;
                                p1 = p2;
                                p2 = p3;
                                p3 = p4;
                                p4 = p5;
                                p5 = p6;
                                i_need_jump = 2;
                            }
                        }
                    }
                    else {
                        if (i_contain_a > 0) {
                            std::cout << "    -==== ����:[" << layer << "]:ǰ��������������� �߶η���:[" << ((i_segment > 0) ? "�����߶�" : (i_segment < 0 ? "�����߶�" : "�����߶�")) << "], P1:idx[" << p1->idx << "],date[" << p1->date << "],peak[" << p1->line_peak << "],line[" << p1->line_point << "] | P2:idx[" << p2->idx << "],date[" << p2->date << "],peak[" << p2->line_peak << "],line[" << p2->line_point << "] | P3:idx[" << p3->idx << "],date[" << p3->date << "],peak[" << p3->line_peak << "],line[" << p3->line_point << "] | P4:idx[" << p4->idx << "],date[" << p4->date << "],peak[" << p4->line_peak << "],line[" << p4->line_point << "] ====-" << std::endl;
                        }
                        std::cout << "    -==== segment_is_contain:C ====-" << std::endl;
                        int i_contain_b = segment_is_contain(layer, i_segment, p3, p4, p5, p6);
                        if (i_contain_b > 0) {
                            p4 = p6;
                            i_need_jump = 2;
                        }
                        else {
                            std::cout << "    -==== LEVEL:[" << layer << "], �жϷ���:�߶η���:[" << ((i_segment > 0) ? "�����߶�" : (i_segment < 0 ? "�����߶�" : "�����߶�")) << "], P1:idx[" << p1->idx << "],date[" << p1->date << "],peak[" << p1->line_peak << "],line[" << p1->line_point << "] | P2:idx[" << p2->idx << "],date[" << p2->date << "],peak[" << p2->line_peak << "],line[" << p2->line_point << "] | P3:idx[" << p3->idx << "],date[" << p3->date << "],peak[" << p3->line_peak << "],line[" << p3->line_point << "] | P4:idx[" << p4->idx << "],date[" << p4->date << "],peak[" << p4->line_peak << "],line[" << p4->line_point << "] | P5:idx[" << p5->idx << "],date[" << p5->date << "],peak[" << p5->line_peak << "],line[" << p5->line_point << "] | P6:idx[" << p6->idx << "],date[" << p6->date << "],peak[" << p6->line_peak << "],line[" << p6->line_point << "] ====-" << std::endl;
                            int i_peak = segment_is_peak(layer, i_segment, p1, p2, p3, p4, p5, p6);
                            if (0 != i_peak) {
                                now_status = 1;
                                i_peak_type_2 = 0;
                                p1_wait_check = p1;
                                p2_wait_check = p2;
                                p3_wait_check = p3;
                                p4_wait_check = p4;
                                last_peak = p3;
                                if (layer > 0) {
                                    std::cout << "   -==== �˳�ѭ��:B ====-" << std::endl;
                                    break;
                                }
                                p1 = p2;
                                p2 = p3;
                                p3 = p4;
                                p4 = p5;
                                p5 = p6;
                            }
                            else {
                                p1 = p3;
                                p2 = p4;
                                p3 = p5;
                                p4 = p6;
                                i_need_jump = 2;
                            }
                        }
                    }
                }
            }
            idx += 1;
            if (i_need_jump > 0) {
                i_need_jump -= 1;
            }
            std::cout << "   -==== *** end ====" << std::endl;
        }
    }
    if (NULL != last_peak) {
        if (NULL != last_append) {
            if (last_append->line_idx < last_peak->line_idx) {
                p_parent_points->push_back(last_peak);
                result_count++;
                std::cout << "����A last_append idx:" << last_append->idx << ", date:" << last_append->date << " last_peak idx:" << last_peak->idx << ", date:" << last_peak->date << std::endl;
            }
        }
        else {
            p_parent_points->push_back(last_peak);
            result_count++;
            std::cout << "����B" << std::endl;
        }
    }
    return result_count;
}

int segment_wait_check_2(int layer, int i_dir, LP_BAR_EXT p3, LP_BAR_EXT p4, LP_BAR_EXT p5, LP_BAR_EXT p6)
{
    int result = 0;
    if (i_dir > 0) {
        if (p5->line_point > p3->line_point) {
            result = 2;
        }
        else if (p6->line_point < p4->line_point) {
            result = 1;
        }
    }
    else if (i_dir < 0) {
        if (p5->line_point < p3->line_point) {
            result = -2;
        }
        else if (p6->line_point > p4->line_point) {
            result = -1;
        }
    }
    std::cout << "    -==== �ж������Ƿ񱻴�͸B:LEVEL:[" << layer << "], ����[" << ((i_dir > 0) ? "�����߶�" : (i_dir < 0 ? "�����߶�" : "�����߶�")) << "], ����:[" << ((2 == abs(result)) ? "��͸" : (1 == abs(result) ? "����" : "û�жϳ���������")) << "] p3:idx[" << p3->idx << "],date[" << p3->date << "],peak[" << p3->line_peak << "],line[" << p3->line_point << "] | p4:idx[" << p4->idx << "],date[" << p4->date << "],peak[" << p4->line_peak << "],line[" << p4->line_point << "] | p5:idx[" << p5->idx << "],date[" << p5->date << "],peak[" << p5->line_peak << "],line[" << p5->line_point << "] | p6:idx[" << p6->idx << "],date[" << p6->date << "],peak[" << p6->line_peak << "],line[" << p6->line_point << "] ====-" << std::endl;
    return result;
}

int segment_is_peak(int layer, int i_dir, LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LP_BAR_EXT p5, LP_BAR_EXT p6)
{
    int result = 0;
    if (i_dir > 0) {
        if (p3->line_peak > 0) {
            if (p3->line_point > p1->line_point && p4->line_point > p2->line_point && p3->line_point > p5->line_point && p4->line_point > p6->line_point) {
                result = 1;
            }
        }
        else {
            std::cout << "    -==== ����:LEVEL:[" << layer << "], [A]�жϷ���ʧ�ܣ���λ���׵����߶η���һ�£�" << std::endl;
        }
    }
    if (i_dir < 0) {
        if (p3->line_peak < 0) {
            if (p3->line_point < p1->line_point && p4->line_point < p2->line_point && p3->line_point < p5->line_point && p4->line_point < p6->line_point) {
                result = -1;
            }
        }
        else {
            std::cout << "    -==== ����:LEVEL:[" << layer << "], [B]�жϷ���ʧ�ܣ���λ���׵����߶η���һ�£�" << std::endl;
        }
    }
    std::cout << "    -==== LEVEL:[" << layer << "], �жϷ���:��ǰ�߶η���:[" << ((i_dir > 0) ? "����" : (i_dir < 0 ? "����" : "û���򣨳��������쳣��")) << "] ����:[" << ((result > 0) ? "������" : (result < 0 ? "�׷���" : "���Ƿ���")) << "] ====-" << std::endl;
    return result;
}

int segment_wait_check(int layer, int i_dir, LP_BAR_EXT p1, LP_BAR_EXT p3, LP_BAR_EXT p5, LP_BAR_EXT p6)
{
    int result = 0;
    if (i_dir > 0) {
        if (p6->line_point > p3->line_point) {
            result = 2;
        }
        else if (p5->line_point < p1->line_point) {
            result = -1;
        }
    }
    else if (i_dir < 0) {
        if (p6->line_point < p3->line_point) {
            result = -2;
        }
        else if (p5->line_point > p1->line_point) {
            result = 1;
        }
    }
    std::cout << "    -==== �ж������Ƿ񱻴�͸A:LEVEL:[" << layer << "], ����[" << ((i_dir > 0) ? "�����߶�" : (i_dir < 0 ? "�����߶�" : "�����߶�")) << "], ����:[" << ((abs(result) > 1) ? "��͸" : (abs(result) > 0 ? "����" : "����")) << "] p1:idx[" << p1->idx << "],date[" << p1->date << "],peak[" << p1->line_peak << "],line[" << p1->line_point << "] | p3:idx[" << p3->idx << "],date[" << p3->date << "],peak[" << p3->line_peak << "],line[" << p3->line_point << "] | p5:idx[" << p5->idx << "],date[" << p5->date << "],peak[" << p5->line_peak << "],line[" << p5->line_point << "] | p6:idx[" << p6->idx << "],date[" << p6->date << "],peak[" << p6->line_peak << "],line[" << p6->line_point << "] ====-" << std::endl;
    return result;
}

int axis_is_axis(LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LP_BAR_EXT p5);
int axis_in_axis(LP_BAR_EXT p4, LP_BAR_EXT p5, LP_AXIS pAxis);

void make_axis_of_lday(const LP_BAR_ARRAY in_array) {
    TRACE0("make_axis_of_lday\n");
    AXIS_LIST axis_list;
    if (in_array->line_array->bar_count > 4) {
        LP_BAR_EXT p1 = NULL;
        LP_BAR_EXT p2 = NULL;
        LP_BAR_EXT p3 = NULL;
        LP_BAR_EXT p4 = NULL;
        size_t seg_idx = 0;
        LP_AXIS pAxis = NULL;
        int b_is_axis = 0; // 0:��ǰ��������; 1��-1:��ǰ��������
        int i_last_axis_end_idx = 0;
        int i_need_jump = 0;
        while (seg_idx < in_array->line_array->bar_count) {
            LP_BAR_EXT p5 = *(in_array->line_array->pbar_array + seg_idx);
            std::cout << "p5->date:" << p5->date << std::endl;
            if (i_need_jump > 0) {
                p1 = p2; p2 = p3; p3 = p4; p4 = p5;
            }
            else {
                if (0 == b_is_axis) {
                    // ��ǰ��������״̬
                    if ((seg_idx - i_last_axis_end_idx) > 3) {
                        b_is_axis = axis_is_axis(p1, p2, p3, p4, p5);
                    }
                    else if ((3 == (seg_idx - i_last_axis_end_idx)) && (i_last_axis_end_idx > 0)) {
                        b_is_axis = axis_is_axis(p1, p2, p3, p4, p5);
                    }
                    if (0 != b_is_axis) {
                        i_last_axis_end_idx = p5->line_idx;
                        pAxis = new AXIS;
                        axis_list.push_back(pAxis);
                        if (b_is_axis > 0) {
                            pAxis->zg = (p4->high > p2->high) ? p4->high : p2->high;
                            pAxis->gg = (p4->high > p2->high) ? p2->high : p4->high;
                            pAxis->zd = (p5->low < p3->low) ? p5->low : p3->low;
                            pAxis->dd = (p5->low < p3->low) ? p3->low : p5->low;
                        }
                        else {
                            pAxis->zg = (p5->high > p3->high) ? p5->high : p3->high;
                            pAxis->gg = (p5->high > p3->high) ? p3->high : p5->high;
                            pAxis->zd = (p4->low < p2->low) ? p4->low : p2->low;
                            pAxis->dd = (p4->low < p2->low) ? p2->low : p4->low;
                        }
                        pAxis->idx_left = p2->idx;
                        pAxis->date_left = p2->date;
                        pAxis->idx_right = p5->idx;
                        pAxis->date_right = p5->date;
                    }
                }
                else {
                    // ��ǰ������״̬���ж������Ƿ�����
                    b_is_axis = axis_in_axis(p4, p5, pAxis);
                    if (0 != b_is_axis) {
                        if (!((p5->line_point > pAxis->gg) || (p5->line_point < pAxis->dd))) {
                            pAxis->idx_right = p5->idx;
                            pAxis->date_right = p5->date;
                            i_last_axis_end_idx = p5->seg_idx;
                        }
                        else {
                            pAxis->idx_right = p4->idx;
                            pAxis->date_right = p4->date;
                            i_last_axis_end_idx = p4->seg_idx;
                        }
                    }
                }
                p1 = p2; p2 = p3; p3 = p4; p4 = p5;
            }
            seg_idx++;
        }
        size_t axis_count = axis_list.size();
        TRACE1("axis_count:%d\n", axis_count);
        if (axis_count > 0) {
            AXIS_LIST::iterator iter;
            in_array->axis_array = new LP_AXIS[axis_count];
            in_array->axis_count = axis_count;
            size_t axis_idx = 0;
            for (iter = axis_list.begin(); iter != axis_list.end(); iter++, axis_idx++) {
                *(in_array->axis_array + axis_idx) = *iter;
            }
        }
    }
}

int axis_is_axis(LP_BAR_EXT p1, LP_BAR_EXT p2, LP_BAR_EXT p3, LP_BAR_EXT p4, LP_BAR_EXT p5)
{
    int result = 0;
    if (p1->line_peak > 0) {
        // ��һ�����������߶�
        if ((p5->line_point > p2->line_point) && (p1->line_point > p4->line_point)) {
            result = -1;
        }
    }
    else {
        // ��һ�����������߶�
        if ((p5->line_point < p2->line_point) && (p1->line_point < p4->line_point)) {
            result = 1;
        }
    }
    return result;
}

int axis_in_axis(LP_BAR_EXT p4, LP_BAR_EXT p5, LP_AXIS pAxis)
{
    int result = 1;
    if (p4->line_peak > 0) {
        if ((p4->high > pAxis->gg) && (p5->low > pAxis->gg)) {
            result = 0;
        }
    }
    else {
        if ((p4->low < pAxis->dd) && (p5->high < pAxis->dd)) {
            result = 0;
        }
    }
    return result;
}
