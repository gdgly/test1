#pragma once

#include "MyComm.h"
#include "CtrlItem.h"

#define WM_DEV_STRING              (WM_USER+190)

// 使用多个内存来向WIND发送消息
#define MSG_BUFFER_LEN              (256)
#define MSG_BUFFER_COUNT            (128)
enum {MSG_T_DEV=0, MSG_T_REPORT};         // 发送给WIN字符串类型

#define COM_BUFFER_LEN              (1024*4)         // 串口接收缓冲区


enum {THREAD_NONE=0, THREAD_BURN=0x01, THREAD_BURN_APO=0x02,
	THREAD_BT_ADDR=0x04, THREAD_FIX_PARAM=0x08,
	THREAD_CHECK=0x10, THREAD_RECORD_0=0x20, THREAD_RECORD_1 = 0x40, THREAD_PLAY = 0x80,
	THREAD_CRYSTGALTRIM=0x100, THREAD_CRYSTGALTRIM_READ=0x200, THREAD_CRYSTGALTRIM_WRITE = 0x400,
	THREAD_WAKEUP = 0x1000, THREAD_SENSOR=0x2000, THREAD_PLC=0x4000, THREAD_TAP=0x8000,

	THREAD_COM=0x10000,
};

class CCtrlDevice
{
public:
	CCtrlDevice();
	~CCtrlDevice();

	// 盒子升级相关
public:
	CMyComm m_hCom;
	int     m_comPort, m_comBaud;
	void    SetComPort(int port, CString sName) { m_comPort = port; m_sID = sName;}
	void    SetThreadFunc(UINT func) { m_iThreadFunc = func; }
	int		RuningUpgrade(CString sFile);

public:
	int Start(HWND hWnd);
	int Stop(void);
	BOOL IsRuning() { return (m_ctrlThread == INVALID_HANDLE_VALUE) ? FALSE : TRUE; }
	int RuningProc(void);
	void RuningEnd(void);

public:
	UINT m_checkItems;
	void SetCheckItemCount(UINT count) { m_checkItems = count; }
	
private:
	int SendText(UINT wParam, CString sText);
	int SendReportText(CString sText);
	int SendDevText(CString sText);

	int ComDataSend(BYTE *buffer, UINT count);        // 返回剩下的数据长度
	int RuningComStart(CString sLog);

private:
	CString m_sID;
	CString m_sFlashImage;
	UINT m_checkStatus;
	char m_bdAddr[32], m_bdName[32];
	BOOL m_btWrite;
	int  m_xtalCap, m_xtalTrim;
	CString m_sLicense;
	BOOL  m_bEnableErase;

private:
	BYTE m_msgNo;
	BYTE m_msgData[MSG_BUFFER_COUNT][MSG_BUFFER_LEN+1];
	BYTE * GetMsgBuffer();

	UINT  m_rdLen;
	BYTE  m_rdBuff[COM_BUFFER_LEN];
	
private:
	INT   m_secTimeout, m_secRecord;
	INT   m_SensorDiff;

	UINT m_curTick;
	HWND m_hWnd;
	BOOL m_bExit;
	UINT m_iThreadFunc;
	HANDLE m_ctrlThread;
	static int DeviceCtrlProc(void *param);

	UINT32  m_devHandle;


};

