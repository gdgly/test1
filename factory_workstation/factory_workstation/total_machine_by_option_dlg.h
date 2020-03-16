#pragma once

#include "glob_param.h"
// total_machine_by_option_dlg 对话框

class total_machine_by_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(total_machine_by_option_dlg)

public:
	total_machine_by_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~total_machine_by_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_total_machine_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	struct total_machine_by_option_str total_machine_by_option_instance;
	void get_param(struct glob_param *g_p);
	void output_param(struct total_machine_by_option_str *bc_p);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck3();
};
