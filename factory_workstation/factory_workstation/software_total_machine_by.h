#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ColorStatic.h"
#include "CurlCtrl.h"
#include "wlanapi.h"
#include <string>
#include <mmsystem.h>  
#include "G722Play.h"
using namespace std;
#pragma comment(lib, "Wlanapi")

// software_total_machine_by 对话框
struct msg_by_total_info{
	int msg_type;
	int data1;
	int data2;
};
enum {
	BY_TOTAL_MSG_TYPE_WIFI_MODE = 0,
	BY_TOTAL_MSG_TYPE_USB_MODE,
	BY_TOTAL_MSG_TYPE_UNKNOW_MODE,
	BY_TOTAL_MSG_TYPE_WIFI_CONNECT_ERR,
	BY_TOTAL_MSG_TYPE_WIFI_GET_NAME,
	BY_TOTAL_MSG_TYPE_WIFI_CONNECT_SUC,
	BY_TOTAL_MSG_TYPE_WIFI_CONNECT_TIMEOUT,
	BY_TOTAL_MSG_TYPE_WIFI_ADB_TIMEOUT,
	BY_TOTAL_MSG_TYPE_USB_ADB_TIMEOUT,
	BY_TOTAL_MSG_TYPE_ADB_SEC,
	BY_TOTAL_MSG_TYPE_PART_ERR,
	BY_TOTAL_MSG_TYPE_PART_SUC,
	BY_TOTAL_MSG_TYPE_PIXEL_ERR,
	BY_TOTAL_MSG_TYPE_KEY_TS_SUC,
	BY_TOTAL_MSG_TYPE_WIFI_BT_SUC,
	BY_TOTAL_MSG_TYPE_RECORD_DB_VALUE,
	BY_TOTAL_MSG_TYPE_RECORD_START,
	BY_TOTAL_MSG_TYPE_RECORD_TIME,
	BY_TOTAL_MSG_TYPE_PLAY_TIME,
	BY_TOTAL_MSG_TYPE_PLAY_DB_VALUE,
	BY_TOTAL_MSG_TYPE_BAT_POWER_VALUE,
	BY_TOTAL_MSG_TYPE_ALL_SEC,
	BY_TOTAL_MSG_TYPE_MES_ERR,
	BY_TOTAL_MSG_TYPE_MES_SUC,
};
class software_total_machine_by : public CDialogEx
{
	DECLARE_DYNAMIC(software_total_machine_by)

public:
	software_total_machine_by(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_total_machine_by();

// 对话框数据
	enum { IDD = IDD_DIALOG_total_machine };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFile m_hLogFile;
	int pass_green_color;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct total_machine_by_option_str total_machine_by_option_instance;
	void get_param(struct glob_param *g_p);

	unsigned char sSetSN[20];
	unsigned char sn_read[20];
	unsigned char fs_version[16];
	unsigned char kernel_version[50];

	CCurlCtrl     mesInfo;
	INT MesPass(int idata);
	INT MesSend(int idata,BYTE *ifirver,BYTE *isouver);


	DWORD dwCurVersion;
	HANDLE hClient;
	DWORD dwResult;
	PWLAN_INTERFACE_INFO_LIST pInterfaceList;
	PWLAN_AVAILABLE_NETWORK pNet;
	PWLAN_INTERFACE_INFO pInterface;
	PWLAN_AVAILABLE_NETWORK_LIST pNetList;
	PWLAN_CONNECTION_PARAMETERS pConnPara;
	string target;
	string targetKey;

	LPCWSTR StringToLPCWSTR(std::string orig);
	bool OpenWLANService();
	bool WLANNetwork();
	bool SetProfile();
	bool WLANConnect();
	bool SearchNet();
	bool get_wifi_status();
	bool get_connect_msg();
	bool WLANDisconnect();
	bool CloseWLANService();

	CString rec_left_file_path;
	CString rec_right_file_path;
	unsigned int rec_aud_db_left;
	unsigned int rec_aud_db_right;

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
	void AddEvent2List(CString sText);
	void SetAllItemRestart(void);
	int need_file_ifexit();
	volatile unsigned int init_flag;
	volatile unsigned int init_err_flag;
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	static int WorkerThreadProc(void *param);
	TCHAR* StringToChar(CString& str);
	int if_response_err(CString response);
	int process_response_connect(CString response);
	int process_response_devices(CString response);
	int ts_process(CString* response);
	CString ExecuteCmd_tsprocess(CString cmd_name, CString cmd_param);
	int key_process(CString response);
	CString ExecuteCmd_keyprocess(CString cmd_name, CString cmd_param);
	PROCESS_INFORMATION wifi_trans_pi;
	CString ExecuteCmd_wifi_trans(CString cmd_name, CString cmd_param);
	CString ExecuteCmd(CString cmd_name, CString cmd_param);

	static int create_app_sn_init(void *param);
	static int create_app_key_init(void *param);
	static int create_app_ts_init(void *param);
	static int create_app_start_play(void *param);
	static int create_app_start_play_init(void *param);
	static int create_app_start_record(void *param);
	static int create_app_wifi_trans_init(void *param);
	static int create_app_bt_trans_init(void *param);
	unsigned int PlaysoundThreadProc_exit;
	static int PlaysoundThreadProc(void *param);
	static int kill_adbd(void *param);
	static int get_bat_power(void *param);
	unsigned int read_bat_power;

	PROCESS_INFORMATION iperf;
	CString ExecuteCmd_iperf(CString cmd_name, CString cmd_param);
	static int create_app_iperf(void *param);

	CString	m_sFileName;
	G722Play m_G722Play;
	
	int calibration_value;
	CFont m_editFont;
	CEdit m_EditSn;
	CStatic m_readSn;
	CStatic m_kernel_version;
	CStatic m_fs_version;
	CListBox m_listEvt;
	CStatic m_Epass;

	CStatic m_wifihot_name;
	CStatic m_wifihot_status;
	CColorStatic m_btnBoot;
	CColorStatic m_btnWifi;
	CColorStatic m_btnBt;
	CColorStatic m_btnDmic;
	CColorStatic m_btnPA;
	CColorStatic m_btnKeyU;
	CColorStatic m_btnKeyD;
	CColorStatic m_btnKeyPower;
	CColorStatic m_ts_event;
	CColorStatic m_snRead;
	CListBox list_audio;
	CStatic m_calibration;
	CStatic m_play_db;
	CStatic m_rec_right_db;
	CStatic m_rec_left_db;
	CStatic m_recplay_sec;
	CStatic m_player_sec;
	CStatic m_bat_power;

	afx_msg void OnBnClickedButton13();
	afx_msg void OnEnChangeSetSn();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedButton20();
};
