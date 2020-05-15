#include "stdafx.h"
#include "mes_zhuoyi_dll.h"

int mes_zhuoyi_dll::Check_Station(CString sn)
{
#if MES_ZHUOYI_DLL
	char mes_ret[1024] = {0};
	int ret = 0;
	MES_Init(mes_ret);
	MES_LogReset();
	if( !MES_CheckSerialNum(sn, mes_ret)){
		ret = -1;
	}
	MES_Free(mes_ret);
	return ret;
#endif
	return -1;
}

CString mes_zhuoyi_dll::Get_Data(CString sn)
{
#if MES_ZHUOYI_DLL
	CString ret;
	char mes_ret[1024] = {0};
	CString stext;CString work_form;
	MES_Init(mes_ret);
	work_form = MES_QueryParamValue(PARAMNAME_WO);
	stext = "I_CMDTYPE=36;I_CMD=HMMAC," + work_form + ",";
	if(MES_SendCmd(stext, mes_ret)){
		ret = mes_ret;
		if( ret.Find("OK") >= 0 ){
			ret = ret.Mid(6,12);
		}else{
			ret = "000000000000";
		}
	}else{
		ret = "000000000000";
	}
	MES_Free(mes_ret);
	return ret;
#endif
	return "000000000000";
}

int mes_zhuoyi_dll::Pass_Station(bool flag, CString sn)
{
#if MES_ZHUOYI_DLL
	int ret = 0;
	char mes_ret[1024] = {0};
	MES_Init(mes_ret);
	MES_LogReset();

	MES_LogSet("Result",(flag?"PASS":"FAIL"),"");
	MES_LogSet("lotType","BSN","");
	MES_LogSet("lotSN",sn,"");

	if(!flag){
		MES_LogSet("ErrCode","123456","");
		MES_LogSet("ErrMSG","123456","");
	}

	if(!MES_LogCommit(mes_ret)){
		ret = -1;
		MES_Free(mes_ret);
	}

	MES_Free(mes_ret);
	return ret;
#endif
	return -1;
}

int mes_zhuoyi_dll::Pass_Station_withmac(bool flag, CString sn, CString mac)
{
#if MES_ZHUOYI_DLL
	int ret = 0;
	char mes_ret[1024] = {0};
	MES_Init(mes_ret);
	MES_LogReset();

	MES_LogSet("Result",(flag?"PASS":"FAIL"),"");
	MES_LogSet("lotType","BSN","");
	MES_LogSet("lotSN",sn,"");
	MES_LogSet("SnType1",mac);
	MES_LogSet("SnValue1",sn);

	if(!flag){
		MES_LogSet("ErrCode","123456","");
		MES_LogSet("ErrMSG","123456","");
	}

	if(!MES_LogCommit(mes_ret)){
		ret = -1;
		MES_Free(mes_ret);
	}

	MES_Free(mes_ret);
	return ret;
#endif
	return -1;
}

mes_zhuoyi_dll::mes_zhuoyi_dll(void)
{
}

mes_zhuoyi_dll::~mes_zhuoyi_dll(void)
{

}


