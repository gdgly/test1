#pragma once

#include "glob_param.h"
#include "afxwin.h"
// general_option_dlg 对话框

class general_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(general_option_dlg)

public:
	general_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~general_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_general_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox comb_ttybaud;
	struct general_option_str general_option_instance;
	CString ini_path;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void get_param(struct glob_param *g_p);
	void output_param(struct general_option_str*g_p);
	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedCheck1();
	CComboBox m_mes_type;
	afx_msg void OnCbnSelchangeCombo9();
};
