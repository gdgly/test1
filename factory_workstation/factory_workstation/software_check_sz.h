#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ColorStatic.h"
#include "DeviceCtrl.h"
#include <mmsystem.h>  
#include "G722Play.h"
#include "acodec.h"
#include "CurlCtrl.h"
// software_check_sz 对话框

class software_check_sz : public CDialogEx
{
	DECLARE_DYNAMIC(software_check_sz)

public:
	software_check_sz(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_check_sz();

// 对话框数据
	enum { IDD = IDD_DIALOG_check_sz };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int em20168_init_ok;

	int check_init_num;
	int err_flag;
	int pass_green_color;
	CString last_sel_soft;
	CDeviceCtrl m_devCtrl;
	struct general_option_str general_option_instance;
	struct check_sz_option_str check_sz_option_instance;

	CString sn_no;
	CCurlCtrl     mesInfo;
	CFile m_mes_file;
	int software_check_sz::mes_sannuo_write_logfile(CString SN, CString MAC);

	void software_check_sz::get_param(struct glob_param *g_p);
	void software_check_sz::AddEvent2List(CString sText);
	void software_check_sz::StartDevContrl();
	LRESULT software_check_sz::OnDevCtrlError(WPARAM wParam, LPARAM lParam);
	LRESULT software_check_sz::OnDevCtrlReport(WPARAM wParam, LPARAM lParam);
	void software_check_sz::set_init_check_status(LPARAM lParam);
	unsigned int PlaysoundThreadProc_exit;
	unsigned int Playsound_value;//1小薇小薇，2 1khz音频
	static int PlaysoundThreadProc(void *param);
	int dbvalue_right,dbvalue_left;
	CString right_file_path,left_file_path;
	int software_check_sz::get_file_dbvalue(int *db_value, CString file_path);
	static int pc_thread_record(void *param);
	int pc_record_flag;

	unsigned int aud_db;
	CFile rec_file;
	CString rec_file_path;
	FILE *pf;
	HWAVEIN hWaveIn;
	WAVEFORMATEX waveFormat;
	bool isGetSound;
	static DWORD CALLBACK MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	int pc_record();

	static int software_check_sz::check_reboot(void *param);


	CFile m_hLogFile;
	CFont m_editFont;
	CListBox m_listEvt;
	CEdit m_EditSn;
	CStatic m_fw_version;
	CStatic m_btaddr;
	CColorStatic m_btnBoot;
	CColorStatic m_light;
	CColorStatic m_tap;
	CColorStatic m_plc;
	CColorStatic m_apollo;
	CColorStatic m_bonemic;
	CColorStatic m_get_ps;
	CColorStatic m_set_ps;
	CColorStatic m_wait_light;
	CColorStatic m_wait_tap;
	CColorStatic m_wait_plc;
	CColorStatic m_wait_apollo;
	CColorStatic m_wait_openusb;
	CColorStatic m_wait_closeusb;
	CStatic m_play_db;
	CStatic m_rec_right_db;
	CStatic m_rec_left_db;
	CStatic m_recplay_sec;
	CStatic m_player_sec;
	CStatic m_Epass;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
