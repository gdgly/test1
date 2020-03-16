#include "stdafx.h"
#include <string>
#include "barcode.h"

static char *codeset[] = {
    "212222", "222122", "222221", "121223", "121322",  /*  0 -  4 */
    "131222", "122213", "122312", "132212", "221213",
    "221312", "231212", "112232", "122132", "122231",  /* 10 - 14 */
    "113222", "123122", "123221", "223211", "221132",
    "221231", "213212", "223112", "312131", "311222",  /* 20 - 24 */
    "321122", "321221", "312212", "322112", "322211",
    "212123", "212321", "232121", "111323", "131123",  /* 30 - 34 */
    "131321", "112313", "132113", "132311", "211313",
    "231113", "231311", "112133", "112331", "132131",  /* 40 - 44 */
    "113123", "113321", "133121", "313121", "211331",
    "231131", "213113", "213311", "213131", "311123",  /* 50 - 54 */
    "311321", "331121", "312113", "312311", "332111",
    "314111", "221411", "431111", "111224", "111422",  /* 60 - 64 */
    "121124", "121421", "141122", "141221", "112214",
    "112412", "122114", "122411", "142112", "142211",  /* 70 - 74 */
    "241211", "221114", "413111", "241112", "134111",
    "111242", "121142", "121241", "114212", "124112",  /* 80 - 84 */
    "124211", "411212", "421112", "421211", "212141",
    "214121", "412121", "111143", "111341", "131141",  /* 90 - 94 */
    "114113", "114311", "411113", "411311", "113141",
    "114131", "311141", "411131", "211412", "211214",  /* 100 - 104 */
    "211232", "2331112"
};

int barcode::code128a(string *out_code, string input_string)
{
	const char *loc_p;
	int i,code128_lc;
	int chk_code = 103;
	int input_length = input_string.length();
	const char *p = input_string.c_str();
	*out_code = codeset[103];
	for(i=0; i<input_length; i++){
		loc_p = p+i;
		if(*loc_p > 95 || *loc_p < 0){
			return -1;
		}
		if(*loc_p >= 32){
			code128_lc = *loc_p - 32;
		}else{
			code128_lc = *loc_p + 64;
		}
		chk_code += code128_lc * (i+1);
		*out_code += codeset[code128_lc];
	}
	chk_code = chk_code % 103;
	*out_code += codeset[chk_code];
	*out_code += codeset[106];
	TRACE("input_length = %d\n input:%s\n output: %s\n",input_length, input_string.c_str(), (*out_code).c_str());

//	string b = "112233";
//	TRACE("%d\n", b.length());
	return 0;
}

int barcode::code128b(string *out_code, string input_string)
{
	const char *loc_p;
	int i,code128_lc;
	int chk_code = 104;
	int input_length = input_string.length();
	const char *p = input_string.c_str();
	*out_code = codeset[104];
	for(i=0; i<input_length; i++){
		loc_p = p+i;
		if(*loc_p > 127 || *loc_p < 32){
			return -1;
		}
		code128_lc = *loc_p - 32;
		chk_code += code128_lc * (i+1);
		*out_code += codeset[code128_lc];
	}
	chk_code = chk_code % 103;//校验码都是对103取余
	*out_code += codeset[chk_code];
	*out_code += codeset[106];
	TRACE("input_length = %d\n input:%s\n output: %s\n",input_length, input_string.c_str(), (*out_code).c_str());

//	string b = "112233";
//	TRACE("%d\n", b.length());
	return 0;
}

void barcode::draw_black( CDC* pDC, CRect rect )  
{  
    pDC->FillSolidRect(rect, RGB(0, 0, 0)); 
}

void barcode::draw_white( CDC* pDC, CRect rect )  
{  
    pDC->FillSolidRect(rect, RGB(255, 255, 255)); 
}
void barcode::draw_bar(CDC *pDC, CRect rect, string a)
{
	int length = a.length();
	const char *p = a.c_str();
	int i,pix_per;
	int pix_count = 0;
	for(i=0; i<length; i++){
		if(*(p+i) == '1'){
			pix_count += 1;
		}else if(*(p+i) == '2'){
			pix_count += 2;
		}else if(*(p+i) == '3'){
			pix_count += 3;
		}else{
			pix_count += 4;
		}
	}
	//pix_per = 216 / pix_count;
	if( rect.Width() < pix_count ){
		TRACE("pixer not enough!\n");
		return ;
	}
	pix_per = 1;
	int l_cur = (rect.Width() - pix_count) / 2;
	int bar_width;
	for(i=0; i<length; i++){
		if(*(p+i) == '1'){
			bar_width = pix_per;
		}else if(*(p+i) == '2'){
			bar_width = pix_per*2;
		}else if(*(p+i) == '3'){
			bar_width = pix_per*3;
		}else{
			bar_width = pix_per*4;
		}

		if(i%2 == 0)
			draw_black(pDC, CRect(l_cur, 1, l_cur + bar_width, rect.Height()-1));//(a,b,c,d)ab左上角坐标，cd右下角坐标
		else
			draw_white(pDC, CRect(l_cur, 1, l_cur + bar_width, rect.Height()-1));

		l_cur += bar_width;
	}
	TRACE("pix_per = %d!\n", pix_per);
}

void barcode::out_draw_bar(CDC *pDC, CRect rect, CString in_str)
{
	//CWnd *pWnd=GetDlgItem(IDC_PICTURE); //获得pictrue控件窗口的句柄
	//CRect rect;
	//pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	//CDC *pDC=pWnd->GetDC(); //获得pictrue控件的DC
	//pDC->FillSolidRect(rect, RGB(255, 255, 255));

	string a = codeset[104];
	string b = in_str;
	if(code128b(&a, b) == -1){
		TRACE("wrong string!\n");
		return ;
	}
	draw_bar(pDC, rect, a);
}
