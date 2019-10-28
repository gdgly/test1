#include "stdafx.h"
#include "DeviceCtrl.h"

#pragma comment(lib,"bluesite\\lib\\TestEngine.lib")
#pragma comment(lib,"bluesite\\lib\\TestFlash.lib")

#define  MESSAGE2DIALOG         PostMessage

CDeviceCtrl::CDeviceCtrl()
{
	m_bEnableErase = FALSE;

	m_curTick = 0;
	m_devHandle = 0;
	memset(m_bdAddr, 0, sizeof(m_bdAddr));
	m_ctrlThread = INVALID_HANDLE_VALUE;
}


CDeviceCtrl::~CDeviceCtrl()
{
}

int CDeviceCtrl::DeviceCtrlProc(void *param)                //相当于于一个main
{
	CDeviceCtrl *pdev = (CDeviceCtrl*)param;

	pdev->RuningProc();

	pdev->RuningEnd();
	return 0;
}

void CDeviceCtrl::RuningEnd(void)
{
	if (m_devHandle > 0)
		CloseEngine();

	m_ctrlThread = INVALID_HANDLE_VALUE;
}


CString CDeviceCtrl::Error2String(int eCoder)
{
	CString sText;


	return sText;
}

static CString _sReport[] = {
	"ERROR_CHIPID",
	"REPORT_ERASE_START",
	"REPORT_ERASE_PROGRESS",
	"REPORT_PROGRAM_START",
	"REPORT_PROGRAM_PERCENT",
	"REPORT_PROGRAM_END",
	"REPORT_OPEN_ENGINE",
	"REPORT_RDBD_ADDR",
	"REPORT_WRBD_ADDR",
	"REPORT_CLOSE_ENGINE",
	
	"REPORT_WAIT_RESTART",
	"REPORT_COMMU_START",
	"REPORT_COMMU_SUCC",
	"REPORT_COMMU_READ",
};

CString CDeviceCtrl::Report2String(int rCode)
{
	CString sText;

	if (rCode >= 200 || rCode < REPORT_LAST) {
		rCode -= ERROR_CHIPID;
		sText = _sReport[rCode];
	}

	return sText;
}

int CDeviceCtrl::Start(HWND hWnd)
{
	DWORD dwID;

	if (INVALID_HANDLE_VALUE != m_ctrlThread) {
		AfxMessageBox("Please Ended First");
		return 0;
	}

	m_bExit = FALSE;
	m_hWnd = hWnd;
	if (INVALID_HANDLE_VALUE == m_ctrlThread) {
		m_ctrlThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CDeviceCtrl::DeviceCtrlProc,      //创建线程
			this, 0, &dwID);
	}

	return 0;
}


int CDeviceCtrl::RuningProc(void)
{
	int ret = 0;

#if 0
	if((ret = Burning()) < 0)
		return ret;

	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WAIT_RESTART, (LPARAM)0);
	Sleep(500);	
	if ((ret = SetAllParam()) < 0)
		return ret;
#endif

	if ((ret=CheckDevice()) < 0)
		return ret;

	return 0;
}

int CDeviceCtrl::SetBtAddr(CString addr)
{
	if (addr.IsEmpty())
		return -1;
	
	memset(m_bdAddr, 0, sizeof(m_bdAddr));

	memcpy(m_bdAddr, addr.GetBuffer(), addr.GetLength());
	addr.ReleaseBuffer();

	return 0;
}


#define MAX_STR_LEN      (256)
#define  ERROROUT(m,w,l,r)      do {MESSAGE2DIALOG(m_hWnd, (m), (WPARAM)(w), (LPARAM)(l)); ret=(r); goto out;}while(0)
int CDeviceCtrl::Burning(void)
{
	INT ret, progress;
	UINT16 maxLen, count = 0;
	char portsStr[MAX_STR_LEN];
	char transStr[MAX_STR_LEN];

	// The human readable port strings (e.g. "USBDBG(100)")
	// The transport option strings (e.g. "SPITRANS=USBDBG SPIPORT=1")
	int32 status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	if (status != TFL_OK && maxLen != 0) {		
		MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_GETPORT, (LPARAM)status);		
		return -1;
	}

	if (status != TFL_OK || count == 0 )	{
		TRACE("Error getting ports, or none found\n");
		MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_GETPORT_NOPORT, (LPARAM)status);
		return -2;
	}

	TRACE("PORT:%s   |%s\n", portsStr, transStr);
	char sVer[32];
	//	atlTraceGeneral - PORT:USBDBG(112) | SPITRANS = USBDBG SPIPORT = 1

#if 0
	if (flOpen(devMask, 10, 26, TFL_USBDBG) != TFL_OK) {
		TRACE("Failed to open devices %d err:%d\n", flGetBitErrorField(),
			flGetLastError());
	}

#else
	if ((status=flOpenTrans("SPITRANS=USBDBG", 10, 26)) != TFL_OK) {
		TRACE("Failed to flmOpenTrans devices %d\n", flmGetBitErrorField());
		MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_OPENTRANS, (LPARAM)status);
		return -3;
	}
#endif

	status = flGetChipId();   // 0 is :ERROR
	TRACE("chipID=%x\n", status);
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, ERROR_CHIPID, (LPARAM)status);

	status = flGetVersion(sVer);
	TRACE("VER:%s\n", sVer);

	status = flReadProgramFiles(m_sFlashImage);
	if (status != TFL_OK) {
		TRACE("Failed to read flash program files:%d\n", flGetLastError());
		ERROROUT(WM_DEV_ERROR, ERROR_OPEN_FLASHFILE, status, -3);
	}

	if (flSetFlashType(TFL_TYPE_SQIF) != TFL_OK) {
		TRACE("Failed to set flash type\n");
	}

#if 0
	// 调用不成功
	uint16 sectors, sizeMbits; uint32 manid, devid;
	if (flGetFlashInfo(&sectors, &sizeMbits, &manid, &devid) == TFL_OK) {
		TRACE("FLASH: %d %dMB %d %d\n", sectors, sizeMbits, manid, devid);
	}
#endif
	if ((status=flSetSubsysBank(4, 0)) != TFL_OK) {
		TRACE("Failed to set subsystem and bank\n");
		ERROROUT(WM_DEV_ERROR, ERROR_ERASE_SETBANK, status, -5);
	}
	
	if (m_bEnableErase == TRUE) {
		m_curTick = ::GetTickCount();
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_ERASE_START, (LPARAM)0);
		if ((status = flErase()) == TFL_OK) {
			while (status >= 0 && status < 100) {
				Sleep(500);
				status = flGetProgress();
				TRACE("ERASE %d percent\n", status);
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_ERASE_PROGRESS, (LPARAM)status);
				if ((::GetTickCount() - m_curTick) > (30 * 1000)) {
					ERROROUT(WM_DEV_ERROR, ERROR_FLASH_TIMEOUT, 0, -14);
					break;
				}
			}
		}
		else {
			TRACE("Failed to flErase:%d\n", flGetLastError());
			//	ERROROUT(WM_DEV_ERROR, ERROR_ERASE_START, status, -4);
		}
	}

	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_PROGRAM_START, (LPARAM)0);
	if (flProgramSpawn() != TFL_OK)	{
		TRACE("Failed to spawn flmash program thread\n");
		ERROROUT(WM_DEV_ERROR, ERROR_PROGRAM_START, status, -6);
	}

	m_curTick = ::GetTickCount();
	do
	{
		Sleep(500);
		progress = flGetProgress();
		TRACE("progress = %d\n", progress);
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_PROGRAM_PERCENT, (LPARAM)progress);
		if (progress < 0 || progress >= 100)
			break;

		if ((::GetTickCount() - m_curTick) >(30 * 1000)) {
			ERROROUT(WM_DEV_ERROR, ERROR_FLASH_TIMEOUT, 0, -15);
			break;
		}
	} while (1);

	TRACE("Completed\n");
	if (progress >= 100) {
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_PROGRAM_END, (LPARAM)0);
	}
	else {
		ERROROUT(WM_DEV_ERROR, ERROR_PROGRAM_FAIL, status, -6);
	}

	ret = 0;

out:

	flClose();
	return ret;
}


#define PSKEY_BUFFER_LEN            (128)

int CDeviceCtrl::SetAllParam(void)
{
	char data[PSKEY_BUFFER_LEN]; uint32 datalen; uint16 datalen16;
	int status, ret;
	const char* const CFG_DB_PARAM = "hydracore_config.sdb:QCC512X_CONFIG";
	CString sText;

	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_OPEN_ENGINE, (LPARAM)0);	
	if (OpenEngine() < 0) {
		TRACE("Error openTestEngine\n");
		ERROROUT(WM_DEV_ERROR, ERROR_OPEN_ENGINE, 0, -1);
	}

	//  注意需要 D:\QCC512X\BlueSuite 3.2.2
	//  所有的DLL文件 及hpm文件
	if ((status = teConfigCacheInit(m_devHandle, CFG_DB_PARAM)) != TE_OK) {
		TRACE("ERROR CACHE INIT\n");
		ERROROUT(WM_DEV_ERROR, ERROR_CACHE_INIT, status, -2);
	}

	if (TE_OK != (status = teConfigCacheRead(m_devHandle, NULL, 0))) {
		TRACE("ERRO Cache Read\n");
		ERROROUT(WM_DEV_ERROR, ERROR_CACHE_READ, status, -3);
	}

	datalen = PSKEY_BUFFER_LEN;
	memset(data, 0, sizeof(data));
	if ((ret = teConfigCacheReadItem(m_devHandle, "bt2:pskey_bdaddr", data, &datalen)) != TE_OK) {
		TRACE("BTADDR teConfigCacheReadItem\n");
		ERROROUT(WM_DEV_ERROR, ERROR_READ_BDADDR, status, -4);
	}
	else {
		TRACE("BTADDR teConfigCacheReadItem:%s LEN=%d\n", data, datalen);
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_RDBD_ADDR, (LPARAM)data);		// {0xff09, 0x5b, 0x02}
	}
	// psReadBdAddr  有一部分不正确,不知道为什么出现a5a5  // 0x00a5a5 5b 0002

	/* 修改蓝牙地址 OK*/
	status = teConfigCacheWriteItem(m_devHandle, "bt2:pskey_bdaddr", m_bdAddr);
	if(status != TE_OK) {
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_BTADDR, status, -5);
	}
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);

	status = teConfigCacheWrite(m_devHandle, NULL, 0);
	if (status != TE_OK) {
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_CACHE, status, -6);
	}
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRBD_ADDR, (LPARAM)m_bdAddr);
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);

	// 写固定参数区域
	datalen16 = 64;
	ret = tePsRead(m_devHandle, 10192, sizeof(FixParam) / 2, (uint16*)data, &datalen16);
	TRACE("LINE:%d ret=%d ERROR=%d\n", __LINE__, ret, teGetLastError(m_devHandle));


	ret = 0;

out:
	CloseEngine();

	return ret;
}

//  启动
// PC ----> check deviceinfo
//   <----- check LIS25 PASS/FAIL
//   <----- check DM430 PASS/FAIL
//   <----- check BTADDR xx:xx
//
enum {
	CHECK_ST_INFO, CHECK_ST_INFO_WAIT,             // check deviceinfo
};

int CDeviceCtrl::CheckDevice(void)
{
	int ret;
	CString sText;
	UINT16 cmdbuf[128], cmdlen;
	uint8 channel;
	UINT16 rdbuf[128], rdlen;

	m_checkStatus = 0;
	if (OpenEngine() < 0) {
		MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_OPEN_ENGINE, (LPARAM)0);
		return -1;
	}
	
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_SUCC, (LPARAM)cmdbuf);
	while (1) {
		switch (m_checkStatus) {
		case CHECK_ST_INFO:
			cmdlen = sprintf_s((char*)cmdbuf, sizeof(cmdbuf), "check deviceinfo");
			ret = teAppWrite(m_devHandle, 0, cmdbuf, cmdlen / 2);
			if (ret == TE_OK) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_SUCC, (LPARAM)cmdbuf);
			}
			else {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, RERROT_COMMU_FAIL, (LPARAM)cmdbuf);
				TRACE("ERROR Send Cmd\n");
				break;
			}
			break;
		case CHECK_ST_INFO_WAIT:
			memset(rdbuf, 0, sizeof(rdbuf));
			ret = teAppRead(m_devHandle, &channel, rdbuf, 128, &rdlen, 500);
			if (ret == TE_OK) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_READ, (LPARAM)rdbuf);
			}
			break;

		default:
			break;

		}

		if (m_checkStatus % 2 == 0) {
			m_curTick = ::GetTickCount();
			m_checkStatus += 1;
		}
		else {
			if ((::GetTickCount() - m_curTick) > (3 * 1000)) {
				m_checkStatus -= 1;
				TRACE("Time out\n");
			}
		}
	}


	return 0;
}

int CDeviceCtrl::OpenEngine(void)
{
	for (int i = 0; i < 50; i++) {
		m_devHandle = openTestEngine(USBDBG, "1", 0, 5000, 1000);
		if (m_devHandle > 0)
			break;
		TRACE("openTestEngine FAILE,loops=%d\n", i);
		Sleep(300);
	}

	if (m_devHandle > 0)
		return 0;

	return -1;
}

int CDeviceCtrl::CloseEngine(void)
{
	if (m_devHandle > 0) {
		closeTestEngine(m_devHandle);
		m_devHandle = 0;
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_CLOSE_ENGINE, (LPARAM)0);
	}

	return 0;
}

