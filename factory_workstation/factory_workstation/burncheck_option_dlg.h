#pragma once

#include "glob_param.h"
#include "afxwin.h"
// burncheck_option_dlg 对话框

class burncheck_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(burncheck_option_dlg)

public:
	burncheck_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~burncheck_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_burncheck_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	struct burncheck_option_str burncheck_option_instance;
	CString ini_path;
	CString firm_folder_path;
	CComboBox comb_current_ttybaud;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void get_param(struct glob_param *g_p);
	void output_param(struct burncheck_option_str*bc_p);
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck8();
};
