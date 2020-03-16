#pragma once

#include "glob_param.h"
#include "afxwin.h"
// burn_by_option_dlg 对话框

class burn_by_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(burn_by_option_dlg)

public:
	burn_by_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~burn_by_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_burn_by_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct burn_by_option_str burn_by_option_instance;
	CString ini_path;

	void get_param(struct glob_param *g_p);
	void output_param(struct burn_by_option_str *bc_p);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	CComboBox part1_mode;
	CComboBox part2_mode;
	CComboBox part3_mode;
	CComboBox part4_mode;
	CComboBox part5_mode;
	CComboBox part6_mode;
	CComboBox part7_mode;
	CComboBox part8_mode;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheck7();
};
