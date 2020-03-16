#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ColorStatic.h"
#include "wlanapi.h"
#include <mmsystem.h>  
#include "G722Play.h"
// software_air_by 对话框
#include <string>
using namespace std;
struct msg_by_air_info{
	int msg_type;
	int data1;
	int data2;
};
enum {
	BY_AIR_MSG_TYPE_WIFI_MODE = 0,
	BY_AIR_MSG_TYPE_USB_MODE,
	BY_AIR_MSG_TYPE_UNKNOW_MODE,
	BY_AIR_MSG_TYPE_WIFI_CONNECT_ERR,
	BY_AIR_MSG_TYPE_WIFI_GET_NAME,
	BY_AIR_MSG_TYPE_WIFI_CONNECT_SUC,
	BY_AIR_MSG_TYPE_WIFI_CONNECT_TIMEOUT,
	BY_AIR_MSG_TYPE_WIFI_ADB_TIMEOUT,
	BY_AIR_MSG_TYPE_USB_ADB_TIMEOUT,
	BY_AIR_MSG_TYPE_ADB_SEC,
	BY_AIR_MSG_TYPE_RECORD_DB_VALUE,
	BY_AIR_MSG_TYPE_RECORD_START,
	BY_AIR_MSG_TYPE_RECORD_TIME,
	BY_AIR_MSG_TYPE_PART_ERR,
	BY_AIR_MSG_TYPE_PART_SUC,

	BY_AIR_MSG_TYPE_ALL_SEC,
};

class software_air_by : public CDialogEx
{
	DECLARE_DYNAMIC(software_air_by)

public:
	software_air_by(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_air_by();

// 对话框数据
	enum { IDD = IDD_DIALOG_air_by };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int test_times;
	CFile m_hLogFile;
	int pass_green_color;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct air_by_option_str air_by_option_instance;
	void get_param(struct glob_param *g_p);

	unsigned char last_sSetSN[20];
	unsigned char sSetSN[20];
	unsigned char sn_read[20];
	unsigned char fs_version[16];
	unsigned char kernel_version[50];

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

	unsigned int PlaysoundThreadProc_exit;
	static int PlaysoundThreadProc(void *param);

	CString rec_left_file_path;
	CString rec_right_file_path;
	unsigned int rec_aud_db_left;
	unsigned int rec_aud_db_right;
	static int create_app_start_record(void *param);
	static int create_app_sn_init(void *param);

	volatile unsigned int init_flag;
	volatile unsigned int init_err_flag;
	int process_response_connect(CString response);
	int process_response_devices(CString response);
	int if_response_err(CString response);
	TCHAR* StringToChar(CString& str);
	CString ExecuteCmd(CString cmd_name, CString cmd_param);
	static int WorkerThreadProc(void *param);

	void AddEvent2List(CString sText);
	void init_part_process(unsigned int flag);
	void init_err_process(unsigned int flag);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	
	static int kill_adbd(void *param);
	CString	m_sFileName;
	G722Play m_G722Play;

	int need_file_ifexit();
	void SetAllItemRestart(void);

	afx_msg void OnBnClickedButton13();
	afx_msg void OnEnChangeSetSn();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();

	CFont m_editFont;
	CEdit m_EditSn;
	CStatic m_readSn;
	CStatic m_kernel_version;
	CStatic m_fs_version;
	CStatic m_wifihot_name;
	CStatic m_wifihot_status;
	CStatic m_rec_right_db;
	CStatic m_rec_left_db;
	CStatic m_Epass;
	CListBox m_listEvt;
	CListBox list_audio;
	CColorStatic m_btnBoot;
	CColorStatic m_snRead;
	CColorStatic m_fstime;
	CColorStatic m_sctime;
	CStatic m_rec_right_db2;
	CStatic m_rec_left_db2;
	CStatic m_recplay_sec;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
