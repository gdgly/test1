// burn_sn_sz_option_dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "burn_sn_sz_option_dlg.h"
#include "afxdialogex.h"


// burn_sn_sz_option_dlg �Ի���

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
// burn_sn_sz_option_dlg ��Ϣ�������


void burn_sn_sz_option_dlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT8)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burnsn_sz_option_instance.g_burn_sn_len);

	ini.SetPathName(ini_path);
	ini.WriteUInt("SZ_BURNSN" ,"g_burn_sn_len",burnsn_sz_option_instance.g_burn_sn_len,10);

	CDialogEx::OnOK();
}


void burn_sn_sz_option_dlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


BOOL burn_sn_sz_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString sText;

	sText.Format("%d", burnsn_sz_option_instance.g_burn_sn_len);
	GetDlgItem(IDC_EDIT8)->SetWindowTextA(sText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
