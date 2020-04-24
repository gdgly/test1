// software_sz_checkbox.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_sz_checkbox.h"
#include "afxdialogex.h"


// software_sz_checkbox 对话框

IMPLEMENT_DYNAMIC(software_sz_checkbox, CDialogEx)

software_sz_checkbox::software_sz_checkbox(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_sz_checkbox::IDD, pParent)
{

}

software_sz_checkbox::~software_sz_checkbox()
{
}

void software_sz_checkbox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC11, m_Epass);
	DDX_Control(pDX, IDC_STC_BOOT14, m_reboot);
	DDX_Control(pDX, IDC_STC_BOOT13, m_watchdog);
	DDX_Control(pDX, IDC_STC_BOOT15, m_max20340);
	DDX_Control(pDX, IDC_STC_BOOT16, m_max77813);
	DDX_Control(pDX, IDC_STC_BOOT17, m_plc_left);
	DDX_Control(pDX, IDC_STC_BOOT18, m_plc_right);
	DDX_Control(pDX, IDC_STC_BOOT19, m_open_case);
	DDX_Control(pDX, IDC_STC_BOOT20, m_close_case);
	DDX_Control(pDX, IDC_STC_BOOT21, m_key_down);
	DDX_Control(pDX, IDC_STC_BOOT22, m_key_up);
	DDX_Control(pDX, IDC_STC_BOOT23, m_usb_in);
	DDX_Control(pDX, IDC_STC_BOOT24, m_usb_out);
	DDX_Control(pDX, IDC_STC_BOOT25, m_cal_value);
	DDX_Control(pDX, IDC_LIST4, m_listEvt);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_EDIT1, edit_out);
}


BEGIN_MESSAGE_MAP(software_sz_checkbox, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &software_sz_checkbox::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_EDIT2, &software_sz_checkbox::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_RADIO_MUSIC1, &software_sz_checkbox::OnBnClickedRadioMusic1)
	ON_BN_CLICKED(IDC_RADIO_MUSIC2, &software_sz_checkbox::OnBnClickedRadioMusic2)
	ON_BN_CLICKED(IDC_RADIO_MUSIC3, &software_sz_checkbox::OnBnClickedRadioMusic3)
	ON_BN_CLICKED(IDC_RADIO_MUSIC4, &software_sz_checkbox::OnBnClickedRadioMusic4)
	ON_BN_CLICKED(IDC_BUTTON2, &software_sz_checkbox::OnBnClickedButton2)
END_MESSAGE_MAP()

void software_sz_checkbox::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	checkbox_sz_option_instance = g_p->checkbox_sz_option_instance;
}
// software_sz_checkbox 消息处理程序

void software_sz_checkbox::AddEvent2List(CString sText)
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
enum{
	BIT_POWERON = 0,
	BIT_WATCHDOG,
	BIT_MAX20340,
	BIT_MAX77813,
	BIT_CAL_VOL,
	BIT_PLCLEFT,
	BIT_PLCRIGHT,
	BIT_HALCLOSE,
	BIT_HALOPEN,
	BIT_KEYDOWN,
	BIT_KEYUP,
	BIT_USBOUT,
	BIT_USBIN,
	BIT_MAX20340_ERROR,
	BIT_MAX77813_ERROR,
};

int software_sz_checkbox::ParseLine(void *param, BYTE *buf, int len)
{
	CString sText(buf);
	software_sz_checkbox *p = (software_sz_checkbox*)param;
	if(sText.Find("Power On") >= 0){
		p->right_flag |= 1<<BIT_POWERON;
		p->m_reboot.SetColor(STATIC_WND_SUCCESS);
		p->AddEvent2List("检测到盒子重启成功！");
	}
	if(sText.Find("WDOG inited") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_wdt){
			p->right_flag |= 1<<BIT_WATCHDOG;
			p->m_watchdog.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到看门狗初始化完成！");
		}
	}
	if(sText.Find("max77813 init ok") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_max77813){
			p->right_flag |= 1<<BIT_MAX77813;
			p->m_max77813.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到max77813初始化完成！");
		}
	}
	if(sText.Find("max20340 init ok") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_max20340){
			p->right_flag |= 1<<BIT_MAX20340;
			p->m_max20340.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到max20340初始化完成！");
		}
	}
	if(sText.Find("Left Ear 0 to 1") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_plc_commu){
			p->right_flag |= 1<<BIT_PLCLEFT;
			p->m_plc_left.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到左耳通信！");
		}
	}
	if(sText.Find("Right Ear 0 to 1") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_plc_commu_right){
			p->right_flag |= 1<<BIT_PLCRIGHT;
			p->m_plc_right.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到右耳通信！");
		}
	}
	if(sText.Find("hal close") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_hal){
			if( p->checkbox_sz_option_instance.g_if_check_hal_after_plc && 
				p->checkbox_sz_option_instance.g_if_check_plc_commu){
				if(	(p->right_flag&(1<<BIT_PLCRIGHT)) && (p->right_flag&(1<<BIT_PLCLEFT)) ){
					p->right_flag |= 1<<BIT_HALCLOSE;
					p->m_close_case.SetColor(STATIC_WND_SUCCESS);
					p->AddEvent2List("检测到盒盖！");
				}
			}else{
				p->right_flag |= 1<<BIT_HALCLOSE;
				p->m_close_case.SetColor(STATIC_WND_SUCCESS);
				p->AddEvent2List("检测到盒盖！");			
			}
		}
	}
	if(sText.Find("hal open") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_hal){
			if( p->checkbox_sz_option_instance.g_if_check_hal_after_plc && 
				p->checkbox_sz_option_instance.g_if_check_plc_commu){
				if(	(p->right_flag&(1<<BIT_PLCRIGHT)) && (p->right_flag&(1<<BIT_PLCLEFT)) ){
					p->right_flag |= 1<<BIT_HALOPEN;
					p->m_open_case.SetColor(STATIC_WND_SUCCESS);
					p->AddEvent2List("检测到开盖！");
				}
			}else{
				p->right_flag |= 1<<BIT_HALOPEN;
				p->m_open_case.SetColor(STATIC_WND_SUCCESS);
				p->AddEvent2List("检测到开盖！");		
			}
		}
	}
	if(sText.Find("key down") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_key){
			p->right_flag |= 1<<BIT_KEYDOWN;
			p->m_key_down.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到按键按下！");
		}
	}
	if(sText.Find("key up") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_key){
			p->right_flag |= 1<<BIT_KEYUP;
			p->m_key_up.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到按键抬起！");
		}
	}
	if(sText.Find("usbdet out") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_usb){
			p->right_flag |= 1<<BIT_USBOUT;
			p->m_usb_out.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到usb拔出！");
		}
	}
	if(sText.Find("usbdet in") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_usb){
			p->right_flag |= 1<<BIT_USBIN;
			p->m_usb_in.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("检测到usb插入！");
		}
	}
	if(sText.Find("max77813 i2c read id error") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_max77813){
			p->error_flag |= 1<<BIT_MAX77813_ERROR;
			p->m_max77813.SetColor(STATIC_WND_WARNING);
			p->AddEvent2List("检测到max77813初始化失败！");
		}
	}
	if(sText.Find("max20340 read id error") >= 0){
		if(p->checkbox_sz_option_instance.g_if_check_max20340){
			p->error_flag |= 1<<BIT_MAX20340_ERROR;
			p->m_max20340.SetColor(STATIC_WND_WARNING);
			p->AddEvent2List("检测到max20340初始化失败！");
		}
	}
	BYTE vbuf[10];memset(vbuf, 0, 10);
	if(sText.Find("check VolRef ") >= 0){
		memcpy(vbuf,&buf[sText.Find("check VolRef ") + 13],4);
		CString vol_value(vbuf);
		p->m_cal_value.SetWindowTextA(vol_value);
		p->vol_value_sav = vol_value;

		if(sText.Find("succ") >= 0){
			p->m_cal_value.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("校准成功！");
			p->right_flag |= 1<<BIT_CAL_VOL;
		}else{
			p->m_cal_value.SetColor(STATIC_WND_WARNING);
			p->AddEvent2List("校准失败！");		
			p->error_flag |= 1<<BIT_CAL_VOL;
		}
	}
	return 0;
}

int software_sz_checkbox::Parse(Com_callback_type com_cb_ins)
{
	int from, start, linelen, getend = 0;
	int i;
	//BYTE commbuf[40];
	start = from = 0;

	if(strstr((char *)bufCom, "Starting")) {
		TRACE("DEV:%s\n", bufCom);
	}

NextLine:
	for(; from < (int)bufReaded; from += 1) {
		// 获取一行数据
		if('\n' == bufCom[from] || '\r' == bufCom[from]) {
			for(i=start; i<from; i++){
				if(bufCom[i] == '\0')
					bufCom[i] = 255;
			}
			bufCom[from] = '\0';
			getend = 1;
			break;
		}
	}
	
	if(getend) {                       //如果找到换行符
		linelen = from-start;

		if(linelen > 2){
			//ParseLine(&bufCom[start], linelen);       //如果第一个字节为/0的话，会被认为是空的buf
			com_cb_ins(this, &bufCom[start], linelen);       //如果第一个字节为/0的话，会被认为是空的buf
			if( ((CButton *)GetDlgItem(IDC_CHECK6))->GetCheck() ){
				char buffer[4096];
				memset(buffer, 0, 4096);
				if(linelen >= 4096){
					memcpy(buffer, &bufCom[start], 4096);
				}else{
					memcpy(buffer, &bufCom[start], linelen);
				}
				edit_out.ReplaceSel(buffer);
				edit_out.ReplaceSel("\r\n");
			}
		}

		from += 1;
		start = from;
		getend = 0;
		goto NextLine;
	}
	
	for(; start < (int)bufReaded; start++)               //处理完一段数据后，会让start跳过/0
		if(bufCom[start] != '\0') break;

	// 数据是否为一整行,将剩下的数据放到开始处
	if(start > 0 && start <= (int)(bufReaded)) {
		bufReaded = bufReaded - start;
		memcpy(bufCom, &bufCom[start], bufReaded);
	}else if(bufReaded >= COM_BUF_SIZE)
		bufReaded = 0;

	return 0;
}

int software_sz_checkbox::Process(Com_callback_type com_cb_ins)
{
	int rdlen, less = COM_BUF_SIZE - bufReaded;
	int hand_len = 0,offset = 0;

	if((rdlen = com_board.ReadCommBlock((LPSTR)&bufCom[bufReaded], less)) <= 0){
		if(rdlen==0)
			return 0;
	}

	// 分析读取到的数据           
	if(rdlen>0){
		bufReaded += rdlen;
	}

	Parse(com_cb_ins);
	return rdlen;
}

int software_sz_checkbox::mes_sannuo_write_logfile(CString SN)
{
	CString sFname;
	CreateDirectory(SANNUO_MES_LOG_PATH, 0);
	sFname = SN + "_PASS.txt";
	sFname = SANNUO_MES_LOG_PATH + sFname;
	if(m_mes_file.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		sFname.Format("********************************\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Function   :  盒子测试\r\n");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("Device SN:     ");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname = SN + "\r\n";
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname.Format("CAL VOL VALUE:     ");
		m_mes_file.Write(sFname, sFname.GetLength());
		sFname = vol_value_sav + "\r\n";
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

int software_sz_checkbox::process_com()
{
	unsigned int pass_flag,pass_init_flag;
	Com_callback_type com_cb_ins = software_sz_checkbox::ParseLine;

	process_com_exit = 0;
	error_flag = 0;
	right_flag = 0;

	pass_flag = 1<<BIT_POWERON;
	if(checkbox_sz_option_instance.g_if_check_wdt){ pass_flag += 1<<BIT_WATCHDOG; }
	if(checkbox_sz_option_instance.g_if_check_max20340){ pass_flag += 1<<BIT_MAX20340; }
	if(checkbox_sz_option_instance.g_if_check_max77813){ pass_flag += 1<<BIT_MAX77813; }
	pass_init_flag = pass_flag;
	if(checkbox_sz_option_instance.g_if_check_plc_commu){ pass_flag += (1<<BIT_PLCLEFT); }
	if(checkbox_sz_option_instance.g_if_check_plc_commu_right){ pass_flag += (1<<BIT_PLCRIGHT); }
	if(checkbox_sz_option_instance.g_if_check_hal){ pass_flag += (1<<BIT_HALCLOSE) + (1<<BIT_HALOPEN); }
	if(checkbox_sz_option_instance.g_if_check_key){ pass_flag += (1<<BIT_KEYDOWN) + (1<<BIT_KEYUP); }
	if(checkbox_sz_option_instance.g_if_check_usb){ pass_flag += (1<<BIT_USBOUT) + (1<<BIT_USBIN); }
	if(checkbox_sz_option_instance.g_if_check_cal_vol){ pass_flag += (1<<BIT_CAL_VOL); }

	const char *Commkey = "reset case\r\n";
	com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));
	while(process_com_exit != 1){
		Process(com_cb_ins);
		if(right_flag == pass_flag){
			if(general_option_instance.g_check_mes){
				if(general_option_instance.g_check_mes_type == 4){
					mes_sannuo_write_logfile(sn_no);
					AddEvent2List("mes log文件已生成！");
					pass_green_color = 1;
					m_Epass.SetWindowTextA("PASS");
					Invalidate();	
				}else{
					AddEvent2List("不支持的mes类型，请重新配置");
					pass_green_color = 0;
					m_Epass.SetWindowTextA("FAIL");
					Invalidate();
				}
			}else{
				pass_green_color = 1;
				m_Epass.SetWindowTextA("PASS");
				Invalidate();			
			}
			break;
		}
		if(error_flag != 0){
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		}
	}
	return 0;
}

int software_sz_checkbox::WorkerThreadProc(void *param)
{
	software_sz_checkbox *p = (software_sz_checkbox*)param;
	CString sTemp;
	p->WorkerThreadProc_in_flag = 1;
	if(FALSE == p->com_board.InitComm(p->general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		p->general_option_instance.g_ttycom_baud)) {
		sTemp.Format("串口%d打开失败！",p->general_option_instance.g_ttycom_port);
		p->AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return -1;
	}else{
		sTemp.Format("串口%d打开成功！",p->general_option_instance.g_ttycom_port);
		p->AddEvent2List(sTemp);
		p->AddEvent2List("测试开始！");
		p->bufReaded = 0;
	}
	p->process_com();
	p->com_board.EndComm();
	p->WorkerThreadProc_in_flag = 0;
	return 0;
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_sz_checkbox::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	CString stmp;
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 14){//v3
		if(WorkerThreadProc_in_flag == 0){
			sn_no = sText;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_checkbox::WorkerThreadProc, this, 0, NULL);
			stmp = "sn号输入成功，等待上电" + sText;
			AddEvent2List(stmp);
		}else{
			AddEvent2List("校准程序未退出，等待校准完成后测试！");
		}
	}
	//if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
	//	if( sText.Find("-") < 0 ){
	//		if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
	//						general_option_instance.g_sn_start, 
	//						general_option_instance.g_sn_count) != TRUE) {
	//			AfxMessageBox("当前产品SN号(v2)不正确");
	//		}else{
	//			sn_no = sText;
	//			AddEvent2List("sn号输入成功，等待上电");
	//		}
	//	}
	//}
	//if(sText.GetLength() >= 19){
	//	if(CheckSN(sText, general_option_instance.g_sn_prompt, 
	//						general_option_instance.g_sn_start, 
	//						general_option_instance.g_sn_count) != TRUE) {
	//		AfxMessageBox("当前产品SN号(v1)不正确");
	//	}else{
	//		sn_no = sText;
	//		AddEvent2List("sn号输入成功，等待上电");
	//	}
	//}
	// TODO:  在此添加控件通知处理程序代码
}

void software_sz_checkbox::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	pass_green_color = 0;
	process_com_exit = 1;
	WorkerThreadProc_in_flag = 0;

	m_reboot.SetColor(STATIC_WND_DISABLE);
	m_watchdog.SetColor(STATIC_WND_DISABLE);
	m_max20340.SetColor(STATIC_WND_DISABLE);
	m_max77813.SetColor(STATIC_WND_DISABLE);
	m_plc_left.SetColor(STATIC_WND_DISABLE);
	m_plc_right.SetColor(STATIC_WND_DISABLE);
	m_open_case.SetColor(STATIC_WND_DISABLE);
	m_close_case.SetColor(STATIC_WND_DISABLE);
	m_key_down.SetColor(STATIC_WND_DISABLE);
	m_key_up.SetColor(STATIC_WND_DISABLE);
	m_usb_in.SetColor(STATIC_WND_DISABLE);
	m_usb_out.SetColor(STATIC_WND_DISABLE);
	m_cal_value.SetColor(STATIC_WND_DISABLE);

	m_cal_value.SetWindowTextA("");

	edit_out.SetWindowTextA("");

	m_listEvt.ResetContent();
	m_EditSn.SetFocus();
	AddEvent2List("#####测试需打开盒盖#####");
	AddEvent2List("#####等待用户输入sn号#####");
}

void software_sz_checkbox::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetAllItemRestart();
}

#define ARRAY_DIM(a) (sizeof(a) / sizeof((a)[0]))
typedef struct{
    int if_select;
    int check_idc;
}sz_if_checkbox_str;
sz_if_checkbox_str check_idc_arr[] = {
	{0,IDC_CHECK1}, 
	{0,IDC_CHECK3}, 
	{0,IDC_CHECK5}, 
	{0,IDC_CHECK13}, 
	{0,IDC_CHECK14}, 
	{0,IDC_CHECK15}, 
	{0,IDC_CHECK16}, 
	{0,IDC_CHECK17}, 
};

BOOL software_sz_checkbox::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	
	CString dir_path;
	dir_path = LOG_PATH_CHECKBOX_sz;

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

	check_idc_arr[0].if_select = checkbox_sz_option_instance.g_if_check_wdt;
	check_idc_arr[1].if_select = checkbox_sz_option_instance.g_if_check_max20340;
	check_idc_arr[2].if_select = checkbox_sz_option_instance.g_if_check_max77813;
	check_idc_arr[3].if_select = checkbox_sz_option_instance.g_if_check_plc_commu;
	check_idc_arr[4].if_select = checkbox_sz_option_instance.g_if_check_hal;
	check_idc_arr[5].if_select = checkbox_sz_option_instance.g_if_check_key;
	check_idc_arr[6].if_select = checkbox_sz_option_instance.g_if_check_usb;
	check_idc_arr[7].if_select = checkbox_sz_option_instance.g_if_check_plc_commu_right;
	for(int i=0; i<ARRAY_DIM(check_idc_arr); i++){
		((CButton*)GetDlgItem(check_idc_arr[i].check_idc))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(check_idc_arr[i].check_idc))->SetCheck(check_idc_arr[i].if_select);
	}

	SetAllItemRestart();
	return TRUE;  // return TRUE unless you set the focus to a control
}

HBRUSH software_sz_checkbox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC11)//如果是静态编辑框
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

BOOL software_sz_checkbox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

void software_sz_checkbox::OnBnClickedRadioMusic1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTemp;
	if(FALSE == com_board.InitComm(general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		general_option_instance.g_ttycom_baud)) {
		sTemp.Format("串口%d打开失败！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return;
	}else{
		sTemp.Format("串口%d打开成功！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AddEvent2List("观察红灯！");
	}
	
	const char *Commkey = "ledlight 0\r\n";
	com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));

	if(WorkerThreadProc_in_flag == 0)//主循环没跑才可以关串口
		com_board.EndComm();
}

void software_sz_checkbox::OnBnClickedRadioMusic2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTemp;
	if(FALSE == com_board.InitComm(general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		general_option_instance.g_ttycom_baud)) {
		sTemp.Format("串口%d打开失败！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return;
	}else{
		sTemp.Format("串口%d打开成功！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AddEvent2List("观察绿灯！");
	}
	
	const char *Commkey = "ledlight 1\r\n";
	com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));

	if(WorkerThreadProc_in_flag == 0)//主循环没跑才可以关串口
		com_board.EndComm();
}

void software_sz_checkbox::OnBnClickedRadioMusic3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTemp;
	if(FALSE == com_board.InitComm(general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		general_option_instance.g_ttycom_baud)) {
		sTemp.Format("串口%d打开失败！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return;
	}else{
		sTemp.Format("串口%d打开成功！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AddEvent2List("观察白灯！");
	}
	
	const char *Commkey = "ledlight 6\r\n";
	com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));

	if(WorkerThreadProc_in_flag == 0)//主循环没跑才可以关串口
		com_board.EndComm();
}

void software_sz_checkbox::OnBnClickedRadioMusic4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sTemp;
	if(FALSE == com_board.InitComm(general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		general_option_instance.g_ttycom_baud)) {
		sTemp.Format("串口%d打开失败！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return;
	}else{
		sTemp.Format("串口%d打开成功！",general_option_instance.g_ttycom_port);
		AddEvent2List(sTemp);
		AddEvent2List("观察橙灯！");
	}
	
	const char *Commkey = "ledlight 3\r\n";
	com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));

	if(WorkerThreadProc_in_flag == 0)//主循环没跑才可以关串口
		com_board.EndComm();
}

int software_sz_checkbox::ParseLine_cal(void *param, BYTE *buf, int len)
{
	CString sText(buf);
	software_sz_checkbox *p = (software_sz_checkbox*)param;
	BYTE vbuf[10];memset(vbuf, 0, 10);

	if(sText.Find("check VolRef ") >= 0){
		memcpy(vbuf,&buf[sText.Find("check VolRef ") + 13],4);
		CString vol_value(vbuf);
		p->m_cal_value.SetWindowTextA(vol_value);

		if(sText.Find("succ") >= 0){
			p->m_cal_value.SetColor(STATIC_WND_SUCCESS);
			p->AddEvent2List("校准成功！");
			p->process_com_exit = 1;
		}else{
			p->m_cal_value.SetColor(STATIC_WND_WARNING);
			p->AddEvent2List("校准失败！");		
			p->process_com_exit = 1;
		}
	}
	return 0;
}

int software_sz_checkbox::WorkerThreadProc_cal(void *param)
{
	software_sz_checkbox *p = (software_sz_checkbox*)param;
	//Com_callback_type com_cb_ins = software_sz_checkbox::ParseLine_cal;

	//CString sTemp;
	//p->process_com_exit = 0;
	//p->WorkerThreadProc_in_flag = 1;
	//if(FALSE == p->com_board.InitComm(p->general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
	//	p->general_option_instance.g_ttycom_baud)) {
	//	sTemp.Format("串口%d打开失败！",p->general_option_instance.g_ttycom_port);
	//	p->AddEvent2List(sTemp);
	//	AfxMessageBox(sTemp);
	//	return -1;
	//}else{
	//	sTemp.Format("串口%d打开成功！",p->general_option_instance.g_ttycom_port);
	//	p->AddEvent2List(sTemp);
	//	p->AddEvent2List("开始校准！");
	//}
	//
	//const char *Commkey = "check VolRef\r\n";
	//p->com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));

	//CTime tNow = CTime::GetCurrentTime();CTime tsav = CTime::GetCurrentTime();
	//while(p->process_com_exit != 1){
	//	p->Process(com_cb_ins);
	//	CTime tNow = CTime::GetCurrentTime();
	//	if(tNow - tsav > 1){
	//		p->AddEvent2List("未收到回复，超时退出！");
	//		break;
	//	}
	//}

	//p->com_board.EndComm();
	//p->bufReaded = 0;
	//p->WorkerThreadProc_in_flag = 0;

	CString sTemp;
	if(FALSE == p->com_board.InitComm(p->general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		p->general_option_instance.g_ttycom_baud)) {
		sTemp.Format("串口%d打开失败！",p->general_option_instance.g_ttycom_port);
		p->AddEvent2List(sTemp);
		AfxMessageBox(sTemp);
		return -1;
	}else{
		sTemp.Format("串口%d打开成功！",p->general_option_instance.g_ttycom_port);
		p->AddEvent2List(sTemp);
		p->AddEvent2List("开始校准！");
	}
	
	const char *Commkey = "check VolRef\r\n";
	p->com_board.WriteCommBlock((LPSTR)Commkey, strlen(Commkey));

	return 0;
}

void software_sz_checkbox::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(checkbox_sz_option_instance.g_if_check_cal_vol){
		if(WorkerThreadProc_in_flag == 1){
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_sz_checkbox::WorkerThreadProc_cal, this, 0, NULL);
		}else{
			AddEvent2List("等待测试开始后开始校准！");
		}
	}else{
		AddEvent2List("未勾选校准测试！");
	}
}
