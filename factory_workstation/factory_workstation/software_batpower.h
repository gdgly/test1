#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ContrlComm_bluetooth.h"
#include "protocol.h"
#include "CurlCtrl.h"
#include "MySqlite.h"
// software_batpower 对话框

#define STR_SN_LEN     (19)
#define STR_TIME_LEN   (14)
#define STR_SPLIT_LEN  (4)       // 空格
#define STR_ERR_LEN    (5)       // 出错
#define STR_CKCNT_LEN  (4)       // 检测次数

class software_batpower : public CDialogEx
{
	DECLARE_DYNAMIC(software_batpower)

public:
	software_batpower(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_batpower();

// 对话框数据
	enum { IDD = IDD_DIALOG_batpower };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MySqliteInfo m_sqlite;
	CCurlCtrl     mesInfo;
	UINT    Mesvalue;
	char Mesresring[256];

	short old0_3_08_version;
	UINT       iBatVal;             // 获取电压
	BYTE       firversion[8];
	BYTE	   souversion[8];
	WORD       rebootnum;
	WORD       voltage;
	WORD       current;

	CFile m_hLogFile;

	BOOL process_end;
	BOOL m_bChkAllItem;
	INT loop_times;
	CString m_chkSN;
	INT m_chkNo;
	CString m_chkString;
	int com_open_flag;
	CContrlComm_bluetooth m_ctlCom;

	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct batpower_option_str batpower_option_instance;


	INT MesPass(int idata);
	INT MesSend(int idata,BYTE *ifirver,BYTE *isouver);

	int last_power, need_power;
	void bat_insert_sql(int iTimes, CString status, CTime tNow);
	CString old_version_check();
	CString first_time_check_without_current();
	CString second_time_check();
	CString first_time_check();
	int twice_current_check();
	void power_process();
	void exited_process();
	void AddEvent2List(CString sText);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	BOOL CompareSameName(CString sItem);
	void open_process_thread(INT list_num);
	void dlg_pram_init();
	void endisable_input(BOOL process_end);

	CListBox m_listEvt;
	CListBox list;
	CEdit m_Esetsn;
	CStatic m_current_num;
	CStatic m_current_sn;

	afx_msg void OnBnClickedButton3();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeSetSn();

	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton14();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
