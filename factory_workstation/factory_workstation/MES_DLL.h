
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
 
	
	MES_API BOOL WINAPI MES_Login( OUT LPSTR lpReturn );      // �����̵߳���

		/*		
			MES_Init: ��ʼ��MES������ BOOL��32λInteger���ͣ�����ֵ 1 Ϊ�ɹ����� 1 ��ʧ�ܡ�����ͬ��
			ע�� ���з�������ַ����������ڴ泤�ȶ�����Ϊ MES_MAX_RETURN_BUFSIZE �� 1024 �ֽڣ�
				 ��������ڴ�й©��Σ��
		*/






	MES_API BOOL WINAPI MES_Init( OUT LPSTR lpReturn );    // ���̳߳����� ���̵߳��� 
	MES_API BOOL WINAPI MES_Free( OUT LPSTR lpReturn );    // ���̳߳����� ���̵߳��� 
		/*
			MES_Free: �ͷ�MES�ڴ�
		*/

	MES_API BOOL WINAPI MES_SendCmd( LPCSTR lpszCmd, OUT LPSTR lpReturn );
		/*
			MES_SendCmd: �Զ��巢��MES ���
			lpszCmd: ���MES˵��
		*/

	MES_API BOOL WINAPI MES_CheckWorkerId( LPCSTR lpszWG	// ���룬 ����/��λ �����¶��ƹ���
										 , LPCSTR lpszWS	// ���룬 ��վ������վ̨�� 
										 , LPCSTR lpszEMP	// ���룬 ����Ա����
										 , LPCSTR lpszPWD	// ���룬 ����Ա��¼����
										 , OUT LPSTR lpReturn );
		/*		
			MES_CheckWorkerId: �ֶ�У�� ���򡢹�վ������վ̨�������ź͹��ŵ�¼���롣
								BOOL����ֵ 1 Ϊ�ɹ����� 1 ��ʧ�ܡ�����ͬ��
			�� MES_Init ����У�飬�ɲ������ظ����á�
		*/

	MES_API BOOL WINAPI MES_Check_Process( char* lpszSN 		
										  , OUT LPSTR lpReturn );
	
	MES_API BOOL WINAPI MES_CheckSerialNum( LPCSTR lpszSN 		// ���룬 ���̹ܿ�SN
										  , OUT LPSTR lpReturn );

	/*MES_API BOOL WINAPI MES_CheckSerialNum( LPCSTR lpszSN 		// ���룬 ���̹ܿ�SN
										  ,	LPCSTR lpszTP 		// ���룬 ��������
										  , OUT LPSTR lpReturn );*/
		/*
			MES_CheckSerialNum: У������SN�Ƿ��ڵ�ǰ���Թ���WG����BOOL����ֵ �� 0 ���ɹ�������ͬ��
		*/

	MES_API BOOL WINAPI MES_CheckSerialNumBind( LPCSTR lpszSN			// ���룬���̹ܿ�SN
					, LPCSTR lpszLinkSNType1, LPCSTR lpszLinkSNValue1   // ���룬��SN����/ֵ1 
					, LPCSTR lpszLinkSNType2, LPCSTR lpszLinkSNValue2   // ���룬��SN����/ֵ2
					, LPCSTR lpszLinkSNType3, LPCSTR lpszLinkSNValue3   // ���룬��SN����/ֵ3
					, LPCSTR lpszLinkSNType4, LPCSTR lpszLinkSNValue4   // ���룬��SN����/ֵ4
					, LPCSTR lpszLinkSNType5, LPCSTR lpszLinkSNValue5   // ���룬��SN����/ֵ5
					, OUT LPSTR lpReturn );
		/*
			MES_CheckSerialNumBind: ����� SN �Ƿ��ǰ󶨹�ϵ			
			lpszLinkSNType1:  SN���ͣ���MAC������TSN������BSN���ַ� �ȵȣ������MES�ӿ�˵��
			lpszLinkSNValue1: SNֵ�� MAC��TSN��BSN ֵ �ȵȣ���SN���Ͷ�Ӧ����ͬ��
					��ͬʱУ�� 5 ��SN
		*/



	MES_API BOOL WINAPI MES_GetWO( LPCSTR lpszSN, OUT LPSTR psWO , OUT LPSTR lpReturn );
		/*
			MES_GetWO:	ͨ������ SN ��ȡ����
			lpszSN:		���룬 ����SN
			psWO:		����� ���ع���
		*/
	MES_API BOOL WINAPI MES_GetWO2( LPCSTR lpszSN, OUT LPSTR psWO ,OUT LPSTR psPN , OUT LPSTR lpReturn );
		/*
			MES_GetWO:	ͨ������ SN ��ȡ����
			lpszSN:		���룬 ����SN
			psWO:		����� ���ع���
			psPN:		����� �����Ϻ�
		*/

	MES_API BOOL WINAPI MES_GetPN( LPCSTR psWO, OUT LPSTR psPN ,OUT LPSTR lpReturn );
		/*
			MES_GetWO:	ͨ������ ��ȡ�Ϻ�
			lpszSN:		���룬 ����
			psWO:		����� �����Ϻ�
		*/

	MES_API BOOL WINAPI MES_GetCustomData( LPCSTR lpszSN, OUT LPSTR lpData, OUT LPSTR lpFormat, OUT LPSTR lpReturn );	
		/*
			MES_GetCustomData:  ��ȡ��������
			lpszSN:		���룬����SN
			lpData:		�������������ֵ
			lpFormat:	������������ݸ�ʽ����ֵ���⣩
			
		*/
	MES_API BOOL WINAPI MES_GetWifiData( LPCSTR lpszSN, LPCSTR lpszWG, OUT LPSTR lpPowers, OUT LPSTR lpFreq, OUT LPSTR lpReturn );
		/*
			MES_GetWifiData:  ��ȡ��Ʒ��WIFIվ���ԵĹ��ʺ�Ƶƫֵ����
			lpszSN:		���룬����SN
			lpszWG:		���룬WIFIվ����ͨ��Ϊ WC��
			lpPowers:	���������ֵ
			lpFreq:		�����Ƶƫֵ
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