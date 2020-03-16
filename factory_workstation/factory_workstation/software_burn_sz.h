#pragma once

#include "glob_param.h"
#include "DeviceCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "CurlCtrl.h"
// software_burn_sz 对话框
#define SZ_DEFAULT_BTADDR "70B3D54D3064"//"{0x4d3064, 0xd5, 0x70b3}"//"00:ff:01:5b:00:02"
#define SZ_DEFAULT_BTNAME "\"iFLYBUDS\""

class software_burn_sz : public CDialogEx
{
	DECLARE_DYNAMIC(software_burn_sz)

public:
	software_burn_sz(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_burn_sz();

// 对话框数据
	enum { IDD = IDD_DIALOG_burn_sz };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int err_flag;
	int pass_green_color;
	CString last_sel_soft;
	CDeviceCtrl m_devCtrl;
	struct general_option_str general_option_instance;
	struct burn_sz_option_str burn_sz_option_instance;

	CString software_burn_sz::con_btaddr_format(CString btaddr_12B);

	CString sn_no;
	CCurlCtrl     mesInfo;

	void software_burn_sz::all_init();
	void software_burn_sz::AddEvent2List(CString sText);
	void software_burn_sz::get_param(struct glob_param *g_p);
	void software_burn_sz::StartDevContrl();
	LRESULT software_burn_sz::OnDevCtrlError(WPARAM wParam, LPARAM lParam);
	LRESULT software_burn_sz::OnDevCtrlReport(WPARAM wParam, LPARAM lParam);
	CFile m_apollo_burn_file;
	void software_burn_sz::set_apollo_burn_file();
	int software_burn_sz::need_file_ifexit();
	CFile m_box_burn_file;
	void software_burn_sz::set_box_burn_file();
	int software_burn_sz::qcc5126_erase_flash(void);
	static int WorkerThreadProc(void *param);

	CFile m_hLogFile;
	CFont m_editFont;
	CStatic m_Epass;
	CEdit m_EditSn;
	CListBox m_listEvt;
	CStatic m_firm_version;
	CEdit m_btaddr;
	CEdit m_btname;
	CProgressCtrl m_burn_progress;
	CProgressCtrl m_apollo_progress;	
	CProgressCtrl m_box_progress;


	afx_msg void OnBnClickedButton1();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
