#pragma once

#include "glob_param.h"
#include "afxwin.h"
// charge_check_option_dlg 对话框

class charge_check_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(charge_check_option_dlg)

public:
	charge_check_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~charge_check_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_charge_check_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct charge_option_str charge_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct charge_option_str *bc_p);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CComboBox comb_current_ttybaud;
};
