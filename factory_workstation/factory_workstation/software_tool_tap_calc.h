#pragma once
#include "glob_param.h"
#include "MyComm.h"

#define BUFFER_SIZE      (4096)

// software_tool_tap_calc 对话框

class software_tool_tap_calc : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_tap_calc)

public:
	software_tool_tap_calc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_tap_calc();

public:
	void get_param(struct glob_param *g_p);

private:
	CMyComm m_hCom;
	char    m_comBuff[BUFFER_SIZE];
	UINT  m_iComPort;
	CString ini_path;
	struct tools_option_str tools_option_instance;
	int SendAndWait(CString sCmd, int timeout = 450);
	int GetPromptValue(CString sPrompt1, CString sPrompt2);
	BOOL StartOpenSensor(int step, int value);
	CString GetPromptString(CString sPrompt1, CString sPrompt2, int outMax = 12);
	int Get3ValueFromString(CString sText, short &value0, short &value1, short &value2);

public:
// 对话框数据
	enum { IDD = IDD_DIALOG_tap_calc };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStartTap();
	afx_msg void OnBnClickedBtnReadTap();
	afx_msg void OnBnClickedBtnTapSaveParam();
	afx_msg void OnBnClickedBtnReadTapParam();
	CString m_sCalcVal;
	CString m_sCalcOrign;
	CString m_sTapParamW;
	CString m_sTapParamR;
	afx_msg void OnBnClickedBtnReadOrign();
};
