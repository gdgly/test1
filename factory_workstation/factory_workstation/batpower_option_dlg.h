#pragma once

#include "glob_param.h"
// batpower_option_dlg �Ի���

class batpower_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(batpower_option_dlg)

public:
	batpower_option_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~batpower_option_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_batpower_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	struct batpower_option_str batpower_option_instance;
	CString ini_path;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void get_param(struct glob_param *g_p);
	void output_param(struct batpower_option_str*bc_p);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
};
