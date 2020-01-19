#include "stdafx.h"
#include "ContrlComm.h"
#include "protocol.h"
#include <math.h>
#include "box_audioDlg.h"


#define SEND_USER_MSG(wnd,m,w,l)    ::SendMessage((wnd), (m), (w), (l));

int max_count(INT16 *pcmdata,  long nmemb)
{	
	int i, count = 0;
	//max =  pcmdata[0];
	int state = -1;
	for(i = 1; i < nmemb-1; i++)
	{
			if(pcmdata[i] >0)
			{
				if(pcmdata[i]>pcmdata[i-1] && pcmdata[i]>pcmdata[i+1])
				{
		
					if(state == -1)
					{
						state = 1;
						//fprintf(fp , "%d\n", pcmdata[i]);
					}
					else
					{
						continue;
					}
				}	
			}
		if(pcmdata[i] < 0)
		{
			if(pcmdata[i]<pcmdata[i-1] && pcmdata[i]<pcmdata[i+1])
			{
			
				if(state == 1)
				{
					//fprintf(fp , "%d\n", pcmdata[i]);
					count++;
					state = -1;
				}
				else
				{
					continue;
				}
			}	
		}
		
	}
	
	return count;
}
extern unsigned short g_command;
int CContrlComm::WorkerThreadProc(void *param)
{
	INT rdsize, readed=0, pktdatlen;
	
	int olen, result, getsn_succ_end = 0,len;
	int data_sec_cnt = 4000;             // 数据 时间长度
	unsigned short cmdseq = 0;
	unsigned char cmd_dat[26];
	CString sText;
	CTime tNow = CTime::GetCurrentTime();
	CTime tEnd;
	CTimeSpan timeSpan;
	NetProto cmdPkt;
	CFile hAud2;
	CString m_sFilename21;
	CContrlComm *comInfo = (CContrlComm*)param;
	CCurlCtrl *mesptr = &comInfo->mesInfo;
	unsigned char *p_temp;
	unsigned short command = g_command;

	comInfo->m_iAudInfo.recvLen = 0;
	comInfo->m_iAudInfo.audMsec = 0;
	comInfo->m_sFilename.Empty();

	comInfo->m_iAudInfo.recvLen2 = 0;
	comInfo->m_iAudInfo.audMsec2 = 0;
	comInfo->m_sFilename2.Empty();


	len=pto_GenerateRaw(&cmdPkt, 0x7, 1234, comInfo->Mesresring, comInfo->file1db);

	
	comInfo->WriteCommBlock((LPSTR)&cmdPkt, len);
	
	while(1){
		rdsize = comInfo->ReadCommBlock((LPSTR)&comInfo->pRdbuf[readed], 2000-readed);
		if(rdsize > 0) {
			readed += rdsize;

		}
		if(readed > 8) {
			olen = 0;
			memset(&cmdPkt, 0, sizeof(cmdPkt));
			result = pto_Parse(comInfo->pRdbuf, readed, &cmdPkt, &olen);
			if(result == 0){
				comInfo->pRdbuf[readed]=0;
				return 0;//common 传输成功
			}
		}
		tEnd = CTime::GetCurrentTime();
		timeSpan = tEnd - tNow;
		int nTSeconds = timeSpan.GetTotalSeconds();
		if(nTSeconds > 2){
			comInfo->pRdbuf[readed]=0;
			return 1;//common 接收超时；
		}
	}

	return 0;
}

void CContrlComm::AudioDataMessage(void)
{
	switch(m_iAudInfo.audType) {
	case AUDIO_T_G722:
		m_iAudInfo.audMsec = (m_iAudInfo.recvLen * 1000) / (16000 * 2 / 16);     // ms
		break;
	case AUDIO_T_PCM_MONO:
		m_iAudInfo.audMsec = (m_iAudInfo.recvLen * 1000) / (16000 * 2);
		break;
	case AUDIO_T_PCM_STERO:
	case AUDIO_T_PCM_QIMIXING:
		m_iAudInfo.audMsec = (m_iAudInfo.recvLen * 1000) / (16000 * 2);
		m_iAudInfo.audMsec2 = (m_iAudInfo.recvLen2 * 1000) / (16000 * 2);
		break;
	}

	SEND_USER_MSG(m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_AUDIO_LENGTH, (LPARAM)m_iAudInfo.audMsec);
}

CContrlComm::CContrlComm(void)
	:portCom(0),bufReaded(0),current_mA(0),bitChange(FALSE)
{
	m_hComThread = 0;
	baudCom = 1000000;

	m_iAudInfo.recvLen = 0;
	m_iAudInfo.pAudBuf = new BYTE[1<<20];      // 申请1M内存

	m_iAudInfo.recvLen2 = 0;
	m_iAudInfo.pAudBuf2 = new BYTE[1<<20];      // 申请1M内存

	m_sFilename.Empty();
	firsts = 0;
	seconds = 0;
}


CContrlComm::~CContrlComm(void)
{
	delete m_iAudInfo.pAudBuf, m_iAudInfo.pAudBuf = NULL;
	delete m_iAudInfo.pAudBuf2, m_iAudInfo.pAudBuf2 = NULL;
}

// 清除缓冲区中的数据
void CContrlComm::ResetComm()
{
	bufReaded   = 0;
	bitStatus   = 0;
	current_mA  = 0;
	bitChange   = FALSE;
	memset(bufCom, 0, COM_BUF_SIZE);

	CMyComm::ResetComm();
}


int CContrlComm::ParseLine(BYTE *buf, int len)
{
	CString sText(buf);

	if(sText.Find("OK") < 0)
		return -1;

	if(sText.Find("Run96M") >= 0)
		bitStatus |= (1<<BIT_SYS_RUN96M);
	if(sText.Find("TouchUp") >= 0)
		bitStatus |= (1<<BIT_KEY_TOUCH_UP);//                // TouchUp OK
	if(sText.Find("TouchDown") >= 0)
		bitStatus |= (1<<BIT_KEY_TOUCH_DOWN); //               // TouchDown OK
	if(sText.Find("CS47L35") >= 0)
		bitStatus |= (1<<BIT_CS47L35);		                  // CS47L35 OK												
	if(sText.Find("TFA9896") >= 0)
		bitStatus |= (1<<BIT_SPEAKER); //                      // TEA9896 OK
	if(sText.Find("TouchInit") >= 0)
		bitStatus |= (1<<BIT_KEY_TOUCH_INIT); //              // TouchInit OK  
	else {
		TRACE("LineData:%s\n", sText);
		return -2;
	}

	bitChange = TRUE;

	return 0;
}

int CContrlComm::Parse(void)
{
	int from, start, linelen, getend = 0;

	start = from = 0;
	for(from = 0; from < (int)bufReaded; from += 1) {
		// 获取一行数据
		if('\n' == bufCom[from] || '\r' == bufCom[from]) {
			bufCom[from] = '\0';
			getend = 1;
		}

		if(getend) {                       //如果找到换行符
			linelen = from-start;

			if(linelen > 2){
				TRACE("bufCom:%s\n",bufCom);
				ParseLine(&bufCom[start], linelen);
			}


			start = from+1;
			getend = 0;
		}
	}

	// 数据是否为一整行,将剩下的数据放到开始处
	if(start > 0 && start < (int)bufReaded) {
		bufReaded = bufReaded - start;
		memcpy(&bufCom[0], &bufCom[start], bufReaded);
	}

	return 0;
}


UINT CContrlComm::StartComThread(void)
{
	DWORD idThread = 0;

	if(m_hComThread) {
		AfxMessageBox("已经启动");
		return 0;
	}
	
	m_sMac.Empty();
	m_sSN.Empty();	
	m_iTFA8686val = 0;
	m_start_resp = m_stop_resp = 0;       // 发送反馈命令

	m_iExitThread = 0;
	m_hComThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CContrlComm::WorkerThreadProc, this, 0, &idThread);

	return 0;
}
