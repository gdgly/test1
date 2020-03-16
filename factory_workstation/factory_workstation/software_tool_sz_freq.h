#pragma once

#include "glob_param.h"
#include "DeviceCtrl.h"
#include "afxwin.h"
// software_tool_sz_freq 对话框

class software_tool_sz_freq : public CDialogEx
{
	DECLARE_DYNAMIC(software_tool_sz_freq)

public:
	software_tool_sz_freq(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_tool_sz_freq();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_freq };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int thread_exit_flag;int rw_flag;
	CDeviceCtrl m_devCtrl;
	static int WorkerThreadProc(void *param);
	LRESULT software_tool_sz_freq::OnDevCtrlReport(WPARAM wParam, LPARAM lParam);
	LRESULT software_tool_sz_freq::OnDevCtrlError(WPARAM wParam, LPARAM lParam);

	CListBox m_listEvt;
	CEdit m_cap;
	CEdit m_trim;

	void software_tool_sz_freq::AddEvent2List(CString sText);

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	virtual BOOL OnInitDialog();
};
