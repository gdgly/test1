#pragma once


// menu_use_option_dlg �Ի���

class menu_use_option_dlg : public CDialogEx
{
	DECLARE_DYNAMIC(menu_use_option_dlg)

public:
	menu_use_option_dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~menu_use_option_dlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_readme };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CFont m_editFont;
	virtual BOOL OnInitDialog();
};
