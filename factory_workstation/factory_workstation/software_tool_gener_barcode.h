#pragma once

#include "glob_param.h"
#include "ContrlComm_bluetooth.h"
#include "protocol.h"
#include "afxwin.h"
#include "barcode.h"
// software_tool_gener_barcode �Ի���

class software_tool_gener_barcode : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_gener_barcode)

public:
	software_tool_gener_barcode(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~software_tool_gener_barcode();

// �Ի�������
	enum { IDD = IDD_DIALOG_gener_barcode };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	barcode m_barcode;
	afx_msg void OnBnClickedButton3();
	CEdit m_edit;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
};
