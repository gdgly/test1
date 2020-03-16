#include "stdafx.h"
#include "ContrlComm_bluetooth.h"

//extern CString g_hardware_version,g_out_str;
CContrlComm_bluetooth::CContrlComm_bluetooth(void)
	:portCom(0),bufReaded(0),current_mA(0),bitChange(FALSE)
{
	chagr_curret_mA = 0;

	memset(devAddr, 0, sizeof(devAddr));
	memset(devSN, 0, sizeof(devSN));
	memset(firmVer, 0, sizeof(firmVer));

#ifdef COM_DEBUG_LOG
	m_hLogFile.m_hFile = INVALID_HANDLE_VALUE;
#endif
}

CContrlComm_bluetooth::~CContrlComm_bluetooth(void)
{
#ifdef COM_DEBUG_LOG
	if(INVALID_HANDLE_VALUE != m_hLogFile.m_hFile)
		m_hLogFile.Close(), m_hLogFile.m_hFile = INVALID_HANDLE_VALUE;
#endif
}

#ifdef COM_DEBUG_LOG
BOOL CContrlComm_bluetooth::InitComm(unsigned short wPort, unsigned char bData, 
			unsigned char bStop, unsigned char bParity,	DWORD wBaud)
{
	CString sFname;
	
	sFname.Format("log\\logCom%d.txt", wPort);

	if(m_hLogFile.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		m_hLogFile.SeekToEnd();
		sFname.Format("===============================================\r\n");
		m_hLogFile.Write(sFname, sFname.GetLength());
	}

	return CMyComm::InitComm(wPort, bData, bStop, bParity, wBaud);
}
#endif


// 清除缓冲区中的数据
void CContrlComm_bluetooth::ResetComm()
{
	bufReaded   = 0;
	bitStatus   = 0;
	current_mA  = 0;
	bitChange   = FALSE;
	memset(bufCom, 0, COM_BUF_SIZE);

	CMyComm::ResetComm();
}

int CContrlComm_bluetooth::HardwareVer(CString Sn,CString inputstr)
{
	CString sText,hArdware,sText1;
	
	if(Sn.ReverseFind('-') >= 0)
	{
		hArdware = 	Sn.Mid(Sn.ReverseFind('-') + 1);
		hArdware = hArdware.Left(2);
	}
	sText = inputstr;
	while(sText.Find(",") >= 0)
	{
		sText1 = sText.Left(2);
		sText = sText.Mid(sText.Find(",") + 1);
		if(sText1 == hArdware)
			return TRUE;
	}
	return FALSE;
}

//extern UINT    g_check_only;
int CContrlComm_bluetooth::ParseLine(BYTE *buf, int len)
{
	int offset;
	CString sText(buf);
	BYTE currtbuf[10];

	if(sText.Find("OK") < 0 && sText.Find("End") < 0)
		return -1;
	if(sText.Find("ZHYTEKCurrent1") >= 0){
			memcpy(currtbuf,&buf[sText.Find("=") + 1],6);
			current_mA = atoi((const char *) currtbuf)/1000;
	}
	if(sText.Find("ZHYTEKCurrent2") >= 0){
		memcpy(currtbuf,&buf[sText.Find("=") + 1],6);
		chagr_curret_mA = atoi((const char *) currtbuf)/1000;
	}

	if(sText.Find("Run96M") >= 0)
		bitStatus |= (1<<BIT_SYS_RUN96M);
	if(sText.Find("TouchUp") >= 0)
		bitStatus |= (1<<BIT_KEY_TOUCH_UP);//                // TouchUp OK
	if(sText.Find("TouchDown") >= 0)
		bitStatus |= (1<<BIT_KEY_TOUCH_DOWN); //               // TouchDown OK
	if(sText.Find("CS47L35") >= 0)
		bitStatus |= (1<<BIT_CS47L35);		                  // CS47L35 OK		
	if(sText.Find("NAU88L24") >= 0)
		bitStatus |= (1<<BIT_NAU88L24);
	if(sText.Find("SmartPA") >= 0){
		if(sText.Find(g_out_str) >= 0)
			bitStatus |= (1<<BIT_SPEAKER);   // TEA9896 OK   tfa9890 fls161
	}
	if(sText.Find("TouchInit") >= 0)
		bitStatus |= (1<<BIT_KEY_TOUCH_INIT); //              // TouchInit OK  
	if(sText.Find("KeyUp") >= 0 || sText.Find("KeyDown") >= 0)
		bitStatus |= (1<<BIT_KEY_BOARD); //                  //  KeyUp OK  
	if((offset=sText.Find("BLEADDR:")) >= 0) {      // BLEADDR: 5A:59:CD:CD:CD:CD OK
		memcpy(devAddr, &buf[offset+8], 17);
		bitStatus |= (1<<BIT_ADDR_OK);
	}
	if((offset=sText.Find("Device SN:")) >= 0) {     //Device SN: xxxx OK
		bitStatus |= (1<<BIT_SN_OK);
	}
	if((offset=sText.Find("FWVersion:")) >= 0) {     //FWVersion:0.01.20 OK
		memcpy(firmVer, &buf[offset+10], 7);
		bitStatus |= (1<<BIT_FIRMVER_OK);
	}else {
		//TRACE("LineData:%s\n", sText);
		return -2;
	}

	bitChange = TRUE;

	return 0;
}

int CContrlComm_bluetooth::Parse(void)
{
	int from, start, linelen, getend = 0;
	int i;
	//BYTE commbuf[40];
	start = from = 0;

	if(strstr((char *)bufCom, "Starting")) {
		TRACE("DEV:%s\n", bufCom);
	}

NextLine:
	for(; from < (int)bufReaded; from += 1) {
		// 获取一行数据
		if('\n' == bufCom[from] || '\r' == bufCom[from]) {
			for(i=start; i<from; i++){
				if(bufCom[i] == '\0')
					bufCom[i] = 255;
			}
			bufCom[from] = '\0';
			getend = 1;
			break;
		}
	 }
	
	if(getend) {                       //如果找到换行符
		linelen = from-start;

		
#ifdef COM_DEBUG_LOG
	{
		CString sText("================================================\r\n");
		m_hLogFile.Write(sText, sText.GetLength());
		m_hLogFile.Write(&bufCom[start], linelen);m_hLogFile.Write("\r\n", 2);
		m_hLogFile.Flush();
	}
#endif
		//TRACE("LineData:%d start=%d [%s]\n", linelen, start, (char*)&bufCom[start]);
		if(linelen > 2){
			ParseLine(&bufCom[start], linelen);       //如果第一个字节为/0的话，会被认为是空的buf
		}

		from += 1;
		start = from;
		getend = 0;

		goto NextLine;
	}
	
	// SKIP Start '\0'
	for(; start < (int)bufReaded; start++)               //处理完一段数据后，会让start跳过/0
		if(bufCom[start] != '\0') break;

	// 数据是否为一整行,将剩下的数据放到开始处
	if(start > 0 && start <= (int)(bufReaded)) {
		bufReaded = bufReaded - start;
		memcpy(bufCom, &bufCom[start], bufReaded);
	}
	/*else if(start >= (int)(bufReaded-1))
	bufReaded = 0;*/
	else if(bufReaded >= COM_BUF_SIZE)
		bufReaded = 0;

	/*if(bufReaded > 0)  {
	if(bufCom[0] == '\0')
	TRACE("XXXXXXX:%s\n", &bufCom[1]);*/
	//}

	return 0;
}

int CContrlComm_bluetooth::Process(void)
{
	int rdlen, less = COM_BUF_SIZE - bufReaded;
	int hand_len = 0,offset = 0;

	if((rdlen = ReadCommBlock((LPSTR)&bufCom[bufReaded], less)) <= 0){
		if(rdlen==0)
			return 0;
	}

	// 分析读取到的数据           
	if(rdlen>0){
		bufReaded += rdlen;
//		bufCom[bufReaded] = '\0';

	}
	
	//TRACE("COMDATA %d[%d]:%s\n",portCom, bufReaded, (char*)bufCom);
#ifdef COM_DEBUG_LOG
	CString sText("---------------------------------------------------------------------------\r\n");
	m_hLogFile.Write(sText, sText.GetLength());
	m_hLogFile.Write(bufCom, bufReaded);m_hLogFile.Write("\r\n", 2);
	m_hLogFile.Flush();
#endif

	Parse();
//	com_data_proess(bufCom,(int *)&bufReaded,line_databuf);
	
	return rdlen;
}


int CContrlComm_bluetooth::SendComand(int comand)
{
	const char *Commkey[END_COM] = {"\r\nkey_dbg on\r\n", "\r\ncheckfirm on\r\n", "\r\naud2out on\r\n", "\r\ncontrol_relay(1)\r\n",
		"\r\ncontrol_relay(0)\r\n", "\r\naud2out off\r\n"};

	if(comand < 0 || comand >= END_COM) {
		TRACE("SendCommand, Unknown:%d\n", comand);
		return FALSE;
	}

	TRACE("SendCommand:%s", Commkey[comand]);

	if(WriteCommBlock((LPSTR )Commkey[comand], strlen(Commkey[comand])) == TRUE)
		return TRUE;

	return FALSE;
}
