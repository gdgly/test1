#pragma once

#include "glob_param.h"
#include "afxwin.h"
// before_press_current_by_option_dlg 对话框

class before_press_current_by_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(before_press_current_by_option_dlg)

public:
	before_press_current_by_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~before_press_current_by_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_by_current_before_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	struct bp_current_by_option_str bp_current_by_option_instance;
	void get_param(struct glob_param *g_p);
	void output_param(struct bp_current_by_option_str *bc_p);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CComboBox comb_current_ttybaud;
};
