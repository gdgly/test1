#pragma once

#include "glob_param.h"
// air_by_option_dlg �Ի���

class air_by_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(air_by_option_dlg)

public:
	air_by_option_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~air_by_option_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_air_by_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString ini_path;
	struct air_by_option_str air_by_option_instance;
	void get_param(struct glob_param *g_p);
	void output_param(struct air_by_option_str *bc_p);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck3();
	virtual BOOL OnInitDialog();
};
