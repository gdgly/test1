#pragma once
#include "afxwin.h"

#include "glob_param.h"
// software_tool_by_getfile 对话框

class software_tool_by_getfile : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_by_getfile)

public:
	software_tool_by_getfile(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_by_getfile();

// 对话框数据
	enum { IDD = IDD_DIALOG_by_getfile_tool };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	struct tools_option_str tools_option_instance;
	void get_param(struct glob_param *g_p);

	int process_response_devices(CString response);
	void AddEvent2List(CString sText);
	int if_response_err(CString response);
	TCHAR* StringToChar(CString& str);
	CString ExecuteCmd(CString cmd_name, CString cmd_param);
	static int WorkerThreadProc(void *param);

	int need_file_ifexit();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton23();
	CListBox m_listEvt;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
