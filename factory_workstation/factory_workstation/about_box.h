#pragma once


// about_box �Ի���

class about_box : public CDialogEx
{
	DECLARE_DYNAMIC(about_box)

public:
	about_box(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~about_box();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
