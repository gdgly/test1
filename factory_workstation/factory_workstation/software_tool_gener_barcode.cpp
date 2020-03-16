// software_tool_gener_barcode.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_gener_barcode.h"
#include "afxdialogex.h"


// software_tool_gener_barcode 对话框

IMPLEMENT_DYNAMIC(software_tool_gener_barcode, CDialogEx)

software_tool_gener_barcode::software_tool_gener_barcode(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_gener_barcode::IDD, pParent)
{

}

software_tool_gener_barcode::~software_tool_gener_barcode()
{
}

void software_tool_gener_barcode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(software_tool_gener_barcode, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &software_tool_gener_barcode::OnBnClickedButton3)
END_MESSAGE_MAP()


// software_tool_gener_barcode 消息处理程序


void software_tool_gener_barcode::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	CRect rect;
	CDC *pDC;
	CWnd *pWnd;
	m_edit.GetWindowTextA(sText);
	pWnd=GetDlgItem(IDC_STATIC_barcode); //获得pictrue控件窗口的句柄
	pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域
	pDC=pWnd->GetDC(); //获得pictrue控件的DC
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	m_barcode.out_draw_bar(pDC, rect, sText);
}


BOOL software_tool_gener_barcode::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


BOOL software_tool_gener_barcode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "条码生成工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	return TRUE;  // return TRUE unless you set the focus to a control
}
