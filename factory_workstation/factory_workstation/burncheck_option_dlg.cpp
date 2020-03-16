// burncheck_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "burncheck_option_dlg.h"
#include "afxdialogex.h"


// burncheck_option_dlg 对话框

IMPLEMENT_DYNAMIC(burncheck_option_dlg, CDialogEx)

burncheck_option_dlg::burncheck_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(burncheck_option_dlg::IDD, pParent)
{

}

burncheck_option_dlg::~burncheck_option_dlg()
{
}

void burncheck_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, comb_current_ttybaud);
}


BEGIN_MESSAGE_MAP(burncheck_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &burncheck_option_dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &burncheck_option_dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &burncheck_option_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &burncheck_option_dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK1, &burncheck_option_dlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &burncheck_option_dlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK8, &burncheck_option_dlg::OnBnClickedCheck8)
END_MESSAGE_MAP()


// burncheck_option_dlg 消息处理程序

void burncheck_option_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(burncheck_option_instance.g_sJLinkPath);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(burncheck_option_instance.g_wrFirm);
	burncheck_option_instance.g_wrFirm_folder = firm_folder_path;
	burncheck_option_instance.g_burn_current_test = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();

	burncheck_option_instance.g_current_test = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	GetDlgItem(IDC_EDIT1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burncheck_option_instance.g_addCurrent);
	GetDlgItem(IDC_EDIT5)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burncheck_option_instance.g_addlarge);
	GetDlgItem(IDC_EDIT7)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burncheck_option_instance.g_chargCurrent);
	burncheck_option_instance.g_sleep_current_test = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();
	burncheck_option_instance.g_sleep_current_delay = ((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck();
	GetDlgItem(IDC_EDIT2)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burncheck_option_instance.g_sleepCurrent);

	GetDlgItem(IDC_EDIT6)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burncheck_option_instance.g_currentcom_port);
	GetDlgItem(IDC_COMBO1)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burncheck_option_instance.g_currentcom_baud);

	GetDlgItem(IDC_EDIT8)->GetWindowTextA(burncheck_option_instance.g_out_str);

	burncheck_option_instance.g_if_check_version_str = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();
	GetDlgItem(IDC_EDIT33)->GetWindowTextA(burncheck_option_instance.g_out_sn_str);
	GetDlgItem(IDC_EDIT16)->GetWindowTextA(burncheck_option_instance.g_out_languages_str);

	ini.SetPathName(ini_path);
	ini.WriteString("BURNCHECK" ,"g_sJLinkPath",burncheck_option_instance.g_sJLinkPath);
	ini.WriteString("BURNCHECK" ,"g_wrFirm",burncheck_option_instance.g_wrFirm);
	ini.WriteString("BURNCHECK" ,"g_wrFirm_folder",burncheck_option_instance.g_wrFirm_folder);
	ini.WriteUInt("BURNCHECK" ,"g_burn_current_test",burncheck_option_instance.g_burn_current_test,10);

	ini.WriteUInt("BURNCHECK" ,"g_current_test",burncheck_option_instance.g_current_test,10);
	ini.WriteUInt("BURNCHECK" ,"g_addCurrent",burncheck_option_instance.g_addCurrent,10);
	ini.WriteUInt("BURNCHECK" ,"g_addlarge",burncheck_option_instance.g_addlarge,10);
	ini.WriteUInt("BURNCHECK" ,"g_chargCurrent",burncheck_option_instance.g_chargCurrent,10);
	ini.WriteUInt("BURNCHECK" ,"g_sleep_current_test",burncheck_option_instance.g_sleep_current_test,10);
	ini.WriteUInt("BURNCHECK" ,"g_sleep_current_delay",burncheck_option_instance.g_sleep_current_delay,10);
	ini.WriteUInt("BURNCHECK" ,"g_sleepCurrent",burncheck_option_instance.g_sleepCurrent,10);

	ini.WriteUInt("BURNCHECK" ,"g_currentcom_port",burncheck_option_instance.g_currentcom_port,10);
	ini.WriteUInt("BURNCHECK" ,"g_currentcom_baud",burncheck_option_instance.g_currentcom_baud,10);
	ini.WriteString("BURNCHECK" ,"g_out_str",burncheck_option_instance.g_out_str);

	ini.WriteUInt("BURNCHECK" ,"g_if_check_version_str",burncheck_option_instance.g_if_check_version_str,10);
	ini.WriteString("BURNCHECK" ,"g_out_sn_str",burncheck_option_instance.g_out_sn_str);
	ini.WriteString("BURNCHECK" ,"g_out_languages_str",burncheck_option_instance.g_out_languages_str);
	CDialogEx::OnCancel();
}

void burncheck_option_dlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

void burncheck_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	burncheck_option_instance = g_p->burncheck_option_instance;
}

void burncheck_option_dlg::output_param(struct burncheck_option_str*bc_p)
{
	*bc_p = burncheck_option_instance;
}

BOOL burncheck_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(burncheck_option_instance.g_sJLinkPath);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(burncheck_option_instance.g_wrFirm);
	GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);	
	GetDlgItem(IDC_EDIT4)->EnableWindow(FALSE);	
	firm_folder_path = burncheck_option_instance.g_wrFirm_folder;
	((CButton *)GetDlgItem(IDC_CHECK6))->SetCheck(burncheck_option_instance.g_burn_current_test);
	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(burncheck_option_instance.g_current_test);
	sText.Format("%d", burncheck_option_instance.g_addCurrent);
	GetDlgItem(IDC_EDIT1)->SetWindowTextA(sText);
	sText.Format("%d", burncheck_option_instance.g_addlarge);
	GetDlgItem(IDC_EDIT5)->SetWindowTextA(sText);
	sText.Format("%d", burncheck_option_instance.g_chargCurrent);
	GetDlgItem(IDC_EDIT7)->SetWindowTextA(sText);
	((CButton *)GetDlgItem(IDC_CHECK2))->SetCheck(burncheck_option_instance.g_sleep_current_test);
	((CButton *)GetDlgItem(IDC_CHECK4))->SetCheck(burncheck_option_instance.g_sleep_current_delay);
	sText.Format("%d", burncheck_option_instance.g_sleepCurrent);
	GetDlgItem(IDC_EDIT2)->SetWindowTextA(sText);

	sText.Format("%d", burncheck_option_instance.g_currentcom_port);
	GetDlgItem(IDC_EDIT6)->SetWindowTextA(sText);
	sText.Format("%d", burncheck_option_instance.g_currentcom_baud);
	GetDlgItem(IDC_COMBO1)->SetWindowTextA(sText);
	comb_current_ttybaud.AddString("1000000");
	comb_current_ttybaud.AddString("921600");
	comb_current_ttybaud.AddString("115200");

	GetDlgItem(IDC_EDIT8)->SetWindowTextA(burncheck_option_instance.g_out_str);

	((CButton *)GetDlgItem(IDC_CHECK8))->SetCheck(burncheck_option_instance.g_if_check_version_str);
	GetDlgItem(IDC_EDIT33)->SetWindowTextA(burncheck_option_instance.g_out_sn_str);
	GetDlgItem(IDC_EDIT16)->SetWindowTextA(burncheck_option_instance.g_out_languages_str);

	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);	
		}
	}else{
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);	
	}

	if( ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
}


void burncheck_option_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString g_sJLinkPath;
	char fliter[]="j_link文件(*.exe)\0*.exe\0"
		"所有文件(*.*)\0*.*\0\0";
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrFilter = fliter;
	if(IDOK==fdlg.DoModal())
	{
		 g_sJLinkPath = fdlg.GetPathName();
		 GetDlgItem(IDC_EDIT3)->SetWindowTextA(g_sJLinkPath);
	}
}

void burncheck_option_dlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	char fliter[]="烧写文件(*.bin)\0*.bin\0"
		"所有文件(*.*)\0*.*\0\0";
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrFilter = fliter;
    
	CString file_path;
	if(IDOK==fdlg.DoModal())
	{
		firm_folder_path = fdlg.GetFolderPath();
		file_path = fdlg.GetPathName();
		GetDlgItem(IDC_EDIT4)->SetWindowTextA(file_path);
	}
}


void burncheck_option_dlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);	
		}
	}else{
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT1))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT5))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT7))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK2))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);	
	}
}


void burncheck_option_dlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_EDIT2))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_CHECK4))->EnableWindow(FALSE);	
	}
}


void burncheck_option_dlg::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_EDIT33))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT16))->EnableWindow(FALSE);	
	}
}
