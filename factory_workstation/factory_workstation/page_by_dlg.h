#pragma once

#include "glob_param.h"
// page_by_dlg 对话框

class page_by_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(page_by_dlg)

public:
	page_by_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~page_by_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_by };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	void *glob_param_p;
	void set_param(void *p);
	void refresh_view();

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton24();
};
