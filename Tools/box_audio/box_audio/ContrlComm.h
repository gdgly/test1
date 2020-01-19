#pragma once
#include "mycomm.h"
#include "CurlCtrl.h"

//#define CONFIG_SUPPORT_MES_SEND          //ʹ��MES�ϴ���־
#define WM_USERMSG_MASSPRODUCT         (WM_USER+33)
#define    WPARAM_GETFILE                 (0x10)
#define    WPARAM_THREAD_EXITED           (0x11)
#define    WPARAM_AUDIO_LENGTH            (0x12)
#define    WPARAM_MAC_SN                  (0x13)
#define    WPARAM_TFA9896_VALUE           (0x14)
#define	   WPARAM_MES_STRING              (0x15)
#define	   WPARAM_MES_ERR                 (0x16)
#define	   WPARAM_SN                      (0x17)
#define	   WPARAM_MES_EERR                (0x18)
#define	   WPARAM_MES_TURE                (0x19)
#define	   WPARAM_CYCLE_DIS               (0x20)
#define COM_BUF_SIZE            (1024)   // ���ڻ�������С
enum {COM_No_BOARD=0, COM_No_CHECK, COM_No_COUNT};
int com_data_proess(BYTE *In,int * readedlen,BYTE *out);
int get_line(BYTE *Inbuf,int read_len);

// ÿһλ��ʾһ �ֳɹ��ļ��״̬
enum {BIT_SYS_RUN96M=0,              // Systen Run96M OK
	BIT_CS47L35,                     // CS47L35 OK
	BIT_SPEAKER,                     // TEA9896 OK
	BIT_KEY_TOUCH_INIT,              // TouchInit OK

	BIT_KEY_TOUCH_UP,                // TouchUp OK
	BIT_KEY_TOUCH_DOWN,              // TouchDown OK
	BIT_KEY_BOARD,                   // KeyUp OK   | KeyDown OK

	BIT_AUDIO_IN_OUT,                // User Select


	BIT_CURRENT_mA,                  // CURRENT=xxxmA OK

	BIT_END,
};

#define BIT_START_INIT         ((1<<BIT_SYS_RUN96M) | (1<<BIT_CS47L35) | (1<<BIT_SPEAKER) | (1<<BIT_KEY_TOUCH_INIT))
#define BIT_KEY_CHECKED        (BIT_START_INIT | (1<<BIT_KEY_TOUCH_UP) | (1<<BIT_KEY_TOUCH_DOWN) | (1<<BIT_KEY_BOARD))

enum {AUDIO_T_G722=0, AUDIO_T_PCM_MONO, AUDIO_T_PCM_STERO,AUDIO_T_PCM_QIMIXING};
typedef struct tagAUDINFO {
	UINT       audType;             // 0: g722 1:wav

	UINT       audMsec;             // �Ѿ�¼�Ƴ���
	UINT       recvLen;
	BYTE       *pAudBuf;

	UINT       audMsec2;             // �Ѿ�¼�Ƴ���
	UINT       recvLen2;
	BYTE       *pAudBuf2;

}AudInfo, *AudIPtr;

class CContrlComm : public CMyComm
{
public:
	CContrlComm(void);
	~CContrlComm(void);

public:
	UINT          portCom, baudCom;
	void SetPort(int port, int baud) {portCom = port; baudCom = baud;}
	UINT GetPort(void) {return portCom;}
	static int WorkerThreadProc(void *param);
	HWND m_hWnd;
	void SetWnd(HWND hWnd) { m_hWnd = hWnd; }
	void AudioDataMessage(void);

	void ResetComm();
//	int Process(void);
//	int com_data_proess(BYTE *In,int *readedlen,BYTE *out);
	BOOL StatusIsChange() { return bitChange; }
	UINT GetStatus() { return bitStatus;};
	UINT GetCurrent() { return 20; return current_mA;}

	
	
	

	BYTE pRdbuf[2000];
	UINT StartComThread(void);
	HANDLE m_hComThread;
	UINT m_iExitThread;
	AudInfo m_iAudInfo;
	CCurlCtrl     mesInfo;
	CString m_sFilename, m_sFilename2;

	CString m_sMac, m_sSN, m_setSN;
	CTime   m_tLastSendCmd;
	int     m_iTFA8686val;
	int     m_start_resp, m_stop_resp;       // ���ͷ�������
	char	Mesresring[256];
	int     file1db;
	int     file2db;
	UINT    cycle;
	UINT    firsts,seconds;
private:
	int ParseLine(BYTE *buf, int len);
	int Parse(void);
	

private:
	UINT          current_mA;              // ��ǰ����
	UINT          bitChange;
	UINT          bitStatus;               // ÿһλ��ʾһ���ɹ���״̬
	UINT          bufReaded;
	BYTE          bufCom[COM_BUF_SIZE];
};

