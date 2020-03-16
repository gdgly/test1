#include "stdafx.h"
#include "ContrlComm_burncheck.h"

//extern CString g_hardware_version,g_out_str;
CContrlComm::CContrlComm(void)
	:portCom(0),bufReaded(0),current_mA(0),bitChange(FALSE)
{
	chagr_curret_mA = 0;
	sleep_curret_uA = 0;

	memset(devAddr, 0, sizeof(devAddr));
	memset(devSN, 0, sizeof(devSN));
	memset(firmVer, 0, sizeof(firmVer));

#ifdef COM_DEBUG_LOG
	m_hLogFile.m_hFile = INVALID_HANDLE_VALUE;
#endif
}


CContrlComm::~CContrlComm(void)
{
#ifdef COM_DEBUG_LOG
	if(INVALID_HANDLE_VALUE != m_hLogFile.m_hFile)
		m_hLogFile.Close(), m_hLogFile.m_hFile = INVALID_HANDLE_VALUE;
#endif
}

#ifdef COM_DEBUG_LOG
BOOL CContrlComm::InitComm(unsigned short wPort, unsigned char bData, 
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


// ����������е�����
void CContrlComm::ResetComm()
{
	bufReaded   = 0;
	bitStatus   = 0;
	current_mA  = 0;
	sleep_curret_uA = 0;
	chagr_curret_mA = 0;
	bitChange   = FALSE;
	memset(bufCom, 0, COM_BUF_SIZE);

	CMyComm::ResetComm();
}

int CContrlComm::HardwareVer(CString Sn,CString inputstr)
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
bool find___str(BYTE *buf)
{
	unsigned char *sn_offset_p;
	int i;
	sn_offset_p = buf;
	for(i=0; i<18; i++){
		if( *(sn_offset_p+i) == '-' ){
			return TRUE;
		}
	}
	return FALSE;
}
//extern UINT    g_check_only;
int CContrlComm::ParseLine(BYTE *buf, int len)
{
	int offset;
	CString sText(buf);
	BYTE currtbuf[10];
	unsigned char c_buf[10];

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
	if(sText.Find("ZHYTEKCurrent3") >= 0){
		memcpy(currtbuf,&buf[sText.Find("=") + 1],6);
		sleep_curret_uA = atoi((const char *) currtbuf);
	}
	if( ( offset=sText.Find("languages") ) >= 0 ){
		memset(c_buf, 0, 10);
		memcpy(c_buf, &buf[offset+10], 5);
		g_out_languages_str.Format("%s", (char*)c_buf);
		bitStatus |= (1<<BIT_LANGUAGES_STR);
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
		memset(devSN, 0, 20);
		if( !find___str(&buf[offset+10]) ){//sn�Ų���-�� ��18�ֽڵ�sn��
			memcpy(devSN, &buf[offset+10], 18);
		}else{
			memcpy(devSN, &buf[offset+10], 19);
		}
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

int CContrlComm::Parse(void)
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
		// ��ȡһ������
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
	
	if(getend) {                       //����ҵ����з�
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
			ParseLine(&bufCom[start], linelen);       //�����һ���ֽ�Ϊ/0�Ļ����ᱻ��Ϊ�ǿյ�buf
		}

		from += 1;
		start = from;
		getend = 0;

		goto NextLine;
	}
	
	// SKIP Start '\0'
	for(; start < (int)bufReaded; start++)               //������һ�����ݺ󣬻���start����/0
		if(bufCom[start] != '\0') break;

	// �����Ƿ�Ϊһ����,��ʣ�µ����ݷŵ���ʼ��
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

int CContrlComm::Process(void)
{
	int rdlen, less = COM_BUF_SIZE - bufReaded;
	int hand_len = 0,offset = 0;

	if((rdlen = ReadCommBlock((LPSTR)&bufCom[bufReaded], less)) <= 0){
		if(rdlen==0)
			return 0;
	}

	// ������ȡ��������           
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


int CContrlComm::SendComand(int comand)
{
	const char *Commkey[END_COM] = {"\r\nkey_dbg on\r\n", "\r\ncheckfirm on\r\n", "\r\naud2out on\r\n", "\r\ncontrol_relay(1)\r\n",
		"\r\ncontrol_relay(0)\r\n", "\r\naud2out off\r\n", "\r\ncontrol_relay(2)\r\n", "\r\ncontrol_relay(3)\r\n", "\r\nsleep\r\n", 
		"\r\necho on > /sys/bus/platform/devices/sm5007-charger/usb_onoff\r\n","\r\ncontrol_relay(4)\r\n","\r\ncontrol_relay(5)\r\n"};

	if(comand < 0 || comand >= END_COM) {
		TRACE("SendCommand, Unknown:%d\n", comand);
		return FALSE;
	}

	TRACE("SendCommand:%s", Commkey[comand]);

	if(WriteCommBlock((LPSTR )Commkey[comand], strlen(Commkey[comand])) == TRUE)
		return TRUE;

	return FALSE;
}
