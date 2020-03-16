#pragma once
#include "afxwin.h"

#include "glob_param.h"
#include "ContrlComm_burncheck.h"
// software_tool_current_board 对话框

class software_tool_current_board : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_current_board)

public:
	software_tool_current_board(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_current_board();

// 对话框数据
	enum { IDD = IDD_DIALOG_curboard_tool };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	char buf[COM_MAX_RDSZIZE];
	int buf_len;
	BOOL display_finish_flag;
	CContrlComm m_ctlCom;
	struct general_option_str general_option_instance;

	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	
	CEdit edit_out;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
};
