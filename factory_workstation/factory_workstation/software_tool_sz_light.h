#pragma once

#include "DeviceCtrl.h"
#include "afxwin.h"
#include "glob_param.h"
// software_tool_sz_light �Ի���

class software_tool_sz_light : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_sz_light)

public:
	software_tool_sz_light(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~software_tool_sz_light();

// �Ի�������
	enum { IDD = IDD_DIALOG_light_tool };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int thread_exit_flag;
	int thread_exit;
	int value;
	CDeviceCtrl m_devCtrl;
	static int WorkerThreadProc(void *param);
	CListBox m_listEvt;
	void AddEvent2List(CString sText);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
