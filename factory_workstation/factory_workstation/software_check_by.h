#pragma once

#include "glob_param.h"
#include <mmsystem.h>  
#include "afxwin.h"
#include "ColorStatic.h"
#include "ContrlComm_burncheck.h"
#include "CurlCtrl.h"
// software_check_by 对话框
struct msg_by_check_info{
	int msg_type;
	int data1;//分区
	int data2;//百分比
};
enum {
	BY_CHECK_MSG_TYPE_COM_OPENERR = 0,
	BY_CHECK_MSG_TYPE_COM_OPENSUC,
	BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR,
	BY_CHECK_MSG_TYPE_CURRENT_VALUE,
	BY_CHECK_MSG_TYPE_CURRENT_SUC,
	BY_CHECK_MSG_TYPE_USB_TIMEOUT,
	BY_CHECK_MSG_TYPE_USBDEVICE_DET,
	BY_CHECK_MSG_TYPE_CREATE_INIT,
	BY_CHECK_MSG_TYPE_INIT_ERR,
	BY_CHECK_MSG_TYPE_INIT_PART,
	BY_CHECK_MSG_TYPE_INIT_SUC,
	BY_CHECK_MSG_TYPE_MES_ERR,
	BY_CHECK_MSG_TYPE_MES_SUC,
	BY_CHECK_MSG_TYPE_KEYALL_SUC,
	BY_CHECK_MSG_TYPE_KEYALL_ERR,
	BY_CHECK_MSG_TYPE_START_AUDIO,
	BY_CHECK_MSG_TYPE_AUDIO_ERR,
	BY_CHECK_MSG_TYPE_AUDIO_TIME,
	BY_CHECK_MSG_TYPE_AUDIO_SUC,
	BY_CHECK_MSG_TYPE_ALL_SUC,
};
enum {
	BY_CHECK_NORMAL_CURRENT = 0,
	BY_CHECK_DEVICES,
	BY_CHECK_CREATE_INIT,
	BY_CHECK_CHECK_INIT,
	BY_CHECK_CHECK_KEY,
	BY_CHECK_CHECK_WAIT_KEY,
	BY_CHECK_CHECK_BACKLIGHT,
	BY_CHECK_CHECK_AUDIO,
	BY_CHECK_CHECK_WAITAUDIO,
	BY_CHECK_CHARGE_CURRENT,
	BY_CHECK_SLEEP_CURRENT,
	BY_CHECK_SCUCESS,
};
enum {
	BY_CHECK_INIT_FLAG_SN = 0,
	BY_CHECK_INIT_FLAG_WIFI,
	BY_CHECK_INIT_FLAG_BT,
	BY_CHECK_INIT_FLAG_DMIC,
	BY_CHECK_INIT_FLAG_PA,
	BY_CHECK_INIT_FLAG_KEYUP,
	BY_CHECK_INIT_FLAG_KEYDOWN,
	BY_CHECK_INIT_FLAG_KEYPOWER,
	BY_CHECK_INIT_FLAG_AUDIO,
	BY_CHECK_INIT_FLAG_USBERR,
};
enum {
	BY_DISPLAY_CURRENT_VALUE_NORMAL = 0,
	BY_DISPLAY_CURRENT_VALUE_CHARGE,
	BY_DISPLAY_CURRENT_VALUE_BACKLIGHT,
	BY_DISPLAY_CURRENT_VALUE_SLEEP,
};
class software_check_by : public CDialogEx
{
	DECLARE_DYNAMIC(software_check_by)

public:
	software_check_by(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_check_by();

// 对话框数据
	enum { IDD = IDD_DIALOG_check_by };

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
