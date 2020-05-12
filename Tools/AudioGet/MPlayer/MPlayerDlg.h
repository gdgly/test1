
// MPlayerDlg.h : ͷ�ļ�
//

#pragma once

#include "mp3.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "G722Play.h"
#include <iostream>
#include <io.h>
#include <string>
#include <vector>

#define SINGLE 1
#define R_SINGLE 2
#define ALL 3
#define R_ALL 4
#define WM_MYMSG WM_USER + 1
// CMPlayerDlg �Ի���
class CMPlayerDlg : public CDialogEx          //������
{
// ����
public:
	CMPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//afx_msg LRESULT OnMyMsgHandler(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
public:
	void getFiles(const std::string & path, std::vector<std::string> & files);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	int con;
	CString push[500];
	CString pushFile[500];
	CString Preservation[500];
	CString szDisplayName;

	int m_iSectSize;
	BYTE  *m_pRdBuffer, *m_pWrBuffer[6];
	
	int FindTags(BYTE *pbuffer, UINT count);
	int FindTags(CFile &hFile, UINT iFrom);	
	int SaveOneTags(CFile &hFile, UINT iFrom, CString outPrefix, int sector);
	int ConvertFile(CString sDir, CString inFile);

	int hour;
	int minute;
	int second;
	int ch;
	int cm;
	int cs;
	afx_msg void OnLbnSelchangeList();
	CListBox list;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedAdd();
	Mp3 player;
	G722Play m_G722Play;
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedDelete();
	CSliderCtrl vol;
	afx_msg void OnCustomdrawSliderVol(NMHDR *pNMHDR, LRESULT *pResult);
	int volume;
	afx_msg void OnCustomdrawSliderTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList();
	int mode;
	afx_msg void OnBnClickedButtonCon();
	INT Write_Wav(CString filename);
	INT Write_WavHead(int datalen, int sample_rate);
	void NextPplay();
	CFile wavFile;
	void Play_Ord(CString filename);
	CFile wavFile1;
	CFile wavFile2;
	CFile wavFile3;
	CFile wavFile4;
	CFile wavFile5;
	CFile g722File;
	BYTE m_g722Buf[40];
	BYTE m_wavBuf[40*16];
	BYTE m_holdbuf[16*1024];
	BYTE wavFile1buf[4*1024];
	BYTE wavFile2buf[4*1024];
	BYTE wavFile3buf[4*1024];
	BYTE wavFile4buf[4*1024];
	CEdit m_Econver;
	afx_msg void OnBnClickedButtonOrdplay();
	CEdit m_Efllename;
	CString filename;
	INT m_ncount;
	int m_listncount;
	afx_msg void OnBnClickedButtonStopplay();
	afx_msg void OnLbnSelchangePause();
	CListBox m_ListFile;
	CButton button;
	UINT m_iChNum;
	UINT m_TrunkSize;
};


typedef struct WAV_HEADER_S   {
	char            riffType[4];    //4byte,��Դ�����ļ���־:RIFF     
	unsigned int    riffSize;       //4byte,���¸���ַ���ļ���β�����ֽ���   
	char            waveType[4];    //4byte,wav�ļ���־:WAVE      
	char            formatType[4];  //4byte,�����ļ���־:FMT(���һλ�ո��)   
	unsigned int    formatSize;     //4byte,��Ƶ����(compressionCode,numChannels,sampleRate,bytesPerSecond,blockAlign,bitsPerSample)��ռ�ֽ���  
	unsigned short  compressionCode;//2byte,��ʽ����(1-����pcm-WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM)  
	unsigned short  numChannels;    //2byte,ͨ����  
	unsigned int    sampleRate;     //4byte,������  
	unsigned int    bytesPerSecond; //4byte,��������  
	unsigned short  blockAlign;     //2byte,���ݿ�Ķ��룬��DATA���ݿ鳤��  
	unsigned short  bitsPerSample;  //2byte,��������-PCMλ��  
	char            dataType[4];    //4byte,���ݱ�־:data  
	unsigned int    dataSize;       //4byte,���¸���ַ���ļ���β�����ֽ�����������wav header�����pcm data length  
}WavHeader, *WavHPtr;