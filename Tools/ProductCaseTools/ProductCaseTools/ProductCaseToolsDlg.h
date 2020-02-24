
// ProductCaseToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "CtrlMain.h"

// CProductCaseToolsDlg �Ի���
class CProductCaseToolsDlg : public CDialogEx
{
// ����
public:
	CProductCaseToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PRODUCTCASETOOLS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
		
public:
	CCtrlMain m_CtrlMain;
	afx_msg LRESULT OnCtrlDevice(WPARAM wParam, LPARAM lParam);

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
	UINT m_iPort;
	CListCtrl m_ListCtrl;
	afx_msg void OnNMDblclkListCheckdev(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
};
