
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
	IniParam    m_Param;


	void StartDevContrl(UINT type);

	void CheckItemInit(void);

	afx_msg LRESULT OnDevCtrlError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDevCtrlReport(WPARAM wParam, LPARAM lParam);

public:
	int OnReportCheck(CString sText, int rows = -1);

private:
	CString m_sLicense;
	BOOL Btaddr2License(void);

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
	CEdit m_edHWver;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnDestroy();
	BOOL m_bEraseAll;
	CEdit m_edName;
	afx_msg void OnBnClickedButtonPswrite();
	CListCtrl m_ListCheck;
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnBurnApo();
	afx_msg void OnBnClickedBtnBurnAddr();
	afx_msg void OnBnClickedBtnCheck();
	afx_msg void OnBnClickedBtnRecord();
	afx_msg void OnBnClickedBtnCritrim();
	afx_msg void OnBnClickedBtnRecord1();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnWakeup();
	afx_msg void OnBnClickedBtnSensor();
	afx_msg void OnBnClickedBtnCaseplc();
	afx_msg void OnBnClickedBtnTap();
	afx_msg void OnBnClickedBtnDutMode();
	afx_msg void OnBnClickedBtnPlay();
	afx_msg void OnBnClickedBtnAll();
	afx_msg void OnBnClickedBtnCritrimRead();
	afx_msg void OnBnClickedBtnCritrimWrite();
	int m_edCap;
	int m_edTrim;
	BOOL m_btWrite;
};



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32 open_ps(int32 port,int32 device);


#ifdef __cplusplus
}
#endif /* __cplusplus */


