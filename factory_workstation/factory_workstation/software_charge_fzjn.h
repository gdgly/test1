#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ContrlComm_burncheck.h"
#include "MySqlite.h"
// software_charge_fzjn 对话框

class software_charge_fzjn : public CDialogEx
{
	DECLARE_DYNAMIC(software_charge_fzjn)

public:
	software_charge_fzjn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_charge_fzjn();

// 对话框数据
	enum { IDD = IDD_DIALOG_charge_check };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MySqliteInfo m_sqlite;
	unsigned int charge_current;
	CContrlComm   comInfo;

	bool com_thread_quit;
	unsigned char sSetSN[20];
	int pass_green_color;
	CFile m_hLogFile;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct charge_option_str charge_option_instance;

	static int COMThreadProc(void *param);
	static int WorkerThreadProc(void *param);
	void get_param(struct glob_param *g_p);
	void SetAllItemRestart(void);
	void AddEvent2List(CString sText);

	CFont m_editFont;
	CStatic m_Epass;
	CEdit m_EditSn;
	CListBox m_listEvt;
	CStatic m_chargeCurrect;

	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
