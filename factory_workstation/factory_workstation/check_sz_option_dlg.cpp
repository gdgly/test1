// check_sz_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "check_sz_option_dlg.h"
#include "afxdialogex.h"


// check_sz_option_dlg 对话框

IMPLEMENT_DYNAMIC(check_sz_option_dlg, CDialogEx)

check_sz_option_dlg::check_sz_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(check_sz_option_dlg::IDD, pParent)
{

}

check_sz_option_dlg::~check_sz_option_dlg()
{
}

void check_sz_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(check_sz_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &check_sz_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &check_sz_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &check_sz_option_dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK14, &check_sz_option_dlg::OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_CHECK18, &check_sz_option_dlg::OnBnClickedCheck18)
	ON_BN_CLICKED(IDC_CHECK20, &check_sz_option_dlg::OnBnClickedCheck20)
	ON_BN_CLICKED(IDC_CHECK19, &check_sz_option_dlg::OnBnClickedCheck19)
	ON_BN_CLICKED(IDC_CHECK23, &check_sz_option_dlg::OnBnClickedCheck23)
	ON_BN_CLICKED(IDC_CHECK26, &check_sz_option_dlg::OnBnClickedCheck26)
END_MESSAGE_MAP()


void check_sz_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	check_sz_option_instance = g_p->check_sz_option_instance;
}

void check_sz_option_dlg::output_param(struct check_sz_option_str *bc_p)
{
	*bc_p = check_sz_option_instance;
}
// check_sz_option_dlg 消息处理程序


void check_sz_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;

	check_sz_option_instance.g_glob_if_check_init = ((CButton *)GetDlgItem(IDC_CHECK14))->GetCheck();
	check_sz_option_instance.g_glob_if_check_light = ((CButton *)GetDlgItem(IDC_CHECK18))->GetCheck();
	check_sz_option_instance.g_glob_if_check_tap = ((CButton *)GetDlgItem(IDC_CHECK15))->GetCheck();
	check_sz_option_instance.g_glob_if_check_plc = ((CButton *)GetDlgItem(IDC_CHECK17))->GetCheck();
	check_sz_option_instance.g_glob_if_check_apollo = ((CButton *)GetDlgItem(IDC_CHECK16))->GetCheck();
	check_sz_option_instance.g_glob_if_check_record = ((CButton *)GetDlgItem(IDC_CHECK20))->GetCheck();
	check_sz_option_instance.g_glob_if_check_play = ((CButton *)GetDlgItem(IDC_CHECK19))->GetCheck();

	check_sz_option_instance.g_if_check_light = ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck();
	check_sz_option_instance.g_if_check_tap = ((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck();
	check_sz_option_instance.g_if_check_plc = ((CButton *)GetDlgItem(IDC_CHECK5))->GetCheck();
	check_sz_option_instance.g_if_check_apollo = ((CButton *)GetDlgItem(IDC_CHECK13))->GetCheck();
	check_sz_option_instance.g_if_check_bonemic = ((CButton *)GetDlgItem(IDC_CHECK23))->GetCheck();

	check_sz_option_instance.g_glob_if_check_reboot = ((CButton *)GetDlgItem(IDC_CHECK26))->GetCheck();
	
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_check_timeout_s);
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_check_light_addps_value);

	GetDlgItem(IDC_EDIT11)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_record_time_sec);
	GetDlgItem(IDC_EDIT16)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_record_passdb);
	GetDlgItem(IDC_EDIT17)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_record_subdb);
	GetDlgItem(IDC_EDIT14)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_play_time_sec);
	GetDlgItem(IDC_EDIT33)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_play_passdb);

	GetDlgItem(IDC_EDIT20)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_wait_reboot_time_sec);
	GetDlgItem(IDC_EDIT40)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_sz_option_instance.g_wait_key_time_sec);

	ini.SetPathName(ini_path);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_init",check_sz_option_instance.g_glob_if_check_init,10);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_light",check_sz_option_instance.g_glob_if_check_light,10);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_tap",check_sz_option_instance.g_glob_if_check_tap,10);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_plc",check_sz_option_instance.g_glob_if_check_plc,10);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_apollo",check_sz_option_instance.g_glob_if_check_apollo,10);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_record",check_sz_option_instance.g_glob_if_check_record,10);
	ini.WriteUInt("SZ_CHECK" ,"g_glob_if_check_play",check_sz_option_instance.g_glob_if_check_play,10);

	ini.WriteUInt("SZ_CHECK" ,"g_if_check_light",check_sz_option_instance.g_if_check_light,10);
	ini.WriteUInt("SZ_CHECK" ,"g_if_check_tap",check_sz_option_instance.g_if_check_tap,10);
	ini.WriteUInt("SZ_CHECK" ,"g_if_check_plc",check_sz_option_instance.g_if_check_plc,10);
	ini.WriteUInt("SZ_CHECK" ,"g_if_check_apollo",check_sz_option_instance.g_if_check_apollo,10);
	ini.WriteUInt("SZ_CHECK" ,"g_if_check_bonemic",check_sz_option_instance.g_if_check_bonemic,10);

	ini.WriteUInt("SZ_CHECK" ,"g_check_timeout_s",check_sz_option_instance.g_check_timeout_s,10);
	ini.WriteUInt("SZ_CHECK" ,"g_check_light_addps_value",check_sz_option_instance.g_check_light_addps_value,10);

	ini.WriteUInt("SZ_CHECK" ,"g_record_time_sec",check_sz_option_instance.g_record_time_sec,10);
	ini.WriteUInt("SZ_CHECK" ,"g_record_passdb",check_sz_option_instance.g_record_passdb,10);
	ini.WriteUInt("SZ_CHECK" ,"g_record_subdb",check_sz_option_instance.g_record_subdb,10);
	ini.WriteUInt("SZ_CHECK" ,"g_play_time_sec",check_sz_option_instance.g_play_time_sec,10);
	ini.WriteUInt("SZ_CHECK" ,"g_play_passdb",check_sz_option_instance.g_play_passdb,10);

	ini.WriteUInt("SZ_CHECK" ,"g_wait_reboot_time_sec",check_sz_option_instance.g_wait_reboot_time_sec,10);
	ini.WriteUInt("SZ_CHECK" ,"g_wait_key_time_sec",check_sz_option_instance.g_wait_key_time_sec,10);

	CDialogEx::OnOK();
}


void check_sz_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

BOOL check_sz_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	((CButton *)GetDlgItem(IDC_CHECK14))->SetCheck(check_sz_option_instance.g_glob_if_check_init);
	((CButton *)GetDlgItem(IDC_CHECK18))->SetCheck(check_sz_option_instance.g_glob_if_check_light);
	((CButton *)GetDlgItem(IDC_CHECK15))->SetCheck(check_sz_option_instance.g_glob_if_check_tap);
	((CButton *)GetDlgItem(IDC_CHECK17))->SetCheck(check_sz_option_instance.g_glob_if_check_plc);
	((CButton *)GetDlgItem(IDC_CHECK16))->SetCheck(check_sz_option_instance.g_glob_if_check_apollo);
	((CButton *)GetDlgItem(IDC_CHECK20))->SetCheck(check_sz_option_instance.g_glob_if_check_record);
	((CButton *)GetDlgItem(IDC_CHECK19))->SetCheck(check_sz_option_instance.g_glob_if_check_play);

	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(check_sz_option_instance.g_if_check_light);
	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(check_sz_option_instance.g_if_check_tap);
	((CButton *)GetDlgItem(IDC_CHECK5))->SetCheck(check_sz_option_instance.g_if_check_plc);
	((CButton *)GetDlgItem(IDC_CHECK13))->SetCheck(check_sz_option_instance.g_if_check_apollo);
	((CButton *)GetDlgItem(IDC_CHECK23))->SetCheck(check_sz_option_instance.g_if_check_bonemic);

	((CButton *)GetDlgItem(IDC_CHECK26))->SetCheck(check_sz_option_instance.g_glob_if_check_reboot);

	sText.Format("%d", check_sz_option_instance.g_check_timeout_s);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);
	sText.Format("%d", check_sz_option_instance.g_check_light_addps_value);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(sText);

	sText.Format("%d", check_sz_option_instance.g_record_time_sec);
	GetDlgItem(IDC_EDIT11)->SetWindowTextA(sText);
	sText.Format("%d", check_sz_option_instance.g_record_passdb);
	GetDlgItem(IDC_EDIT16)->SetWindowTextA(sText);
	sText.Format("%d", check_sz_option_instance.g_record_subdb);
	GetDlgItem(IDC_EDIT17)->SetWindowTextA(sText);
	sText.Format("%d", check_sz_option_instance.g_play_time_sec);
	GetDlgItem(IDC_EDIT14)->SetWindowTextA(sText);
	sText.Format("%d", check_sz_option_instance.g_play_passdb);
	GetDlgItem(IDC_EDIT33)->SetWindowTextA(sText);

	sText.Format("%d", check_sz_option_instance.g_wait_reboot_time_sec);
	GetDlgItem(IDC_EDIT20)->SetWindowTextA(sText);
	sText.Format("%d", check_sz_option_instance.g_wait_key_time_sec);
	GetDlgItem(IDC_EDIT40)->SetWindowTextA(sText);

	if( ((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK13))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK23))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK13))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK23))->EnableWindow(FALSE);
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT11))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT17))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT11))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT17))->EnableWindow(FALSE);	
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT14))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT14))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(FALSE);
	}
	if( ((CButton*)GetDlgItem(IDC_CHECK26))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}


void check_sz_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码

}

void check_sz_option_dlg::OnBnClickedCheck14()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK13))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK23))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_CHECK1))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK3))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK13))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK23))->EnableWindow(FALSE);
	}
}

void check_sz_option_dlg::OnBnClickedCheck18()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK18))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
	}
}


void check_sz_option_dlg::OnBnClickedCheck20()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK20))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT11))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT17))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT11))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT17))->EnableWindow(FALSE);	
	}
}


void check_sz_option_dlg::OnBnClickedCheck19()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK19))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT14))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT14))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(FALSE);
	}
}


void check_sz_option_dlg::OnBnClickedCheck23()
{
	// TODO: 在此添加控件通知处理程序代码
}


void check_sz_option_dlg::OnBnClickedCheck26()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK26))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT20))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT40))->EnableWindow(FALSE);
	}
}
