// batpower_by_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "batpower_by_option_dlg.h"
#include "afxdialogex.h"


// batpower_by_option_dlg 对话框

IMPLEMENT_DYNAMIC(batpower_by_option_dlg, CDialogEx)

batpower_by_option_dlg::batpower_by_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(batpower_by_option_dlg::IDD, pParent)
{

}

batpower_by_option_dlg::~batpower_by_option_dlg()
{
}

void batpower_by_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comb_current_ttybaud);
}


BEGIN_MESSAGE_MAP(batpower_by_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &batpower_by_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &batpower_by_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK1, &batpower_by_option_dlg::OnBnClickedCheck1)
END_MESSAGE_MAP()

void batpower_by_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	batpower_by_option_instance = g_p->batpower_by_option_instance;
}

void batpower_by_option_dlg::output_param(struct batpower_by_option_str *bc_p)
{
	*bc_p = batpower_by_option_instance;
}
// batpower_by_option_dlg 消息处理程序

void batpower_by_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	batpower_by_option_instance.g_current_test = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_normal_current_max);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_normal_current_min);
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_chargCurrent);
	GetDlgItem(IDC_EDIT16)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_startcheck_times);
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_usb_timeout);

	GetDlgItem(IDC_EDIT6)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_currentcom_port);
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_currentcom_baud);

	batpower_by_option_instance.g_if_setup_reset = ((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();

	GetDlgItem(IDC_EDIT1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_by_option_instance.g_power_percent);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BY_BATPOWER" ,"g_current_test",batpower_by_option_instance.g_current_test,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_normal_current_max",batpower_by_option_instance.g_normal_current_max,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_normal_current_min",batpower_by_option_instance.g_normal_current_min,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_chargCurrent",batpower_by_option_instance.g_chargCurrent,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_startcheck_times",batpower_by_option_instance.g_startcheck_times,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_currentcom_port",batpower_by_option_instance.g_currentcom_port,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_currentcom_baud",batpower_by_option_instance.g_currentcom_baud,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_if_setup_reset",batpower_by_option_instance.g_if_setup_reset,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_power_percent",batpower_by_option_instance.g_power_percent,10);
	ini.WriteUInt("BY_BATPOWER" ,"g_usb_timeout",batpower_by_option_instance.g_usb_timeout,10);

	CDialogEx::OnOK();
}


void batpower_by_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL batpower_by_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString sText;

	sText.Format("%d", batpower_by_option_instance.g_currentcom_port);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(sText);
	sText.Format("%d", batpower_by_option_instance.g_currentcom_baud);
	GetDlgItem(IDC_COMBO1)->SetWindowTextA(sText);
	comb_current_ttybaud.AddString("1000000");
	comb_current_ttybaud.AddString("921600");
	comb_current_ttybaud.AddString("115200");

	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(batpower_by_option_instance.g_current_test);
	sText.Format("%d", batpower_by_option_instance.g_normal_current_max);
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(sText);
	sText.Format("%d", batpower_by_option_instance.g_normal_current_min);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sText);
	sText.Format("%d", batpower_by_option_instance.g_chargCurrent);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(sText);
	sText.Format("%d", batpower_by_option_instance.g_startcheck_times);
	GetDlgItem(IDC_EDIT16)->SetWindowTextA(sText);
	sText.Format("%d", batpower_by_option_instance.g_usb_timeout);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);

	sText.Format("%d", batpower_by_option_instance.g_power_percent);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sText);

	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(batpower_by_option_instance.g_if_setup_reset);

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void batpower_by_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		//((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
		//((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
		//((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
	}
}
