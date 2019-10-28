
// ProductDevToolsDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#include "DeviceCtrl.h"
#include "afxcmn.h"

// CProductDevToolsDlg 对话框
class CProductDevToolsDlg : public CDialogEx
{
// 构造
public:
	CProductDevToolsDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCTDEVTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	uint32  m_devHandle;
	CDeviceCtrl m_devCtrl;

	afx_msg LRESULT OnDevCtrlError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDevCtrlReport(WPARAM wParam, LPARAM lParam);

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
	afx_msg void OnBnClickedOk();
	CEdit m_edSend;
	CEdit m_edRecv;
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonRecv();
	afx_msg void OnBnClickedButtonClose();
	CEdit m_edBtAddr;
	afx_msg void OnBnClickedBtnStart();
	CEdit m_edFirmName;
	afx_msg void OnBnClickedBtnOpenFile();
	CListCtrl m_ListCtrl;
	CEdit m_edAddr;
};



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32 open_ps(int32 port,int32 device);


#ifdef __cplusplus
}
#endif /* __cplusplus */


