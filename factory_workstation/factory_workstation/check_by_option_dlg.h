#pragma once

#include "glob_param.h"
#include "afxwin.h"
// check_by_option_dlg 对话框

class check_by_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(check_by_option_dlg)

public:
	check_by_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~check_by_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_check_by_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct check_by_option_str check_by_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct check_by_option_str *bc_p);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck4();
	virtual BOOL OnInitDialog();
	CComboBox comb_current_ttybaud;
};
