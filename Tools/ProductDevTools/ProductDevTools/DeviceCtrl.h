#pragma once

#include "bluesite\\include\\TestFlash.h"
#include "bluesite\\include\\TestEngine.h"

#define WM_DEV_ERROR              (WM_USER+100)
enum {
	ERROR_GETPORT = 10,                  // flGetAvailablePorts
	ERROR_GETPORT_NOPORT,                // 获取不成功
	ERROR_OPENTRANS,                     // flOpenTrans
	ERROR_OPEN_FLASHFILE,
	ERROR_ERASE_START,
	ERROR_ERASE_SETBANK,
	ERROR_PROGRAM_START,
	ERROR_PROGRAM_FAIL,
	ERROR_FLASH_TIMEOUT,

	ERROR_OPEN_ENGINE=40,               // TestEngine
	ERROR_CACHE_INIT,
	ERROR_CACHE_READ,
	ERROR_READ_DEVNAME,
	ERROR_READ_BDADDR,
	ERROR_WRITE_BTADDR,
	ERROR_WRITE_CACHE,
	ERROR_WRITE_FIXPARAM,

	ERROR_COMMU_FAIL=70,              // COMMU

};

#define WM_DEV_REPORT             (WM_USER+101)
enum {
	ERROR_CHIPID = 200,
	REPORT_ERASE_START,
	REPORT_ERASE_PROGRESS,
	REPORT_PROGRAM_START,
	REPORT_PROGRAM_PERCENT,
	REPORT_PROGRAM_END,
	REPORT_OPEN_ENGINE,
	REPORT_RDBD_ADDR,
	REPORT_RDBD_NAME,
	REPORT_WRBD_ADDR,
	REPORT_WRITE_FIXPARAM,
	REPORT_CLOSE_ENGINE,

	REPORT_WAIT_RESTART,
	REPORT_COMMU_START,
	REPORT_COMMU_SUCC,
	REPORT_COMMU_READ,
	REPORT_COMMU_OPEN,
	RERROT_COMMU_TIMEOUT,

	REPORT_USER_EXIT,
	REPORT_LAST,
};

#define PSKEY_BUFFER_LEN            (128)
#define MSG_BUFFER_COUNT            (31)

#define  DEV_HWVER_LEN            (3)
typedef struct tagFIXPARAM {
	uint8          hw_ver[DEV_HWVER_LEN];                  // 硬件版本
	uint8          rev1;
	int16          aud_adj;                    // 音频微调

	uint8          rev[8];
}FixParam, *FixPrmPtr;

class CDeviceCtrl
{
public:
	CDeviceCtrl();
	~CDeviceCtrl();

public:
	int Start(HWND hWnd);
	int Stop(void);
	BOOL IsRuning() { return (m_ctrlThread == INVALID_HANDLE_VALUE) ? FALSE : TRUE; }
	int RuningProc(void);
	void RuningEnd(void);

public:
	CString Error2String(int eCoder);
	CString Report2String(int rCode);
	void SetFlashImage(CString sFile) { m_sFlashImage = sFile; }
	int SetHwVersion(CString sText);
	int SetBtAddr(CString addr);       // {0x00ff09, 0x5b, 0x02}
private:
	CString m_sFlashImage;
	UINT m_checkStatus;
	char m_bdAddr[32];
	FixParam m_FixParam;
	BOOL  m_bEnableErase;

	int Burning(void);
	int SetAllParam(void);
	int CheckDevice(void);

	int OpenEngine(void);
	int CloseEngine(void);
	
private:
	uint8 m_msgNo;
	uint8 m_msgData[MSG_BUFFER_COUNT][PSKEY_BUFFER_LEN];
	uint8 * GetMsgBuffer();

	UINT m_curTick;
	HWND m_hWnd;
	BOOL m_bExit;
	HANDLE m_ctrlThread;
	static int DeviceCtrlProc(void *param);

	UINT32  m_devHandle;


};

