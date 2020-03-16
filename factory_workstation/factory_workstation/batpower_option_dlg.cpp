// batpower_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "batpower_option_dlg.h"
#include "afxdialogex.h"


// batpower_option_dlg 对话框

IMPLEMENT_DYNAMIC(batpower_option_dlg, CDialogEx)

batpower_option_dlg::batpower_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(batpower_option_dlg::IDD, pParent)
{

}

batpower_option_dlg::~batpower_option_dlg()
{
}

void batpower_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(batpower_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &batpower_option_dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &batpower_option_dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &batpower_option_dlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// batpower_option_dlg 消息处理程序


void batpower_option_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_option_instance.g_power_percent);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_option_instance.g_reboot_num);
	batpower_option_instance.g_check_current = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_option_instance.g_current_value);
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_option_instance.g_twotc_lasttime);
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_option_instance.g_twotc_consume_power);
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&batpower_option_instance.g_com_timeout_times);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BATPOWER" ,"g_power_percent",batpower_option_instance.g_power_percent,10);
	ini.WriteUInt("BATPOWER" ,"g_reboot_num",batpower_option_instance.g_reboot_num,10);
	ini.WriteUInt("BATPOWER" ,"g_check_current",batpower_option_instance.g_check_current,10);
	ini.WriteUInt("BATPOWER" ,"g_current_value",batpower_option_instance.g_current_value,10);
	ini.WriteUInt("BATPOWER" ,"g_twotc_lasttime",batpower_option_instance.g_twotc_lasttime,10);
	ini.WriteUInt("BATPOWER" ,"g_twotc_consume_power",batpower_option_instance.g_twotc_consume_power,10);
	ini.WriteUInt("BATPOWER" ,"g_com_timeout_times",batpower_option_instance.g_com_timeout_times,10);

	CDialogEx::OnCancel();
}


void batpower_option_dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void batpower_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	batpower_option_instance.g_power_percent=			g_p->batpower_option_instance.g_power_percent;
	batpower_option_instance.g_reboot_num =				g_p->batpower_option_instance.g_reboot_num;
	batpower_option_instance.g_check_current =			g_p->batpower_option_instance.g_check_current;
	batpower_option_instance.g_current_value =			g_p->batpower_option_instance.g_current_value;
	batpower_option_instance.g_twotc_lasttime =			g_p->batpower_option_instance.g_twotc_lasttime;
	batpower_option_instance.g_twotc_consume_power =	g_p->batpower_option_instance.g_twotc_consume_power;
	batpower_option_instance.g_com_timeout_times =		g_p->batpower_option_instance.g_com_timeout_times;
}

void batpower_option_dlg::output_param(struct batpower_option_str*bc_p)
{
	bc_p->g_power_percent=			batpower_option_instance.g_power_percent;
	bc_p->g_reboot_num=				batpower_option_instance.g_reboot_num;
	bc_p->g_check_current=			batpower_option_instance.g_check_current;
	bc_p->g_current_value=			batpower_option_instance.g_current_value;
	bc_p->g_twotc_lasttime=			batpower_option_instance.g_twotc_lasttime;
	bc_p->g_twotc_consume_power=	batpower_option_instance.g_twotc_consume_power;
	bc_p->g_com_timeout_times=		batpower_option_instance.g_com_timeout_times;
}

BOOL batpower_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	sText.Format("%d", batpower_option_instance.g_power_percent);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sText);
	sText.Format("%d", batpower_option_instance.g_reboot_num);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sText);
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(batpower_option_instance.g_check_current);
	sText.Format("%d", batpower_option_instance.g_current_value);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(sText);
	sText.Format("%d", batpower_option_instance.g_twotc_lasttime);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);
	sText.Format("%d", batpower_option_instance.g_twotc_consume_power);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(sText);
	sText.Format("%d", batpower_option_instance.g_com_timeout_times);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(sText);

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void batpower_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT8))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
	}
}
