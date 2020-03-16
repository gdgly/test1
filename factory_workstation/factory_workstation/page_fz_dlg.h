#pragma once

#include "software_burn.h"
#include "software_check_fz.h"
#include "software_check_jn.h"
#include "software_bluetooth_fz.h"
#include "software_air_fz.h"
#include "software_batpower.h"
#include "software_charge_fzjn.h"
// page_fz_dlg �Ի���

class page_fz_dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(page_fz_dlg)

public:
	page_fz_dlg();
	virtual ~page_fz_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_fz };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	void *glob_param_p;
	void set_param(void *p);
	void page_fz_dlg::refresh_view();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	virtual BOOL OnInitDialog();
};
