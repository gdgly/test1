#pragma once
#include "MyComm.h"
#include "afxcmn.h"
#include "glob_param.h"
#include "fz_jn_burncheck_mpi.h"

#undef COM_DEBUG_LOG

class CContrlComm_bluetooth : public CMyComm
{
public:
	CContrlComm_bluetooth(void);
	~CContrlComm_bluetooth(void);

public:
#ifdef COM_DEBUG_LOG
	CFile  m_hLogFile;
	BOOL InitComm(unsigned short wPort, 
						unsigned char bData = DEIGHT, 
						unsigned char bStop = SONE, 
						unsigned char bParity = PODD,
						DWORD wBaud = 115200);
#endif

	INT          portCom,baudCom;
	void SetPort(int port) {portCom = port;}
	INT GetPort(void) {return portCom;}

	int SendComand(int comand);
	void ResetComm();
	int Process(void);
	int com_data_proess(BYTE *In,int *readedlen,BYTE *out);
	int Checkcomproess(void);
	BOOL StatusIsChange() { return bitChange; }
	UINT GetStatus() { return bitStatus;};
	UINT GetCurrent() {return current_mA;}
	UINT GetChargCurrent() {return chagr_curret_mA;}
	int HardwareVer(CString Sn,CString inputstr);
private:
	int ParseLine(BYTE *buf, int len);
	int CheckParseLine(BYTE *buf, int len);
	int Parse(void);


public:
	UINT          bitStatus;               // 每一位表示一个成功的状态
	BYTE          devAddr[20], devSN[20];   // 地址及序号
	BYTE          firmVer[16];
	UINT          current_mA; 
	CString			g_out_str;

private:
	UINT          chagr_curret_mA;
	UINT          bitChange;
	
	UINT          bufReaded;
	BYTE          bufCom[COM_BUF_SIZE];
	BYTE          checkbufCom[COM_BUF_SIZE];
};

