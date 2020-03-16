// check_sz_box_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "check_sz_box_option_dlg.h"
#include "afxdialogex.h"


// check_sz_box_option_dlg 对话框

IMPLEMENT_DYNAMIC(check_sz_box_option_dlg, CDialogEx)

check_sz_box_option_dlg::check_sz_box_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(check_sz_box_option_dlg::IDD, pParent)
{

}

check_sz_box_option_dlg::~check_sz_box_option_dlg()
{
}

void check_sz_box_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(check_sz_box_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &check_sz_box_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &check_sz_box_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK14, &check_sz_box_option_dlg::OnBnClickedCheck14)
	ON_BN_CLICKED(IDC_CHECK17, &check_sz_box_option_dlg::OnBnClickedCheck17)
END_MESSAGE_MAP()

void check_sz_box_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	checkbox_sz_option_instance = g_p->checkbox_sz_option_instance;
}

void check_sz_box_option_dlg::output_param(struct checkbox_sz_option_str *bc_p)
{
	*bc_p = checkbox_sz_option_instance;
}



// check_sz_box_option_dlg 消息处理程序


void check_sz_box_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	checkbox_sz_option_instance.g_if_check_wdt = ((CButton *)GetDlgItem(IDC_CHECK1))->GetCheck();
	checkbox_sz_option_instance.g_if_check_max20340 = ((CButton *)GetDlgItem(IDC_CHECK3))->GetCheck();
	checkbox_sz_option_instance.g_if_check_max77813 = ((CButton *)GetDlgItem(IDC_CHECK5))->GetCheck();
	checkbox_sz_option_instance.g_if_check_plc_commu = ((CButton *)GetDlgItem(IDC_CHECK13))->GetCheck();
	checkbox_sz_option_instance.g_if_check_hal = ((CButton *)GetDlgItem(IDC_CHECK14))->GetCheck();
	checkbox_sz_option_instance.g_if_check_key = ((CButton *)GetDlgItem(IDC_CHECK15))->GetCheck();
	checkbox_sz_option_instance.g_if_check_usb = ((CButton *)GetDlgItem(IDC_CHECK16))->GetCheck();
	checkbox_sz_option_instance.g_if_check_hal_after_plc = ((CButton *)GetDlgItem(IDC_CHECK24))->GetCheck();
	checkbox_sz_option_instance.g_if_check_cal_vol = ((CButton *)GetDlgItem(IDC_CHECK25))->GetCheck();
	checkbox_sz_option_instance.g_if_check_plc_commu_right = ((CButton *)GetDlgItem(IDC_CHECK17))->GetCheck();
	

	ini.SetPathName(ini_path);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_wdt",checkbox_sz_option_instance.g_if_check_wdt,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_max20340",checkbox_sz_option_instance.g_if_check_max20340,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_max77813",checkbox_sz_option_instance.g_if_check_max77813,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_plc_commu",checkbox_sz_option_instance.g_if_check_plc_commu,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_hal",checkbox_sz_option_instance.g_if_check_hal,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_key",checkbox_sz_option_instance.g_if_check_key,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_usb",checkbox_sz_option_instance.g_if_check_usb,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_hal_after_plc",checkbox_sz_option_instance.g_if_check_hal_after_plc,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_cal_vol",checkbox_sz_option_instance.g_if_check_cal_vol,10);
	ini.WriteUInt("SZ_CHECKBOX" ,"g_if_check_plc_commu_right",checkbox_sz_option_instance.g_if_check_plc_commu_right,10);
	

	CDialogEx::OnOK();
}


void check_sz_box_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL check_sz_box_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(checkbox_sz_option_instance.g_if_check_wdt);
	((CButton *)GetDlgItem(IDC_CHECK3))->SetCheck(checkbox_sz_option_instance.g_if_check_max20340);
	((CButton *)GetDlgItem(IDC_CHECK5))->SetCheck(checkbox_sz_option_instance.g_if_check_max77813);
	((CButton *)GetDlgItem(IDC_CHECK13))->SetCheck(checkbox_sz_option_instance.g_if_check_plc_commu);
	((CButton *)GetDlgItem(IDC_CHECK14))->SetCheck(checkbox_sz_option_instance.g_if_check_hal);
	((CButton *)GetDlgItem(IDC_CHECK15))->SetCheck(checkbox_sz_option_instance.g_if_check_key);
	((CButton *)GetDlgItem(IDC_CHECK16))->SetCheck(checkbox_sz_option_instance.g_if_check_usb);
	((CButton *)GetDlgItem(IDC_CHECK24))->SetCheck(checkbox_sz_option_instance.g_if_check_hal_after_plc);
	((CButton *)GetDlgItem(IDC_CHECK25))->SetCheck(checkbox_sz_option_instance.g_if_check_cal_vol);
	((CButton *)GetDlgItem(IDC_CHECK17))->SetCheck(checkbox_sz_option_instance.g_if_check_plc_commu_right);
	
	if( ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK24))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_CHECK24))->EnableWindow(FALSE);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void check_sz_box_option_dlg::OnBnClickedCheck14()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK24))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_CHECK24))->EnableWindow(FALSE);	
	}
}


void check_sz_box_option_dlg::OnBnClickedCheck17()
{
	// TODO: 在此添加控件通知处理程序代码
}
