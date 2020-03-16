// bluetooth_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "bluetooth_option_dlg.h"
#include "afxdialogex.h"


// bluetooth_option_dlg 对话框

IMPLEMENT_DYNAMIC(bluetooth_option_dlg, CDialogEx)

bluetooth_option_dlg::bluetooth_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(bluetooth_option_dlg::IDD, pParent)
{

}

bluetooth_option_dlg::~bluetooth_option_dlg()
{
}

void bluetooth_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(bluetooth_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &bluetooth_option_dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &bluetooth_option_dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &bluetooth_option_dlg::OnBnClickedCheck1)
END_MESSAGE_MAP()


// bluetooth_option_dlg 消息处理程序


void bluetooth_option_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT11)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_db_value_fz);
	GetDlgItem(IDC_EDIT14)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_sub_value_fz);
	GetDlgItem(IDC_EDIT13)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_cali_max_value_fz);
	GetDlgItem(IDC_EDIT12)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_cali_min_value_fz);

	GetDlgItem(IDC_EDIT18)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_sub_value_air);

	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_db_value_jn);
	bluetooth_option_instance.g_check_cycle_jn = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_cycle_max_jn);
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bluetooth_option_instance.g_cycle_min_jn);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BLUETOOTH" ,"g_db_value_fz",bluetooth_option_instance.g_db_value_fz,10);
	ini.WriteUInt("BLUETOOTH" ,"g_sub_value_fz",bluetooth_option_instance.g_sub_value_fz,10);
	ini.WriteUInt("BLUETOOTH" ,"g_cali_max_value_fz",bluetooth_option_instance.g_cali_max_value_fz,10);
	ini.WriteUInt("BLUETOOTH" ,"g_cali_min_value_fz",bluetooth_option_instance.g_cali_min_value_fz,10);
	ini.WriteUInt("BLUETOOTH" ,"g_sub_value_air",bluetooth_option_instance.g_sub_value_air,10);
	ini.WriteUInt("BLUETOOTH" ,"g_db_value_jn",bluetooth_option_instance.g_db_value_jn,10);
	ini.WriteUInt("BLUETOOTH" ,"g_check_cycle_jn",bluetooth_option_instance.g_check_cycle_jn,10);
	ini.WriteUInt("BLUETOOTH" ,"g_cycle_max_jn",bluetooth_option_instance.g_cycle_max_jn,10);
	ini.WriteUInt("BLUETOOTH" ,"g_cycle_min_jn",bluetooth_option_instance.g_cycle_min_jn,10);

	CDialogEx::OnCancel();
}


void bluetooth_option_dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void bluetooth_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	bluetooth_option_instance.g_db_value_fz=		g_p->bluetooth_option_instance.g_db_value_fz;
	bluetooth_option_instance.g_sub_value_fz=		g_p->bluetooth_option_instance.g_sub_value_fz;
	bluetooth_option_instance.g_cali_max_value_fz=	g_p->bluetooth_option_instance.g_cali_max_value_fz;
	bluetooth_option_instance.g_cali_min_value_fz=	g_p->bluetooth_option_instance.g_cali_min_value_fz;

	bluetooth_option_instance.g_sub_value_air=		g_p->bluetooth_option_instance.g_sub_value_air;

	bluetooth_option_instance.g_db_value_jn=		g_p->bluetooth_option_instance.g_db_value_jn;
	bluetooth_option_instance.g_check_cycle_jn=		g_p->bluetooth_option_instance.g_check_cycle_jn;
	bluetooth_option_instance.g_cycle_max_jn=		g_p->bluetooth_option_instance.g_cycle_max_jn;
	bluetooth_option_instance.g_cycle_min_jn=		g_p->bluetooth_option_instance.g_cycle_min_jn;
}

void bluetooth_option_dlg::output_param(struct bluetooth_option_str*bc_p)
{
	bc_p->g_db_value_fz=		bluetooth_option_instance.g_db_value_fz;
	bc_p->g_sub_value_fz=		bluetooth_option_instance.g_sub_value_fz;
	bc_p->g_cali_max_value_fz=	bluetooth_option_instance.g_cali_max_value_fz;
	bc_p->g_cali_min_value_fz=	bluetooth_option_instance.g_cali_min_value_fz;
	bc_p->g_sub_value_air=		bluetooth_option_instance.g_sub_value_air;
	bc_p->g_db_value_jn=		bluetooth_option_instance.g_db_value_jn;
	bc_p->g_check_cycle_jn=		bluetooth_option_instance.g_check_cycle_jn;
	bc_p->g_cycle_max_jn=		bluetooth_option_instance.g_cycle_max_jn;
	bc_p->g_cycle_min_jn=		bluetooth_option_instance.g_cycle_min_jn;
}

BOOL bluetooth_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	sText.Format("%d", bluetooth_option_instance.g_db_value_fz);
	GetDlgItem(IDC_EDIT11)->SetWindowTextA(sText);
	sText.Format("%d", bluetooth_option_instance.g_sub_value_fz);
	GetDlgItem(IDC_EDIT14)->SetWindowTextA(sText);
	sText.Format("%d", bluetooth_option_instance.g_cali_max_value_fz);
	GetDlgItem(IDC_EDIT13)->SetWindowTextA(sText);
	sText.Format("%d", bluetooth_option_instance.g_cali_min_value_fz);
	GetDlgItem(IDC_EDIT12)->SetWindowTextA(sText);

	sText.Format("%d", bluetooth_option_instance.g_sub_value_air);
	GetDlgItem(IDC_EDIT18)->SetWindowTextA(sText);

	sText.Format("%d", bluetooth_option_instance.g_db_value_jn);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(bluetooth_option_instance.g_check_cycle_jn);
	sText.Format("%d", bluetooth_option_instance.g_cycle_max_jn);
	GetDlgItem(IDC_EDIT10)->SetWindowTextA(sText);
	sText.Format("%d", bluetooth_option_instance.g_cycle_min_jn);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(sText);

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT10))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT10))->EnableWindow(FALSE);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void bluetooth_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT10))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT10))->EnableWindow(FALSE);	
	}
}
