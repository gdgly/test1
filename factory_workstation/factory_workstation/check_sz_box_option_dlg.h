#pragma once

#include "glob_param.h"
// check_sz_box_option_dlg 对话框

class check_sz_box_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(check_sz_box_option_dlg)

public:
	check_sz_box_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~check_sz_box_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_check_box_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct checkbox_sz_option_str checkbox_sz_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct checkbox_sz_option_str *bc_p);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck14();
	afx_msg void OnBnClickedCheck17();
};
