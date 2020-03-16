// total_machine_by_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "total_machine_by_option_dlg.h"
#include "afxdialogex.h"


// total_machine_by_option_dlg 对话框

IMPLEMENT_DYNAMIC(total_machine_by_option_dlg, CDialogEx)

total_machine_by_option_dlg::total_machine_by_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(total_machine_by_option_dlg::IDD, pParent)
{

}

total_machine_by_option_dlg::~total_machine_by_option_dlg()
{
}

void total_machine_by_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(total_machine_by_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &total_machine_by_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &total_machine_by_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &total_machine_by_option_dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK3, &total_machine_by_option_dlg::OnBnClickedCheck3)
END_MESSAGE_MAP()

void total_machine_by_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	total_machine_by_option_instance = g_p->total_machine_by_option_instance;
}

void total_machine_by_option_dlg::output_param(struct total_machine_by_option_str *bc_p)
{
	*bc_p = total_machine_by_option_instance;
}
// total_machine_by_option_dlg 消息处理程序

void total_machine_by_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		total_machine_by_option_instance.g_connect_type = 1;//usb
	}else if( ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck() ){
		total_machine_by_option_instance.g_connect_type = 2;//wifi
	}else{
		total_machine_by_option_instance.g_connect_type = 3;//都没选
	}

	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_usb_timeout);
	GetDlgItem(IDC_EDIT12)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_cali_min_value);
	GetDlgItem(IDC_EDIT13)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_cali_max_value);
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_ts_err_times);
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_ts_pix_off);
	GetDlgItem(IDC_EDIT11)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_record_time_sec);
	GetDlgItem(IDC_EDIT16)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_record_passdb);
	GetDlgItem(IDC_EDIT17)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_record_subdb);
	GetDlgItem(IDC_EDIT14)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_play_time_sec);
	GetDlgItem(IDC_EDIT33)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_play_passdb);

	GetDlgItem(IDC_EDIT36)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_wifi_timeout);
	GetDlgItem(IDC_EDIT37)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_wifi_speed);
	GetDlgItem(IDC_EDIT18)->GetWindowTextA(total_machine_by_option_instance.g_wifi_ip);
	GetDlgItem(IDC_EDIT34)->GetWindowTextA(total_machine_by_option_instance.g_wifi_ssid);
	GetDlgItem(IDC_EDIT35)->GetWindowTextA(total_machine_by_option_instance.g_wifi_passwd);

	GetDlgItem(IDC_EDIT39)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_wifihot_timeout);
	GetDlgItem(IDC_EDIT20)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_ip);
	//GetDlgItem(IDC_EDIT37)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_ssid);
	GetDlgItem(IDC_EDIT38)->GetWindowTextA(total_machine_by_option_instance.g_wifihot_passwd);
	GetDlgItem(IDC_EDIT40)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_wifihot_adb_timeout);
	GetDlgItem(IDC_EDIT41)->GetWindowTextA(total_machine_by_option_instance.g_local_net_name);
	GetDlgItem(IDC_EDIT42)->GetWindowTextA(total_machine_by_option_instance.g_local_ip);
	GetDlgItem(IDC_EDIT43)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&total_machine_by_option_instance.g_pass_bat_power);
	GetDlgItem(IDC_EDIT25)->GetWindowTextA(total_machine_by_option_instance.g_bluetooth_board_mac);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_connect_type",total_machine_by_option_instance.g_connect_type,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_usb_timeout",total_machine_by_option_instance.g_usb_timeout,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_cali_min_value",total_machine_by_option_instance.g_cali_min_value,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_cali_max_value",total_machine_by_option_instance.g_cali_max_value,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_ts_err_times",total_machine_by_option_instance.g_ts_err_times,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_ts_pix_off",total_machine_by_option_instance.g_ts_pix_off,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_record_time_sec",total_machine_by_option_instance.g_record_time_sec,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_record_passdb",total_machine_by_option_instance.g_record_passdb,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_record_subdb",total_machine_by_option_instance.g_record_subdb,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_play_time_sec",total_machine_by_option_instance.g_play_time_sec,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_play_passdb",total_machine_by_option_instance.g_play_passdb,10);

	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_wifi_timeout",total_machine_by_option_instance.g_wifi_timeout,10);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_wifi_speed",total_machine_by_option_instance.g_wifi_speed,10);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_wifi_ip",total_machine_by_option_instance.g_wifi_ip);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_wifi_ssid",total_machine_by_option_instance.g_wifi_ssid);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_wifi_passwd",total_machine_by_option_instance.g_wifi_passwd);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_wifihot_timeout",total_machine_by_option_instance.g_wifihot_timeout,10);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_wifihot_ip",total_machine_by_option_instance.g_wifihot_ip);
	//ini.WriteString("BY_TOTAL_MACHINE" ,"g_wifihot_ssid",total_machine_by_option_instance.g_wifihot_ssid);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_wifihot_passwd",total_machine_by_option_instance.g_wifihot_passwd);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_wifihot_adb_timeout",total_machine_by_option_instance.g_wifihot_adb_timeout,10);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_local_net_name",total_machine_by_option_instance.g_local_net_name);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_local_ip",total_machine_by_option_instance.g_local_ip);
	ini.WriteUInt("BY_TOTAL_MACHINE" ,"g_pass_bat_power",total_machine_by_option_instance.g_pass_bat_power,10);
	ini.WriteString("BY_TOTAL_MACHINE" ,"g_bluetooth_board_mac",total_machine_by_option_instance.g_bluetooth_board_mac);

	CDialogEx::OnOK();
}


void total_machine_by_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL total_machine_by_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sText;
	sText.Format("%d", total_machine_by_option_instance.g_usb_timeout);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_cali_min_value);
	GetDlgItem(IDC_EDIT12)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_cali_max_value);
	GetDlgItem(IDC_EDIT13)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_ts_err_times);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_ts_pix_off);
	GetDlgItem(IDC_EDIT10)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_record_time_sec);
	GetDlgItem(IDC_EDIT11)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_record_passdb);
	GetDlgItem(IDC_EDIT16)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_record_subdb);
	GetDlgItem(IDC_EDIT17)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_play_time_sec);
	GetDlgItem(IDC_EDIT14)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_play_passdb);
	GetDlgItem(IDC_EDIT33)->SetWindowTextA(sText);

	sText.Format("%d", total_machine_by_option_instance.g_wifi_timeout);
	GetDlgItem(IDC_EDIT36)->SetWindowTextA(sText);
	sText.Format("%d", total_machine_by_option_instance.g_wifi_speed);
	GetDlgItem(IDC_EDIT37)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT18)->SetWindowTextA(total_machine_by_option_instance.g_wifi_ip);
	GetDlgItem(IDC_EDIT34)->SetWindowTextA(total_machine_by_option_instance.g_wifi_ssid);
	GetDlgItem(IDC_EDIT35)->SetWindowTextA(total_machine_by_option_instance.g_wifi_passwd);
	sText.Format("%d", total_machine_by_option_instance.g_wifihot_timeout);
	GetDlgItem(IDC_EDIT39)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT20)->SetWindowTextA(total_machine_by_option_instance.g_wifihot_ip);
	//GetDlgItem(IDC_EDIT37)->SetWindowTextA(total_machine_by_option_instance.g_wifihot_ssid);
	GetDlgItem(IDC_EDIT38)->SetWindowTextA(total_machine_by_option_instance.g_wifihot_passwd);
	sText.Format("%d", total_machine_by_option_instance.g_wifihot_adb_timeout);
	GetDlgItem(IDC_EDIT40)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT41)->SetWindowTextA(total_machine_by_option_instance.g_local_net_name);
	GetDlgItem(IDC_EDIT42)->SetWindowTextA(total_machine_by_option_instance.g_local_ip);
	sText.Format("%d", total_machine_by_option_instance.g_pass_bat_power);
	GetDlgItem(IDC_EDIT43)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT25)->SetWindowTextA(total_machine_by_option_instance.g_bluetooth_board_mac);

	if(total_machine_by_option_instance.g_connect_type == 1){//usb
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);
	}else if(total_machine_by_option_instance.g_connect_type == 2){//wifi
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(FALSE);	
	}

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT18))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT34))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT35))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT36))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT37))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT18))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT34))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT35))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT36))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT37))->EnableWindow(FALSE);	
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


void total_machine_by_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT18))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT34))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT35))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT36))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT37))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT18))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT34))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT35))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT36))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT37))->EnableWindow(FALSE);	
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


void total_machine_by_option_dlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT18))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT34))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT35))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT36))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT37))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT18))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT34))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT35))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT36))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT37))->EnableWindow(FALSE);	
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
