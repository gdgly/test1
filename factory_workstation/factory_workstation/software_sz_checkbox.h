#pragma once

#include "glob_param.h"
#include "afxwin.h"
#include "ColorStatic.h"
#include "MyComm.h"
// software_sz_checkbox 对话框

class software_sz_checkbox : public CDialogEx
{
	DECLARE_DYNAMIC(software_sz_checkbox)

public:
	software_sz_checkbox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~software_sz_checkbox();

// 对话框数据
	enum { IDD = IDD_DIALOG_sz_check_box };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct checkbox_sz_option_str checkbox_sz_option_instance;

	void software_sz_checkbox::get_param(struct glob_param *g_p);

	CFile m_mes_file;
	CString vol_value_sav;
	int software_sz_checkbox::mes_sannuo_write_logfile(CString SN);

	CFile m_hLogFile;
	CFont m_editFont;
	CColorStatic m_reboot;
	CColorStatic m_watchdog;
	CColorStatic m_max20340;
	CColorStatic m_max77813;
	CColorStatic m_plc_left;
	CColorStatic m_plc_right;
	CColorStatic m_open_case;
	CColorStatic m_close_case;
	CColorStatic m_key_down;
	CColorStatic m_key_up;
	CColorStatic m_usb_in;
	CColorStatic m_usb_out;
	CColorStatic m_cal_value;
	CStatic m_Epass;
	CListBox m_listEvt;
	CEdit m_EditSn;
	CEdit edit_out;

	void software_sz_checkbox::AddEvent2List(CString sText);
	int pass_green_color;
	void software_sz_checkbox::SetAllItemRestart(void);

	CString sn_no;
	CMyComm com_board;
	int process_com_exit;
	UINT          bufReaded;
#define COM_BUF_SIZE            (2048)
	BYTE          bufCom[COM_BUF_SIZE];
	unsigned int error_flag,right_flag;
typedef int (*Com_callback_type)( void *param, BYTE *buf, int len );
	static int software_sz_checkbox::ParseLine(void *param, BYTE *buf, int len);
	int software_sz_checkbox::Parse(Com_callback_type com_cb_ins);
	int software_sz_checkbox::Process(Com_callback_type com_cb_ins);
	int software_sz_checkbox::process_com();
	UINT WorkerThreadProc_in_flag;
	static int software_sz_checkbox::WorkerThreadProc(void *param);

	static int software_sz_checkbox::ParseLine_cal(void *param, BYTE *buf, int len);
	static int software_sz_checkbox::WorkerThreadProc_cal(void *param);

	afx_msg void OnBnClickedButton1();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedRadioMusic1();
	afx_msg void OnBnClickedRadioMusic2();
	afx_msg void OnBnClickedRadioMusic3();
	afx_msg void OnBnClickedRadioMusic4();
	afx_msg void OnBnClickedButton2();
};
