#pragma once

#include "glob_param.h"
#include "afxwin.h"
// btaddr_sz_option_dlg 对话框

class btaddr_sz_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(btaddr_sz_option_dlg)

public:
	btaddr_sz_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~btaddr_sz_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_btaddr_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct btaddr_sz_option_str btaddr_sz_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct btaddr_sz_option_str *bc_p);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CEdit m_bt_name;
	CEdit m_hw_version;
	CEdit m_rl_flag;
	afx_msg void OnBnClickedCheck14();
	CEdit m_station;
};
