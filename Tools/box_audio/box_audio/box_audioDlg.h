
// box_audioDlg.h : 头文件
//

#pragma once
#include "Resource.h"
#include "ContrlComm.h"
// Cbox_audioDlg 对话框
class Cbox_audioDlg : public CDialogEx
{
// 构造
public:
	Cbox_audioDlg(CWnd* pParent = NULL);	// 标准构造函数
	void LoadCheckConf();
	CContrlComm my_com;
	CContrlComm *comInfo;
	CString g_con_path;
	volatile int running_flag;
	UINT common_port;
	UINT baudrate;
	unsigned short command;
	CEdit m_Esetsn;
	CEdit m_Esetsn1;
	CEdit m_Esetsn2;
	CEdit m_Esetsn3;
	CEdit m_Esetsn4;
	CEdit m_Esetsn5;
	CEdit m_Esetsn6;
	CEdit m_Esetsn7;
	CEdit m_Esetsn8;
	CEdit m_common_port;
	CEdit m_command;
	CEdit m_baudrate;
// 对话框数据
	enum { IDD = IDD_BOX_AUDIO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk3();
	afx_msg void OnBnClickedOk4();
	afx_msg void OnBnClickedOk6();
	afx_msg void OnBnClickedOk5();
	afx_msg void OnBnClickedOk10();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk7();
};
