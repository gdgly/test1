// burn_sn_sz_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "burn_sn_sz_option_dlg.h"
#include "afxdialogex.h"


// burn_sn_sz_option_dlg 对话框

IMPLEMENT_DYNAMIC(burn_sn_sz_option_dlg, CDialogEx)

burn_sn_sz_option_dlg::burn_sn_sz_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(burn_sn_sz_option_dlg::IDD, pParent)
{

}

burn_sn_sz_option_dlg::~burn_sn_sz_option_dlg()
{
}

void burn_sn_sz_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(burn_sn_sz_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &burn_sn_sz_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &burn_sn_sz_option_dlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void burn_sn_sz_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	burnsn_sz_option_instance = g_p->burnsn_sz_option_instance;
}

void burn_sn_sz_option_dlg::output_param(struct burnsn_sz_option_str *bc_p)
{
	*bc_p = burnsn_sz_option_instance;
}
// burn_sn_sz_option_dlg 消息处理程序


void burn_sn_sz_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burnsn_sz_option_instance.g_burn_sn_len);
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(burnsn_sz_option_instance.g_l_sn_prompt);
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(burnsn_sz_option_instance.g_r_sn_prompt);
	GetDlgItem(IDC_EDIT10)->GetWindowTextA(burnsn_sz_option_instance.g_c_sn_prompt);

	ini.SetPathName(ini_path);
	ini.WriteUInt("SZ_BURNSN" ,"g_burn_sn_len",burnsn_sz_option_instance.g_burn_sn_len,10);
	ini.WriteString("SZ_BURNSN" ,"g_l_sn_prompt",burnsn_sz_option_instance.g_l_sn_prompt);
	ini.WriteString("SZ_BURNSN" ,"g_r_sn_prompt",burnsn_sz_option_instance.g_r_sn_prompt);
	ini.WriteString("SZ_BURNSN" ,"g_c_sn_prompt",burnsn_sz_option_instance.g_c_sn_prompt);

	CDialogEx::OnOK();
}


void burn_sn_sz_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL burn_sn_sz_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sText;

	sText.Format("%d", burnsn_sz_option_instance.g_burn_sn_len);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);

	GetDlgItem(IDC_EDIT7)->SetWindowTextA(burnsn_sz_option_instance.g_l_sn_prompt);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(burnsn_sz_option_instance.g_r_sn_prompt);
	GetDlgItem(IDC_EDIT10)->SetWindowTextA(burnsn_sz_option_instance.g_c_sn_prompt);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
