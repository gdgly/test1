#pragma once
#include "Resource.h"
#include "glob_param.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "ExecProcess.h"
#include "ContrlComm_burncheck.h"
#include "stdio.h"
#include "CurlCtrl.h"
#include "burncheck_mpi.h"

// software_burn 对话框

class software_burn : public CDialogEx
{
	DECLARE_DYNAMIC(software_burn)

public:
	software_burn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_burn();

// 对话框数据
	enum { IDD = IDD_DIALOG_burn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int fz_jn_flag;//1 fz, 2 jn
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct burncheck_option_str burncheck_option_instance;
	CFile m_hLogFile;
	int pass_green_color;
	int snok_green_color;//1 green 2 red
	volatile MassProdInfo m_MPInfo;

	CListBox m_listEvt;
	CEdit m_EditSn;
	CStatic m_EditBtAddr;
	CStatic m_snRead;
	CProgressCtrl m_WrProgress;
	CFont m_editFont;
	CStatic m_Epass;
	
	static int WorkerThreadProc(void *param);
	void AddEvent2List(CString sText);
	void Burndisplay(MPIPtr mpi, MsgInfo *pmsg);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	void SetAllItemRestart(void);
	void get_param(struct glob_param *g_p);

	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEdit2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CStatic m_readSn;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

extern CString get_firmwarefile_version(CString file_name);
extern bool check_firmware_canuse_v2sn(CString g_wrFirm);
void MassProdReinit(MPIPtr mpi);

