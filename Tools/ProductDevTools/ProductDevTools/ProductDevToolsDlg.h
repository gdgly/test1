
// ProductDevToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include "DeviceCtrl.h"
#include "afxcmn.h"

// CProductDevToolsDlg �Ի���
class CProductDevToolsDlg : public CDialogEx
{
// ����
public:
	CProductDevToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRODUCTDEVTOOLS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	uint32  m_devHandle;
	CDeviceCtrl m_devCtrl;

	afx_msg LRESULT OnDevCtrlError(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDevCtrlReport(WPARAM wParam, LPARAM lParam);

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
};



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int32 open_ps(int32 port,int32 device);


#ifdef __cplusplus
}
#endif /* __cplusplus */


