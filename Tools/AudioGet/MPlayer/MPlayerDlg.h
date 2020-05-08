
// MPlayerDlg.h : 头文件
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
// CMPlayerDlg 对话框
class CMPlayerDlg : public CDialogEx          //派生类
{
// 构造
public:
	CMPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	char            riffType[4];    //4byte,资源交换文件标志:RIFF     
	unsigned int    riffSize;       //4byte,从下个地址到文件结尾的总字节数   
	char            waveType[4];    //4byte,wav文件标志:WAVE      
	char            formatType[4];  //4byte,波形文件标志:FMT(最后一位空格符)   
	unsigned int    formatSize;     //4byte,音频属性(compressionCode,numChannels,sampleRate,bytesPerSecond,blockAlign,bitsPerSample)所占字节数  
	unsigned short  compressionCode;//2byte,格式种类(1-线性pcm-WAVE_FORMAT_PCM,WAVEFORMAT_ADPCM)  
	unsigned short  numChannels;    //2byte,通道数  
	unsigned int    sampleRate;     //4byte,采样率  
	unsigned int    bytesPerSecond; //4byte,传输速率  
	unsigned short  blockAlign;     //2byte,数据块的对齐，即DATA数据块长度  
	unsigned short  bitsPerSample;  //2byte,采样精度-PCM位宽  
	char            dataType[4];    //4byte,数据标志:data  
	unsigned int    dataSize;       //4byte,从下个地址到文件结尾的总字节数，即除了wav header以外的pcm data length  
}WavHeader, *WavHPtr;