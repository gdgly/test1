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
	m_sFlashImage.Empty();
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
	"REPORT_THREAD_START",
	"REPORT_CHIPID",
	"REPORT_ERASE_START",
	"REPORT_ERASE_PROGRESS",
	"REPORT_PROGRAM_START",
	"REPORT_PROGRAM_PERCENT",
	"REPORT_PROGRAM_END",
	"REPORT_OPEN_ENGINE",
	"REPORT_RDBD_ADDR",
	"REPORT_WRBD_ADDR",
	"REPORT_RDBD_NAME",
	"REPORT_WRBD_NAME",
	"REPORT_WRITE_FIXPARAM",
	"REPORT_CLOSE_ENGINE",
	
	"REPORT_WAIT_RESTART",
	"REPORT_COMMU_START",
	"REPORT_COMMU_SUCC",
	"REPORT_COMMU_READ",
	"REPORT_COMMU_OPEN",
	"RERROT_COMMU_TIMEOUT",

	"REPORT_USER_EXIT",
	"REPORT_LAST",
};

CString CDeviceCtrl::Report2String(int rCode)
{
	CString sText;

	if (rCode >= 200 || rCode < REPORT_LAST) {
		rCode -= REPORT_FIRST;
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

	m_msgNo = 0;
	memset(m_msgData, 0, sizeof(m_msgData));

	m_bExit = FALSE;
	m_hWnd = hWnd;
	if (INVALID_HANDLE_VALUE == m_ctrlThread) {
		m_ctrlThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CDeviceCtrl::DeviceCtrlProc,      //创建线程
			this, 0, &dwID);
	}

	return 0;
}


int CDeviceCtrl::Stop(void)
{
	m_bExit = TRUE;
	return 0;
}


int CDeviceCtrl::RuningProc(void)
{
	int ret = 0;

	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_THREAD_START, (LPARAM)0);

	if (!m_sFlashImage.IsEmpty()) {
		if ((ret = Burning()) < 0)
			return ret;

		if (m_bExit == TRUE) goto out;

		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WAIT_RESTART, (LPARAM)0);
		Sleep(500);
		if (m_bExit == TRUE) goto out;
	}

	if (m_bdAddr[0] != '\0') {
		if ((ret = SetAllParam()) < 0)
			return ret;

		if (m_bExit == TRUE) goto out;
	}

	if ((ret=CheckDevice()) < 0)
		return ret;

out:
	if (m_bExit == TRUE)
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_USER_EXIT, (LPARAM)0);

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

void CDeviceCtrl::SetBtName(CString sName)
{
	if (sName.IsEmpty())
		return ;

	memset(m_bdName, 0, sizeof(m_bdName));

	memcpy(m_bdName, sName.GetBuffer(), sName.GetLength());
	sName.ReleaseBuffer();
}

int CDeviceCtrl::SetHwVersion(CString sText)
{
	CString sTmp;
	if (sText.IsEmpty())
		return -1;

	for (int i = 0; i < 3; i++) {
		sTmp = sText.Mid(i * 2, 2);
		m_FixParam.hw_ver[i] = _ttoi(sTmp);
	}

	return 0;
}

uint8 * CDeviceCtrl::GetMsgBuffer()
{
	uint8 *ptr = m_msgData[m_msgNo];

	memset(ptr, 0, PSKEY_BUFFER_LEN);
	m_msgNo += 1;
	if (m_msgNo >= MSG_BUFFER_COUNT)
		m_msgNo = 0;

	return ptr;
}


#define MAX_STR_LEN      (PSKEY_BUFFER_LEN)
#define  ERROROUT(m,w,l,r)      do {MESSAGE2DIALOG(m_hWnd, (m), (WPARAM)(w), (LPARAM)(l)); ret=(r); goto out;}while(0)
int CDeviceCtrl::Burning(void)
{
	INT ret, progress;
	UINT16 maxLen = MAX_STR_LEN, count = 0;
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
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_CHIPID, (LPARAM)status);

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
				if (m_bExit == TRUE) break;
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
	} while (FALSE == m_bExit);

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


int CDeviceCtrl::SetAllParam(void)
{
	char *msgbuf;  uint32 datalen; uint16 datalen16;
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

	// 读取名称
	datalen = PSKEY_BUFFER_LEN;
	msgbuf = (char*)GetMsgBuffer();
	if ((ret = teConfigCacheReadItem(m_devHandle, "bt2:pskey_device_name", msgbuf, &datalen)) != TE_OK) {
		TRACE("BTADDR teConfigCacheReadItem NAME\n");
		ERROROUT(WM_DEV_ERROR, ERROR_READ_DEVNAME, status, -14);
	}
	else {
		TRACE("BTADDR teConfigCacheReadItem:%s LEN=%d\n", msgbuf, datalen);
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_RDBD_NAME, (LPARAM)msgbuf);		// {0xff09, 0x5b, 0x02}
	}

	/* 读取蓝牙地址*/
	datalen = PSKEY_BUFFER_LEN;
	msgbuf = (char*)GetMsgBuffer();
	if ((ret = teConfigCacheReadItem(m_devHandle, "bt2:pskey_bdaddr", msgbuf, &datalen)) != TE_OK) {
		TRACE("BTADDR teConfigCacheReadItem\n");
		ERROROUT(WM_DEV_ERROR, ERROR_READ_BDADDR, status, -4);
	}
	else {
		TRACE("BTADDR teConfigCacheReadItem:%s LEN=%d\n", msgbuf, datalen);
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_RDBD_ADDR, (LPARAM)msgbuf);		// {0xff09, 0x5b, 0x02}
	}
	// psReadBdAddr  有一部分不正确,不知道为什么出现a5a5  // 0x00a5a5 5b 0002

	/* 修改蓝牙地址 OK*/
	status = teConfigCacheWriteItem(m_devHandle, "bt2:pskey_bdaddr", m_bdAddr);
	if(status != TE_OK) {
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_BTADDR, status, -5);
	}
	else {
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRBD_ADDR, (LPARAM)m_bdAddr);
	}
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);
	
	if ((ret = teConfigCacheWriteItem(m_devHandle, "bt2:pskey_device_name", m_bdName)) != TE_OK) {
		TRACE("BTADDR teConfigCacheWriteItem NAME\n");
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_DEVNAME, status, -15);
	}
	else {
		TRACE("BTADDR teConfigCacheWriteItem:name=%s\n", m_bdName);
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRBD_NAME, (LPARAM)m_bdName);		// BTADDR ANME
	}	
	
	status = teConfigCacheWrite(m_devHandle, NULL, 0);
	if (status != TE_OK) {
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_CACHE, status, -6);
	}
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);

	// 写固定参数区域
	datalen16 = 64;
	msgbuf = (char*)GetMsgBuffer();
	ret = tePsRead(m_devHandle, 10192, sizeof(FixParam) / 2, (uint16*)msgbuf, &datalen16);
	TRACE("LINE:%d ret=%d ERROR=%d\n", __LINE__, ret, teGetLastError(m_devHandle));

	FixPrmPtr prm = (FixPrmPtr)msgbuf;
	TRACE("FIXPRM adr=%d hwVER=%02x %02x %02x -->%02x %02x %02x\n", prm->aud_adj, 
		prm->hw_ver[0], prm->hw_ver[1], prm->hw_ver[2], m_FixParam.hw_ver[0], m_FixParam.hw_ver[1], m_FixParam.hw_ver[2]);
	memcpy(prm->hw_ver, m_FixParam.hw_ver, 3);
	ret = tePsWrite(m_devHandle, 10192, sizeof(FixParam) / 2, (uint16*)msgbuf);
	if (ret != TE_OK) {
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_FIXPARAM, status, -7);
	}
	else
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRITE_FIXPARAM, (LPARAM)0);


out:
	CloseEngine();

	return ret;
}

int CDeviceCtrl::PsWrite(int psKey, void *buffer, int buflen)
{
	int ret = 0;

	if (OpenEngine() < 0) {
		TRACE("Error openTestEngine\n");
		ERROROUT(WM_DEV_ERROR, ERROR_OPEN_ENGINE, 0, -1);
	}

	ret = tePsWrite(m_devHandle, psKey, (buflen + 1) / 2, (uint16*)buffer);
	if (ret != TE_OK)
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_FIXPARAM, ret, -7);
	else
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRITE_FIXPARAM, (LPARAM)0);

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
	int ret,conneted = 0;
	CString sText;
	UINT16 *cmdbuf, cmdlen;
	uint8 channel;
	UINT16 *rdbuf, rdlen;

	m_checkStatus = 0;
	if (OpenEngine() < 0) {
		MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_OPEN_ENGINE, (LPARAM)0);
		return -1;
	}
	
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_OPEN, (LPARAM)0);
	while (1) {
		if (m_bExit) break;
		switch (m_checkStatus) {
		case CHECK_ST_INFO:
			cmdbuf = (UINT16*)GetMsgBuffer();
			cmdlen = sprintf_s((char*)cmdbuf, PSKEY_BUFFER_LEN, "check deviceinfo");
			ret = teAppWrite(m_devHandle, 0, cmdbuf, cmdlen / 2);
			if (ret == TE_OK) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_SUCC, (LPARAM)cmdbuf);
			}
			else {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_COMMU_FAIL, (LPARAM)cmdbuf);
				TRACE("ERROR Send Cmd\n");
				break;
			}
			break;
		case CHECK_ST_INFO_WAIT:
			rdbuf = (UINT16*)GetMsgBuffer();
			ret = teAppRead(m_devHandle, &channel, rdbuf, PSKEY_BUFFER_LEN/2, &rdlen, 500);
			if (ret == TE_OK) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_READ, (LPARAM)rdbuf);
				conneted = 1;
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
			if ((::GetTickCount() - m_curTick) > (5 * 1000) && (conneted == 0)) {
				m_checkStatus -= 1;
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, RERROT_COMMU_TIMEOUT, (LPARAM)0);
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
		if (m_bExit == TRUE)
			return -2;
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

