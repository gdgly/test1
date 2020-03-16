// page_fz_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "page_fz_dlg.h"
#include "afxdialogex.h"
#include "factory_workstationDlg.h"

// page_fz_dlg 对话框

IMPLEMENT_DYNAMIC(page_fz_dlg, CPropertyPage)

page_fz_dlg::page_fz_dlg()
	: CPropertyPage(page_fz_dlg::IDD)
{

}

page_fz_dlg::~page_fz_dlg()
{
}

void page_fz_dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(page_fz_dlg, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON3, &page_fz_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &page_fz_dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &page_fz_dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &page_fz_dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &page_fz_dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON19, &page_fz_dlg::OnBnClickedButton19)
END_MESSAGE_MAP()


// page_fz_dlg 消息处理程序
void page_fz_dlg::set_param(void *p)
{
	glob_param_p = p;
}

void page_fz_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_FZ_BURN;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_burn  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_fz_dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_FZ_CHECK;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_check_fz  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_fz_dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_FZ_BLUETOOTH;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_bluetooth_fz  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_fz_dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_FZ_AIR;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_air_fz  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_fz_dlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_FZ_POWER;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_batpower  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_fz_dlg::OnBnClickedButton19()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_FZ_CHARGE;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_charge_fzjn Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_fz_dlg::refresh_view()
{
	if(!(( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_if_open_nouse))
		((CButton*)GetDlgItem(IDC_BUTTON19))->EnableWindow(FALSE);
	else
		((CButton*)GetDlgItem(IDC_BUTTON19))->EnableWindow(TRUE);
}

BOOL page_fz_dlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if(!(( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_if_open_nouse))
		((CButton*)GetDlgItem(IDC_BUTTON19))->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
