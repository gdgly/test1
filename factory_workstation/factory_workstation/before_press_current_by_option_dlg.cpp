// before_press_current_by_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "before_press_current_by_option_dlg.h"
#include "afxdialogex.h"


// before_press_current_by_option_dlg 对话框

IMPLEMENT_DYNAMIC(before_press_current_by_option_dlg, CDialogEx)

before_press_current_by_option_dlg::before_press_current_by_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(before_press_current_by_option_dlg::IDD, pParent)
{

}

before_press_current_by_option_dlg::~before_press_current_by_option_dlg()
{
}

void before_press_current_by_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comb_current_ttybaud);
}


BEGIN_MESSAGE_MAP(before_press_current_by_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &before_press_current_by_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &before_press_current_by_option_dlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// before_press_current_by_option_dlg 消息处理程序
void before_press_current_by_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	bp_current_by_option_instance = g_p->bp_current_by_option_instance;
}

void before_press_current_by_option_dlg::output_param(struct bp_current_by_option_str *bc_p)
{
	*bc_p = bp_current_by_option_instance;
}

void before_press_current_by_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bp_current_by_option_instance.g_sleep_current_max);
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bp_current_by_option_instance.g_sleep_current_min);

	GetDlgItem(IDC_EDIT6)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bp_current_by_option_instance.g_currentcom_port);
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&bp_current_by_option_instance.g_currentcom_baud);

	ini.SetPathName(ini_path);
	ini.WriteUInt("BY_BP_CURRENT" ,"g_sleep_current_max",bp_current_by_option_instance.g_sleep_current_max,10);
	ini.WriteUInt("BY_BP_CURRENT" ,"g_sleep_current_min",bp_current_by_option_instance.g_sleep_current_min,10);
	ini.WriteUInt("BY_BP_CURRENT" ,"g_currentcom_port",bp_current_by_option_instance.g_currentcom_port,10);
	ini.WriteUInt("BY_BP_CURRENT" ,"g_currentcom_baud",bp_current_by_option_instance.g_currentcom_baud,10);
	CDialogEx::OnOK();
}


void before_press_current_by_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL before_press_current_by_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sText;
	sText.Format("%d", bp_current_by_option_instance.g_sleep_current_max);
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(sText);
	sText.Format("%d", bp_current_by_option_instance.g_sleep_current_min);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sText);

	sText.Format("%d", bp_current_by_option_instance.g_currentcom_port);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(sText);
	sText.Format("%d", bp_current_by_option_instance.g_currentcom_baud);
	GetDlgItem(IDC_COMBO1)->SetWindowTextA(sText);
	comb_current_ttybaud.AddString("1000000");
	comb_current_ttybaud.AddString("921600");
	comb_current_ttybaud.AddString("115200");

	return TRUE;  // return TRUE unless you set the focus to a control
}
