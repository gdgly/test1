
// factory_workstationDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "page_fz_dlg.h"
#include "page_jn_dlg.h"
#include "page_by_dlg.h"
#include "page_sz_dlg.h"
#include "default_dlg.h"
#include "tools_dlg.h"
#include "general_option_dlg.h"
#include "burncheck_option_dlg.h"
#include "bluetooth_option_dlg.h"
#include "batpower_option_dlg.h"
#include "burn_by_option_dlg.h"
#include "check_by_option_dlg.h"
#include "total_machine_by_option_dlg.h"
#include "air_by_option_dlg.h"
#include "charge_check_option_dlg.h"
#include "before_press_current_by_option_dlg.h"
#include "batpower_by_option_dlg.h"
#include "burn_sz_option_dlg.h"
#include "check_sz_option_dlg.h"
#include "software_tool_sz_light.h"
#include "btaddr_sz_option_dlg.h"
#include "software_tool_sz_freq.h"
#include "check_sz_box_option_dlg.h"
#include "glob_param.h"


// Cfactory_workstationDlg 对话框
class Cfactory_workstationDlg : public CDialogEx
{
// 构造
public:
	Cfactory_workstationDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_FACTORY_WORKSTATION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	
	
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int pDialog_num;
	CDialog* pDialog[10];
	page_fz_dlg page_fz_dlg_int;
	page_jn_dlg page_jn_dlg_int;
	page_by_dlg page_by_dlg_int;
	page_sz_dlg page_sz_dlg_int;
	default_dlg default_dlg_int;
	tools_dlg tools_dlg_int;
	CTabCtrl platform_ctrl_tab;
	struct glob_param glob_param_instance;
	Cfactory_workstationDlg* parent_dlg_p;

	int LoadCheckConf();
	void first_time_disp_tab();
	void notfirst_time_disp_tab();
	void tabdlg_set_param();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void Onoption();
	afx_msg void Onburncheck();
	afx_msg void Onbluetooth();
	afx_msg void Onbatpower();
	afx_msg void Onbyburn();
	afx_msg void Onbycheck();
	afx_msg void Onbytotalmachine();
	afx_msg void Onbyair();
	afx_msg void Onchargecurrent();
	afx_msg void Onbeforepresscurrent();
	afx_msg void Onbypower();
	afx_msg void Onszburn();
	afx_msg void Onuse();
	afx_msg void Onszcheck();
	afx_msg void Onszbtaddr();
	afx_msg void Onszcheckbox();
	afx_msg void Onszburnsn();
};
