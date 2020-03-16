// burn_sz_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "burn_sz_option_dlg.h"
#include "afxdialogex.h"


// burn_sz_option_dlg 对话框

IMPLEMENT_DYNAMIC(burn_sz_option_dlg, CDialogEx)

burn_sz_option_dlg::burn_sz_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(burn_sz_option_dlg::IDD, pParent)
{

}

burn_sz_option_dlg::~burn_sz_option_dlg()
{
}

void burn_sz_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(burn_sz_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &burn_sz_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &burn_sz_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3, &burn_sz_option_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK6, &burn_sz_option_dlg::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_CHECK9, &burn_sz_option_dlg::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_BUTTON4, &burn_sz_option_dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_CHECK12, &burn_sz_option_dlg::OnBnClickedCheck12)
	ON_BN_CLICKED(IDC_CHECK8, &burn_sz_option_dlg::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK10, &burn_sz_option_dlg::OnBnClickedCheck10)
	ON_BN_CLICKED(IDC_CHECK21, &burn_sz_option_dlg::OnBnClickedCheck21)
	ON_BN_CLICKED(IDC_CHECK22, &burn_sz_option_dlg::OnBnClickedCheck22)
END_MESSAGE_MAP()

// burn_sz_option_dlg 消息处理程序
void burn_sz_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	burn_sz_option_instance = g_p->burn_sz_option_instance;
}

void burn_sz_option_dlg::output_param(struct burn_sz_option_str *bc_p)
{
	*bc_p = burn_sz_option_instance;
}

void burn_sz_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	CString sText;

	GetDlgItem(IDC_EDIT3)->GetWindowTextA(burn_sz_option_instance.g_wrFirm_folder);
	burn_sz_option_instance.g_if_burn_qcc = ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck();
	burn_sz_option_instance.g_if_erase_flash = ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck();
	burn_sz_option_instance.g_if_burn_param = ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck();
	burn_sz_option_instance.g_if_test_after_burn = ((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck();
	burn_sz_option_instance.g_if_burn_apollo = ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck();
	burn_sz_option_instance.g_if_burn_box = ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck();
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(burn_sz_option_instance.g_sJLinkPath);
	GetDlgItem(IDC_EDIT15)->GetWindowTextA(burn_sz_option_instance.g_hw_version);
	GetDlgItem(IDC_EDIT44)->GetWindowTextA(burn_sz_option_instance.g_usbdbg_num);
	burn_sz_option_instance.g_if_read_freq = ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck();
	burn_sz_option_instance.g_if_write_freq = ((CButton*)GetDlgItem(IDC_CHECK22))->GetCheck();
	GetDlgItem(IDC_EDIT48)->GetWindowTextA(sText);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burn_sz_option_instance.g_readfreq_waittime);

	ini.SetPathName(ini_path);
	ini.WriteString("SZ_BURN" ,"g_wrFirm_folder",burn_sz_option_instance.g_wrFirm_folder);
	ini.WriteUInt("SZ_BURN" ,"g_if_burn_qcc",burn_sz_option_instance.g_if_burn_qcc,10);
	ini.WriteUInt("SZ_BURN" ,"g_if_erase_flash",burn_sz_option_instance.g_if_erase_flash,10);
	ini.WriteUInt("SZ_BURN" ,"g_if_burn_param",burn_sz_option_instance.g_if_burn_param,10);
	ini.WriteUInt("SZ_BURN" ,"g_if_test_after_burn",burn_sz_option_instance.g_if_test_after_burn,10);
	ini.WriteUInt("SZ_BURN" ,"g_if_burn_apollo",burn_sz_option_instance.g_if_burn_apollo,10);
	ini.WriteUInt("SZ_BURN" ,"g_if_burn_box",burn_sz_option_instance.g_if_burn_box,10);
	ini.WriteString("SZ_BURN" ,"g_sJLinkPath",burn_sz_option_instance.g_sJLinkPath);
	ini.WriteString("SZ_BURN" ,"g_hw_version",burn_sz_option_instance.g_hw_version);
	ini.WriteString("SZ_BURN" ,"g_usbdbg_num",burn_sz_option_instance.g_usbdbg_num);
	ini.WriteUInt("SZ_BURN" ,"g_if_read_freq",burn_sz_option_instance.g_if_read_freq,10);
	ini.WriteUInt("SZ_BURN" ,"g_readfreq_waittime",burn_sz_option_instance.g_readfreq_waittime,10);
	ini.WriteUInt("SZ_BURN" ,"g_if_write_freq",burn_sz_option_instance.g_if_write_freq,10);

	CDialogEx::OnOK();
}


void burn_sz_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL burn_sz_option_dlg::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(burn_sz_option_instance.g_wrFirm_folder);
	((CButton *)GetDlgItem(IDC_CHECK12))->SetCheck(burn_sz_option_instance.g_if_burn_qcc);
	((CButton *)GetDlgItem(IDC_CHECK10))->SetCheck(burn_sz_option_instance.g_if_erase_flash);
	((CButton *)GetDlgItem(IDC_CHECK9))->SetCheck(burn_sz_option_instance.g_if_burn_param);
	((CButton *)GetDlgItem(IDC_CHECK11))->SetCheck(burn_sz_option_instance.g_if_test_after_burn);
	((CButton *)GetDlgItem(IDC_CHECK6))->SetCheck(burn_sz_option_instance.g_if_burn_apollo);
	((CButton *)GetDlgItem(IDC_CHECK8))->SetCheck(burn_sz_option_instance.g_if_burn_box);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(burn_sz_option_instance.g_sJLinkPath);
	GetDlgItem(IDC_EDIT15)->SetWindowTextA(burn_sz_option_instance.g_hw_version);
	GetDlgItem(IDC_EDIT44)->SetWindowTextA(burn_sz_option_instance.g_usbdbg_num);
	((CButton *)GetDlgItem(IDC_CHECK21))->SetCheck(burn_sz_option_instance.g_if_read_freq);
	((CButton *)GetDlgItem(IDC_CHECK22))->SetCheck(burn_sz_option_instance.g_if_write_freq);
	sText.Format("%d", burn_sz_option_instance.g_readfreq_waittime);
	GetDlgItem(IDC_EDIT48)->SetWindowTextA(sText);

	if( ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck() ||
		((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()){
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE);	
	}

	if( ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK12))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK9))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK11))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
	}else{
		((CButton*)GetDlgItem(IDC_CHECK12))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK9))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK11))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(FALSE);	
		}

		if( ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(TRUE);
			if( ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(TRUE);	
			}else{
				((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
			}
			((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(TRUE);	
			((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(TRUE);	
			if( ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(TRUE);	
			}else{
				((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
			}
		}else{
			((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
		}
		if( ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE);	
		}	
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}


void burn_sz_option_dlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR szFolderPath[MAX_PATH] = {0}; 
	CString strFolderPath = TEXT("");

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("请选择处理结果存储路径");  
	//sInfo.ulFlags   = BIF_RETURNONLYFSDIRS|BIF_EDITBOX|BIF_DONTGOBELOWDOMAIN;  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
			GetDlgItem(IDC_EDIT3)->SetWindowTextA(strFolderPath);
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}
}


void burn_sz_option_dlg::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck() ||
		((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck()){
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE);	
	}
}


void burn_sz_option_dlg::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(FALSE);	
	}
}


void burn_sz_option_dlg::OnBnClickedButton4()
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
		 GetDlgItem(IDC_EDIT4)->SetWindowTextA(g_sJLinkPath);
	}
}


void burn_sz_option_dlg::OnBnClickedCheck12()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
		}
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(TRUE);	
		((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(TRUE);	
		if( ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
		}
	}else{
		((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
	}
}


void burn_sz_option_dlg::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_CHECK12))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK9))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK11))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
	}else{
		((CButton*)GetDlgItem(IDC_CHECK12))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK9))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK11))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_CHECK6))->EnableWindow(TRUE);
		if( ((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT15))->EnableWindow(FALSE);	
		}

		if( ((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(TRUE);
			if( ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(TRUE);	
			}else{
				((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
			}
			((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(TRUE);	
			((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(TRUE);	
			if( ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck() ){
				((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(TRUE);	
			}else{
				((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
			}
		}else{
			((CButton*)GetDlgItem(IDC_CHECK10))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK21))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_CHECK22))->EnableWindow(FALSE);	
			((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
		}
		if( ((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck() ){
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(TRUE);	
		}else{
			((CButton*)GetDlgItem(IDC_EDIT4))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BUTTON4))->EnableWindow(FALSE);	
		}	
	}
}


void burn_sz_option_dlg::OnBnClickedCheck10()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_EDIT44))->EnableWindow(FALSE);	
	}
}


void burn_sz_option_dlg::OnBnClickedCheck21()
{
	// TODO: 在此添加控件通知处理程序代码
	if( ((CButton*)GetDlgItem(IDC_CHECK21))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(TRUE);	
	}else{
		((CButton*)GetDlgItem(IDC_EDIT48))->EnableWindow(FALSE);	
	}
}


void burn_sz_option_dlg::OnBnClickedCheck22()
{
	// TODO: 在此添加控件通知处理程序代码
}
