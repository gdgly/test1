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
#define WND_MSG_STR_SIZE        (256)   // 界面显示的字符串大小
#define MAX_AUDIO_PLAY           (6)    // 播放12种不同的音乐

enum {RUNST_NONE, RUNST_WAIT_BOARD, RUNST_GET_BOARD,
	RUNST_START_WRITE0, RUNST_START_WRITE1, RUNST_START_WRITE2,RUNST_START_RESET,
	RUNST_CHECK_INIT,   // 启动信息检查
	RUNST_CHECK_AUDIO,  RUNST_AUDIO_WAITUSER, // 
	RUNST_CHECK_CHARG_CURRET,        // 发送充电命令
	RUNST_CHECK_WAIT_CHARGE,         // 比较充电电流
	RUNST_CHECK_SLEEP_CURRENT,
	RUNST_CHECK_WAIT_SLEEP,
	RUNST_ALL_SUESS,
	RUNST_BUREALL_SUESS,
	RUNST_ALL_SUCC_IDLE,             // 结束之后，可以更新电流
	RUNST_CURRENT_ERR,
	RUNST_SYS_ERROR,
};           // 线程的主要状态机

typedef struct tagMASSPRODUCTINFO {	
//#ifdef CONFIG_SUPPORT_AUDPLAY_THREAD
	// 音乐播放线程相关
	HANDLE        hAudThread;
	DWORD         idAudThread;
	UINT          iAudSetloops, iAudPlayloops;        // 线程控制及当前播放次数
//#endif

	HANDLE        hThread;
	DWORD         idThread;
	HWND          hWnd;


	UINT          runStatus;
	CTime         tStatus;    //状态改变的时间

	UINT          ch;
	UINT          doExit;         // 线程退出
	UINT          audNo;          // 播放的音频文件序号
	UINT          playTimes;      // 播放次数

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
	
	BYTE          sSetSN[20];                    // 窗口设置
	BYTE          sBtAddr[20], sNoSN[20], sFirmVer[16];        // 显示使用
	UINT          iNoBtAddr, iNoSn;              // 蓝牙地址及序号值
	UINT          iWrFileSize;                   // 烧写文件大小
	UINT          iWriteProgress;                // 烧写进度
	UINT          bitStatusMsg;                  // 发送给界面的一种状态
	MsgLParam     lparam;
	BYTE          sWndMsg[WND_MSG_STR_SIZE];     // 传递给界面的消息
	UINT		  Mesvalue;

	char		  Mesresring[256];
	BOOL          bEnableWriter;
	BOOL		  bcheckStart;
	UINT          audSelect;                     // 用户选择的播放序号
	CString		language;

	int fz_jn_flag;//1 fz, 2 jn
	struct general_option_str general_option_instance;
	struct burncheck_option_str burncheck_option_instance;

}MassProdInfo, *MPIPtr;


extern bool find___str(BYTE *buf);
extern INT CreateParamFile(CString src, MPIPtr mpi);
extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);

