// general_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "general_option_dlg.h"
#include "afxdialogex.h"


// general_option_dlg 对话框

IMPLEMENT_DYNAMIC(general_option_dlg, CDialogEx)

general_option_dlg::general_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(general_option_dlg::IDD, pParent)
{
}

general_option_dlg::~general_option_dlg()
{
}

void general_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comb_ttybaud);
	DDX_Control(pDX, IDC_COMBO9, m_mes_type);
}


BEGIN_MESSAGE_MAP(general_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &general_option_dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &general_option_dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_CHECK1, &general_option_dlg::OnBnClickedCheck1)
	ON_CBN_SELCHANGE(IDC_COMBO9, &general_option_dlg::OnCbnSelchangeCombo9)
END_MESSAGE_MAP()

// general_option_dlg 消息处理程序

void general_option_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	general_option_instance.g_check_mes = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(general_option_instance.g_mes_ip);
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&general_option_instance.g_mes_port);
	
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(general_option_instance.g_sn_prompt);
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&general_option_instance.g_sn_start);
	GetDlgItem(IDC_EDIT6)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&general_option_instance.g_sn_count);

	GetDlgItem(IDC_EDIT3)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&general_option_instance.g_ttycom_port);
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&general_option_instance.g_ttycom_baud);
	GetDlgItem(IDC_COMBO9)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&general_option_instance.g_check_mes_type);
	//general_option_instance.g_quit_config_dilog = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	general_option_instance.g_if_open_nouse= ((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck();
	GetDlgItem(IDC_EDIT9)->GetWindowTextA(general_option_instance.g_station_name);

	ini.SetPathName(ini_path);
	ini.WriteUInt("GENERAL" ,"g_check_mes",general_option_instance.g_check_mes,10);
	ini.WriteUInt("GENERAL" ,"g_check_mes_type",general_option_instance.g_check_mes_type,10);
	ini.WriteString("GENERAL" ,"g_mes_ip",general_option_instance.g_mes_ip);
	ini.WriteUInt("GENERAL" ,"g_mes_port",general_option_instance.g_mes_port,10);
	ini.WriteString("GENERAL" ,"g_sn_prompt",general_option_instance.g_sn_prompt);
	ini.WriteUInt("GENERAL" ,"g_sn_start",general_option_instance.g_sn_start,10);
	ini.WriteUInt("GENERAL" ,"g_sn_count",general_option_instance.g_sn_count,10);
	ini.WriteUInt("GENERAL" ,"g_ttycom_port",general_option_instance.g_ttycom_port,10);
	ini.WriteUInt("GENERAL" ,"g_ttycom_baud",general_option_instance.g_ttycom_baud,10);
	//ini.WriteUInt("GENERAL" ,"g_quit_config_dilog",general_option_instance.g_quit_config_dilog,10);
	ini.WriteUInt("GENERAL" ,"g_if_open_nouse",general_option_instance.g_if_open_nouse,10);
	ini.WriteString("GENERAL" ,"g_station_name",general_option_instance.g_station_name);

	CDialogEx::OnCancel();
}

void general_option_dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void general_option_dlg::get_param(struct glob_param*g_p)
{
	ini_path = g_p->sFName;
	general_option_instance = g_p->general_option_instance;
}

void general_option_dlg::output_param(struct general_option_str*g_p)
{
	*g_p = general_option_instance;
}

BOOL general_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(general_option_instance.g_check_mes);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(general_option_instance.g_mes_ip);
	sText.Format("%d", general_option_instance.g_mes_port);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sText);
	GetDlgItem(IDC_EDIT9)->SetWindowTextA(general_option_instance.g_station_name);

	GetDlgItem(IDC_EDIT7)->SetWindowTextA(general_option_instance.g_sn_prompt);
	sText.Format("%d", general_option_instance.g_sn_start);
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(sText);
	sText.Format("%d", general_option_instance.g_sn_count);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(sText);

	sText.Format("%d", general_option_instance.g_ttycom_port);
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(sText);
	sText.Format("%d", general_option_instance.g_ttycom_baud);
	GetDlgItem(IDC_COMBO1)->SetWindowTextA(sText);
	sText.Format("%d", general_option_instance.g_check_mes_type);
	GetDlgItem(IDC_COMBO9)->SetWindowTextA(sText);
	m_mes_type.AddString("1");
	m_mes_type.AddString("2");
	m_mes_type.AddString("3");

	comb_ttybaud.AddString("1000000");
	comb_ttybaud.AddString("921600");
	comb_ttybaud.AddString("115200");
	//((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(general_option_instance.g_quit_config_dilog);

	((CButton *)GetDlgItem(IDC_CHECK5))->SetCheck(general_option_instance.g_if_open_nouse);

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_COMBO9))->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO9)->GetWindowTextA(sText);
		if(sText == "1" || sText == "3"){
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);			
		}
		if(sText == "3"){
			((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
		}else{
			((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
		}
	}else{
		((CButton*)GetDlgItem(IDC_COMBO9))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);	
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void general_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_COMBO9))->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO9)->GetWindowTextA(sText);
		if(sText == "1" || sText == "3"){
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);			
		}
		if(sText == "3"){
			((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
		}else{
			((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
		}
	}else{
		((CButton*)GetDlgItem(IDC_COMBO9))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);	
	}
}

void general_option_dlg::OnCbnSelchangeCombo9()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	m_mes_type.GetLBText(m_mes_type.GetCurSel(),sText);
	if(sText == "1" || sText == "3"){
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);		
	}else{
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);		
	}
	if(sText == "3"){
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT9))->EnableWindow(FALSE);
	}
}
