#pragma once
#include "afxwin.h"

#include "glob_param.h"
#include "ContrlComm_burncheck.h"
// software_by_before_press_current 对话框

class software_by_before_press_current : public CDialogEx
{
	DECLARE_DYNAMIC(software_by_before_press_current)

public:
	software_by_before_press_current(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_by_before_press_current();

// 对话框数据
	enum { IDD = IDD_DIALOG_by_current_before_press };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool com_thread_quit;
	unsigned char sSetSN[20];
	int pass_green_color;
	CFile m_hLogFile;
	CFont m_editFont;
	CListBox m_listEvt;
	CEdit m_EditSn;
	CStatic m_Epass;
	CStatic m_current_value;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct bp_current_by_option_str bp_current_by_option_instance;

	unsigned int sleep_current;
	CContrlComm   comInfo;

	void all_init();
	void AddEvent2List(CString sText);
	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	static int COMThreadProc(void *param);

	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton13();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
