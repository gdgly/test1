// page_sz_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "page_sz_dlg.h"
#include "afxdialogex.h"
#include "factory_workstationDlg.h"
#include "software_burn_sz.h"
#include "software_check_sz.h"
#include "software_sz_btaddr.h"
#include "software_sz_checkbox.h"

// page_sz_dlg 对话框
IMPLEMENT_DYNAMIC(page_sz_dlg, CDialogEx)

page_sz_dlg::page_sz_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(page_sz_dlg::IDD, pParent)
{

}

page_sz_dlg::~page_sz_dlg()
{
}

void page_sz_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(page_sz_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &page_sz_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &page_sz_dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &page_sz_dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &page_sz_dlg::OnBnClickedButton6)
END_MESSAGE_MAP()

// page_sz_dlg 消息处理程序
void page_sz_dlg::set_param(void *p)
{
	glob_param_p = p;
}

void page_sz_dlg::refresh_view()
{
	if(!(( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_if_open_nouse)){
	
	}else{
	
	}
}

void page_sz_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_SZ_BURN;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_burn_sz  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_sz_dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_SZ_CHECK;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_check_sz  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_sz_dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_SZ_BTADDR;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_sz_btaddr  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_sz_dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_SZ_CHECKBOX;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_sz_checkbox  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


BOOL page_sz_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(!(( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_if_open_nouse)){
	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}



