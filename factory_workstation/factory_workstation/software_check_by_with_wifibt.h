#pragma once

#include "glob_param.h"
#include <mmsystem.h>  
#include "afxwin.h"
#include "ColorStatic.h"
#include "ContrlComm_burncheck.h"
#include "CurlCtrl.h"
// software_check_by_with_wifibt 对话框

class software_check_by_with_wifibt : public CDialogEx
{
	DECLARE_DYNAMIC(software_check_by_with_wifibt)

public:
	software_check_by_with_wifibt(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_check_by_with_wifibt();

// 对话框数据
	enum { IDD = IDD_DIALOG_check_by_with_wifibt };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFile m_hLogFile;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct check_by_option_str check_by_option_instance;
	void get_param(struct glob_param *g_p);

	unsigned int current;
	unsigned int charge_current;
	unsigned int backlight_current;
	unsigned int sleep_current;
	CContrlComm   comInfo;
	CContrlComm   comInfo_board;
	CCurlCtrl     mesInfo;
	int pass_green_color;
	unsigned int audNo;
	unsigned int audSelect;
	unsigned int PlaysoundThreadProc_exit;
	volatile unsigned int init_flag;
	volatile unsigned int init_err_flag;
	unsigned char sSetSN[20];
	unsigned char sn_read[20];
	unsigned char fs_version[16];
	unsigned char kernel_version[50];

	unsigned int aud_db;
	CFile rec_file;
	CString rec_file_path;
	FILE *pf;
	HWAVEIN hWaveIn;
	WAVEFORMATEX waveFormat;
	bool isGetSound;
	static DWORD CALLBACK MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	int pc_record();

	void init_part_process(unsigned int flag);
	void init_err_process(unsigned int flag);
	int if_response_err(CString response);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	void AddEvent2List(CString sText);
	void SetAllItemRestart(void);
	int need_file_ifexit();
	TCHAR* StringToChar(CString& str);
	int process_response_devices(CString response);
	int key_process(CString response);
	CString ExecuteCmd_keyprocess(CString cmd_name, CString cmd_param);
	CString ExecuteCmd(CString cmd_name, CString cmd_param);
	int key_init();
	static int create_app_aplay_init(void *param);
	static int create_app_sn_init(void *param);
	static int create_app_wifi_init(void *param);
	static int create_app_bt_init(void *param);
	static int create_app_start_play_init(void *param);
	static int create_app_key_init(void *param);
	static int create_app_start_play(void *param);
	static int WorkerThreadProc(void *param);
	static int PlaysoundThreadProc(void *param);
	static int kill_adbd(void *param);
	bool com_thread_quit;
	static int COMThreadProc(void *param);

	PROCESS_INFORMATION iperf;
	CString ExecuteCmd_iperf(CString cmd_name, CString cmd_param);
	static int create_app_iperf(void *param);

	PROCESS_INFORMATION wifi_trans_pi;
	CString ExecuteCmd_wifi_trans(CString cmd_name, CString cmd_param);

	CFont m_editFont;
	CListBox m_listEvt;
	CEdit m_EditSn;
	CStatic m_readSn;
	CStatic m_kernel_version;
	CStatic m_fs_version;
	CStatic m_Epass;
	CStatic m_rec_sec;
	CStatic m_rec_db;

	CColorStatic m_snRead;
	CColorStatic m_btnBoot;
	CColorStatic m_btnWifi;
	CColorStatic m_btnBt;
	CColorStatic m_btnDmic;
	CColorStatic m_btnPA;
	//CColorStatic m_btnKeyU;
	//CColorStatic m_btnKeyD;
	CColorStatic m_btnKeyPower;
	CColorStatic m_btnAud;
	CColorStatic m_normalCurrect;
	CColorStatic m_chargeCurrect;
	CColorStatic m_sleepCurrect;
	CColorStatic m_backlightCurrect;

	CButton current_checkBox;
	CButton sleep_current_checkBox;
	CButton backlight_current_checkBox;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton13();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//afx_msg void OnBnClickedRadioMusic1();
	//afx_msg void OnBnClickedRadioMusic2();
	//afx_msg void OnBnClickedRadioMusic3();
	//afx_msg void OnBnClickedRadioMusic4();
	//afx_msg void OnBnClickedRadioMusic5();
	//afx_msg void OnBnClickedRadioMusic6();
	afx_msg void OnBnClickedButton21();
};
