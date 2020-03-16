#pragma once

#include "glob_param.h"
#include "afxwin.h"
// batpower_by_option_dlg 对话框

class batpower_by_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(batpower_by_option_dlg)

public:
	batpower_by_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~batpower_by_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_by_batpower_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct batpower_by_option_str batpower_by_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct batpower_by_option_str *bc_p);


	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CComboBox comb_current_ttybaud;
	afx_msg void OnBnClickedCheck1();
};
