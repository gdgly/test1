#pragma once
#include "afxwin.h"

#include "DeviceCtrl.h"
#include "glob_param.h"
// software_tool_sz_btaddr 对话框

class software_tool_sz_btaddr : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_sz_btaddr)

public:
	software_tool_sz_btaddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_sz_btaddr();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_local_btaddr_tool };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	void software_tool_sz_btaddr::AddEvent2List(CString sText);

	int thread_exit_flag;int rw_flag;
	CDeviceCtrl m_devCtrl;
	static int software_tool_sz_btaddr::WorkerThreadProc(void *param);
	LRESULT software_tool_sz_btaddr::OnDevCtrlReport(WPARAM wParam, LPARAM lParam);
	LRESULT software_tool_sz_btaddr::OnDevCtrlError(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CListBox m_listEvt;
	CEdit m_btaddr;
	CEdit m_btname;
	CEdit m_hw_version;
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton2();
};
