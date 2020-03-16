#pragma once

#include "glob_param.h"
// bluetooth_option_dlg �Ի���

class bluetooth_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(bluetooth_option_dlg)

public:
	bluetooth_option_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~bluetooth_option_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_bluetooth_option };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	struct bluetooth_option_str bluetooth_option_instance;
	CString ini_path;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void get_param(struct glob_param *g_p);
	void output_param(struct bluetooth_option_str*bc_p);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck1();
};
