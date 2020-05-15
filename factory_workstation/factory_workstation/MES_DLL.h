
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MES_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MES_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifndef MES_DLL_H_0309_________
#define MES_DLL_H_0309_________


#define MES_MAX_RETURN_BUFSIZE 1024*40


#ifdef MES_EXPORTS
#define MES_API __declspec(dllexport)
#else
#define MES_API __declspec(dllimport)
#pragma comment (lib, "MES.lib")

#endif


#ifdef __cplusplus
extern "C" {
#endif
 
	
	MES_API BOOL WINAPI MES_Login( OUT LPSTR lpReturn );      // 由主线程调用

		/*		
			MES_Init: 初始化MES参数， BOOL（32位Integer类型）返回值 1 为成功，非 1 则失败。（下同）
			注意 所有返回输出字符串参数的内存长度都至少为 MES_MAX_RETURN_BUFSIZE （ 1024 字节）
				 否则会有内存泄漏的危险
		*/






	MES_API BOOL WINAPI MES_Init( OUT LPSTR lpReturn );    // 多线程程序由 子线程调用 
	MES_API BOOL WINAPI MES_Free( OUT LPSTR lpReturn );    // 多线程程序由 子线程调用 
		/*
			MES_Free: 释放MES内存
		*/

	MES_API BOOL WINAPI MES_SendCmd( LPCSTR lpszCmd, OUT LPSTR lpReturn );
		/*
			MES_SendCmd: 自定义发送MES 命令。
			lpszCmd: 详见MES说明
		*/

	MES_API BOOL WINAPI MES_CheckWorkerId( LPCSTR lpszWG	// 输入， 工序/工位 （以下都称工序）
										 , LPCSTR lpszWS	// 输入， 工站（测试站台） 
										 , LPCSTR lpszEMP	// 输入， 测试员工号
										 , LPCSTR lpszPWD	// 输入， 测试员登录密码
										 , OUT LPSTR lpReturn );
		/*		
			MES_CheckWorkerId: 手动校验 工序、工站（测试站台）、工号和工号登录密码。
								BOOL返回值 1 为成功，非 1 则失败。（下同）
			在 MES_Init 中有校验，可不必再重复调用。
		*/

	MES_API BOOL WINAPI MES_Check_Process( char* lpszSN 		
										  , OUT LPSTR lpReturn );
	
	MES_API BOOL WINAPI MES_CheckSerialNum( LPCSTR lpszSN 		// 输入， 流程管控SN
										  , OUT LPSTR lpReturn );

	/*MES_API BOOL WINAPI MES_CheckSerialNum( LPCSTR lpszSN 		// 输入， 流程管控SN
										  ,	LPCSTR lpszTP 		// 输入， 检验流程
										  , OUT LPSTR lpReturn );*/
		/*
			MES_CheckSerialNum: 校验流程SN是否在当前测试工序（WG）。BOOL返回值 非 0 即成功。（下同）
		*/

	MES_API BOOL WINAPI MES_CheckSerialNumBind( LPCSTR lpszSN			// 输入，流程管控SN
					, LPCSTR lpszLinkSNType1, LPCSTR lpszLinkSNValue1   // 输入，绑定SN类型/值1 
					, LPCSTR lpszLinkSNType2, LPCSTR lpszLinkSNValue2   // 输入，绑定SN类型/值2
					, LPCSTR lpszLinkSNType3, LPCSTR lpszLinkSNValue3   // 输入，绑定SN类型/值3
					, LPCSTR lpszLinkSNType4, LPCSTR lpszLinkSNValue4   // 输入，绑定SN类型/值4
					, LPCSTR lpszLinkSNType5, LPCSTR lpszLinkSNValue5   // 输入，绑定SN类型/值5
					, OUT LPSTR lpReturn );
		/*
			MES_CheckSerialNumBind: 检验各 SN 是否是绑定关系			
			lpszLinkSNType1:  SN类型：“MAC”、“TSN”、“BSN”字符 等等，具体见MES接口说明
			lpszLinkSNValue1: SN值： MAC、TSN、BSN 值 等等，与SN类型对应（下同）
					可同时校验 5 种SN
		*/



	MES_API BOOL WINAPI MES_GetWO( LPCSTR lpszSN, OUT LPSTR psWO , OUT LPSTR lpReturn );
		/*
			MES_GetWO:	通过流程 SN 获取工单
			lpszSN:		输入， 流程SN
			psWO:		输出， 返回工单
		*/
	MES_API BOOL WINAPI MES_GetWO2( LPCSTR lpszSN, OUT LPSTR psWO ,OUT LPSTR psPN , OUT LPSTR lpReturn );
		/*
			MES_GetWO:	通过流程 SN 获取工单
			lpszSN:		输入， 流程SN
			psWO:		输出， 返回工单
			psPN:		输出， 返回料号
		*/

	MES_API BOOL WINAPI MES_GetPN( LPCSTR psWO, OUT LPSTR psPN ,OUT LPSTR lpReturn );
		/*
			MES_GetWO:	通过工单 获取料号
			lpszSN:		输入， 工单
			psWO:		输出， 返回料号
		*/

	MES_API BOOL WINAPI MES_GetCustomData( LPCSTR lpszSN, OUT LPSTR lpData, OUT LPSTR lpFormat, OUT LPSTR lpReturn );	
		/*
			MES_GetCustomData:  获取定制数据
			lpszSN:		输入，流程SN
			lpData:		输出，定制数据值
			lpFormat:	输出，定制数据格式（各值标题）
			
		*/
	MES_API BOOL WINAPI MES_GetWifiData( LPCSTR lpszSN, LPCSTR lpszWG, OUT LPSTR lpPowers, OUT LPSTR lpFreq, OUT LPSTR lpReturn );
		/*
			MES_GetWifiData:  获取产品在WIFI站测试的功率和频偏值数据
			lpszSN:		输入，流程SN
			lpszWG:		输入，WIFI站工序（通常为 WC）
			lpPowers:	输出，功率值
			lpFreq:		输出，频偏值
		*/


	//MES_API BOOL WINAPI MES_CheckWorkStation( LPCSTR lpszWG, LPCSTR lpszWS, OUT LPSTR lpReturn );	
	


	MES_API LPCSTR WINAPI MES_QueryParamValue( LPCSTR lpszParamName );
	MES_API BOOL WINAPI MES_SetParamValue( LPCSTR lpszParamName, LPCSTR lpszParamValue );

	MES_API BOOL WINAPI MES_LogReset( );
	MES_API BOOL WINAPI MES_LogSet( LPCSTR lpszItem, LPCSTR lpszValue, LPCSTR lpszSpec= "" );
	MES_API BOOL WINAPI MES_LogCommit( LPSTR lpReturn );




	#define PARAMNAME_WO		"WO"
	#define PARAMNAME_WG		"WG"
	#define PARAMNAME_WS		"WS"
	#define PARAMNAME_PN		"PN"
	#define PARAMNAME_WID		"WID"
	#define PARAMNAME_ProgVersion	"ProgVersion"
	#define PARAMNAME_ProgVerTime	"ProgVerTime"
	#define PARAMNAME_ProgAuthor	"ProgAuthor"

	#define PARAMNAME_MesCfgFile	"MesCfgFile"
	#define PARAMNAME_AutoCfgFile	"AutoCfgFile"
	#define PARAMNAME_TestCfgFile	"TestCfgFile"
	#define PARAMNAME_TestLogPath	"TestLogPath"
	#define PARAMNAME_MesLogPath	"MesLogPath"


#ifdef __cplusplus
}
#endif







#endif