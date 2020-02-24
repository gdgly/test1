#include "stdafx.h"
#include "CtrlDevice.h"


#define  MESSAGE2DIALOG         PostMessage

CCtrlDevice::CCtrlDevice()
{
	m_bEnableErase = FALSE;

	m_secRecord = 6;
	m_secTimeout = 6;
	m_SensorDiff = 200;


	m_curTick = 0;
	m_devHandle = 0;
	memset(m_bdAddr, 0, sizeof(m_bdAddr));
	m_btWrite = FALSE;
	m_sFlashImage.Empty();
	m_iThreadFunc = THREAD_NONE;
	m_ctrlThread = INVALID_HANDLE_VALUE;

	m_checkItems    = 0;

	m_comBaud       = 115200;
	m_comPort       = 1;
	m_sID.Empty();
}


CCtrlDevice::~CCtrlDevice()
{
}

int CCtrlDevice::DeviceCtrlProc(void *param)                //相当于于一个main
{
	CCtrlDevice *pdev = (CCtrlDevice*)param;

	pdev->RuningProc();

	pdev->RuningEnd();
	return 0;
}

void CCtrlDevice::RuningEnd(void)
{

	m_ctrlThread = INVALID_HANDLE_VALUE;
}

int CCtrlDevice::Start(HWND hWnd)
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
		m_ctrlThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CCtrlDevice::DeviceCtrlProc,      //创建线程
			this, 0, &dwID);
	}

	return 0;
}


int CCtrlDevice::Stop(void)
{
	m_bExit = TRUE;
	return 0;
}

BYTE * CCtrlDevice::GetMsgBuffer()
{
	BYTE *ptr = m_msgData[m_msgNo];

	memset(ptr, 0, MSG_BUFFER_LEN);
	m_msgNo += 1;
	if (m_msgNo >= MSG_BUFFER_COUNT)
		m_msgNo = 0;

	return ptr;
}

int CCtrlDevice::SendText(UINT wParam, CString sText)
{
	BYTE *pbuf = GetMsgBuffer();
	int len = sText.GetLength();

	if(len > MSG_BUFFER_LEN) len = MSG_BUFFER_LEN;

	memcpy(pbuf, sText.GetBuffer(), len);
	sText.ReleaseBuffer();
	
	MESSAGE2DIALOG(m_hWnd, WM_DEV_STRING, wParam, (LPARAM)pbuf);
	return 0;
}

int CCtrlDevice::SendReportText(CString sText)
{
	return SendText(MSG_T_REPORT, sText);
}

int CCtrlDevice::SendDevText(CString sText)
{
//	TRACE("DEVTXT:%s", sText);
	return SendText(MSG_T_DEV, sText);
}

int CCtrlDevice::RuningProc(void)
{
	int ret = 0;

	SendReportText("ThreadStart");	

	if ((m_iThreadFunc & THREAD_COM)) {
		if ((ret=RuningComStart(m_sID)) < 0)
			goto out;
	}
		

out:

	if (m_bExit == TRUE)
		SendReportText("StopByUser");
	
	SendReportText((0==ret) ? "EndSucc" : "EndFail");

	return 0;
}



int CCtrlDevice::ComDataSend(BYTE *buffer, UINT count)        // 返回剩下的数据长度(非整行）
{	
	UINT i, start = 0, lineByte;

	for(i = 0; i < count; i++) {
		if(buffer[i] == '\n') {
			BYTE *pbuf = GetMsgBuffer();
			lineByte = i - start;
			if(lineByte > MSG_BUFFER_LEN)
				lineByte = MSG_BUFFER_LEN - 1;

			memcpy(pbuf, &buffer[start], lineByte);
			SendDevText(CString(pbuf));
			start = i + (((i - start) <= MSG_BUFFER_LEN) ? 1 : 0);
		}
	}

	if(start == 0)
		return count;      // 一行都没有找到

	if(start < count && start > 0) {
		count = count - start;
		memcpy(&buffer[0], &buffer[start], count);
		return count;
	}

	return 0;
}

int CCtrlDevice::RuningComStart(CString sFile)
{
	int ret = 0, rdsize, start_loops = 0;
	unsigned int lastCheckNo = 0, checkCnt = 0, bWaitSend = 0;
	CtrlIPtr pCtrlI = NULL;
	CFile hFile;
	CString sText, tmp;
	CTime tNow, tStart;


	hFile.m_hFile = INVALID_HANDLE_VALUE;
	if (m_hCom.InitComm(m_comPort, DEIGHT, SONE, PNONE, m_comBaud) == FALSE) {
		sText.Format("FailOpenCom port=%d", m_comPort);
		SendReportText(sText);
		return -1;
	}
	TRACE("Open Com success\n");

	::CreateDirectory("logs", NULL);
	tStart = tNow = CTime::GetCurrentTime();
	tmp.Format("logs/log-%04d%02d%02d-%s.txt", tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), m_sID);
	if (hFile.Open(tmp, CFile::modeWrite | CFile::typeBinary | CFile::modeCreate) == FALSE) {
		sText.Format("OpenLogFail:%s", tmp);
		SendReportText(sText);
		ret = -3;
		goto out;
	}

	hFile.SeekToBegin();

	// 发送命令处理，仅查找第一条是否需要发送
	m_hCom.ClearReceiveBuffer();
	m_rdLen = 0;
	while (m_bExit == FALSE) {
		pCtrlI = &gItemArr[0];

		if(pCtrlI->command) {
			m_hCom.WriteCommBlock((LPSTR)pCtrlI->command, strlen(pCtrlI->command));
			pCtrlI->cmdsended = 3;
		}
		
		break;
	}

	checkCnt = m_checkItems;
	// 读取需要的返回串口数据
	while (m_bExit == FALSE) {
		int less = COM_BUFFER_LEN - m_rdLen;

		rdsize = m_hCom.ReadCommBlock((LPSTR)&m_rdBuff[m_rdLen], less);
		if(rdsize > 0) {
			hFile.Write(&m_rdBuff[m_rdLen], rdsize);         // 写日志文件
	//		TRACE("Readsize=%d\n", rdsize);
			m_rdLen += rdsize;
			
			m_rdLen = ComDataSend(m_rdBuff, m_rdLen);
		}

		Sleep(1);
		// 计算最后正在检测的条目
		pCtrlI = NULL;
		for(UINT i = lastCheckNo; i < checkCnt; i++) {
			pCtrlI = &gItemArr[i];
			if(pCtrlI->checked == ITEM_CHECK_1AND2)
				lastCheckNo = i;
			else
				break;
		}

		if(lastCheckNo == (checkCnt-1)) {      // 已经检测完成
			SendReportText("CheckAllItem End");
			ret = 0;
			break;
		}

		if(pCtrlI && pCtrlI->checked == ITEM_CHECK_NONE) {     // 本条目没有检测
			if(pCtrlI->command && 0 == pCtrlI->cmdsended) {    // 需要送命令
				bWaitSend = 3;
			}
		}

		if(bWaitSend <= 0)
			continue;

		// 发送自动测试命令，读取需要的返回串口数据
		if(pCtrlI->command && pCtrlI->cmdsended < 3) {
			tNow = CTime::GetCurrentTime();
			CTimeSpan tSpan = tNow - tStart;
			if(tSpan.GetSeconds() >= (LONG)bWaitSend) {
				TRACE("COMMAND:%s", pCtrlI->command);
				m_hCom.WriteCommBlock((LPSTR)pCtrlI->command, strlen(pCtrlI->command));
				tStart = tNow;
				pCtrlI->cmdsended += 1;
				bWaitSend = pCtrlI->waitsend;
				if(bWaitSend < 1) bWaitSend = 3;

				sText.Format("WaitUser row=%d", pCtrlI->nRow);
				SendReportText(sText);
			}
		}
	}
	
out:
	if (hFile.m_hFile != INVALID_HANDLE_VALUE)
		hFile.Close();

	m_hCom.EndComm();
	return ret;
}
