#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ContrlComm_bluetooth.h"
#include "protocol.h"
#include "CurlCtrl.h"
#include "G722Play.h"
// software_bluetooth_fz �Ի���

class software_bluetooth_fz : public CDialogEx
{
	DECLARE_DYNAMIC(software_bluetooth_fz)

public:
	software_bluetooth_fz(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~software_bluetooth_fz();

// �Ի�������
	enum { IDD = IDD_DIALOG_bluetooth_fz };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CCurlCtrl     mesInfo;
	char Mesresring[256];
	int pass_green_color;
	int calibration_flag;
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct bluetooth_option_str bluetooth_option_instance;
	UINT m_ctlRuning;
	CContrlComm_bluetooth m_ctlCom;

	CString	m_sFileName;
	G722Play m_G722Play;
	CString aud_right_file_path;
	CString aud_left_file_path;
	CFile aud_right_f;
	CFile aud_left_f;
	int aud_right_db;
	int aud_left_db;
	UINT audMsec_right;
	UINT audMsec_left;

	int m_iTFA8686val;
	CString m_sSN;
	CString m_sMac;
	CString m_setSN;

	HANDLE        hThread;
	DWORD         idThread;
	void dlg_pram_init();
	void get_param(struct glob_param *g_p);
	bool WorkerThreadProc_exit;
	static int WorkerThreadProc(void *param);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	INT MesPass();
	void AddEvent2List(CString sText);

	CFile m_hLogFile;
	CFile m_hLog_calibration_file;
	CFont m_editFont;
	CStatic m_ECpass;
	CListBox m_listEvt;
	CEdit m_Esetsn;
	CStatic m_sMacAddr;
	CStatic m_snValue;
	CStatic m_iTFAVal;
	//CStatic m_Ecycle;
	CStatic m_audiodb_right;
	CStatic m_audiodb_left;
	CListBox list_audio;

	afx_msg void OnBnClickedButton13();
	afx_msg void OnEnChangeSetSn();
	virtual BOOL OnInitDialog();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton20();
};
