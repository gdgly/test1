// software_burn_sz.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_burn_sz.h"
#include "afxdialogex.h"


// software_burn_sz 对话框

IMPLEMENT_DYNAMIC(software_burn_sz, CDialogEx)

software_burn_sz::software_burn_sz(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_burn_sz::IDD, pParent)
{

}

software_burn_sz::~software_burn_sz()
{
}

void software_burn_sz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC9, m_Epass);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_firm_version);
	DDX_Control(pDX, IDC_EDIT15, m_btaddr);
	DDX_Control(pDX, IDC_EDIT45, m_btname);
	DDX_Control(pDX, IDC_PROGRESS9, m_burn_progress);
	DDX_Control(pDX, IDC_PROGRESS10, m_apollo_progress);
	DDX_Control(pDX, IDC_PROGRESS11, m_box_progress);
}


BEGIN_MESSAGE_MAP(software_burn_sz, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &software_burn_sz::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT2, &software_burn_sz::OnEnChangeEdit2)
	ON_MESSAGE(WM_DEV_ERROR, &software_burn_sz::OnDevCtrlError)
	ON_MESSAGE(WM_DEV_REPORT, &software_burn_sz::OnDevCtrlReport)
	ON_BN_CLICKED(IDC_BUTTON3, &software_burn_sz::OnBnClickedButton3)
END_MESSAGE_MAP()

void software_burn_sz::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	burn_sz_option_instance = g_p->burn_sz_option_instance;
}

// software_burn_sz 消息处理程序
void software_burn_sz::AddEvent2List(CString sText)
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

LRESULT software_burn_sz::OnDevCtrlError(WPARAM wParam, LPARAM lParam)
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
			stmp.Format("%s", (char*)lParam);
			if(stmp.Find("No Found burnApollo.bat") >= 0){
				if(burn_sz_option_instance.g_if_burn_box)
					AddEvent2List("未找到盒子烧写脚本！");
				else
					AddEvent2List("未找到阿波罗烧写脚本！");
			}else if(stmp.Find("unknown burn file") >= 0){
				if(burn_sz_option_instance.g_if_burn_box)
					AddEvent2List("未找到盒子烧写固件！");
				else
					AddEvent2List("未找到阿波罗烧写固件！");
			}else if(stmp.Find("not poweron") >= 0){
				if(burn_sz_option_instance.g_if_burn_box)
					AddEvent2List("未找到盒子设备！");
				else
					AddEvent2List("未找到阿波罗设备！");
			}else if(stmp.Find("Cannot connect to target") >= 0){
				if(burn_sz_option_instance.g_if_burn_box)
					AddEvent2List("未能连接盒子设备！");
				else
					AddEvent2List("未能连接阿波罗设备！");
			}
			break;
		case ERROR_READ_RECORD:
			sText = "ERROR_READ_RECORD";
			break;
		case ERROR_COMMU_FAIL:
			sText = "ERROR_COMMU_FAIL";
			break;
        case ERROR_READ_XTALTRIM:
			sText = "ERROR_READ_XTALTRIM";
			break;
        case ERROR_READ_XTALCAP:
			sText = "ERROR_READ_XTALCAP";
			break;
        case ERROR_WRITE_XTALTRIM:
			sText = "ERROR_WRITE_XTALTRIM";
			break;
        case ERROR_WRITE_XTALCAP:
			sText = "ERROR_WRITE_XTALCAP";
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

int software_burn_sz::mes_sannuo_write_logfile(CString SN, CString MAC)
{
	CString sFname;
	CreateDirectory(SANNUO_MES_LOG_PATH, 0);
	sFname = SN + "_PASS.txt";
	sFname = SANNUO_MES_LOG_PATH + sFname;
	if(m_mes_file.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Function   :  烧写固件\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Device SN:     ");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname = SN + "\r\n";
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Device MAC:     ");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname = MAC + "\r\n";
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Test Result:     PASS\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Cycle Time:      0 s\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("ErrorCode:\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		CTime tNow = CTime::GetCurrentTime();
		sFname.Format("%04d%02d%02d_%02d%02d%02d\r\n", 
			tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), 
			tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
		sFname = "Start Time :     " + sFname;
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		m_mes_file.Flush();
		m_mes_file.Close();
	}
	return 0;
}

LRESULT software_burn_sz::OnDevCtrlReport(WPARAM wParam, LPARAM lParam)
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
		m_burn_progress.SetPos((int)lParam);
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
				if( general_option_instance.g_check_mes ){ 
					if( (general_option_instance.g_check_mes_type == 3) ){
						if( (sText = mesInfo.sz_post_passfail(sn_no, 
									general_option_instance.g_station_name,
									1, 1) ) == "OK"){
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
					}else if( (general_option_instance.g_check_mes_type == 4) ){
						m_btaddr.GetWindowText(sText);
						mes_sannuo_write_logfile(sn_no,sText);
						AddEvent2List("mes log文件生成成功！");
						pass_green_color = 1;
						m_Epass.SetWindowTextA("PASS");
						Invalidate();
					}else{
						AddEvent2List("不支持的mes类型，请重新配置！");
						pass_green_color = 0;
						m_Epass.SetWindowTextA("FAIL");
						Invalidate();					
					}
				}else{
					pass_green_color = 1;
					m_Epass.SetWindowTextA("PASS");
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
		if(sText.Find("Start J-Link Success") >= 0){
			if(burn_sz_option_instance.g_if_burn_box){
				m_box_progress.SetPos(20);
				AddEvent2List("开始擦除ht32 flash！");			
			}else{
				m_apollo_progress.SetPos(20);
				AddEvent2List("开始擦除阿波罗flash！");
			}
		}else if(sText.Find("Script completed") >= 0){
			if(burn_sz_option_instance.g_if_burn_box){
				m_box_progress.SetPos(100);
				AddEvent2List("ht32烧写完成！");			
			}else{
				m_apollo_progress.SetPos(100);
				AddEvent2List("阿波罗烧写完成！");
			}

		}
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
    case REPORT_READ_XTALTRIM:
		sText.Format("读取频偏trim值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
    case REPORT_READ_XTALCAP:
		sText.Format("读取频偏cap值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
    case REPORT_WRITE_XTALTRIM:
		sText.Format("写入频偏trim值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
    case REPORT_WRITE_XTALCAP:
		sText.Format("写入频偏cap值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
	}

	//CString sText;
	//sText.Format("report msg %d",wParam);
	//AddEvent2List(sText);
	return 0;
}

int software_burn_sz::qcc5126_erase_flash(void)
{
	int ret = 0, rdlen;
	char sText[256];
	CString sCommand, sTmp;
	CExecProcess  *execProg = new CExecProcess();

	Sleep(2000);

	if(PathFileExists("bluesite_lib\\nvscmd.exe") != TRUE){
		AfxMessageBox("错误：程序路径下未找到擦除程序");
		return -1;
	}
	//sCommand.Format("%s -CommanderScript burnApollo.bat", m_iniParam.sJlinkPath);
	//bluesite_lib\\nvscmd.exe erase -deviceid 4 0 -usbdbg 416 -nvstype sqif -norun
	sCommand.Format("bluesite_lib\\nvscmd.exe erase -deviceid 4 0 -usbdbg ");
	sCommand += burn_sz_option_instance.g_usbdbg_num;
	sCommand += " -nvstype sqif -norun";
	//sCommand = sCommand;

	execProg->Create(CExecProcess::ASYNC);
	execProg->Execute(sCommand);

	int erase_ok, download_ok, scrip_ok;
	erase_ok = 0; download_ok = 0; scrip_ok = 0;
	while (true) {
		memset(sText, 0, sizeof(sText));
		if ((rdlen = execProg->ReadLine(sText, 256)) > 0) {
			TRACE("Get%d:%s", rdlen, (char*)sText);
			sTmp.Format("%s", sText);
			if (sTmp.Find("Erase the SQIF...") >= 0) { 
				AddEvent2List("开始擦除flash");
			}else if (sTmp.Find("Success") >= 0) { //  Could not open J-Link Command File 'burnApollo.bat'
				AddEvent2List("主芯片flash擦除成功");
				break;
			}else if (sTmp.Find("Failed") >= 0) { //  Could not open J-Link Command File 'burnApollo.bat'
				AddEvent2List("主芯片flash擦除失败");
				ret = -1;
				break;
			}else if (sTmp.Find("found in ISP device cache") >= 0) { //  Could not open J-Link Command File 'burnApollo.bat'
				AddEvent2List("未找到设备");
				ret = -1;
				break;
			}
		}
	}
	delete execProg;

	return ret;
}

struct bt_lic_ins_str_burn{
	UINT uap;
	UINT nap;
	UINT lap_min;
	UINT lap_max;
	CString license;
};
CString software_burn_sz::con_btaddr_format(CString btaddr_12B)
{
	CString sText;
	sText = "{0x";
	sText += btaddr_12B.Mid(6,6);
	sText += ", 0x";
	sText += btaddr_12B.Mid(4,2);
	sText += ", 0x";
	sText += btaddr_12B.Mid(0,4);
	sText += "}";
	return sText;
}
int software_burn_sz::WorkerThreadProc(void *param)
{
	CIni ini;
	CString sFName, license = "",mac_addr;
	int i, lic_num;
	UINT uap, nap, lap;
	CString sText;UINT type;
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	software_burn_sz *p = (software_burn_sz*)param;
	int cap = 0, trim = 0;
	((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
	p->m_devCtrl.SethWnd(p->m_hWnd);
	if(p->burn_sz_option_instance.g_if_read_freq &&
		p->burn_sz_option_instance.g_if_burn_qcc){
		if (p->m_devCtrl.CrystalTrimmingRead(trim, cap, 1) == 0) {
			sText.Format("cap=%d, trim=%d", cap, trim);
			sText = "频偏值读取成功！" + sText;
			p->AddEvent2List(sText);
		}else{
			p->pass_green_color = 0;
			p->m_Epass.SetWindowTextA("FAIL");
			p->Invalidate();
			((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			return -1;
		}
		Sleep(p->burn_sz_option_instance.g_readfreq_waittime*1000);
	}
	
	if(p->burn_sz_option_instance.g_if_erase_flash &&
		p->burn_sz_option_instance.g_if_burn_qcc){
		if( p->qcc5126_erase_flash() != 0 ){
			p->pass_green_color = 0;
			p->m_Epass.SetWindowTextA("FAIL");
			p->Invalidate();	
			((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			return -1;
		}
	}

	sText = p->burn_sz_option_instance.g_wrFirm_folder+"\\flash_image.xuv";
	p->m_devCtrl.SetFlashImage(sText);
	p->m_btname.GetWindowText(sText); p->m_devCtrl.SetBtName(sText);
	p->m_btaddr.GetWindowText(sText);
	CString con_sText;con_sText = p->con_btaddr_format(sText);
	p->m_devCtrl.SetBtAddr(con_sText); mac_addr = con_sText;
	p->m_devCtrl.SetHwVersion(p->burn_sz_option_instance.g_hw_version);
	//m_devCtrl.SetEraseAll(burn_sz_option_instance.g_if_erase_flash);
	p->m_devCtrl.SetJlinkPath(p->burn_sz_option_instance.g_sJLinkPath);

	sText.Format("%s",path); sText += "\\burnApollo.bat";
	p->m_devCtrl.SetApolloBurnfile(sText);

	if(p->burn_sz_option_instance.g_if_burn_param){
		sText = mac_addr.Mid(3,6);
		lap = strtol(sText,NULL,16);
		sText = mac_addr.Mid(13,2);
		uap = strtol(sText,NULL,16);
		sText = mac_addr.Mid(19,4);
		nap = strtol(sText,NULL,16);
		sFName = "bluesite_lib\\bt_license.ini";
		ini.SetPathName(sFName);
		lic_num = ini.GetUInt("BT_LIC", "bt_lic_num", 0x11, 10);
		for(i=0; i<lic_num; i++){
			struct bt_lic_ins_str_burn *lic_p = new struct bt_lic_ins_str_burn;
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
			p->pass_green_color = 0;
			p->m_Epass.SetWindowTextA("FAIL");
			p->Invalidate();
			((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
			return -1;
		}
	}

	type = 0;
	if(p->burn_sz_option_instance.g_if_burn_qcc){type |= THREAD_BURN;}
	if(p->burn_sz_option_instance.g_if_burn_apollo){type |= THREAD_BURN_APO;}
	if(p->burn_sz_option_instance.g_if_burn_param){p->m_devCtrl.SetBtWrite(TRUE);type |= THREAD_BT_ADDR;}
	if(p->burn_sz_option_instance.g_if_test_after_burn){type |= THREAD_CHECK;}
	if(p->burn_sz_option_instance.g_if_burn_qcc &&
		p->burn_sz_option_instance.g_if_write_freq){
		p->m_devCtrl.SetXtalTrim(cap, trim);
		type |= THREAD_CRYSTGALTRIM_WRITE;
	}
	p->m_devCtrl.SetThreadFunc(type);
	p->m_devCtrl.Start(p->m_hWnd);
	return 0;
}

void software_burn_sz::StartDevContrl()
{
	CString sText;UINT type;
	char path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	UpdateData(TRUE);
	err_flag = 0;
	if(burn_sz_option_instance.g_if_burn_box){
		m_devCtrl.SetJlinkPath(burn_sz_option_instance.g_sJLinkPath);
		sText.Format("%s",path); sText += "\\burnbox.bat";
		m_devCtrl.SetApolloBurnfile(sText);
		type = THREAD_BURN_APO;//盒子可用与阿波罗相同的方法烧写
		m_devCtrl.SetThreadFunc(type);
		m_devCtrl.Start(this->m_hWnd);
	}else{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_burn_sz::WorkerThreadProc, this, 0, NULL);
	}
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_burn_sz::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	CString stmp;
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 14){//v3
		sn_no = sText;
		StartDevContrl();
		stmp = "sn号输入成功，等待上电" + sText;
		AddEvent2List(stmp);
	}
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
			}else{
				sn_no = sText;
				StartDevContrl();
				stmp = "sn号输入成功，等待上电" + sText;
				AddEvent2List(stmp);
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
		}else{
			sn_no = sText;
			StartDevContrl();
			stmp = "sn号输入成功，等待上电" + sText;
			AddEvent2List(stmp);
		}
	}

	// TODO:  在此添加控件通知处理程序代码
}

void software_burn_sz::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(burn_sz_option_instance.g_if_burn_param){
		m_btaddr.SetWindowTextA("");
	}
	all_init();
}

void software_burn_sz::all_init()
{
	m_EditSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	pass_green_color = 0;

	m_burn_progress.SetPos(0);
	m_apollo_progress.SetPos(0);
	m_box_progress.SetPos(0);

	if(burn_sz_option_instance.g_if_burn_param){
		m_btaddr.SetFocus();
	}else{
		m_EditSn.SetFocus();
	}
	m_listEvt.ResetContent();
	if(burn_sz_option_instance.g_if_burn_box){
		AddEvent2List("盒子烧写需先连接好jlink，否则烧写会导致程序卡住！");	
	}else if(burn_sz_option_instance.g_if_burn_apollo){
		AddEvent2List("apollo烧写需先连接好jlink，否则烧写会导致程序卡住！");
	}
	AddEvent2List("#####等待用户输入sn号#####");
}

void software_burn_sz::set_apollo_burn_file()
{
	CString sText;

	if(m_apollo_burn_file.Open("burnApollo.bat", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) != TRUE) {
		AddEvent2List("burnApollo.bat 文件创建失败！");
		return;
	}
	if(m_apollo_burn_file.m_hFile != INVALID_HANDLE_VALUE) {
		sText.Format("device AMAPH1KK-KCR");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("si SWD");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("speed 1000");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("connect");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("h");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("erase");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("loadbin   ");
		sText += burn_sz_option_instance.g_wrFirm_folder;
		sText += "\\apollo2_blue.bin";
		sText += "  0x00000000";
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("r");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 
		sText.Format("qc");
		m_apollo_burn_file.Write(sText, sText.GetLength());
		m_apollo_burn_file.Write("\r\n", 2); 

		m_apollo_burn_file.Flush();
	}
	m_apollo_burn_file.Close();
}

void software_burn_sz::set_box_burn_file()
{
	CString sText;

	if(m_box_burn_file.Open("burnbox.bat", CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) != TRUE) {
		AddEvent2List("burnbox.bat 文件创建失败！");
		return;
	}
	if(m_box_burn_file.m_hFile != INVALID_HANDLE_VALUE) {
		sText.Format("device HT32F52253");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("si SWD");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("speed 1000");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("connect");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("h");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("erase");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("loadbin   ");
		sText += burn_sz_option_instance.g_wrFirm_folder;
		sText += "\\ALL.bin";
		sText += "  0x00000000";
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("r");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 
		sText.Format("qc");
		m_box_burn_file.Write(sText, sText.GetLength());
		m_box_burn_file.Write("\r\n", 2); 

		m_box_burn_file.Flush();
	}
	m_box_burn_file.Close();
}

int software_burn_sz::need_file_ifexit()
{
	CString sText;
	sText = burn_sz_option_instance.g_wrFirm_folder;
	if(PathFileExists(sText+"\\buildversion") != TRUE){
		AfxMessageBox("警告：固件路径下未找到buildversion");
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
			if( burn_sz_option_instance.g_if_burn_box ){
				sText = "V0.00.00";
				if( (length = version_str.Find("box=")) >= 0){
					sText = " box ht32: " + version_str.Mid(length+sizeof("box=")-1, 8);
				}
			}else{
				sText = "V0.00.00";
				if( (length = version_str.Find("qcc512x=")) >= 0){
					sText = " qcc512x: " + version_str.Mid(length+sizeof("qcc512x=")-1, 8);
					//m_fs_version.SetWindowTextA( version_str.Mid(length+sizeof("qcc512x=")-1, 8) );
				}
				if( (length = version_str.Find("apollo=")) >= 0){
					sText = sText + "    apollo: " + version_str.Mid(length+sizeof("apollo=")-1, 8);
					//m_fs_version.SetWindowTextA( version_str.Mid(length+sizeof("apollo=")-1, 8) );
				}			
			}
			m_firm_version.SetWindowTextA(sText);
			free(g_buf);
		}else{
			AfxMessageBox("警告：buildversion文件打开失败");
			m_firm_version.SetWindowTextA("V0.00.00");
			return 1;
		}	
	}
	sText = burn_sz_option_instance.g_wrFirm_folder;
	if(burn_sz_option_instance.g_if_burn_qcc){
		if(PathFileExists(sText+"\\flash_image.xuv") != TRUE){
			AfxMessageBox("错误：固件路径下未找到flash_image.xuv");
			return 1;
		}
	}
	if(burn_sz_option_instance.g_if_burn_apollo){
		if(PathFileExists(sText+"\\apollo2_blue.bin") != TRUE){
			AfxMessageBox("错误：固件路径下未找到apollo2_blue.bin");
			return 1;
		}
	}
	if(burn_sz_option_instance.g_if_burn_box){
		if(PathFileExists(sText+"\\ALL.bin") != TRUE){
			AfxMessageBox("错误：固件路径下未找到ALL.bin");
			return 1;
		}
	}
	return 0;
}

BOOL software_burn_sz::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);//任务栏图标
	// TODO:  在此添加额外的初始化
	CString dir_path;
	dir_path = LOG_PATH_BURN_sz;

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

	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;

	if(burn_sz_option_instance.g_if_burn_box){
		//set_apollo_burn_file();
		set_box_burn_file();
		m_box_progress.SetRange(0, 100);
		m_box_progress.SetStep(1);
		((CButton*)GetDlgItem(IDC_STATIC_apollo))->EnableWindow(FALSE);
		m_apollo_progress.EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STATIC_main_chip))->EnableWindow(FALSE);
		m_burn_progress.EnableWindow(FALSE);
		m_btaddr.EnableWindow(FALSE);
		m_btname.EnableWindow(FALSE);
		AddEvent2List("盒子烧写需先连接好jlink，否则烧写会导致程序卡住！");
		AddEvent2List("烧写配置包含了盒子烧写！");
	}else{
		((CButton*)GetDlgItem(IDC_STATIC_box))->EnableWindow(FALSE);
		m_box_progress.EnableWindow(FALSE);
		AddEvent2List("烧写配置未包含盒子烧写！");
		if(burn_sz_option_instance.g_if_burn_qcc){
			m_burn_progress.SetRange(0, 100);
			m_burn_progress.SetStep(1);
			AddEvent2List("配置了烧写主芯片！");
		}else{
			((CButton*)GetDlgItem(IDC_STATIC_main_chip))->EnableWindow(FALSE);
			m_burn_progress.EnableWindow(FALSE);
			AddEvent2List("没有配置烧写主芯片！");
		}
		if(burn_sz_option_instance.g_if_erase_flash){
			AddEvent2List("配置了烧写前擦除flash！");
		}else{
			AddEvent2List("没有配置烧写前擦除flash！");
		}
		if(burn_sz_option_instance.g_if_burn_apollo){
			set_apollo_burn_file();
			m_apollo_progress.SetRange(0, 100);
			m_apollo_progress.SetStep(1);
			AddEvent2List("apollo烧写需先连接好jlink，否则烧写会导致程序卡住！");
			AddEvent2List("烧写配置包含了apollo烧写！");
		}else{
			((CButton*)GetDlgItem(IDC_STATIC_apollo))->EnableWindow(FALSE);
			m_apollo_progress.EnableWindow(FALSE);
			AddEvent2List("烧写配置未包含apollo烧写！");
		}

		m_btaddr.SetWindowTextA(SZ_DEFAULT_BTADDR);
		m_btname.SetWindowTextA(SZ_DEFAULT_BTNAME);
		if(burn_sz_option_instance.g_if_burn_param){
			AddEvent2List("烧写配置包含了蓝牙等参数烧写！");
		}else{
			m_btaddr.EnableWindow(FALSE);
			m_btname.EnableWindow(FALSE);
			AddEvent2List("烧写配置未包含蓝牙等参数烧写！");
		}
		if(burn_sz_option_instance.g_if_test_after_burn){
			AddEvent2List("配置了烧写完成测试！");
		}else{
			AddEvent2List("没有配置烧写完成测试！");
		}
	}

	if(burn_sz_option_instance.g_if_burn_apollo || 
		burn_sz_option_instance.g_if_burn_box	||
		burn_sz_option_instance.g_if_burn_qcc){
		need_file_ifexit();
	}

	AddEvent2List("#####等待用户输入sn号#####");

	return TRUE;  // return TRUE unless you set the focus to a control
}

HBRUSH software_burn_sz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


BOOL software_burn_sz::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}




void software_burn_sz::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
}
