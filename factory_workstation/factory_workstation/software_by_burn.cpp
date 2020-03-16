// software_by_burn.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_by_burn.h"
#include "afxdialogex.h"

// software_by_burn 对话框

IMPLEMENT_DYNAMIC(software_by_burn, CDialogEx)

software_by_burn::software_by_burn(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_by_burn::IDD, pParent)
{

}

software_by_burn::~software_by_burn()
{
}

void software_by_burn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC9, m_Epass);
	DDX_Control(pDX, IDC_PROGRESS9, init_progress);
	DDX_Control(pDX, IDC_PROGRESS1, part1_progress);
	DDX_Control(pDX, IDC_PROGRESS5, part2_progress);
	DDX_Control(pDX, IDC_PROGRESS2, part3_progress);
	DDX_Control(pDX, IDC_PROGRESS6, part4_progress);
	DDX_Control(pDX, IDC_PROGRESS3, part5_progress);
	DDX_Control(pDX, IDC_PROGRESS7, part6_progress);
	DDX_Control(pDX, IDC_PROGRESS4, part7_progress);
	DDX_Control(pDX, IDC_PROGRESS8, part8_progress);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_kernel_version);
	DDX_Control(pDX, IDC_EDIT_FIRMVER3, m_fs_version);
}

BEGIN_MESSAGE_MAP(software_by_burn, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_by_burn::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON1, &software_by_burn::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &software_by_burn::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void software_by_burn::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);

	if(m_hLogFile.m_hFile != INVALID_HANDLE_VALUE) {
		m_hLogFile.Write(sTemp, sTemp.GetLength());
		m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	}
}

void software_by_burn::progress_setpos(msg_info *pmsg)
{
	switch(pmsg->data1){
		case MSG_T_PROGRESS_INIT:
			init_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART1:
			part1_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART2:
			part2_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART3:
			part3_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART4:
			part4_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART5:
			part5_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART6:
			part6_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART7:
			part7_progress.SetPos(pmsg->data2);
			break;
		case MSG_T_PROGRESS_PART8:
			part8_progress.SetPos(pmsg->data2);
			break;
		default:
			break;
	}
	if(pmsg->data2 == 100){
		switch(pmsg->data1){
			case MSG_T_PROGRESS_INIT:
				AddEvent2List("烧写初始化完成");
				break;
			case MSG_T_PROGRESS_PART1:
				AddEvent2List("uboot烧写完成");
				break;
			case MSG_T_PROGRESS_PART2:
				AddEvent2List("logo烧写完成");
				break;
			case MSG_T_PROGRESS_PART3:
				AddEvent2List("uImage烧写完成");
				break;
			case MSG_T_PROGRESS_PART4:
				AddEvent2List("recovery烧写完成");
				break;
			case MSG_T_PROGRESS_PART5:
				AddEvent2List("rootfs烧写完成");
				break;
			case MSG_T_PROGRESS_PART6:
				AddEvent2List("sn烧写完成");
				break;
			case MSG_T_PROGRESS_PART7:
				AddEvent2List("data烧写完成");
				break;
			case MSG_T_PROGRESS_PART8:
				AddEvent2List("usrfs烧写完成");
				break;
			default:
				break;
		}		
	}
}

void software_by_burn::err_process(msg_info *pmsg)
{
	switch(pmsg->data1){
		case MSG_T_STRING_GETCPU:
			AfxMessageBox("获取cpu信息失败");
			AddEvent2List("获取cpu信息失败");
			break;
		case MSG_T_STRING_GLOB_INFO:
			AfxMessageBox("初始化芯片参数失败");
			AddEvent2List("初始化芯片参数失败");
			break;
		case MSG_T_STRING_OPEN_SPL:
			AfxMessageBox("spl文件打开失败");
			AddEvent2List("spl文件打开失败");
			break;
		case MSG_T_STRING_RUN_SPL:
			AfxMessageBox("spl运行失败");
			AddEvent2List("spl运行失败");
			break;
		case MSG_T_STRING_GETCPU2:
			AfxMessageBox("第二次获取cpu信息失败");
			AddEvent2List("第二次获取cpu信息失败");
			break;
		case MSG_T_STRING_OPEN_UBOOT:
			AfxMessageBox("uboot文件打开失败");
			AddEvent2List("uboot文件打开失败");
			break;
		case MSG_T_STRING_RUN_UBOOT:
			AfxMessageBox("烧写uboot运行失败");
			AddEvent2List("烧写uboot运行失败");
			break;
		case MSG_T_STRING_SEND_CMD:
			AfxMessageBox("发送cmd失败");
			AddEvent2List("发送cmd失败");
			break;
		case MSG_T_STRING_GET_FLASH_INFO:
			AfxMessageBox("获取flash信息失败");
			AddEvent2List("获取flash信息失败");
			break;
		case MSG_T_STRING_SEND_CMD2:
			AfxMessageBox("第二次发送cmd失败");
			AddEvent2List("第二次发送cmd失败");
			break;
		case MSG_T_STRING_BURN_INIT:
			AfxMessageBox("运行erase失败");
			AddEvent2List("运行erase失败");
			break;
		case MSG_T_STRING_CRC_ERR:
			AfxMessageBox("烧写crc校验失败");
			AddEvent2List("烧写crc校验失败");
			break;
		case MSG_T_STRING_EIO_ERR:
			AfxMessageBox("烧写eio失败");
			AddEvent2List("烧写eio失败");
			break;
		case MSG_T_STRING_PERM_ERR:
			AfxMessageBox("烧写perm失败");
			AddEvent2List("烧写perm失败");
			break;
		case MSG_T_STRING_DISCONNECT:
			AfxMessageBox("usb失去连接");
			AddEvent2List("usb失去连接");
			break;
		default:
			break;
	}
}

LRESULT software_by_burn::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	TRACE("lalala\n");
	msg_info *pmsg = (msg_info*)lParam;
	switch(pmsg->msg_type) {
		case MSG_BY_T_BTN_START:
			AddEvent2List("开始启动烧写");
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
			break;
		case MSG_BY_T_PROGRESS:
			progress_setpos(pmsg);
			break;
		case MSG_BY_T_STRING:
			err_process(pmsg);
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case MSG_BY_T_SMES_SEND:
			AddEvent2List("mes开始包已发送! ");
			break;
		case MSG_BY_T_EMES_SEND:
			//AfxMessageBox("mes包已发送! ");
			AddEvent2List("mes包已发送! ");
			break;
		case MSG_BY_T_EMES_ERR:
			AddEvent2List("mes连不上服务器,或者机器已过站! ");
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case MSG_BY_T_ALL_SUSS:
			AddEvent2List("烧写结束，等待用户复位");
			((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			pass_green_color = 1;
			m_Epass.SetWindowTextA("PASS");
			Invalidate();	
			break;
		default:
			break;
	}
	delete pmsg;
	return 0;
}

void software_by_burn::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	burn_by_option_instance = g_p->burn_by_option_instance;
}

void software_by_burn::all_init()
{
	m_EditSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	pass_green_color = 0;

	init_progress.SetPos(0);
	part1_progress.SetPos(0);
	part2_progress.SetPos(0);
	part3_progress.SetPos(0);
	part4_progress.SetPos(0);
	part5_progress.SetPos(0);
	part6_progress.SetPos(0);
	part7_progress.SetPos(0);
	part8_progress.SetPos(0);

	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	libusbDevListener_int.start_burn = FALSE;
	AddEvent2List("#####等待用户输入sn号#####");
}

// software_by_burn 消息处理程序
void software_by_burn::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	all_init();
}

int software_by_burn::need_file_ifexit()
{
	CString sText;
	sText = burn_by_option_instance.g_wrFirm_folder;
	if(PathFileExists("firmware\\burn\\spl_lpddr.bin") != TRUE){
		AfxMessageBox("错误：软件burn路径下未找到spl_lpddr.bin");
		return 1;
	}
	if(PathFileExists("firmware\\burn\\uboot.bin") != TRUE){
		AfxMessageBox("错误：软件burn路径下未找到uboot.bin");
		return 1;
	}
	if(PathFileExists(sText+"\\u-boot-with-spl.bin") != TRUE){
		AfxMessageBox("警告：固件路径下未找到u-boot-with-spl.bin");
		return 1;
	}
	if(PathFileExists(sText+"\\uImage") != TRUE){
		AfxMessageBox("警告：固件路径下未找到uImage");
		return 1;
	}
	if(PathFileExists(sText+"\\recovery") != TRUE){
		AfxMessageBox("警告：固件路径下未找到recovery");
		return 1;
	}
	if(PathFileExists(sText+"\\rootfs.ubifs") != TRUE){
		AfxMessageBox("警告：固件路径下未找到rootfs.ubifs");
		return 1;
	}
	if(PathFileExists(sText+"\\config.ubifs") != TRUE){
		AfxMessageBox("警告：固件路径下未找到config.ubifs");
		return 1;
	}
	if(PathFileExists(sText+"\\buildversion") != TRUE){
		AfxMessageBox("警告：固件路径下未找到buildversion");
		m_fs_version.SetWindowTextA("V0.00.00");
		m_kernel_version.SetWindowTextA("");
		return 1;
	}else{
		CFile version_File;
		CString version_str = "";
		int length;
		int g_size ;
		char* g_buf;
		sText += "\\buildversion";
		if(version_File.Open(sText, CFile::modeRead) == TRUE) {
			version_File.SeekToBegin();
			g_size = (int)version_File.GetLength();
			g_buf = (char*)malloc(g_size+1);
			*(g_buf+g_size) = 0;
			version_File.Read(g_buf,g_size);
			version_File.Close();
			version_str += g_buf;
			if( (length = version_str.Find("rootfsversion=")) >= 0){
				m_fs_version.SetWindowTextA( version_str.Mid(length+sizeof("rootfsversion=")-1, 8) );
			}
			if( (length = version_str.Find("Linux-")) >= 0){
				length = version_str.Delete(0,length);
				m_kernel_version.SetWindowTextA( version_str.Right(length - sizeof("Linux-") + 1) );
			}
			free(g_buf);
		}else{
			AfxMessageBox("警告：buildversion文件打开失败");
			m_fs_version.SetWindowTextA("V0.00.00");
			m_kernel_version.SetWindowTextA("");
			return 1;
		}
	}

	return 0;
}

BOOL software_by_burn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	libusbDevListener_int.hwnd = this->m_hWnd;
	libusbDevListener_int.burn_by_option_instance = burn_by_option_instance;
	libusbDevListener_int.general_option_instance = general_option_instance;

	CString dir_path;
	dir_path = LOG_PATH_BURN_by;

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);
	CString sFname;
	CTime tNow = CTime::GetCurrentTime();
	sFname.Format("LOG-CH%d-%04d%02d%02d-%dh.txt", 0,
		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour());
	sFname = dir_path + sFname;

	if(m_hLogFile.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		m_hLogFile.SeekToEnd();
		sFname.Format("===============================================\r\n");
		m_hLogFile.Write(sFname, sFname.GetLength());
	}

	m_editFont.CreatePointFont(700, "宋体");
	m_Epass.SetFont(&m_editFont);

	init_progress.SetRange(0, 100);
	init_progress.SetStep(1);
	part1_progress.SetRange(0, 100);
	part1_progress.SetStep(1);
	part2_progress.SetRange(0, 100);
	part2_progress.SetStep(1);
	part3_progress.SetRange(0, 100);
	part3_progress.SetStep(1);
	part4_progress.SetRange(0, 100);
	part4_progress.SetStep(1);
	part5_progress.SetRange(0, 100);
	part5_progress.SetStep(1);
	part6_progress.SetRange(0, 100);
	part6_progress.SetStep(1);
	part7_progress.SetRange(0, 100);
	part7_progress.SetStep(1);
	part8_progress.SetRange(0, 100);
	part8_progress.SetStep(1);
	need_file_ifexit();
	all_init();
	return TRUE;  // return TRUE unless you set the focus to a control
}


extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
extern bool check_firmware_canuse_v2sn(CString g_wrFirm);
void software_by_burn::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
				libusbDevListener_int.start_burn = FALSE;
			}else{
				libusbDevListener_int.start_burn = TRUE;
				AddEvent2List("sn号输入成功，等待上电");
			}
			memset(libusbDevListener_int.sn_num, 0 ,sizeof(libusbDevListener_int.sn_num));
			memcpy(libusbDevListener_int.sn_num, sText, 18);
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
			libusbDevListener_int.start_burn = FALSE;
		}else{
			libusbDevListener_int.start_burn = TRUE;
			AddEvent2List("sn号输入成功，等待上电");
		}
		memset(libusbDevListener_int.sn_num, 0 ,sizeof(libusbDevListener_int.sn_num));
		memcpy(libusbDevListener_int.sn_num, sText, 19);
	}
	// TODO:  在此添加控件通知处理程序代码
}


BOOL software_by_burn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH software_by_burn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC9)//如果是静态编辑框
    {
		if(pass_green_color){
			pDC->SetTextColor(RGB(0,255,0));//修改字体的颜色
			pDC->SetBkMode(TRANSPARENT);//把字体的背景变成透明的
		}else{
		    pDC->SetTextColor(RGB(255,0,0));//修改字体的颜色
			pDC->SetBkMode(TRANSPARENT);//把字体的背景变成透明的
		}
    }
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
