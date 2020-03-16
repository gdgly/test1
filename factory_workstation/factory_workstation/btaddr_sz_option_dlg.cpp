// btaddr_sz_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "btaddr_sz_option_dlg.h"
#include "afxdialogex.h"


// btaddr_sz_option_dlg 对话框

IMPLEMENT_DYNAMIC(btaddr_sz_option_dlg, CDialogEx)

btaddr_sz_option_dlg::btaddr_sz_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(btaddr_sz_option_dlg::IDD, pParent)
{

}

btaddr_sz_option_dlg::~btaddr_sz_option_dlg()
{
}

void btaddr_sz_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT45, m_bt_name);
	DDX_Control(pDX, IDC_EDIT15, m_hw_version);
	DDX_Control(pDX, IDC_EDIT47, m_rl_flag);
	//DDX_Control(pDX, IDC_EDIT46, m_station);
}


BEGIN_MESSAGE_MAP(btaddr_sz_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &btaddr_sz_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &btaddr_sz_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK14, &btaddr_sz_option_dlg::OnBnClickedCheck14)
END_MESSAGE_MAP()


// btaddr_sz_option_dlg 消息处理程序
void btaddr_sz_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	btaddr_sz_option_instance = g_p->btaddr_sz_option_instance;
}

void btaddr_sz_option_dlg::output_param(struct btaddr_sz_option_str *bc_p)
{
	*bc_p = btaddr_sz_option_instance;
}


void btaddr_sz_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	m_bt_name.GetWindowTextA(btaddr_sz_option_instance.g_bt_name);
	m_hw_version.GetWindowTextA(btaddr_sz_option_instance.g_hw_version);
	m_rl_flag.GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&btaddr_sz_option_instance.rl_flag);
	//m_station.GetWindowTextA(btaddr_sz_option_instance.g_station_name);
	btaddr_sz_option_instance.g_if_locallic = ((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck();
	sText = btaddr_sz_option_instance.g_bt_name;
	//if( sText.Find("\"") > 0 ){
		sText = "\"" + sText + "\"";
	//}

	ini.SetPathName(ini_path);
	ini.WriteString("SZ_BTADDR" ,"g_hw_version",btaddr_sz_option_instance.g_hw_version);
	ini.WriteString("SZ_BTADDR" ,"g_bt_name",sText);
	ini.WriteUInt("SZ_BTADDR" ,"rl_flag",btaddr_sz_option_instance.rl_flag,10);
	//ini.WriteString("SZ_BTADDR" ,"g_station_name",btaddr_sz_option_instance.g_station_name);
	ini.WriteUInt("SZ_BTADDR" ,"g_if_locallic",btaddr_sz_option_instance.g_if_locallic,10);

	CDialogEx::OnOK();
}


void btaddr_sz_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL btaddr_sz_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString sText;
	// TODO:  在此添加额外的初始化
	m_bt_name.SetWindowTextA(btaddr_sz_option_instance.g_bt_name);
	m_hw_version.SetWindowTextA(btaddr_sz_option_instance.g_hw_version);
	sText.Format("%d",btaddr_sz_option_instance.rl_flag);
	m_rl_flag.SetWindowTextA(sText);
	((CButton *)GetDlgItem(IDC_CHECK14))->SetCheck(btaddr_sz_option_instance.g_if_locallic);
	//m_station.SetWindowTextA(btaddr_sz_option_instance.g_station_name);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void btaddr_sz_option_dlg::OnBnClickedCheck14()
{
	// TODO: 在此添加控件通知处理程序代码
}
