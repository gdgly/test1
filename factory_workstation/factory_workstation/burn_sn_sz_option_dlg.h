#pragma once

#include "glob_param.h"
// burn_sn_sz_option_dlg �Ի���

class burn_sn_sz_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(burn_sn_sz_option_dlg)

public:
	burn_sn_sz_option_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~burn_sn_sz_option_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_sz_sn_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	void get_param(struct glob_param *g_p);
	void output_param(struct burnsn_sz_option_str *bc_p);
	struct burnsn_sz_option_str burnsn_sz_option_instance;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
};
