#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ColorStatic.h"
#include "CurlCtrl.h"
#include "ContrlComm_burncheck.h"
// software_batpower_by 对话框

class software_batpower_by : public CDialogEx
{
	DECLARE_DYNAMIC(software_batpower_by)

public:
	software_batpower_by(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_batpower_by();

// 对话框数据
	enum { IDD = IDD_DIALOG_by_batpower };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFile m_hLogFile;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct batpower_by_option_str batpower_by_option_instance;

	CFont m_editFont;
	CEdit m_EditSn;
	CStatic m_readSn;
	CColorStatic m_snRead;
	CStatic m_fs_version;
	CStatic m_kernel_version;
	CButton current_checkBox;
	CColorStatic m_normalCurrect;
	CColorStatic m_chargeCurrect;
	CColorStatic m_btnBoot;
	CColorStatic m_reset_setup;
	CColorStatic m_bat_power;
	CListBox m_listEvt;
	CStatic m_Epass;

	CContrlComm   comInfo;
	CCurlCtrl     mesInfo;
	int pass_green_color;

	unsigned char sSetSN[20];
	unsigned char sn_read[20];
	unsigned char fs_version[16];
	unsigned char kernel_version[50];

	unsigned int read_bat_power;
	unsigned int current;
	unsigned int charge_current;
	bool com_thread_quit;
	static int COMThreadProc(void *param);
	void get_param(struct glob_param *g_p);
	void AddEvent2List(CString sText);
	void SetAllItemRestart(void);
	static int WorkerThreadProc(void *param);
	TCHAR* StringToChar(CString& str);
	CString ExecuteCmd(CString cmd_name, CString cmd_param);
	int process_response_devices(CString response);
	int if_response_err(CString response);
	//volatile unsigned int init_err_flag;
	int sn_init(void* param);

	INT MesPass();
	INT MesSend();

	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton13();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
