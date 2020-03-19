// default_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "default_dlg.h"
#include "afxdialogex.h"
#include "factory_workstationDlg.h"
#include "software_by_burn.h"
#include "software_check_by.h"
#include "software_check_by_with_wifibt.h"
#include "software_total_machine_by.h"
#include "software_air_by.h"
#include "software_by_before_press_current.h"
#include "software_batpower_by.h"
#include "software_burn_sz.h"
#include "software_check_sz.h"
#include "software_sz_btaddr.h"
#include "software_sz_checkbox.h"
#include "software_sz_burn_sn.h"
// default_dlg 对话框

IMPLEMENT_DYNAMIC(default_dlg, CPropertyPage)

default_dlg::default_dlg()
	: CPropertyPage(default_dlg::IDD)
{

}

default_dlg::~default_dlg()
{
}

void default_dlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(default_dlg, CPropertyPage)
	ON_BN_CLICKED(ID_goon, &default_dlg::OnBnClickedgoon)
END_MESSAGE_MAP()


// default_dlg 消息处理程序
void default_dlg::OnBnClickedgoon()
{
	// TODO: 在此添加控件通知处理程序代码
	CString last_soft = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft;
	//if( ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.general_option_instance.g_quit_config_dilog ){
		HWND hwnd = GetParent()->GetParent()->m_hWnd;
		::SendMessage(hwnd,WM_CLOSE,0,0);
	//}
	if(last_soft == CHILD_DLG_NAME_FZ_BURN){
		software_burn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();
	}else if(last_soft == CHILD_DLG_NAME_FZ_CHECK){
		software_check_fz  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();
	}else if(last_soft == CHILD_DLG_NAME_FZ_BLUETOOTH){
		software_bluetooth_fz  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_FZ_AIR){
		software_air_fz  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_FZ_POWER){
		software_batpower  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_FZ_CHARGE){
		software_charge_fzjn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_JN_BURN){
		software_burn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();
	}else if(last_soft == CHILD_DLG_NAME_JN_CHECK){
		software_check_jn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();
	}else if(last_soft == CHILD_DLG_NAME_JN_BLUETOOTH){
		software_bluetooth_jn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_JN_POWER){
		software_batpower  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_JN_CHARGE){
		software_charge_fzjn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_BURN){
		software_by_burn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_CHECK){
		software_check_by  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_FULL_MACHINE){
		software_total_machine_by  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_AIR){
		software_air_by  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_BP_CURRENT){
		software_by_before_press_current  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_CHECK_WIFIBT){
		software_check_by_with_wifibt  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_BY_BATPOWER){
		software_batpower_by  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_SZ_BURN){
		software_burn_sz  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_SZ_CHECK){
		software_check_sz  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_SZ_BTADDR){
		software_sz_btaddr  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_SZ_CHECKBOX){
		software_sz_checkbox  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}else if(last_soft == CHILD_DLG_NAME_SZ_BURNSN){
		software_sz_burn_sn  Dlg; 
		Dlg.get_param( &( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance ) ;
		Dlg.DoModal();	
	}
}

void default_dlg::set_param(void *p)
{
	glob_param_p = p;
}

BOOL default_dlg::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString last_soft = ( (Cfactory_workstationDlg*)glob_param_p )->glob_param_instance.last_sel_soft;
	m_editFont.CreatePointFont(200, "宋体");
	GetDlgItem(IDC_STATIC)->SetFont(&m_editFont);
	GetDlgItem(IDC_STATIC)->SetWindowTextA(last_soft);
	return TRUE;  // return TRUE unless you set the focus to a control
}
