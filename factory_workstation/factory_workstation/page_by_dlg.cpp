// page_by_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "page_by_dlg.h"
#include "afxdialogex.h"
#include "factory_workstationDlg.h"
#include "software_by_burn.h"
#include "software_check_by.h"
#include "software_check_by_with_wifibt.h"
#include "software_total_machine_by.h"
#include "software_air_by.h"
#include "software_by_before_press_current.h"
#include "software_batpower_by.h"

// page_by_dlg 对话框

IMPLEMENT_DYNAMIC(page_by_dlg, CDialogEx)

page_by_dlg::page_by_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(page_by_dlg::IDD, pParent)
{

}

page_by_dlg::~page_by_dlg()
{
}

void page_by_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(page_by_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &page_by_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &page_by_dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &page_by_dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &page_by_dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &page_by_dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON19, &page_by_dlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON24, &page_by_dlg::OnBnClickedButton24)
END_MESSAGE_MAP()


void page_by_dlg::set_param(void *p)
{
	glob_param_p = p;
}

// page_by_dlg 消息处理程序

void page_by_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_BURN;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_by_burn  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_by_dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_CHECK;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_check_by  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_by_dlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_FULL_MACHINE;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_total_machine_by  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}


void page_by_dlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_AIR;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_air_by  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_by_dlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_BP_CURRENT;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_by_before_press_current  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_by_dlg::OnBnClickedButton19()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_CHECK_WIFIBT;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_check_by_with_wifibt  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_by_dlg::OnBnClickedButton24()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText = CHILD_DLG_NAME_BY_BATPOWER;
	ini_path = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.sFName;
	ini.SetPathName(ini_path);
	ini.WriteString("GLOBLE" ,"last_sel_soft", sText);
	( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft = sText;
	//if( !((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	software_batpower_by  Dlg; 
	Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
	Dlg.DoModal();
}

void page_by_dlg::refresh_view()
{
	if(!(( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_if_open_nouse))
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE);
	else
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);
}

BOOL page_by_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(0);
	if(!(( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_if_open_nouse))
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}


