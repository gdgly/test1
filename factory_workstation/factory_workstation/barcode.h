#pragma once
#include "stdafx.h"
#include <string>
using namespace std;
class barcode
{
// 构造
public:
	void draw_black( CDC* pDC, CRect rect )  ;
	void draw_white( CDC* pDC, CRect rect )  ;
	int code128a(string *out_code, string input_string);
	int code128b(string *out_code, string input_string);
	void draw_bar(CDC *pDC, CRect rect, string a);

// 实现
protected:

public:

	void out_draw_bar(CDC *pDC, CRect rect, CString in_str);
};