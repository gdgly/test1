
// ProductCaseToolsDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "CtrlMain.h"

// CProductCaseToolsDlg 对话框
class CProductCaseToolsDlg : public CDialogEx
{
// 构造
public:
	CProductCaseToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PRODUCTCASETOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
		
public:
	CCtrlMain m_CtrlMain;
	afx_msg LRESULT OnCtrlDevice(WPARAM wParam, LPARAM lParam);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	UINT m_iPort;
	CListCtrl m_ListCtrl;
	afx_msg void OnNMDblclkListCheckdev(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
};
