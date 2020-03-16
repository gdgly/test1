// menu_use_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "menu_use_option_dlg.h"
#include "afxdialogex.h"


// menu_use_option_dlg 对话框

IMPLEMENT_DYNAMIC(menu_use_option_dlg, CDialogEx)

menu_use_option_dlg::menu_use_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(menu_use_option_dlg::IDD, pParent)
{

}

menu_use_option_dlg::~menu_use_option_dlg()
{
}

void menu_use_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(menu_use_option_dlg, CDialogEx)
END_MESSAGE_MAP()


// menu_use_option_dlg 消息处理程序


BOOL menu_use_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sText;
	m_editFont.CreatePointFont(100, "宋体");
	GetDlgItem(IDC_STATIC)->SetFont(&m_editFont);
	sText = "工站使用方法:\n\n";
	sText += "  1、打开软件后，首先点设置设置一下需要的参数，然后点相应的工站进入。\n";
	sText += "  2、打开工站后，扫码或输入序列号即开始工作，直到有pass或者fail出现。\n";
	sText += "      注意：在未完成时请不要重新扫码,卡死情况下请关闭软件后重试！\n";
	sText += "  3、默认的主页面有按钮直接打开上次打开的工站。\n";
	sText += "  4、部分工站调用了第三方软件对中文支持的不是很好，因此路径中不要包含中文。\n";
	sText += "\n注意事项:\n\n";
	sText += "  小工具里面用到的串口都在通用设置里面设置。\n";
	GetDlgItem(IDC_STATIC)->SetWindowTextA(sText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
