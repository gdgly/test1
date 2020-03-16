#pragma once
#include "afxwin.h"

#include "glob_param.h"
#include "ContrlComm_bluetooth.h"
#include "protocol.h"
// software_tool_version �Ի���

class software_tool_version : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_version)

public:
	software_tool_version(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~software_tool_version();

// �Ի�������
	enum { IDD = IDD_DIALOG_version_tool };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString s_version;
	CContrlComm_bluetooth m_ctlCom;
	struct general_option_str general_option_instance;

	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	CStatic m_static_version;
};
