#pragma once

// default_dlg 对话框

class default_dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(default_dlg)

public:
	default_dlg();
	virtual ~default_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_default };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_editFont;
	void *glob_param_p;
	void set_param(void *p);
	afx_msg void OnBnClickedgoon();
	virtual BOOL OnInitDialog();
};
