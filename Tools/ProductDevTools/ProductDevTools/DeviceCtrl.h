#pragma once

#include "ExecProcess.h"

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
	ERROR_WRITE_DEVNAME,
	ERROR_WRITE_CACHE,
	ERROR_WRITE_FIXPARAM,

	ERROR_READ_LICENSE,
	ERROR_WRITE_LICENSE,

	ERROR_APOLLO,

	ERROR_READ_RECORD,
	ERROR_PLAY_TONE,

	ERROR_READ_SENSOR,
	ERROR_WRITE_SENSOR,

	ERROR_READ_XTALTRIM,
	ERROR_READ_XTALCAP,
	ERROR_WRITE_XTALTRIM,
	ERROR_WRITE_XTALCAP,

	ERROR_WAKEUP, ERROR_SENSOR, ERROR_PLC, ERROR_TAP,

	ERROR_COMMU_FAIL=70,              // COMMU

};

#define WM_DEV_REPORT             (WM_USER+101)
enum {
	REPORT_FIRST=200,
	REPORT_THREAD_START= REPORT_FIRST,
	REPORT_CHIPID,
	REPORT_ERASE_START,
	REPORT_ERASE_PROGRESS,
	REPORT_PROGRAM_START,
	REPORT_PROGRAM_PERCENT,
	REPORT_PROGRAM_END,
	REPORT_OPEN_ENGINE,
	REPORT_RDBD_ADDR,
	REPORT_WRBD_ADDR,
	REPORT_RDBD_NAME,
	REPORT_WRBD_NAME,
	REPORT_WRITE_FIXPARAM,
	REPORT_CLOSE_ENGINE,

	REPORT_WAIT_RESTART,
	REPORT_COMMU_START,
	REPORT_COMMU_SUCC,
	REPORT_COMMU_READ,
	REPORT_COMMU_OPEN,
	RERROT_COMMU_TIMEOUT,

	REPORT_READ_LICENSE,
	REPORT_WRITE_LICENSE,

	REPORT_APOLLO,

	REPORT_READ_RECORD,
	REPORT_PLAY_TONE,
	REPORT_WAKEUP,REPORT_SENSOR,REPORT_PLC,REPORT_TAP,

	REPORT_READ_SENSOR,
	REPORT_WRITE_SENSOR,

	REPORT_READ_XTALTRIM,
	REPORT_READ_XTALCAP,
	REPORT_WRITE_XTALTRIM,
	REPORT_WRITE_XTALCAP,

	REPORT_USER_EXIT,
	REPORT_END_ALL,
	REPORT_LAST,
};

#define PSKEY_BUFFER_LEN            (640)
#define MSG_BUFFER_COUNT            (31)

#define  DEV_HWVER_LEN            (3)
typedef struct tagFIXPARAM {
	uint8          hw_ver[DEV_HWVER_LEN];                  // 硬件版本
	uint8          rev1;
	int16          aud_adj;                    // 音频微调

	uint8          rev[8];
}FixParam, *FixPrmPtr;


// 烧写及检测相关参数
typedef struct tagINIPARAM {
	unsigned char  mode;            // 0： 烧写  1:测试
	CString        btName;          // 蓝牙名称
	CString        hwVer, swVer;    // 硬件版本 软件版本

	CString        sJlinkPath;       // APO使用JLINK烧写
	CString		apollo_burn_bat;	//APO烧写脚本路径
}IniParam, *IniPrmPtr;

enum {THREAD_NONE=0, THREAD_BURN=0x01, THREAD_BURN_APO=0x02,
	THREAD_BT_ADDR=0x04, THREAD_FIX_PARAM=0x08,
	THREAD_CHECK=0x10, THREAD_RECORD_0=0x20, THREAD_RECORD_1 = 0x40, THREAD_PLAY = 0x80,
	THREAD_CRYSTGALTRIM=0x100, THREAD_CRYSTGALTRIM_READ=0x200, THREAD_CRYSTGALTRIM_WRITE = 0x400,
	THREAD_WAKEUP = 0x1000, THREAD_SENSOR=0x2000, THREAD_PLC=0x4000, THREAD_TAP=0x8000,
};

typedef enum {INTR_T_WAKEUP = 0, INTR_T_SENSOR, INTR_T_PLC, INTR_T_TAP} IntrType;

class CDeviceCtrl
{
public:
	CDeviceCtrl();
	~CDeviceCtrl();

public:       // 其它关函数
	static int LoadIniParam(CString sFile, IniPrmPtr param);
	int LoadIniParam(CString sFile);

public:
	int Start(HWND hWnd);
	int Stop(void);
	BOOL IsRuning() { return (m_ctrlThread == INVALID_HANDLE_VALUE) ? FALSE : TRUE; }
	int RuningProc(void);
	void RuningEnd(void);

	int PsWrite(int psKey, void *buffer, int buflen);

public:
	CString Error2String(int eCoder);
	CString Report2String(int rCode);
	void SetFlashImage(CString sFile) { m_sFlashImage = sFile; }	
	void SetEraseAll(int bErase) { m_bEnableErase = bErase;  }
	void SetThreadFunc(int iFunc) { m_iThreadFunc = iFunc; }
	void SetXtalTrim(int cap, int trim) { m_xtalCap = cap; m_xtalTrim = trim; }
	void SetLicenseText(CString license) { m_sLicense = license; }
	int SetHwVersion(CString sText);
	int SetBtAddr(CString addr);       // {0x00ff09, 0x5b, 0x02}
	void SetBtName(CString sName);
	void SetRecordTime(int sec);
	void SetJlinkPath(CString path) { m_iniParam.sJlinkPath = path; }	
	void SetApolloBurnfile(CString path) { m_iniParam.apollo_burn_bat = path; }	

	int CrystalTrimmingRead(int &XtalFreqTrim, int &XtalLoadCap, int bCloseEng);
	int CrystalTrimmingWrite(int XtalFreqTrim, int XtalLoadCap, int bCloseEng);

private:
	IniParam m_iniParam;
	CString m_sFlashImage;
	UINT m_checkStatus;
	char m_bdAddr[32], m_bdName[32];
	int  m_xtalCap, m_xtalTrim;
	CString m_sLicense;
	FixParam m_FixParam;
	BOOL  m_bEnableErase;

	int Burning(void);
	int BurningApollo(void);
	int Recording(int mic, int sec = 5, int bCloseEng = 0);
	int PlayTone(int flag = 0, int sec = 5, int bCloseEnable = 0);
	int SetAllParam(int bCloseEng = 0);
	int SetFixParam(int bCloseEng = 0);
	int CheckDevice(int bCloseEng = 0);
	int CheckInterrupt(IntrType type, int timeout=6, int bCloseEnable = 0);
	int CheckSensorRead(int &value);
	int CheckSensorWrite(int value);

	int CrystalTrimming(int value);


	int teWriteAndRead(void *cmdbuf, int cmdlen, char *readresp);
public:
	int EnterDUTMode(void);
	int OpenEngineNeed(int debug = 0);
	int OpenEngine(int debug = 0);
	int CloseEngine(void);
	
private:
	INT   m_secTimeout, m_secRecord;
	INT   m_SensorDiff;
	uint8 m_msgNo;
	uint8 m_msgData[MSG_BUFFER_COUNT][PSKEY_BUFFER_LEN];
	uint8 * GetMsgBuffer();

	UINT m_curTick;
	HWND m_hWnd;
	BOOL m_bExit;
	UINT m_iThreadFunc;
	HANDLE m_ctrlThread;
	static int DeviceCtrlProc(void *param);

	UINT32  m_devHandle;


};

