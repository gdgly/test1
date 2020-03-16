// charge_check_option_dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "charge_check_option_dlg.h"
#include "afxdialogex.h"


// charge_check_option_dlg �Ի���

IMPLEMENT_DYNAMIC(charge_check_option_dlg, CDialogEx)

charge_check_option_dlg::charge_check_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(charge_check_option_dlg::IDD, pParent)
{

}

charge_check_option_dlg::~charge_check_option_dlg()
{
}

void charge_check_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comb_current_ttybaud);
}


BEGIN_MESSAGE_MAP(charge_check_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &charge_check_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &charge_check_option_dlg::OnBnClickedCancel)
END_MESSAGE_MAP()

void charge_check_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	charge_option_instance = g_p->charge_option_instance;
}

void charge_check_option_dlg::output_param(struct charge_option_str *bc_p)
{
	*bc_p = charge_option_instance;
}
// charge_check_option_dlg ��Ϣ�������


void charge_check_option_dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&charge_option_instance.g_chargCurrent);

	GetDlgItem(IDC_EDIT6)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&charge_option_instance.g_currentcom_port);
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&charge_option_instance.g_currentcom_baud);

	ini.SetPathName(ini_path);
	ini.WriteUInt("CHARGECHECK" ,"g_chargCurrent",charge_option_instance.g_chargCurrent,10);
	ini.WriteUInt("CHARGECHECK" ,"g_currentcom_port",charge_option_instance.g_currentcom_port,10);
	ini.WriteUInt("CHARGECHECK" ,"g_currentcom_baud",charge_option_instance.g_currentcom_baud,10);

	CDialogEx::OnOK();
}


void charge_check_option_dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL charge_check_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	sText.Format("%d", charge_option_instance.g_chargCurrent);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(sText);

	sText.Format("%d", charge_option_instance.g_currentcom_port);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(sText);
	sText.Format("%d", charge_option_instance.g_currentcom_baud);
	GetDlgItem(IDC_COMBO1)->SetWindowTextA(sText);
	comb_current_ttybaud.AddString("1000000");
	comb_current_ttybaud.AddString("921600");
	comb_current_ttybaud.AddString("115200");

	return TRUE;  // return TRUE unless you set the focus to a control
}
