// burn_by_option_dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "burn_by_option_dlg.h"
#include "afxdialogex.h"


// burn_by_option_dlg 对话框

IMPLEMENT_DYNAMIC(burn_by_option_dlg, CDialogEx)

burn_by_option_dlg::burn_by_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(burn_by_option_dlg::IDD, pParent)
{

}

burn_by_option_dlg::~burn_by_option_dlg()
{
}

void burn_by_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, part1_mode);
	DDX_Control(pDX, IDC_COMBO2, part2_mode);
	DDX_Control(pDX, IDC_COMBO3, part3_mode);
	DDX_Control(pDX, IDC_COMBO4, part4_mode);
	DDX_Control(pDX, IDC_COMBO5, part5_mode);
	DDX_Control(pDX, IDC_COMBO6, part6_mode);
	DDX_Control(pDX, IDC_COMBO7, part7_mode);
	DDX_Control(pDX, IDC_COMBO8, part8_mode);
}

BEGIN_MESSAGE_MAP(burn_by_option_dlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &burn_by_option_dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &burn_by_option_dlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &burn_by_option_dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &burn_by_option_dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK7, &burn_by_option_dlg::OnBnClickedCheck7)
END_MESSAGE_MAP()

void burn_by_option_dlg::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	burn_by_option_instance.g_wrFirm_folder = g_p->burn_by_option_instance.g_wrFirm_folder;
	memcpy(&burn_by_option_instance.spinand_partition_ins, &g_p->burn_by_option_instance.spinand_partition_ins, sizeof(spinand_partition)*8);
}

void burn_by_option_dlg::output_param(struct burn_by_option_str *bc_p)
{
	bc_p->g_wrFirm_folder = burn_by_option_instance.g_wrFirm_folder;
	memcpy(&bc_p->spinand_partition_ins, &burn_by_option_instance.spinand_partition_ins, sizeof(spinand_partition)*8);
}
// burn_by_option_dlg 消息处理程序

BOOL burn_by_option_dlg::OnInitDialog()
{
	int i;
	CString sText;
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	GetDlgItem(IDC_EDIT3)->SetWindowTextA(burn_by_option_instance.g_wrFirm_folder);

	int name_dlg[8] = {IDC_EDIT25, IDC_EDIT26, IDC_EDIT27, IDC_EDIT28, IDC_EDIT29, IDC_EDIT30, IDC_EDIT31, IDC_EDIT32};
	int offset_dlg[8] = {IDC_EDIT1, IDC_EDIT15, IDC_EDIT9, IDC_EDIT12, IDC_EDIT16, IDC_EDIT19, IDC_EDIT21, IDC_EDIT23};
	int size_dlg[8] = {IDC_EDIT2, IDC_EDIT4, IDC_EDIT10, IDC_EDIT13, IDC_EDIT17, IDC_EDIT20, IDC_EDIT22, IDC_EDIT24};
	int mode_dlg[8] = {IDC_COMBO1, IDC_COMBO2, IDC_COMBO3, IDC_COMBO4, IDC_COMBO5, IDC_COMBO6, IDC_COMBO7, IDC_COMBO8};
	for(i=0; i<8; i++){
		GetDlgItem(name_dlg[i])->SetWindowTextA(burn_by_option_instance.spinand_partition_ins[i].name);
		if(burn_by_option_instance.spinand_partition_ins[i].manager_mode == 0){//MTD_MODE
			GetDlgItem(mode_dlg[i])->SetWindowTextA("MTD_MODE");
		}else if(burn_by_option_instance.spinand_partition_ins[i].manager_mode == 1){//UBI_MANAGER
			GetDlgItem(mode_dlg[i])->SetWindowTextA("UBI_MANAGER");
		}
		sText.Format("0x%x",burn_by_option_instance.spinand_partition_ins[i].offset);
		GetDlgItem(offset_dlg[i])->SetWindowTextA(sText);
		if(i==7){
			if(burn_by_option_instance.spinand_partition_ins[i].size == 0){
				sText.Format("~",burn_by_option_instance.spinand_partition_ins[i].size);
				GetDlgItem(size_dlg[i])->SetWindowTextA(sText);	
			}else{
				sText.Format("0x%x",burn_by_option_instance.spinand_partition_ins[i].size);
				GetDlgItem(size_dlg[i])->SetWindowTextA(sText);			
			}
		}else{
			sText.Format("0x%x",burn_by_option_instance.spinand_partition_ins[i].size);
			GetDlgItem(size_dlg[i])->SetWindowTextA(sText);		
		}
	}
	part1_mode.AddString("MTD_MODE");
	part1_mode.AddString("UBI_MANAGER");
	part2_mode.AddString("MTD_MODE");
	part2_mode.AddString("UBI_MANAGER");
	part3_mode.AddString("MTD_MODE");
	part3_mode.AddString("UBI_MANAGER");
	part4_mode.AddString("MTD_MODE");
	part4_mode.AddString("UBI_MANAGER");
	part5_mode.AddString("MTD_MODE");
	part5_mode.AddString("UBI_MANAGER");
	part6_mode.AddString("MTD_MODE");
	part6_mode.AddString("UBI_MANAGER");
	part7_mode.AddString("MTD_MODE");
	part7_mode.AddString("UBI_MANAGER");
	part8_mode.AddString("MTD_MODE");
	part8_mode.AddString("UBI_MANAGER");

	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(0);
	OnBnClickedCheck7();
	return TRUE;  // return TRUE unless you set the focus to a control
}


void burn_by_option_dlg::OnBnClickedButton3()
{
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


void burn_by_option_dlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	CString part_name;
	CString sText;
	CIni ini;
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(burn_by_option_instance.g_wrFirm_folder);

	int name_dlg[8] = {IDC_EDIT25, IDC_EDIT26, IDC_EDIT27, IDC_EDIT28, IDC_EDIT29, IDC_EDIT30, IDC_EDIT31, IDC_EDIT32};
	int offset_dlg[8] = {IDC_EDIT1, IDC_EDIT15, IDC_EDIT9, IDC_EDIT12, IDC_EDIT16, IDC_EDIT19, IDC_EDIT21, IDC_EDIT23};
	int size_dlg[8] = {IDC_EDIT2, IDC_EDIT4, IDC_EDIT10, IDC_EDIT13, IDC_EDIT17, IDC_EDIT20, IDC_EDIT22, IDC_EDIT24};
	int mode_dlg[8] = {IDC_COMBO1, IDC_COMBO2, IDC_COMBO3, IDC_COMBO4, IDC_COMBO5, IDC_COMBO6, IDC_COMBO7, IDC_COMBO8};
	for(i=0; i<8; i++){
		GetDlgItem(name_dlg[i])->GetWindowTextA(sText);
		memset(burn_by_option_instance.spinand_partition_ins[i].name, 0, 32);
		memcpy(burn_by_option_instance.spinand_partition_ins[i].name, sText, (sText.GetLength()>=31) ? 31:sText.GetLength());

		GetDlgItem(mode_dlg[i])->GetWindowTextA(sText);
		if(sText == "MTD_MODE"){
			burn_by_option_instance.spinand_partition_ins[i].manager_mode = 0;
		}else if(sText == "UBI_MANAGER"){
			burn_by_option_instance.spinand_partition_ins[i].manager_mode = 1;
		}

		GetDlgItem(offset_dlg[i])->GetWindowTextA(sText);
		StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burn_by_option_instance.spinand_partition_ins[i].offset);
		GetDlgItem(size_dlg[i])->GetWindowTextA(sText);
		if(sText == "~"){
			burn_by_option_instance.spinand_partition_ins[i].size = 0;
		}else
			StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&burn_by_option_instance.spinand_partition_ins[i].size);
	}

	ini.SetPathName(ini_path);
	ini.WriteString("BY_BURN" ,"g_wrFirm_folder",burn_by_option_instance.g_wrFirm_folder);
	for(i=0; i<8; i++){
		part_name.Format("BY_BURN_PART%d",i);
		ini.WriteUInt(part_name,"manager_mode",burn_by_option_instance.spinand_partition_ins[i].manager_mode,10);
		ini.WriteUInt(part_name,"offset",burn_by_option_instance.spinand_partition_ins[i].offset,16);
		ini.WriteUInt(part_name,"size",burn_by_option_instance.spinand_partition_ins[i].size,16);
		ini.WriteString(part_name,"name",burn_by_option_instance.spinand_partition_ins[i].name);
	}

	CDialogEx::OnCancel();
}


void burn_by_option_dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void burn_by_option_dlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	spinand_partition partition_ins[8]={
		{"uboot",		0x100000,	0x0,		0, 0},
		{"logo",		0x100000,	0x100000,	0, 0},
		{"kernel",		0x500000,	0x200000,	0, 0},
		{"recovery",	0xf00000,	0x700000,	0, 0},
		{"rootfs",		0x3200000,	0x1600000,	0, 1},
		{"sn",			0x200000,	0x4800000,	0, 0},
		{"data",		0x800000,	0x4a00000,	0, 1},
		{"usrfs",		0x0,		0x5200000,	0, 1},//0xae00000 for 256m flash, 0x2e00000 for 128m flash
	};

	int i;
	CString sText;
	int name_dlg[8] = {IDC_EDIT25, IDC_EDIT26, IDC_EDIT27, IDC_EDIT28, IDC_EDIT29, IDC_EDIT30, IDC_EDIT31, IDC_EDIT32};
	int offset_dlg[8] = {IDC_EDIT1, IDC_EDIT15, IDC_EDIT9, IDC_EDIT12, IDC_EDIT16, IDC_EDIT19, IDC_EDIT21, IDC_EDIT23};
	int size_dlg[8] = {IDC_EDIT2, IDC_EDIT4, IDC_EDIT10, IDC_EDIT13, IDC_EDIT17, IDC_EDIT20, IDC_EDIT22, IDC_EDIT24};
	int mode_dlg[8] = {IDC_COMBO1, IDC_COMBO2, IDC_COMBO3, IDC_COMBO4, IDC_COMBO5, IDC_COMBO6, IDC_COMBO7, IDC_COMBO8};
	for(i=0; i<8; i++){
		GetDlgItem(name_dlg[i])->SetWindowTextA(partition_ins[i].name);
		if(partition_ins[i].manager_mode == 0){//MTD_MODE
			GetDlgItem(mode_dlg[i])->SetWindowTextA("MTD_MODE");
		}else if(partition_ins[i].manager_mode == 1){//UBI_MANAGER
			GetDlgItem(mode_dlg[i])->SetWindowTextA("UBI_MANAGER");
		}
		sText.Format("0x%x",partition_ins[i].offset);
		GetDlgItem(offset_dlg[i])->SetWindowTextA(sText);
		if(i==7){
			if(partition_ins[i].size == 0){
				sText.Format("~",partition_ins[i].size);
				GetDlgItem(size_dlg[i])->SetWindowTextA(sText);	
			}else{
				sText.Format("0x%x",partition_ins[i].size);
				GetDlgItem(size_dlg[i])->SetWindowTextA(sText);			
			}
		}else{
			sText.Format("0x%x",partition_ins[i].size);
			GetDlgItem(size_dlg[i])->SetWindowTextA(sText);		
		}
	}
}


void burn_by_option_dlg::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	int i;
	CString sText;
	int name_dlg[8] = {IDC_EDIT25, IDC_EDIT26, IDC_EDIT27, IDC_EDIT28, IDC_EDIT29, IDC_EDIT30, IDC_EDIT31, IDC_EDIT32};
	int offset_dlg[8] = {IDC_EDIT1, IDC_EDIT15, IDC_EDIT9, IDC_EDIT12, IDC_EDIT16, IDC_EDIT19, IDC_EDIT21, IDC_EDIT23};
	int size_dlg[8] = {IDC_EDIT2, IDC_EDIT4, IDC_EDIT10, IDC_EDIT13, IDC_EDIT17, IDC_EDIT20, IDC_EDIT22, IDC_EDIT24};
	int mode_dlg[8] = {IDC_COMBO1, IDC_COMBO2, IDC_COMBO3, IDC_COMBO4, IDC_COMBO5, IDC_COMBO6, IDC_COMBO7, IDC_COMBO8};
	if( ((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck() ){
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
		for(i=0; i<8; i++){
			((CButton*)GetDlgItem(name_dlg[i]))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(offset_dlg[i]))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(size_dlg[i]))->EnableWindow(TRUE);
			((CButton*)GetDlgItem(mode_dlg[i]))->EnableWindow(TRUE);
		}
	}else{
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
		for(i=0; i<8; i++){
			((CButton*)GetDlgItem(name_dlg[i]))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(offset_dlg[i]))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(size_dlg[i]))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(mode_dlg[i]))->EnableWindow(FALSE);
		}
	}

}
