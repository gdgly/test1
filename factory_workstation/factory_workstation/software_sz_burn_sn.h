#pragma once

#include "glob_param.h"
#include "ColorStatic.h"
#include "MyComm.h"
#include "afxwin.h"
// software_sz_burn_sn 对话框

class software_sz_burn_sn : public CDialogEx
{
	DECLARE_DYNAMIC(software_sz_burn_sn)

public:
	software_sz_burn_sn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_sz_burn_sn();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_sn };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct burnsn_sz_option_str burnsn_sz_option_instance;
	void software_sz_burn_sn::get_param(struct glob_param *g_p);

	CColorStatic m_sn_read_l;
	CColorStatic m_sn_read_r;
	CColorStatic m_sn_read_c;
	CFile m_hLogFile;
	CFont m_editFont;
	CEdit edit_out;
	CListBox m_listEvt;
	CStatic m_Epass;
	CEdit m_EditSn_l;
	CEdit m_EditSn_r;
	CEdit m_EditSn_c;

	LRESULT OnMSGMassProduct(WPARAM wParam, LPARAM lParam);
	void software_sz_burn_sn::AddEvent2List(CString sText);
	int pass_green_color;
	void software_sz_burn_sn::SetAllItemRestart(void);

	CMyComm com_board;
	UINT          bufReaded;
#define COM_BUF_SIZE            (2048)
	BYTE          bufCom[COM_BUF_SIZE];
	unsigned int error_flag,right_flag;
typedef int (*Com_callback_type)( void *param, BYTE *buf, int len );
	int software_sz_burn_sn::Parse(Com_callback_type com_cb_ins);
	int software_sz_burn_sn::Process(Com_callback_type com_cb_ins);
	static int software_sz_burn_sn::ParseLine(void *param, BYTE *buf, int len);

	int left_suc_flag;int right_suc_flag;int case_suc_flag;
	int burn_flag;int com_get_recv;
	int software_sz_burn_sn::process_com();
	int WorkerThreadProc_in_flag;
	static int software_sz_burn_sn::WorkerThreadProc(void *param);

	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit15();
	afx_msg void OnEnChangeEdit3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
