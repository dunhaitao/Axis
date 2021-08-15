#pragma once
#include <iostream>
#include <list>

typedef struct tagMinuBar {
	unsigned short i_date = 0;
	short i_seq = 0;
	float f_open = 0;
	float f_high = 0;
	float f_low = 0;
	float f_close = 0;
	float f_amount = 0;
	int vol = 0;
	int reserved = 0;
} MINU_BAR, * LP_MINU_BAR, ** LPP_MINU_BAR;

typedef struct tagBarExt : tagMinuBar {
	unsigned int date = 0;
	unsigned int idx = 0;
	int line_peak = 0;
	unsigned int line_idx = 0;
	unsigned int seg_idx = 0;
	int open = 0;
	int high = 0;
	int low = 0;
	int close = 0;
	int amount = 0;
	int r_high = 0;
	int r_low = 0;
	int line_point = 0;
} BAR_EXT, * LP_BAR_EXT, ** LPP_BAR_EXT;

typedef struct tagAxis {
	unsigned int idx_left;
	unsigned int date_left;
	unsigned int idx_right;
	unsigned int date_right;
	int gg;
	int dd;
	int zg;
	int zd;
} AXIS, * LP_AXIS, ** LPP_AXIS;

typedef struct tagPBarExtArray {
	LPP_BAR_EXT pbar_array = 0;
	size_t bar_count = 0;
} PBAR_ARRAY, * LP_PBAR_ARRAY;

typedef struct tagBarExtArray {
	LP_BAR_EXT bar_array = 0;
	size_t bar_count = 0;
	LP_PBAR_ARRAY line_array = 0;
	LPP_AXIS axis_array = 0;
	size_t axis_count = 0;
	LP_AXIS axis_of_lday_array = 0;
	size_t axis_of_lday_count = 0;
} BAR_ARRAY, * LP_BAR_ARRAY;

typedef struct tagLineCalculateResult {
	int code;
	int over;
	int point;
	LP_BAR_EXT p1;
	LP_BAR_EXT p2;
} LineCalculateResult, * LPLineCalculateResult;

typedef std::list< LP_BAR_EXT > BAR_LIST, * LP_BAR_LIST;
typedef std::list< LP_AXIS > AXIS_LIST, * LP_AXIS_LIST;

void translate_minu_2_half(const LP_BAR_ARRAY in_array, LP_BAR_ARRAY out_array, const int i_rate, int*, int*, int& minuts);
void translate_half_2_lday(const LP_BAR_ARRAY in_array, LP_BAR_ARRAY out_array);
void make_line(const LP_BAR_ARRAY in_array);
void make_axis_of_lday(const LP_BAR_ARRAY in_array);

void translate_axis_minus_2_lday(LP_BAR_ARRAY p_minu_array, LP_BAR_ARRAY p_lday_array);


void make_segment(const LP_BAR_ARRAY in_array);
void make_axis_of_minu(const LP_BAR_ARRAY in_array);
