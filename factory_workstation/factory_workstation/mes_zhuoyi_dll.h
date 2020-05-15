#pragma once
//#define MES_ZHUOYI_DLL 0
#define MES_ZHUOYI_DLL 1
#if MES_ZHUOYI_DLL
#include "MES_DLL.h"
#endif
class mes_zhuoyi_dll
{
public:
	mes_zhuoyi_dll(void);
	~mes_zhuoyi_dll(void);
	int mes_zhuoyi_dll::Pass_Station(bool flag, CString sn);
	CString mes_zhuoyi_dll::Get_Data(CString sn);
	int mes_zhuoyi_dll::Check_Station(CString sn);
	int mes_zhuoyi_dll::Pass_Station_withmac(bool flag, CString sn, CString mac);
};



