#pragma once
#include "glob_param.h"
#include "MyComm.h"

#define BUFFER_SIZE      (4096)
// software_tool_psensor_calc 对话框

class software_tool_psensor_calc : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_psensor_calc)

public:
	software_tool_psensor_calc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_psensor_calc();

// 对话框数据
	enum { IDD = IDD_DIALOG_psensor_calc };

public:
	void get_param(struct glob_param *g_p);

private:
	UINT  m_iComPort;
	CString ini_path;
	struct tools_option_str tools_option_instance;
	int SendAndWait(CString sCmd, int timeout = 450);
	int GetPromptValue(CString sPrompt1, CString sPrompt2);
	BOOL StartOpenSensor(int step, int value);
	CString GetPromptString(CString sPrompt1, CString sPrompt2, int outMax = 5);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:	
	CMyComm m_hCom;
	char    m_comBuff[BUFFER_SIZE];

	int m_iValue;
	int m_iValue0;
	int m_iValue1;
	int m_iValue5CM;
	int m_iValue3CM;
	afx_msg void OnBnClickedBtnStart1();
	afx_msg void OnBnClickedBtnRead1();
	afx_msg void OnBnClickedBtnStart2();
	afx_msg void OnBnClickedBtnRead2();
	afx_msg void OnBnClickedBtnStart3();
	afx_msg void OnBnClickedBtnRead3();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnRead();
	afx_msg void OnBnClickedBtnSensorCheck();
	afx_msg void OnBnClickedBtnEarRestart();
};
