#pragma once
#include "afxwin.h"

#include "glob_param.h"
#include "ContrlComm_bluetooth.h"
#include "protocol.h"
// software_tool_box_audio 对话框

class software_tool_box_audio : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_box_audio)

public:
	software_tool_box_audio(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_box_audio();

// 对话框数据
	enum { IDD = IDD_DIALOG_box_audio };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	unsigned short g_command;
	CString m_chkSN;
	CContrlComm_bluetooth m_ctlCom;
	struct general_option_str general_option_instance;

	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);

	CEdit m_Esetsn;
	CEdit m_command;
	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
