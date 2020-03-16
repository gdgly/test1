// air_by_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "air_by_option_dlg.h"
#include "afxdialogex.h"


// air_by_option_dlg 对话框

IMPLEMENT_DYNAMIC(air_by_option_dlg, CDialogEx)

air_by_option_dlg::air_by_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(air_by_option_dlg::IDD, pParent)
{

}

air_by_option_dlg::~air_by_option_dlg()
{
}

void air_by_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(air_by_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &air_by_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &air_by_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &air_by_option_dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &air_by_option_dlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// air_by_option_dlg 消息处理程序
void air_by_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	air_by_option_instance = g_p->air_by_option_instance;
}

void air_by_option_dlg::output_param(struct air_by_option_str *bc_p)
{
	*bc_p = air_by_option_instance;
}

void air_by_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		air_by_option_instance.g_connect_type = 1;//usb
	}else if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ){
		air_by_option_instance.g_connect_type = 2;//wifi
	}else{
		air_by_option_instance.g_connect_type = 3;//都没选
	}

	GetDlgItem(IDC_EDIT11)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&air_by_option_instance.g_record_time_sec);
	GetDlgItem(IDC_EDIT17)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&air_by_option_instance.g_record_subdb);
	
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&air_by_option_instance.g_usb_timeout);
	GetDlgItem(IDC_EDIT39)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&air_by_option_instance.g_wifihot_timeout);
	GetDlgItem(IDC_EDIT20)->GetWindowTextA(air_by_option_instance.g_wifihot_ip);
	GetDlgItem(IDC_EDIT38)->GetWindowTextA(air_by_option_instance.g_wifihot_passwd);
	GetDlgItem(IDC_EDIT40)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&air_by_option_instance.g_wifihot_adb_timeout);
	GetDlgItem(IDC_EDIT41)->GetWindowTextA(air_by_option_instance.g_local_net_name);
	GetDlgItem(IDC_EDIT42)->GetWindowTextA(air_by_option_instance.g_local_ip);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BY_AIR" ,"g_connect_type",air_by_option_instance.g_connect_type,10);
	ini.WriteUInt("BY_AIR" ,"g_usb_timeout",air_by_option_instance.g_usb_timeout,10);

	ini.WriteUInt("BY_AIR" ,"g_record_time_sec",air_by_option_instance.g_record_time_sec,10);
	ini.WriteUInt("BY_AIR" ,"g_record_subdb",air_by_option_instance.g_record_subdb,10);
	ini.WriteUInt("BY_AIR" ,"g_wifihot_timeout",air_by_option_instance.g_wifihot_timeout,10);
	ini.WriteString("BY_AIR" ,"g_wifihot_ip",air_by_option_instance.g_wifihot_ip);
	ini.WriteString("BY_AIR" ,"g_wifihot_passwd",air_by_option_instance.g_wifihot_passwd);
	ini.WriteUInt("BY_AIR" ,"g_wifihot_adb_timeout",air_by_option_instance.g_wifihot_adb_timeout,10);
	ini.WriteString("BY_AIR" ,"g_local_net_name",air_by_option_instance.g_local_net_name);
	ini.WriteString("BY_AIR" ,"g_local_ip",air_by_option_instance.g_local_ip);


	CDialogEx::OnOK();
}


void air_by_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void air_by_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(TRUE);
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT38))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT39))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT41))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT42))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT38))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT39))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT41))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT42))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(TRUE);
	}
}


void air_by_option_dlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(TRUE);
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT38))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT39))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT41))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT42))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT38))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT39))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT41))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT42))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(TRUE);
	}
}


BOOL air_by_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString sText;
	sText.Format("%d", air_by_option_instance.g_usb_timeout);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);

	sText.Format("%d", air_by_option_instance.g_wifihot_timeout);
	GetDlgItem(IDC_EDIT39)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT20)->SetWindowTextA(air_by_option_instance.g_wifihot_ip);
	GetDlgItem(IDC_EDIT38)->SetWindowTextA(air_by_option_instance.g_wifihot_passwd);
	sText.Format("%d", air_by_option_instance.g_wifihot_adb_timeout);
	GetDlgItem(IDC_EDIT40)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT41)->SetWindowTextA(air_by_option_instance.g_local_net_name);
	GetDlgItem(IDC_EDIT42)->SetWindowTextA(air_by_option_instance.g_local_ip);

	sText.Format("%d", air_by_option_instance.g_record_time_sec);
	GetDlgItem(IDC_EDIT11)->SetWindowTextA(sText);
	sText.Format("%d", air_by_option_instance.g_record_subdb);
	GetDlgItem(IDC_EDIT17)->SetWindowTextA(sText);

	if(air_by_option_instance.g_connect_type == 1){//usb
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);
	}else if(air_by_option_instance.g_connect_type == 2){//wifi
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);	
	}

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(TRUE);
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT38))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT39))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT41))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT42))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT38))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT39))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT41))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT42))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(TRUE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}
