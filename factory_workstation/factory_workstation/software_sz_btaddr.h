#pragma once
#include "afxwin.h"

#include "glob_param.h"
#include "CurlCtrl.h"
#include "DeviceCtrl.h"


// software_sz_btaddr 对话框

class software_sz_btaddr : public CDialogEx
{
	DECLARE_DYNAMIC(software_sz_btaddr)

public:
	software_sz_btaddr(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_sz_btaddr();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_btaddr };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDeviceCtrl m_devCtrl;
	CString sn_no;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct btaddr_sz_option_str btaddr_sz_option_instance;
	void software_sz_btaddr::get_param(struct glob_param *g_p);
	void software_sz_btaddr::AddEvent2List(CString sText);
	static int WorkerThreadProc(void *param);
	CCurlCtrl     mesInfo;
	LRESULT software_sz_btaddr::OnDevCtrlError(WPARAM wParam, LPARAM lParam);
	LRESULT software_sz_btaddr::OnDevCtrlReport(WPARAM wParam, LPARAM lParam);

	int err_flag;
	int pass_green_color;
	CFile m_hLogFile;
	CFont m_editFont;

	CStatic m_Epass;
	CEdit m_EditSn;
	CListBox m_listEvt;
	CEdit m_btaddr;
	CEdit m_btname;
	CEdit m_hw_version;
	CEdit m_lr_flag;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
