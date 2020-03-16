/////////////////////////////////////////////////////////////////////
//
// MyComm.cpp: implementation of the CMyComm class.
//
// Author:		YuanYiMin
//
// Date:		2005-06-27
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "mycomm.h"
#include "conio.h"
#include "string.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BYTE ComData[4]={DEIGHT, DSEVEN, DSIX, DFIVE};
BYTE ComStop[2]={SONE, STWO};
BYTE ComParify[3]={PEVEN, PODD, PNONE};
DWORD ComBaud[10] = {115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 600, 300};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyComm::CMyComm()
{
	idComDev = 0;
	fConnected = FALSE ;
}

CMyComm::~CMyComm()
{
	EndComm();
}

BOOL CMyComm::InitComm(unsigned short wPort, 
						unsigned char bData, 
						unsigned char bStop, 
						unsigned char bParity,
						DWORD wBaud)
{

	char       szPort[ 15 ];
	BOOL       fRetVal;
	CString    szStr;

	EndComm();

	idComDev = 0;
	fConnected = FALSE;
	m_wPort = wPort;		// comm port , 0 = COM1

	switch (bParity)
	{
		case PEVEN:
			m_bParity = EVENPARITY;
			break;
		case PODD:
			m_bParity = ODDPARITY;
			break;
		case PNONE:
			m_bParity = NOPARITY;
			break;
		default:					//case PODD:
			m_bParity = ODDPARITY;
			break;
	}

	switch (bStop)
	{
		case SONE:
			m_bStop = ONESTOPBIT;
			break;
		case STWO:
			m_bStop = TWOSTOPBITS;
			break;
		default:						//case SONE:
			m_bStop = ONESTOPBIT;
			break;
	}

	switch (wBaud)
	{
		case 1000000:
		case 921600:
		case 115200:
		case 57600:
		case 38400:
		case 19200:
		case 9600:
		case 4800:
		case 2400:
		case 1200:
		case 600:
		case 300:
			m_wBaud = wBaud;
			break;
		default:					//case 38400:
			m_wBaud = 38400;
			break;
	}	

	switch (bData)
	{
		case DEIGHT:
			m_bData = 8;
			break;
		case DSEVEN:
			m_bData = 7;
			break;
		case DSIX:
			m_bData = 6;
			break;
		case DFIVE:
			m_bData = 5;
			break;
		default:					//case DEIGHT:
			m_bData = 8;
			break;
	}


    if (m_wPort > 9)
            wsprintf(szPort, "\\\\.\\COM%d", m_wPort) ;    
    else
	        wsprintf(szPort, "COM%d", m_wPort) ;
   // open COMM device
	if ((idComDev = CreateFile(szPort, GENERIC_READ|GENERIC_WRITE,
                  0,//FILE_SHARE_READ | FILE_SHARE_WRITE,                    // exclusive access
                  NULL,                 // no security attrs
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL| FILE_FLAG_OVERLAPPED,
                  NULL )) == (HANDLE) -1 )
	{
		//szStr.Format("串口%s初始化失败!",szPort);
		//AfxMessageBox(szStr);
		return FALSE;
	}

	osWrite.Offset = 0 ;
	osWrite.OffsetHigh = 0 ;
	osRead.Offset = 0 ;
	osRead.OffsetHigh = 0 ;

	osWrite.hEvent = CreateEvent( NULL,    // no security
                                  TRUE,    // explicit reset req
                                  FALSE,   // initial event reset
                                  NULL ) ; // no name
	if (osWrite.hEvent == NULL)
	{
		CloseHandle(idComDev);
		return FALSE;
	}
	
	osRead.hEvent = CreateEvent( NULL,    // no security
                                 TRUE,    // explicit reset req
                                 FALSE,   // initial event reset
                                 NULL ) ; // no name
	if (osRead.hEvent == NULL)
	{
		CloseHandle(idComDev);
		CloseHandle(osWrite.hEvent);
		return FALSE;
	}
	

	SetupComm(idComDev, 1024, 1024);
	PurgeComm( idComDev , PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
   
	SetCommMask(idComDev, 0);

	DCB        dcb ;
	dcb.DCBlength = sizeof( DCB ) ;
	GetCommState( idComDev , &dcb ) ;
   	dcb.BaudRate = m_wBaud;
	dcb.ByteSize = m_bData;
	dcb.Parity = m_bParity;
	dcb.StopBits = m_bStop;
	dcb.fBinary = TRUE ;
	dcb.fParity = TRUE ;

	fRetVal = SetCommState(idComDev , &dcb );

   	if (fRetVal)
	{
		fConnected = TRUE ;
	}
    else
	{
    	fConnected = FALSE ;
      	CloseHandle( idComDev ) ;
	}

	return ( fRetVal ) ;
}

void CMyComm::EndComm()
{
	if (!fConnected)
		return;

	TRACE("idComDev=%p\n", idComDev);
	fConnected = FALSE;

	EscapeCommFunction( idComDev , CLRDTR );

	PurgeComm( idComDev , PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );

	CloseHandle( idComDev );
	CloseHandle(osWrite.hEvent);
	CloseHandle(osRead.hEvent);

	return;
}

void CMyComm::ClearReceiveBuffer()
{
	PurgeComm( idComDev , //PURGE_TXABORT | PURGE_RXABORT |
					//		PURGE_TXCLEAR | 
							PURGE_RXCLEAR ) ;
}

void CMyComm::ResetComm()
{
	DWORD  rded;
	ClearCommError( idComDev , &rded, NULL) ;

	PurgeComm( idComDev , PURGE_TXABORT | PURGE_RXABORT |
							PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
}

BOOL CMyComm::OutBuffer(unsigned char bszStr[], unsigned short wCnt)
{
	if (fConnected != TRUE)
		return FALSE;

   	DWORD       dwBytesWritten = wCnt;

	return WriteCommBlock((LPSTR)bszStr, dwBytesWritten);

}

BOOL CMyComm::OutChar(unsigned char bCh)
{

	if (fConnected != TRUE)
		return FALSE;

	DWORD       dwBytesWritten = 1;
	return WriteCommBlock((LPSTR)&bCh, dwBytesWritten);
}

BOOL CMyComm::WaitAChar32(BYTE * wait, int limitms,BOOL * exitf)
{
	if (fConnected != TRUE)
		return FALSE;

	BOOL * pExit, EE = FALSE;
	int beg, cur;

	if (exitf == NULL)
		pExit = &EE;
	else
		pExit = exitf;

	beg =  GetTickCount();

 	while (!CharInBuffer())
		{
		cur =  GetTickCount()-beg;
	
		if (cur > limitms)
			return FALSE;
		if(*pExit)
			return FALSE;
		}
	
	ReadCommBlock((LPSTR)wait, 1);

	return TRUE;
	
}
BOOL CMyComm::SendAndWaitAChar32(BYTE send, BYTE * wait)
{
	if (fConnected != TRUE)
		return FALSE;

	OutChar(send);

	int beg, cur;
	beg = GetCurrentTime();

	while (!CharInBuffer())
		{
		cur = GetCurrentTime() - beg;
		if (cur >50)
			return FALSE;
		}

	ReadCommBlock((LPSTR)wait, 1);

	return TRUE;
}

int CMyComm::CharInBuffer()
{
	COMSTAT    ComStat ;
	DWORD  rded;
	
	ClearCommError( idComDev , &rded, &ComStat ) ;
	return (ComStat.cbInQue);
}


int CMyComm::CharOutBuffer()
{
	COMSTAT    ComStat ;
	DWORD  rded;
	
	ClearCommError(idComDev , &rded, &ComStat ) ;
	return (ComStat.cbOutQue);
}

BOOL CMyComm::WaitACharOut(int limitms)
{
	if (fConnected != TRUE)
		return FALSE;

	int beg, cur;
	beg =  GetTickCount();
 
	while (CharOutBuffer())
	{
		cur =  GetTickCount() - beg;
		if (cur > limitms)
			return FALSE;
	}

	return TRUE;
}

BOOL CMyComm::WriteCommBlock(LPSTR lpByte , DWORD dwBytesToWrite)
{
	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD       dwErrorFlags;
	COMSTAT     ComStat;

	if (fConnected != TRUE)
		return FALSE;

	fWriteStat = WriteFile(idComDev, lpByte, dwBytesToWrite,
                           &dwBytesWritten, &osWrite) ;

	if (!fWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
				if (WaitForSingleObject(osWrite.hEvent, 300)) 
					dwBytesWritten = 0;
				else
				{
					GetOverlappedResult(idComDev, &osWrite, &dwBytesWritten, FALSE);
					osWrite.Offset += dwBytesWritten;
				}
		}
		else
		{
  		    ClearCommError(idComDev, &dwErrorFlags, &ComStat ) ;
			return ( FALSE );
		}
	}

	return ( TRUE ) ;
}

DWORD CMyComm::ReadCommBlock(LPSTR lpBlock, int Length)
{
	BOOL fReadStat;
	COMSTAT ComStat;
	DWORD dwErrorFlags, dwLength;

	if (fConnected != TRUE)
		return FALSE;

	ClearCommError(idComDev, &dwErrorFlags, &ComStat);

	if (ComStat.cbInQue == 0)
        return 0L;

	dwLength = min((DWORD)Length,ComStat.cbInQue); // 

	if (dwLength > 0)
	{
		fReadStat = ReadFile(idComDev, lpBlock, dwLength,
											&dwLength, &osRead);
		if (!fReadStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				if (WaitForSingleObject(osRead.hEvent, 300)) dwLength = 0;
				else
				{
					GetOverlappedResult(idComDev, &osRead,
											&dwLength, FALSE);
					osRead.Offset += dwLength;
				}
			}
			else dwLength = 0;
		}
	}
	return dwLength;
}

BOOL CMyComm::SendCMDAndWait(unsigned char bszStr[], unsigned short &wCnt, int limits, unsigned char bRetBuf[],int MaxRetChar)
{
	DWORD bytesRead=0;		

	if (fConnected != TRUE)
	{
//		AfxMessageBox("未连接！");
		return FALSE;
	}


	if (OutBuffer(bszStr, wCnt) == FALSE)
	{
		return FALSE;
	}

	int time = GetTickCount();  
	int tick = 0;

 	while (CharInBuffer()<MaxRetChar)
	{
		tick = GetTickCount();
//		if (tick < time)
//			break;
		tick -= time;
		if (tick > limits)
			break;
	}

//yym add end
	wCnt = 0;
	//Sleep(limits);

//从收到第一字节开始全部字节必须在20毫秒内接收结束
	//如果是9600的波特率,大约是24字节,如果是19200，大约是48字节
	//Sleep(50);  //yym add 2005-04-29

	wCnt = (BYTE)ReadCommBlock((LPSTR)bszStr, MaxRetChar);

	if (wCnt<=0)
		return FALSE;

	ClearReceiveBuffer();
	return TRUE;
}


