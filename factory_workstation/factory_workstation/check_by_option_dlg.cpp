// check_by_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "check_by_option_dlg.h"
#include "afxdialogex.h"


// check_by_option_dlg 对话框

IMPLEMENT_DYNAMIC(check_by_option_dlg, CDialogEx)

check_by_option_dlg::check_by_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(check_by_option_dlg::IDD, pParent)
{

}

check_by_option_dlg::~check_by_option_dlg()
{
}

void check_by_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comb_current_ttybaud);
}


BEGIN_MESSAGE_MAP(check_by_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &check_by_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &check_by_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &check_by_option_dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &check_by_option_dlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK4, &check_by_option_dlg::OnBnClickedCheck4)
END_MESSAGE_MAP()

void check_by_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	check_by_option_instance = g_p->check_by_option_instance;
}

void check_by_option_dlg::output_param(struct check_by_option_str *bc_p)
{
	*bc_p = check_by_option_instance;
}
// check_by_option_dlg 消息处理程序

BOOL check_by_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString sText;
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(check_by_option_instance.g_current_test);
	((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(check_by_option_instance.g_sleep_current_test);
	((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(check_by_option_instance.g_backlight_current_test);
	sText.Format("%d", check_by_option_instance.g_normal_current_max);
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_normal_current_min);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_chargCurrent);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_sleepCurrent);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_backlight_Current);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_startcheck_times);
	GetDlgItem(IDC_EDIT16)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_sleep_waittime);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(sText);

	sText.Format("%d", check_by_option_instance.g_wifi_timeout);
	GetDlgItem(IDC_EDIT36)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_wifi_speed);
	GetDlgItem(IDC_EDIT37)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT18)->SetWindowTextA(check_by_option_instance.g_wifi_ip);
	GetDlgItem(IDC_EDIT34)->SetWindowTextA(check_by_option_instance.g_wifi_ssid);
	GetDlgItem(IDC_EDIT35)->SetWindowTextA(check_by_option_instance.g_wifi_passwd);
	GetDlgItem(IDC_EDIT25)->SetWindowTextA(check_by_option_instance.g_bluetooth_board_mac);

	sText.Format("%d", check_by_option_instance.g_currentcom_port);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_currentcom_baud);
	GetDlgItem(IDC_COMBO1)->SetWindowTextA(sText);
	comb_current_ttybaud.AddString("1000000");
	comb_current_ttybaud.AddString("921600");
	comb_current_ttybaud.AddString("115200");

	sText.Format("%d", check_by_option_instance.g_usb_timeout);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);

	sText.Format("%d", check_by_option_instance.g_record_time);
	GetDlgItem(IDC_EDIT12)->SetWindowTextA(sText);
	sText.Format("%d", check_by_option_instance.g_recode_passdb);
	GetDlgItem(IDC_EDIT14)->SetWindowTextA(sText);

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(TRUE);
			if( ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(TRUE);
			}else{
				((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
			}
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
		}

	}else{
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void check_by_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;

	check_by_option_instance.g_current_test = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	check_by_option_instance.g_sleep_current_test = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	check_by_option_instance.g_backlight_current_test = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_normal_current_max);
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_normal_current_min);
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_chargCurrent);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_sleepCurrent);
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_backlight_Current);
	GetDlgItem(IDC_EDIT16)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_startcheck_times);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_sleep_waittime);

	GetDlgItem(IDC_EDIT6)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_currentcom_port);
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_currentcom_baud);

	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_usb_timeout);

	GetDlgItem(IDC_EDIT12)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_record_time);
	GetDlgItem(IDC_EDIT14)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_recode_passdb);

	GetDlgItem(IDC_EDIT36)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_wifi_timeout);
	GetDlgItem(IDC_EDIT37)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&check_by_option_instance.g_wifi_speed);
	GetDlgItem(IDC_EDIT18)->GetWindowTextA(check_by_option_instance.g_wifi_ip);
	GetDlgItem(IDC_EDIT34)->GetWindowTextA(check_by_option_instance.g_wifi_ssid);
	GetDlgItem(IDC_EDIT35)->GetWindowTextA(check_by_option_instance.g_wifi_passwd);
	GetDlgItem(IDC_EDIT25)->GetWindowTextA(check_by_option_instance.g_bluetooth_board_mac);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BY_CHECK" ,"g_sleep_waittime",check_by_option_instance.g_sleep_waittime,10);
	ini.WriteUInt("BY_CHECK" ,"g_startcheck_times",check_by_option_instance.g_startcheck_times,10);
	ini.WriteUInt("BY_CHECK" ,"g_current_test",check_by_option_instance.g_current_test,10);
	ini.WriteUInt("BY_CHECK" ,"g_sleep_current_test",check_by_option_instance.g_sleep_current_test,10);
	ini.WriteUInt("BY_CHECK" ,"g_backlight_current_test",check_by_option_instance.g_backlight_current_test,10);
	ini.WriteUInt("BY_CHECK" ,"g_normal_current_max",check_by_option_instance.g_normal_current_max,10);
	ini.WriteUInt("BY_CHECK" ,"g_normal_current_min",check_by_option_instance.g_normal_current_min,10);
	ini.WriteUInt("BY_CHECK" ,"g_chargCurrent",check_by_option_instance.g_chargCurrent,10);
	ini.WriteUInt("BY_CHECK" ,"g_backlight_Current",check_by_option_instance.g_backlight_Current,10);
	ini.WriteUInt("BY_CHECK" ,"g_sleepCurrent",check_by_option_instance.g_sleepCurrent,10);
	ini.WriteUInt("BY_CHECK" ,"g_currentcom_port",check_by_option_instance.g_currentcom_port,10);
	ini.WriteUInt("BY_CHECK" ,"g_currentcom_baud",check_by_option_instance.g_currentcom_baud,10);
	ini.WriteUInt("BY_CHECK" ,"g_usb_timeout",check_by_option_instance.g_usb_timeout,10);
	ini.WriteUInt("BY_CHECK" ,"g_record_time",check_by_option_instance.g_record_time,10);
	ini.WriteUInt("BY_CHECK" ,"g_recode_passdb",check_by_option_instance.g_recode_passdb,10);

	ini.WriteUInt("BY_CHECK" ,"g_wifi_timeout",check_by_option_instance.g_wifi_timeout,10);
	ini.WriteUInt("BY_CHECK" ,"g_wifi_speed",check_by_option_instance.g_wifi_speed,10);
	ini.WriteString("BY_CHECK" ,"g_wifi_ip",check_by_option_instance.g_wifi_ip);
	ini.WriteString("BY_CHECK" ,"g_wifi_ssid",check_by_option_instance.g_wifi_ssid);
	ini.WriteString("BY_CHECK" ,"g_wifi_passwd",check_by_option_instance.g_wifi_passwd);
	ini.WriteString("BY_CHECK" ,"g_bluetooth_board_mac",check_by_option_instance.g_bluetooth_board_mac);

	CDialogEx::OnCancel();
}


void check_by_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void check_by_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(TRUE);
			if( ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(TRUE);
			}else{
				((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
			}
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
		}

	}else{
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
	}
}


void check_by_option_dlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
		if( ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(TRUE);
			if( ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(TRUE);
			}else{
				((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
			}
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
			((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(0);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
		}
}


void check_by_option_dlg::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT3))->EnableWindow(FALSE);
	}
}
