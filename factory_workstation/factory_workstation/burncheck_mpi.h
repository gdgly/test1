#pragma once

#include "Resource.h"
#include "glob_param.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ExecProcess.h"
#include "ContrlComm_burncheck.h"
#include "stdio.h"
#include "CurlCtrl.h"
#include "fz_jn_burncheck_mpi.h"

#define MAX_PIPELINE_LEN         (1024)
#define WND_MSG_STR_SIZE        (256)   // ������ʾ���ַ�����С
#define MAX_AUDIO_PLAY           (6)    // ����12�ֲ�ͬ������

enum {RUNST_NONE, RUNST_WAIT_BOARD, RUNST_GET_BOARD,
	RUNST_START_WRITE0, RUNST_START_WRITE1, RUNST_START_WRITE2,RUNST_START_RESET,
	RUNST_CHECK_INIT,   // ������Ϣ���
	RUNST_CHECK_AUDIO,  RUNST_AUDIO_WAITUSER, // 
	RUNST_CHECK_CHARG_CURRET,        // ���ͳ������
	RUNST_CHECK_WAIT_CHARGE,         // �Ƚϳ�����
	RUNST_CHECK_SLEEP_CURRENT,
	RUNST_CHECK_WAIT_SLEEP,
	RUNST_ALL_SUESS,
	RUNST_BUREALL_SUESS,
	RUNST_ALL_SUCC_IDLE,             // ����֮�󣬿��Ը��µ���
	RUNST_CURRENT_ERR,
	RUNST_SYS_ERROR,
};           // �̵߳���Ҫ״̬��

typedef struct tagMASSPRODUCTINFO {	
//#ifdef CONFIG_SUPPORT_AUDPLAY_THREAD
	// ���ֲ����߳����
	HANDLE        hAudThread;
	DWORD         idAudThread;
	UINT          iAudSetloops, iAudPlayloops;        // �߳̿��Ƽ���ǰ���Ŵ���
//#endif

	HANDLE        hThread;
	DWORD         idThread;
	HWND          hWnd;


	UINT          runStatus;
	CTime         tStatus;    //״̬�ı��ʱ��

	UINT          ch;
	UINT          doExit;         // �߳��˳�
	UINT          audNo;          // ���ŵ���Ƶ�ļ����
	UINT          playTimes;      // ���Ŵ���

	UINT          bitStatus;
	UINT          current;
	UINT          chagrcurrent;
	UINT          sleepcurrent;
	CTime         tLastPlay;
	

	UINT          outLineLen;
	BYTE          pipeData[MAX_PIPELINE_LEN];
	CExecProcess  execProcess;

	CContrlComm   comInfo[COM_No_COUNT];
	CCurlCtrl     mesInfo;
	
	BYTE          sSetSN[20];                    // ��������
	BYTE          sBtAddr[20], sNoSN[20], sFirmVer[16];        // ��ʾʹ��
	UINT          iNoBtAddr, iNoSn;              // ������ַ�����ֵ
	UINT          iWrFileSize;                   // ��д�ļ���С
	UINT          iWriteProgress;                // ��д����
	UINT          bitStatusMsg;                  // ���͸������һ��״̬
	MsgLParam     lparam;
	BYTE          sWndMsg[WND_MSG_STR_SIZE];     // ���ݸ��������Ϣ
	UINT		  Mesvalue;

	char		  Mesresring[256];
	BOOL          bEnableWriter;
	BOOL		  bcheckStart;
	UINT          audSelect;                     // �û�ѡ��Ĳ������
	CString		language;

	int fz_jn_flag;//1 fz, 2 jn
	struct general_option_str general_option_instance;
	struct burncheck_option_str burncheck_option_instance;

}MassProdInfo, *MPIPtr;


extern bool find___str(BYTE *buf);
extern INT CreateParamFile(CString src, MPIPtr mpi);
extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);

