#include "stdafx.h"
#include "DeviceCtrl.h"

#pragma comment(lib,"bluesite\\lib\\TestEngine.lib")
#pragma comment(lib,"bluesite\\lib\\TestFlash.lib")

#define  MESSAGE2DIALOG         PostMessage


const char* const CFG_DB_PARAM = "hydracore_config.sdb:QCC512X_CONFIG";

// sFile: ini�ļ�λ�ã���̼��ļ�λ��xuv
int CDeviceCtrl::LoadIniParam(CString sFile, IniPrmPtr param)
{
	int tmp;
	int  keysize;	char keybuf[512];
	CString iniFile;

	if (sFile.IsEmpty() || !param)
		return -1;

	keysize = sizeof(keybuf);
	if ((tmp = sFile.Find(".xuv", 0)) > 0) {
		iniFile = sFile.Left(tmp) + ".ini";
	}
	else if ((tmp = sFile.Find(".ini")) > 0) {
		iniFile = sFile;
	}

	if ((tmp = GetPrivateProfileString("VERSION", "HWVER", "", keybuf, keysize, iniFile)) < 1)
		return -2;
	keybuf[tmp] = '\0';
	param->hwVer.Format("%s", keybuf);

	if ((tmp = GetPrivateProfileString("VERSION", "SWVER", "", keybuf, keysize, iniFile)) < 1)
		return -4;
	keybuf[tmp] = '\0';
	param->swVer.Format("%s", keybuf);

	if ((tmp = GetPrivateProfileString("SYSTEM", "BTNAME", "", keybuf, keysize, iniFile)) < 1)
		return -6;
	keybuf[tmp] = '\0';
	param->btName.Format("%s", keybuf);

	param->mode = GetPrivateProfileInt("SYSTEM", "MODE", 0, iniFile);
	
	if ((tmp = GetPrivateProfileString("SYSTEM", "JLINKPATH", "", keybuf, keysize, iniFile)) < 1)
		return -7;
	keybuf[tmp] = '\0';
	param->sJlinkPath.Format("%s", keybuf);

	return 0;
}

int CDeviceCtrl::LoadIniParam(CString sFile)
{
	return LoadIniParam(sFile, &m_iniParam);
}


CDeviceCtrl::CDeviceCtrl()
{
	m_bEnableErase = FALSE;

	m_curTick = 0;
	m_devHandle = 0;
	memset(m_bdAddr, 0, sizeof(m_bdAddr));
	m_sFlashImage.Empty();
	m_iThreadFunc = THREAD_NONE;
	m_ctrlThread = INVALID_HANDLE_VALUE;
}


CDeviceCtrl::~CDeviceCtrl()
{
}

int CDeviceCtrl::DeviceCtrlProc(void *param)                //�൱����һ��main
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

	"RERROT_APOLLO",

	"REPORT_READ_RECORD",

	"REPORT_USER_EXIT",
	"REPORT_END_ALL",
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
		m_ctrlThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CDeviceCtrl::DeviceCtrlProc,      //�����߳�
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
	
	if ((m_iThreadFunc & THREAD_BURN)) {
		if (!m_sFlashImage.IsEmpty()) {
			if ((ret = Burning()) < 0)
				goto out;

			if (m_bExit == TRUE) goto out;
		}
	}

	if ((m_iThreadFunc & THREAD_BURN_APO)) {
		if ((ret = BurningApollo()) < 0)
			goto out;
		if (m_bExit == TRUE) goto out;
	}

	if ((m_iThreadFunc & THREAD_BT_ADDR)) {
		if (m_bdAddr[0] != '\0')
			if (SetAllParam() < 0)
				goto out;

		if (m_bExit == TRUE) goto out;
	}

	if ((m_iThreadFunc & THREAD_FIX_PARAM)) {
		SetFixParam();
	}

	if ((m_iThreadFunc & THREAD_CHECK)) {
		if ((ret = CheckDevice()) < 0)
			goto out;

		if (m_bExit == TRUE) goto out;
	}

	if ((m_iThreadFunc & THREAD_RECORD_0)) {
		if ((ret = Recording(0)) < 0)
			goto out;
		if (m_bExit == TRUE) goto out;
	}

	if ((m_iThreadFunc & THREAD_RECORD_1)) {
		if ((ret = Recording(1)) < 0)
			goto out;

		if (m_bExit == TRUE) goto out;
	}

	if ((m_iThreadFunc & THREAD_CRYSTGALTRIM)) {
		CrystalTrimming(0);
		if (m_bExit == TRUE) goto out;
	}
		

out:
	CloseEngine();

	if (m_bExit == TRUE)
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_USER_EXIT, (LPARAM)0);
	
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_END_ALL, (LPARAM)ret);

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
	// ���ò��ɹ�
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
	TRACE("End Burn\n");

	return ret;
}

int CDeviceCtrl::BurningApollo(void)
{
	int ret = 0, rdlen;
	char sText[256], *msgbuf;
	CString sCommand, sTmp;
	CExecProcess  *execProg = new CExecProcess();

	sCommand.Format("%s -CommanderScript burnApollo.bat", m_iniParam.sJlinkPath);

	execProg->Create(CExecProcess::ASYNC);
	execProg->Execute(sCommand);

	while (true) {
		memset(sText, 0, sizeof(sText));
		if ((rdlen = execProg->ReadLine(sText, 256)) > 0) {
			TRACE("Get%d:%s", rdlen, (char*)sText);

			sTmp.Format("%s", sText);
			if (sTmp.Find("SEGGER J-Link ") >= 0) {  // SEGGER J-Link Commander V6.42d (Compiled Feb 15 2019 13:54:42)
				msgbuf = (char*)GetMsgBuffer();
				sprintf_s(msgbuf, PSKEY_BUFFER_LEN, "%s", "Start J-Link Success");
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_APOLLO, (LPARAM)msgbuf);				
			}
			else if (sTmp.Find("Could not open J-Link Command File") >= 0) { //  Could not open J-Link Command File 'burnApollo.bat'
				msgbuf = (char*)GetMsgBuffer();
				sprintf_s(msgbuf, PSKEY_BUFFER_LEN, "%s", "No Found burnApollo.bat");
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_APOLLO, (LPARAM)msgbuf);
				break;
			}
			else if (sTmp.Find("SS") >= 0) {
			}
		}
	}

	delete execProg;

	return ret;
}


int CDeviceCtrl::SetAllParam(int bCloseEng)
{
	char *msgbuf;  uint32 datalen;
	int status, ret = 0;
	CString sText;

	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_OPEN_ENGINE, (LPARAM)0);	
	if (m_devHandle == 0) {
		if (OpenEngine() < 0) {
			TRACE("Error openTestEngine\n");
			ERROROUT(WM_DEV_ERROR, ERROR_OPEN_ENGINE, 0, -1);
		}
	}

	//  ע����Ҫ D:\QCC512X\BlueSuite 3.2.2
	//  ���е�DLL�ļ� ��hpm�ļ�
	if ((status = teConfigCacheInit(m_devHandle, CFG_DB_PARAM)) != TE_OK) {
		TRACE("ERROR CACHE INIT\n");
		ERROROUT(WM_DEV_ERROR, ERROR_CACHE_INIT, status, -2);
	}

	if (TE_OK != (status = teConfigCacheRead(m_devHandle, NULL, 0))) {
		TRACE("ERRO Cache Read\n");
		ERROROUT(WM_DEV_ERROR, ERROR_CACHE_READ, status, -3);
	}

	// ��ȡ����
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

	/* ��ȡ������ַ*/
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
	// psReadBdAddr  ��һ���ֲ���ȷ,��֪��Ϊʲô����a5a5  // 0x00a5a5 5b 0002

	/* �޸�������ַ OK*/
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
	ret = 0;

out:
	if(bCloseEng)
		CloseEngine();

	return ret;
}

// û�е��� CloseEngine���������һ���Ե���
int CDeviceCtrl::SetFixParam(int bCloseEng)
{
	int ret = 0;
	char *msgbuf;  uint16 datalen16;

	if (m_devHandle == 0) {
		if (OpenEngine() < 0) {
			TRACE("Error openTestEngine\n");
			ERROROUT(WM_DEV_ERROR, ERROR_OPEN_ENGINE, 0, -1);
		}
	}

	// д�̶���������
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
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_FIXPARAM, ret, -7);
	}
	else
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRITE_FIXPARAM, (LPARAM)0);

	ret = 0;

out:
	return ret;
}

int CDeviceCtrl::PsWrite(int psKey, void *buffer, int buflen)
{
	int ret = 0, opened = 0;

	if (m_devHandle == 0) {
		if (OpenEngine() < 0) {
			TRACE("Error openTestEngine\n");
			ERROROUT(WM_DEV_ERROR, ERROR_OPEN_ENGINE, 0, -1);
		}
		opened = 1;
	}

	ret = tePsWrite(m_devHandle, psKey, (buflen + 1) / 2, (uint16*)buffer);
	if (ret != TE_OK)
		ERROROUT(WM_DEV_ERROR, ERROR_WRITE_FIXPARAM, ret, -7);
	else
		MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_WRITE_FIXPARAM, (LPARAM)0);

out:
	if(opened)
		CloseEngine();
	return ret;
}

//  ����
// PC ----> check deviceinfo
//   <----- check LIS25 PASS/FAIL
//   <----- check DM430 PASS/FAIL
//   <----- check BTADDR xx:xx
//   <----- check ENDCHECK        ����
enum {
	CHECK_ST_INFO, CHECK_ST_INFO_WAIT,             // check deviceinfo

	CHECK_ST_RECSTART, CHECK_ST_RECDAT, CHECK_ST_RECSTOP, CHECK_ST_RECFIN,
};

// д����ȴ�����
int CDeviceCtrl::teWriteAndRead(void *cmdbuf, int cmdlen, char *readresp)
{
	int ret;
	uint8 channel;
	UINT32 g_rdCount = 0, tickStart = 0;

	if (m_devHandle == 0)
		return -1;

	ret = teAppWrite(m_devHandle, 0, (const uint16*)cmdbuf, cmdlen / 2);
	if (ret != TE_OK)
		return -2;

	if (readresp) {
		UINT16 rdbuf[PSKEY_BUFFER_LEN/2], rdlen;
		memset(rdbuf, 0, sizeof(rdbuf));
		ret = teAppRead(m_devHandle, &channel, rdbuf, PSKEY_BUFFER_LEN / 2, &rdlen, 2000);
		if (ret != TE_OK)
			return -3;

		if (strstr((char*)rdbuf, readresp) < 0)
			return -11;
	}

	return 0;
}

int CDeviceCtrl::CheckDevice(int bCloseEng)
{
	int ret = -11, successEnd = 0;
	CString sText;
	UINT16 *cmdbuf, cmdlen;
	uint8 channel;
	UINT16 *rdbuf, rdlen;
	INT32 g_rdCount = 0;
	
	m_curTick = ::GetTickCount();
	m_checkStatus = 0;
	if (m_devHandle == 0) {
		if (OpenEngine() < 0) {
			MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_OPEN_ENGINE, (LPARAM)0);
			return -1;
		}
	}
	
	MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_OPEN, (LPARAM)0);
	m_curTick = ::GetTickCount();
	while (1) {
		if (m_bExit) break;

		switch (m_checkStatus) {
		case CHECK_ST_INFO:
			cmdbuf = (UINT16*)GetMsgBuffer();
			cmdlen = sprintf_s((char*)cmdbuf, PSKEY_BUFFER_LEN, "check DEVICEINFO");
			if(0 == teWriteAndRead(cmdbuf, cmdlen, "checkresp DEVICEINFO")) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_SUCC, (LPARAM)cmdbuf);
				m_checkStatus = CHECK_ST_INFO_WAIT;
				m_curTick = ::GetTickCount();
			}
			else {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_COMMU_FAIL, (LPARAM)cmdbuf);
				TRACE("ERROR Send Cmd\n");
				Sleep(1000);
				break;
			}
			break;

		case CHECK_ST_INFO_WAIT:
			rdbuf = (UINT16*)GetMsgBuffer();
			ret = teAppRead(m_devHandle, &channel, rdbuf, PSKEY_BUFFER_LEN/2, &rdlen, 500);
			if (ret == TE_OK) {
				m_curTick = ::GetTickCount();

				if (strstr((char*)rdbuf, "check ENDCHECK")) {   // ����
					successEnd = 1;
					break;
				}

				TRACE("CHECK %d:%s\n", rdlen, rdbuf);
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_READ, (LPARAM)rdbuf);

#if 0
				if (strstr((char*)rdbuf, "check") == NULL) {    // �ٶȲ��ԣ�һ��һ�ص�����£����Դﵽ80K BYTE
					static UINT16 g_rdNo = 0;

					CFile h;
					if (TRUE == h.Open("Recv.dat", CFile::modeReadWrite | CFile::modeNoTruncate | CFile::modeCreate)) {
						h.SeekToEnd();
						h.Write(rdbuf, rdlen * 2);
						h.Close();
					}

					TRACE("g_rdNo=0x%x REcv[%d]%d Cnt=0x%x Start: 0x%04x (%d)\n", g_rdNo,
						(::GetTickCount()- m_curTick)/1000,rdlen, g_rdCount, rdbuf[0], rdbuf[0]);
					g_rdCount += rdlen * 2;
					for (int i = 0; i < rdlen; i++) {
						if (rdbuf[i] == g_rdNo)
							g_rdNo += 1;
						else {
							TRACE("Error No i=%d(offset=0x%x) %d(0x%x --->%d(0x%x\n", i, g_rdCount-rdlen * 2 +i, g_rdNo, g_rdNo, rdbuf[i], rdbuf[i]);
							g_rdNo = rdbuf[i] + 1;
						}
					}

					cmdbuf = (UINT16*)GetMsgBuffer();
					cmdlen = sprintf_s((char*)cmdbuf, PSKEY_BUFFER_LEN, "check ACCEPT");
					teAppWrite(m_devHandle, 0, cmdbuf, cmdlen / 2);
				}
#endif
			}
			break;

		default:
			break;

		}

		if (successEnd) {     // ������
			ret = 0;
			break;
		}

		// ��ʱ�����
		if ((::GetTickCount() - m_curTick) > (5 * 1000)){
			MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, RERROT_COMMU_TIMEOUT, (LPARAM)0);
			TRACE("Time out\n");
			ret = -110;
			break;
		}
		Sleep(1);
	}

	if (bCloseEng)
		CloseEngine();

	return ret;
}

int CDeviceCtrl::Recording(int mic, int sec, int bCloseEng)
{
	int ret = -2, successEnd = 0;
	unsigned char channel;
	UINT16 *cmdbuf, cmdlen;
	UINT16 *rdbuf, rdlen;
	UINT32 datacnt = 0;

	// OPened
	if (m_devHandle == 0) {
		if (OpenEngine() < 0) {
			return -1;
		}
	}

	TRACE("LINE:%d\n", __LINE__);
	m_checkStatus = CHECK_ST_RECSTART;
	m_curTick = ::GetTickCount();
	while (1) {
		if (m_bExit) break;

		switch (m_checkStatus) {
		case CHECK_ST_RECSTART:
			cmdbuf = (UINT16*)GetMsgBuffer();
			cmdlen = sprintf_s((char*)cmdbuf, PSKEY_BUFFER_LEN, "check STARTRECORD%d", mic);
			if(teWriteAndRead(cmdbuf, cmdlen, "checkresp STARTRECORD") == 0) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_SUCC, (LPARAM)cmdbuf);
				m_checkStatus = CHECK_ST_RECDAT;
				m_curTick = ::GetTickCount();
				datacnt = 0;
				DeleteFile("Recv.g722");
			}
			else {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_COMMU_FAIL, (LPARAM)cmdbuf);
				TRACE("ERROR Send Cmd\n");
				Sleep(1000);
				break;
			}
			break;
		
		case CHECK_ST_RECDAT:
			rdbuf = (UINT16*)GetMsgBuffer();
			ret = teAppRead(m_devHandle, &channel, rdbuf, PSKEY_BUFFER_LEN / 2, &rdlen, 1000);
			if (ret == TE_OK) {
					CFile h;
					if (TRUE == h.Open("Recv.g722", CFile::modeReadWrite | CFile::modeNoTruncate | CFile::modeCreate)) {
						h.SeekToEnd();
						h.Write(rdbuf, rdlen * 2);
						h.Close();
						datacnt += rdlen * 2;
					}

					cmdbuf = (UINT16*)GetMsgBuffer();
					cmdlen = sprintf_s((char*)cmdbuf, PSKEY_BUFFER_LEN, "check RECVREC");
					teAppWrite(m_devHandle, 0, cmdbuf, cmdlen / 2);

				//	TRACE("Recv Audio:%d/%d %s\n", rdlen*2, datacnt,(char*)rdbuf);
					m_curTick = ::GetTickCount();
					MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_READ_RECORD, (LPARAM)datacnt);

					if(datacnt >= (UINT32)(2000* sec))
						m_checkStatus = CHECK_ST_RECSTOP;

			}
			else
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_READ_RECORD, (LPARAM)rdbuf);
			break;

		case CHECK_ST_RECSTOP:
			cmdbuf = (UINT16*)GetMsgBuffer();
			cmdlen = sprintf_s((char*)cmdbuf, PSKEY_BUFFER_LEN, "check STOPRECORD%d", mic);
			
			if (teWriteAndRead(cmdbuf, cmdlen, "checkresp STOPRECORD") == 0) {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_REPORT, REPORT_COMMU_SUCC, (LPARAM)cmdbuf);
				m_checkStatus = CHECK_ST_RECFIN;
				m_curTick = ::GetTickCount();
			}
			else {
				MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, ERROR_COMMU_FAIL, (LPARAM)cmdbuf);
				TRACE("ERROR Send Cmd\n");
				Sleep(200);
				break;
			}
			break;
		case CHECK_ST_RECFIN:
			successEnd = 1;       // Finish, end for next break
			break;
		default:
			break;
		}

		if (1 == successEnd) {
			ret = 0;
			break;
		}

		if (::GetTickCount() - m_curTick > 5 * 1000) {      // N����ղ��������˳�
			ret = -2;
			break;
		}
	}

	// Close
	if (bCloseEng)
		CloseEngine();

	return ret;
}

double ReadFreqOffsetHz(int value)
{

	return 2000.0;
}

int CDeviceCtrl::CrystalTrimming(int value)
{
	// radiotestTxStart transmit settings
	const uint32 KEY_WRITE_BUFFER_LEN = 128;

	const uint16 TX_FREQ = 2441;
	const uint16 INTPA = 50;
	const uint16 EXTPA = 255;
	const uint16 MODULATION = 0;

	const uint16 MAX_TRIM_STEPS = 32;
	const uint16 MAX_XTAL_LCAP = 31;
	const uint16 MIN_XTAL_LCAP = 0;
	const int16 MAX_XTAL_TRIM = 15;
	const int16 MIN_XTAL_TRIM = -16;

	// modify values for MAX_COARSE_OFFSET_HZ and MAX_FINE_OFFSET_HZ
	// to 7500.0 and -7500.0 if devices cannot be trimmed
	const double MAX_COARSE_OFFSET_HZ = 5000.0;
	const double MIN_COARSE_OFFSET_HZ = -5000.0;
	const double MAX_FINE_OFFSET_HZ = 1000.0;
	const double MIN_FINE_OFFSET_HZ = -1000.0;

	if (OpenEngine() < 0)
		return -2;

	UINT32  handle = m_devHandle;

	int16 mibValue = 0;
	uint16 xtalLoadCap = 16;      	// xtalLoadCap has range 0 - 31 so start mid-range
	int16 xtalTrim = 0;	// xtalTrim has range -16 to 15 so start mid-range
	int32 success = radiotestTxstart(handle, TX_FREQ, INTPA, EXTPA,	MODULATION);
	if (success != TE_OK)	{
		TRACE("radiotestTxstart returned error\n");
	}
	else { 	// Init fine trim value prior to coarse value trim
		success = teMcSetXtalFreqTrim(handle,	(uint16)(xtalTrim), &mibValue);
		if (success != TE_OK) {
			TRACE("teMcSetXtalFreqTrim returned error\n");
		}
	}
	
	double measuredFreqOffset = 0.0;
	bool xtalLoadCapFound = false;
	bool xtalTrimFound = false;

	// find xtalLoadCap value for device
	if (success == TE_OK) {
		for (uint16 i = 0; i < MAX_TRIM_STEPS; ++i) {
			success = teMcSetXtalLoadCapacitance(handle, xtalLoadCap);
			if (success != TE_OK)
			{
				TRACE("teMcSetXtalLoadCapacitance returned error\n");
				break;
			}
			else {
				measuredFreqOffset = ReadFreqOffsetHz(TX_FREQ);
				if ((measuredFreqOffset < MAX_COARSE_OFFSET_HZ) && (measuredFreqOffset > MIN_COARSE_OFFSET_HZ)) {
					// found correct XtalLoadCapacitance value
					xtalLoadCapFound = true;
					break;
				}
				else {
					if (measuredFreqOffset > 0.0) {
						++xtalLoadCap;
					}
					else {
						--xtalLoadCap;
					}
				}

				if ((xtalLoadCap > MAX_XTAL_LCAP) || (xtalLoadCap < MIN_XTAL_LCAP)) {
					// xtalLoadCap value out of range, correct value not	found
					TRACE("xtalLoadCap value not found\n");
					break;
				}
			}
		}
	}
	
	// find Xtal Trim for device
	if (success == TE_OK && xtalLoadCapFound) {
		for (uint16 i = 0; i < MAX_TRIM_STEPS; ++i)	{
			success = teMcSetXtalFreqTrim(handle, (uint16)(xtalTrim), &mibValue);
			if (success != TE_OK)		{
				TRACE("teMcSetXtalFreqTrim returned error\n");
				break;
			}
			else	{
				measuredFreqOffset = ReadFreqOffsetHz(TX_FREQ);
				if ((measuredFreqOffset < MAX_FINE_OFFSET_HZ) &&	(measuredFreqOffset > MIN_FINE_OFFSET_HZ))
				{			// correct xtalTrim value found
					xtalTrimFound = true;
					break;
				}
				else	{
					if (measuredFreqOffset > 0.0)		{
						--xtalTrim;
					}
					else	{
						++xtalTrim;
					}
				}
				
				if ((xtalTrim > MAX_XTAL_TRIM) || (xtalTrim < MIN_XTAL_TRIM))
				{		// xtal trim value out of range, correct value notfound
					TRACE("xtal trim value not found\n");
					break;
				}
			}
		}
	}

	// stop the device from transmitting
	radiotestPause(handle);
	// write xtal value to device
	if (success == TE_OK && xtalLoadCapFound && xtalTrimFound)	{
		uint16 unused = 0;
		TRACE("xtalLoadCap and xtalTrim found\n");

		// Initialise the configuration cache
		success = teConfigCacheInit(handle, CFG_DB_PARAM);
		if (success != TE_OK)		{
			TRACE("teConfigCacheInit returned error\n");
		}
		else	{
			// Read the configuration into the cache from the device
			success = teConfigCacheRead(handle, NULL, unused);
		}
			
		if (success != TE_OK)	{
			TRACE("teConfigCacheRead returned error\n");
		}
		else	{
			// write updated XTAL Load Cap value
			char xtalCapWriteValue[KEY_WRITE_BUFFER_LEN];
			_snprintf_s(xtalCapWriteValue, KEY_WRITE_BUFFER_LEN, "%d",		xtalLoadCap);
			success = teConfigCacheWriteItem(handle,"curator15:XtalLoadCapacitance", xtalCapWriteValue);
		}

		if (success != TE_OK)	{
			TRACE("Failed to write XtalLoadCapacitance value to config	cache\n");
		}
		else	{
			// Write updated XTAL trim value
			char xtalTrimWriteValue[KEY_WRITE_BUFFER_LEN];
			_snprintf_s(xtalTrimWriteValue, KEY_WRITE_BUFFER_LEN, "%d", xtalTrim);
			success = teConfigCacheWriteItem(handle,"curator15:XtalFreqTrim", xtalTrimWriteValue);
		}

		if (success != TE_OK)	{
			TRACE("failed to write XtalFreqTrim value to config cache\n");
		}
		else	{
			// Write the configuration cache to the device
			success = teConfigCacheWrite(handle, NULL, unused);
		}

		if (success != TE_OK)	{
			TRACE("teConfigCacheWrite returned error\n");
		}
		else	{
			TRACE("XtalCap and XtalTrim written to device\n");
		}
	}
		
	closeTestEngine(handle);


	return 0;
}


int CDeviceCtrl::OpenEngine(void)
{
	for (int i = 0; i < 100; i++) {
		m_devHandle = openTestEngine(USBDBG, "1", 0, 5000, 1000);
		if (m_devHandle > 0)
			break;

		TRACE("openTestEngine FAILE,loops=%d\n", i);
		if (m_bExit == TRUE)
			return -2;
		Sleep(1000);
	}

	if (m_devHandle > 0)
		return 0;

	MESSAGE2DIALOG(m_hWnd, WM_DEV_ERROR, (WPARAM)ERROR_OPEN_ENGINE, (LPARAM)0);
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

