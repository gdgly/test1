// software_sz_btaddr.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_sz_btaddr.h"
#include "afxdialogex.h"


// software_sz_btaddr 对话框

IMPLEMENT_DYNAMIC(software_sz_btaddr, CDialogEx)

software_sz_btaddr::software_sz_btaddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_sz_btaddr::IDD, pParent)
{

}

software_sz_btaddr::~software_sz_btaddr()
{
}

void software_sz_btaddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC9, m_Epass);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT15, m_btaddr);
	DDX_Control(pDX, IDC_EDIT45, m_btname);
	DDX_Control(pDX, IDC_EDIT46, m_hw_version);
	DDX_Control(pDX, IDC_EDIT47, m_lr_flag);
}


BEGIN_MESSAGE_MAP(software_sz_btaddr, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &software_sz_btaddr::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &software_sz_btaddr::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_DEV_ERROR, &software_sz_btaddr::OnDevCtrlError)
	ON_MESSAGE(WM_DEV_REPORT, &software_sz_btaddr::OnDevCtrlReport)
END_MESSAGE_MAP()


void software_sz_btaddr::AddEvent2List(CString sText)
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

void software_sz_btaddr::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	btaddr_sz_option_instance = g_p->btaddr_sz_option_instance;
}
// software_sz_btaddr 消息处理程序

LRESULT software_sz_btaddr::OnDevCtrlError(WPARAM wParam, LPARAM lParam)
{
	CString sText,stmp;
	switch(wParam){
		case ERROR_GETPORT:
			sText = "ERROR_GETPORT";
			break;
		case ERROR_GETPORT_NOPORT:
			sText = "ERROR_GETPORT_NOPORT";
			break;
		case ERROR_OPENTRANS:
			sText = "ERROR_OPENTRANS";
			break;
		case ERROR_OPEN_FLASHFILE:
			sText = "ERROR_OPEN_FLASHFILE";
			break;
		case ERROR_ERASE_START:
			sText = "ERROR_ERASE_START";
			break;
		case ERROR_ERASE_SETBANK:
			sText = "ERROR_ERASE_SETBANK";
			break;
		case ERROR_PROGRAM_START:
			sText = "ERROR_PROGRAM_START";
			break;
		case ERROR_PROGRAM_FAIL:
			sText = "ERROR_PROGRAM_FAIL";
			break;
		case ERROR_FLASH_TIMEOUT:
			sText = "ERROR_FLASH_TIMEOUT";
			break;
		case ERROR_OPEN_ENGINE:
			sText = "ERROR_OPEN_ENGINE";
			break;
		case ERROR_CACHE_INIT:
			sText = "ERROR_CACHE_INIT";
			break;
		case ERROR_CACHE_READ:
			sText = "ERROR_CACHE_READ";
			break;
		case ERROR_READ_DEVNAME:
			sText = "ERROR_READ_DEVNAME";
			break;
		case ERROR_READ_BDADDR:
			sText = "ERROR_READ_BDADDR";
			break;
		case ERROR_WRITE_BTADDR:
			sText = "ERROR_WRITE_BTADDR";
			break;
		case ERROR_WRITE_DEVNAME:
			sText = "ERROR_WRITE_DEVNAME";
			break;
		case ERROR_WRITE_CACHE:
			sText = "ERROR_WRITE_CACHE";
			break;
		case ERROR_WRITE_FIXPARAM:
			sText = "ERROR_WRITE_FIXPARAM";
			break;
		case ERROR_APOLLO:
			sText = "ERROR_APOLLO";
			break;
		case ERROR_READ_RECORD:
			sText = "ERROR_READ_RECORD";
			break;
		case ERROR_COMMU_FAIL:
			sText = "ERROR_COMMU_FAIL";
			break;
	
	}
	sText = "ERROR: err msg = " + sText;
	AddEvent2List(sText);

	pass_green_color = 0;
	m_Epass.SetWindowTextA("FAIL");
	Invalidate();	
	err_flag = 1;
	return 0;
}

LRESULT software_sz_btaddr::OnDevCtrlReport(WPARAM wParam, LPARAM lParam)
{
	CString sText;

	switch ((int)wParam) {
	case REPORT_THREAD_START:
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
		AddEvent2List("启动烧写进程！");
		break;
	case REPORT_CHIPID:
		AddEvent2List("找到芯片id！");
		break;
	case REPORT_ERASE_START:
		AddEvent2List("开始擦除flash！");
		break;
	case REPORT_ERASE_PROGRESS:
		AddEvent2List("正在擦除flash！");
		break;
	case REPORT_PROGRAM_START:
		AddEvent2List("开始下载固件！");
		break;
	case REPORT_PROGRAM_PERCENT:
		break;
	case REPORT_PROGRAM_END:
		AddEvent2List("固件下载完成！");
		break;
	case REPORT_OPEN_ENGINE:
		break;
	//case REPORT_RDBD_ADDR:
	//	break;
	//case REPORT_WRBD_ADDR:
	//	break;
	//case REPORT_RDBD_NAME:
	//	break;
	//case REPORT_WRBD_NAME:
	//	break;
	case REPORT_WRITE_FIXPARAM:
		break;
	case REPORT_CLOSE_ENGINE:
		break;
	case REPORT_WAIT_RESTART:
		break;
	case REPORT_COMMU_START:
		break;
	//case REPORT_COMMU_SUCC:
	//	break;
	//case REPORT_COMMU_READ:
	//	break;
	case REPORT_COMMU_OPEN:
		break;
	case RERROT_COMMU_TIMEOUT:
		break;
	//case REPORT_APOLLO:
	//	break;
	//case REPORT_READ_RECORD:
	//	break;
	case REPORT_USER_EXIT:
		break;
	case REPORT_END_ALL:
		if(err_flag != 1){
			if( (int)lParam==0 ){
				if( (sText = mesInfo.sz_post_passfail(sn_no, 
							general_option_instance.g_station_name,
							btaddr_sz_option_instance.rl_flag, 1) ) == "OK"){
					AddEvent2List("mes 通信成功！");
					pass_green_color = 1;
					m_Epass.SetWindowTextA("PASS");
					Invalidate();
				}else{
					AddEvent2List("mes 通信失败！错误消息：");
					AddEvent2List(sText);
					pass_green_color = 0;
					m_Epass.SetWindowTextA("FAIL");
					Invalidate();
				}
			}else{
				pass_green_color = 0;
				m_Epass.SetWindowTextA("FAIL");
				Invalidate();
			}
		}
		((CButton*)GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
		break;
	case REPORT_LAST:
		break;

	case REPORT_APOLLO:
		sText.Format("%s", (char*)lParam);
		AddEvent2List(sText);
		break;
	case REPORT_RDBD_NAME:
	case REPORT_WRBD_NAME:
	case REPORT_RDBD_ADDR:
	case REPORT_WRBD_ADDR:
	case REPORT_COMMU_SUCC:
		sText.Format("%s", (char*)lParam);
		AddEvent2List(sText);
		break;
	case REPORT_COMMU_READ:
		sText.Format("%s", (char*)lParam);
		AddEvent2List(sText);
		break;
	case REPORT_READ_RECORD:
		sText.Format("Recv=%d", (int)lParam);
		AddEvent2List(sText);
		break;
	}

	//CString sText;
	//sText.Format("report msg %d",wParam);
	//AddEvent2List(sText);
	return 0;
}

struct bt_lic_ins_str{
	UINT uap;
	UINT nap;
	UINT lap_min;
	UINT lap_max;
	CString license;
};
int software_sz_btaddr::WorkerThreadProc(void *param)
{
	CIni ini;
	CString sFName, license = "";
	int i, lic_num;
	UINT uap, nap, lap;
	CString sText,stmp;UINT type;
	CString mac_addr,response;
	software_sz_btaddr *p = (software_sz_btaddr*)param;
	CCurlCtrl *mesptr = &p->mesInfo;
	p->err_flag = 0;
	if(!p->general_option_instance.g_check_mes){
		p->AddEvent2List("未配置mes，请在通用配置中设置mes后使用");
		return -1;
	}
	if(p->general_option_instance.g_check_mes_type != 3){
		p->AddEvent2List("mes类型不正确，仅支持类型3");
		return -1;
	}
	((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	mesptr->clear_flag();
	mac_addr = mesptr->sz_get_btaddr(p->sn_no, 
		p->general_option_instance.g_station_name, p->btaddr_sz_option_instance.rl_flag, response);

	//if( (sText = mesptr->sz_post_passfail(p->sn_no,
	//						p->general_option_instance.g_station_name,
	//						p->btaddr_sz_option_instance.rl_flag, 1) ) == "OK"){
	//							;
	//}
	//if(mac_addr == "00:00:00:00:00:00"){
	if(mac_addr == "000000000000"){
		p->AddEvent2List("mac地址获取失败,错误消息：:");
		p->AddEvent2List(response);
		goto err_out;
	}else{
		//sText.Format("{0x");
		//stmp = mac_addr.Mid(9,2);
		//sText += stmp;
		//stmp = mac_addr.Mid(12,2);
		//sText += stmp;
		//stmp = mac_addr.Mid(15,2);
		//sText += stmp;
		//sText += ", 0x";
		//stmp = mac_addr.Mid(0,2);
		//sText += stmp;
		//sText += ", 0x";
		//stmp = mac_addr.Mid(3,2);
		//sText += stmp;
		//stmp = mac_addr.Mid(6,2);
		//sText += stmp;
		//sText += "}";

		sText.Format("{0x");
		stmp = mac_addr.Mid(6,6);
		lap = strtol(stmp,NULL,16);
		sText += stmp;
		sText += ", 0x";
		stmp = mac_addr.Mid(4,2);
		uap = strtol(stmp,NULL,16);
		sText += stmp;
		sText += ", 0x";
		stmp = mac_addr.Mid(0,4);
		nap = strtol(stmp,NULL,16);
		sText += stmp;
		sText += "}";

		p->AddEvent2List("mac地址获取成功");
		p->m_btaddr.SetWindowTextA(sText);
		p->m_devCtrl.SetBtAddr(sText);
	}
	if( p->btaddr_sz_option_instance.g_if_locallic ){
		sFName = "bluesite_lib\\bt_license.ini";
		ini.SetPathName(sFName);
		lic_num = ini.GetUInt("BT_LIC", "bt_lic_num", 0x11, 10);
		for(i=0; i<lic_num; i++){
			struct bt_lic_ins_str *lic_p = new struct bt_lic_ins_str;
			sText.Format("BT_LIC_INS%d", i+1);
			lic_p->uap = ini.GetUInt(sText, "uap", 0x11, 16);
			lic_p->nap = ini.GetUInt(sText, "nap", 0x11, 16);
			lic_p->lap_min = ini.GetUInt(sText, "lap_min", 0x11, 16);
			lic_p->lap_max = ini.GetUInt(sText, "lap_max", 0x11, 16);
			lic_p->license = ini.GetString(sText, "license");
			if( (lic_p->uap == uap) && (lic_p->nap == nap) &&
				(lic_p->lap_min <= lap) && (lic_p->lap_max >= lap) ){
				license = lic_p->license;
				delete lic_p;
				break;
			}
			delete lic_p;
		}
		if(license != ""){
			p->m_devCtrl.SetLicenseText(license);
			p->AddEvent2List("本地 license 获取成功！");
		}else{
			license.Empty();
			p->m_devCtrl.SetLicenseText(license);
			p->AddEvent2List("本地 license 获取失败！");
			goto err_out;
		}
	}else{
		p->AddEvent2List("警告：暂时不支持从mes获取license！");
		goto err_out;
	}

	p->m_devCtrl.SetBtWrite(TRUE);
	p->m_devCtrl.SetBtName(p->btaddr_sz_option_instance.g_bt_name);
	p->m_devCtrl.SetHwVersion(p->btaddr_sz_option_instance.g_hw_version);

	type = THREAD_BT_ADDR;
	p->m_devCtrl.SetThreadFunc(type);
	p->m_devCtrl.Start(p->m_hWnd);

	return 0;

err_out:
	p->pass_green_color = 0;
	p->m_Epass.SetWindowTextA("FAIL");
	p->Invalidate();
	((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
	return -1;
}

void software_sz_btaddr::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_EditSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	pass_green_color = 0;
	m_btaddr.SetWindowTextA("");


	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	AddEvent2List("#####等待用户输入sn号#####");
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_sz_btaddr::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 14){//v3
		sn_no = sText;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_btaddr::WorkerThreadProc, this, 0, NULL);
		AddEvent2List("sn号输入成功，开始测试");
		AddEvent2List(sn_no);
	}
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
			}else{
				//StartDevContrl();
				sn_no = sText;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_btaddr::WorkerThreadProc, this, 0, NULL);
				AddEvent2List("sn号输入成功，开始测试");
				AddEvent2List(sn_no);
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
		}else{
			//StartDevContrl();
			sn_no = sText;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_btaddr::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn号输入成功,开始测试");
			AddEvent2List(sn_no);
		}
	}
	// TODO:  在此添加控件通知处理程序代码
}


BOOL software_sz_btaddr::OnInitDialog()
{
	CString sText;
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	CString dir_path;
	dir_path = LOG_PATH_BTADDR_sz;

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

	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;

	m_editFont.CreatePointFont(700, "宋体");
	m_Epass.SetFont(&m_editFont);

	m_hw_version.SetWindowTextA(btaddr_sz_option_instance.g_hw_version);
	m_btname.SetWindowTextA(btaddr_sz_option_instance.g_bt_name);
	sText.Format("%d", btaddr_sz_option_instance.rl_flag);
	m_lr_flag.SetWindowTextA(sText);
	m_btaddr.EnableWindow(FALSE);
	m_btname.EnableWindow(FALSE);
	m_hw_version.EnableWindow(FALSE);
	m_lr_flag.EnableWindow(FALSE);

	if(general_option_instance.g_check_mes){
		AddEvent2List("#####等待用户输入sn号#####");
	}else{
		AfxMessageBox("错误：没有配置mes");
		AddEvent2List("错误：没有配置mes");
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_sz_btaddr::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH software_sz_btaddr::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
