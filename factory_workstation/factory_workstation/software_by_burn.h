#pragma once

#include "libusb/win32/libusb.h"
#include "libusb/win32/libusb_devlistener.h"
#include "glob_param.h"
#include "afxwin.h"
#include "afxcmn.h"
// software_by_burn 对话框

class software_by_burn : public CDialogEx
{
	DECLARE_DYNAMIC(software_by_burn)

public:
	software_by_burn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_by_burn();

// 对话框数据
	enum { IDD = IDD_DIALOG_burn_by };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int pass_green_color;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct burn_by_option_str burn_by_option_instance;
	void get_param(struct glob_param *g_p);
	void AddEvent2List(CString sText);
	void err_process(msg_info *pmsg);
	void progress_setpos(msg_info *pmsg);
	void all_init();
	int need_file_ifexit();
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	libusbDevListener libusbDevListener_int;

	CFile m_hLogFile;
	CFont m_editFont;
	CStatic m_Epass;
	CProgressCtrl init_progress;
	CProgressCtrl part1_progress;
	CProgressCtrl part2_progress;
	CProgressCtrl part3_progress;
	CProgressCtrl part4_progress;
	CProgressCtrl part5_progress;
	CProgressCtrl part6_progress;
	CProgressCtrl part7_progress;
	CProgressCtrl part8_progress;
	CEdit m_EditSn;
	CStatic m_kernel_version;
	CStatic m_fs_version;
	CListBox m_listEvt;

	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnChangeEdit2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
