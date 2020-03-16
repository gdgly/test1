#pragma once


// about_box 对话框

class about_box : public CDialogEx
{
	DECLARE_DYNAMIC(about_box)

public:
	about_box(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~about_box();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
