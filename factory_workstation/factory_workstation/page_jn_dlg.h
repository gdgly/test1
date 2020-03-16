#pragma once

#include "software_bluetooth_jn.h"
// page_jn_dlg 对话框

class page_jn_dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(page_jn_dlg)

public:
	page_jn_dlg();
	virtual ~page_jn_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_jn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	void *glob_param_p;
	void set_param(void *p);
	void page_jn_dlg::refresh_view();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	virtual BOOL OnInitDialog();
};
