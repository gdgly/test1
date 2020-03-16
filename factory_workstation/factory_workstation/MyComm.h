//////////////////////////////////////////////////////////////////////
//
// MyComm.h: interface for the CMyComm class.
//
// Author:		YuanYiMin
//
// Date:		2005-06-17
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYCLASS_H__20262DC3_AB9F_11D4_B682_0000215A0000__INCLUDED_)
#define AFX_MYCLASS_H__20262DC3_AB9F_11D4_B682_0000215A0000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if defined(_WIN32)
// for Windows 95 or Windows NT

#else
// for Windows 3.X or DOS
#define TXR				0	// TX buffer if bit 7 of LCR is 0
#define RXR				0 	// RX buffer if bit 7 of LCR is 0
#define DLR				0 	// Divisor latch LO byte if bit 7 of LCR is 1
#define DHR				1 	// Divisor latch HO byte if bit 7 of LCR is 1
#define LER				1 	// Interrupt Enable Register
#define IIR				2 	// Interrupt Identification Register
#define LCR				3 	// Line Control Register
#define MCR				4 	// Modem Control Register
#define LSR				5 	// Line Status Register
#define MSR				6  // Modem Status Register

#endif		// (_WIN32)

// for para
// date
#define DEIGHT			8
#define DSEVEN			7
#define DSIX			6
#define DFIVE			5
// stop
#define SONE			1
#define STWO			2
// parify
#define PEVEN			2
#define PODD			1
#define PNONE			0


extern BYTE ComData[4];
extern BYTE ComStop[2];
extern BYTE ComParify[3];
extern DWORD ComBaud[10];


class CMyComm  
{
public:
	CMyComm();
	~CMyComm();

    HANDLE  idComDev;
    BOOL    fConnected;

   	unsigned short m_wPort;
	DWORD m_wBaud;
	unsigned char m_bData;
	unsigned char m_bStop;
	unsigned char m_bParity;

// date
protected:
	OVERLAPPED  osWrite, osRead;

public:
	BOOL	OutChar(unsigned char bCh);
	int		CharInBuffer();
    int		CharOutBuffer();
    BOOL    WaitAChar32(BYTE * wait, int limitms = 50, BOOL * exitf = NULL);
	BOOL	WaitACharOut(int limitms);
	BOOL	OutBuffer(unsigned char bszStr[], unsigned short wCnt);
	BOOL	SendCMDAndWait(unsigned char bszStr[], unsigned short &wCnt, int limits, unsigned char bRetBuf[],int MaxRetChar);
	BOOL	WriteCommBlock(LPSTR lpByte , DWORD dwBytesToWrite);
	DWORD	ReadCommBlock(LPSTR lpBlock, int nMaxLength);
	BOOL    SendAndWaitAChar32(BYTE send, BYTE * wait);
	friend DWORD WINAPI CommThread(LPVOID param);
// function
public:
// for comm port
	BOOL InitComm(unsigned short wPort, 
						unsigned char bData = DEIGHT, 
						unsigned char bStop = SONE, 
						unsigned char bParity = PODD,
						DWORD wBaud = 38400);
	void EndComm();
	void ResetComm();
	void ClearReceiveBuffer();
};

typedef struct
{
	CMyComm *pComm;
	CWnd *pWnd;
	BOOL *bStop;
} MyCommStru;

#endif // !defined(AFX_MYCOMM_H__20262DC3_AB9F_11D4_B682_0000215A0000__INCLUDED_)
