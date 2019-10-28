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
	ERROR_READ_BDADDR,
	ERROR_WRITE_BTADDR,
	ERROR_WRITE_CACHE,

	RERROT_COMMU_FAIL=70,              // COMMU

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
	REPORT_WRBD_ADDR,
	REPORT_CLOSE_ENGINE,

	REPORT_WAIT_RESTART,
	REPORT_COMMU_START,
	REPORT_COMMU_SUCC,
	REPORT_COMMU_READ,

	REPORT_LAST,
};


#define  DEV_HWVER_LEN            (3)
typedef struct tagFIXPARAM {
	uint8          hw_ver[DEV_HWVER_LEN];                  // 硬件版本
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
	int RuningProc(void);
	void RuningEnd(void);

public:
	CString Error2String(int eCoder);
	CString Report2String(int rCode);
	void SetFlashImage(CString sFile) { m_sFlashImage = sFile; }
	int SetBtAddr(CString addr);       // {0x00ff09, 0x5b, 0x02}
private:
	CString m_sFlashImage;
	UINT m_checkStatus;
	char m_bdAddr[32];
	BOOL  m_bEnableErase;

	int Burning(void);
	int SetAllParam(void);
	int CheckDevice(void);

	int OpenEngine(void);
	int CloseEngine(void);

private:
	UINT m_curTick;
	HWND m_hWnd;
	BOOL m_bExit;
	HANDLE m_ctrlThread;
	static int DeviceCtrlProc(void *param);

	UINT32  m_devHandle;


};

