#pragma once

#include "glob_param.h"
#include "ContrlComm_bluetooth.h"
#include "protocol.h"
#include "afxwin.h"
#include "barcode.h"
// software_tool_barcode �Ի���

class software_tool_barcode : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_barcode)

public:
	software_tool_barcode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~software_tool_barcode();

// �Ի�������
	enum { IDD = IDD_DIALOG_barcode };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	barcode m_barcode;
	CString m_sSN;
	CString m_sMac;
	CContrlComm_bluetooth m_ctlCom;
	struct general_option_str general_option_instance;

	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton3();
	CStatic m_sMacAddr;
	CStatic m_snValue;
};
