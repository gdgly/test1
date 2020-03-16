#pragma once
#include "afxwin.h"
#include "glob_param.h"
#include "wlanapi.h"
#include <string>
using namespace std;

// software_tool_vnc 对话框

class software_tool_vnc : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_vnc)

public:
	software_tool_vnc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_vnc();

// 对话框数据
	enum { IDD = IDD_DIALOG_vnc_tool };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct total_machine_by_option_str total_machine_by_option_instance;
	void get_param(struct glob_param *g_p);

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

	void AddEvent2List(CString sText);

	unsigned char sSetSN[20];
	static int WorkerThreadProc(void *param);
	
	volatile bool quit_thread;
	//volatile bool main_thread_working;
	HANDLE trans_hThread;
	HWND vnc_hWnd;
	
	HANDLE StartProcess(LPCTSTR program, LPCTSTR args);
	static int kill_adbd(void *param);
	static int WorkerThreadProcvnc(void *param);
	static int WorkerThreadProcdisplay(void *param);

	volatile unsigned int init_flag;
	volatile unsigned int init_err_flag;
	static int create_app_trans(void *param);
	int process_response_connect(CString response);
	TCHAR* StringToChar(CString& str);
	PROCESS_INFORMATION vnc_sh_pi;
	CString ExecuteCmd_sh(CString cmd_name, CString cmd_param);
	CString ExecuteCmd(CString cmd_name, CString cmd_param);
	int if_response_err(CString response);

	int process_response_devices(CString response);
	void SetAllItemRestart(void);
	int need_file_ifexit();

	CEdit m_EditSn;
	CListBox m_listEvt;
	afx_msg void OnBnClickedButton13();
	afx_msg void OnEnChangeSetSn();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton22();
	afx_msg void OnClose();
	//virtual void OnCancel();
};
