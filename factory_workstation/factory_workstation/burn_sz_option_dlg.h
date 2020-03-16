#pragma once

#include "glob_param.h"
// burn_sz_option_dlg 对话框

class burn_sz_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(burn_sz_option_dlg)

public:
	burn_sz_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~burn_sz_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_burn_sz_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct burn_sz_option_str burn_sz_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct burn_sz_option_str *bc_p);


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCheck12();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck10();
	afx_msg void OnBnClickedCheck21();
	afx_msg void OnBnClickedCheck22();
};
