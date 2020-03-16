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
#include <mmsystem.h>  
#include "ColorStatic.h"
// software_check_fz 对话框

class software_check_fz : public CDialogEx
{
	DECLARE_DYNAMIC(software_check_fz)

public:
	software_check_fz(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_check_fz();

// 对话框数据
	enum { IDD = IDD_DIALOG_check_fz };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	int pass_green_color;
	CFile m_hLogFile;
	int fz_jn_flag;//1 fz, 2 jn
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct burncheck_option_str burncheck_option_instance;
	volatile MassProdInfo m_MPInfo;

	void get_param(struct glob_param *g_p);
	static int WorkerThreadProc(void *param);
	static int PlaysoundThreadProc(void *param);
	void CheckNewStatus(MPIPtr mpi, MsgInfo *pmsg);
	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	void AddEvent2List(CString sText);
	void SetAllItemRestart(void);
	void CheckUserSelectAud(int iSelectNo);

	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton13();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	CFont m_editFont;
	CStatic m_Epass;
	CButton current_checkBox;
	CButton sleep_current_checkBox;
	CButton sleep_current_delay_checkBox;
	CListBox m_listEvt;
	CEdit m_EditSn;
	CStatic m_readSn;
	CStatic m_EditBtAddr;
	CColorStatic m_EditCurrect;
	CColorStatic m_normalCurrect;
	CColorStatic m_sleepCurrect;
	CColorStatic m_btnBoot;
	CColorStatic m_btnL35;
	CColorStatic m_btnPA;
	CColorStatic m_btnTouch;
	CColorStatic m_btnKeyU;
	CColorStatic m_btnKeyD;
	CColorStatic m_btnKeyHW;
	CColorStatic m_btnAud;
	CColorStatic m_snRead;
	afx_msg void OnBnClickedRadioMusic1();
	afx_msg void OnBnClickedRadioMusic2();
	afx_msg void OnBnClickedRadioMusic3();
	afx_msg void OnBnClickedRadioMusic4();
	afx_msg void OnBnClickedRadioMusic5();
	afx_msg void OnBnClickedRadioMusic6();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


extern CString get_firmwarefile_version(CString file_name);
extern bool check_firmware_canuse_v2sn(CString g_wrFirm);
extern void MassProdReinit(MPIPtr mpi);        //初始化
extern void ChangeNewStatuc(MPIPtr  mpi, UINT status, int line);

