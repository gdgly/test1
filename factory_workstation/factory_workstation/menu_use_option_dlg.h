#pragma once


// menu_use_option_dlg 对话框

class menu_use_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(menu_use_option_dlg)

public:
	menu_use_option_dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~menu_use_option_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_readme };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CFont m_editFont;
	virtual BOOL OnInitDialog();
};
