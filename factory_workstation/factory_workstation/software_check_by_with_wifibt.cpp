// software_check_by_with_wifibt.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_check_by_with_wifibt.h"
#include "afxdialogex.h"

struct msg_by_check_info{
	int msg_type;
	int data1;//分区
	int data2;//百分比
};
enum {
	BY_CHECK_MSG_TYPE_COM_OPENERR = 0,
	BY_CHECK_MSG_TYPE_COM_OPENSUC,
	BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR,
	BY_CHECK_MSG_TYPE_CURRENT_VALUE,
	BY_CHECK_MSG_TYPE_CURRENT_SUC,
	BY_CHECK_MSG_TYPE_USB_TIMEOUT,
	BY_CHECK_MSG_TYPE_USBDEVICE_DET,
	BY_CHECK_MSG_TYPE_CREATE_INIT,
	BY_CHECK_MSG_TYPE_INIT_ERR,
	BY_CHECK_MSG_TYPE_INIT_PART,
	BY_CHECK_MSG_TYPE_INIT_SUC,
	BY_CHECK_MSG_TYPE_WIFI_CONNECT_TIMEOUT,
	BY_CHECK_MSG_TYPE_MES_ERR,
	BY_CHECK_MSG_TYPE_MES_SUC,
	BY_CHECK_MSG_TYPE_KEYALL_SUC,
	BY_CHECK_MSG_TYPE_KEYALL_ERR,
	BY_CHECK_MSG_TYPE_START_AUDIO,
	BY_CHECK_MSG_TYPE_AUDIO_ERR,
	BY_CHECK_MSG_TYPE_AUDIO_TIME,
	BY_CHECK_MSG_TYPE_AUDIO_SUC,
	BY_CHECK_MSG_TYPE_ALL_SUC,
};
enum {
	BY_CHECK_NORMAL_CURRENT = 0,
	BY_CHECK_DEVICES,
	BY_CHECK_CREATE_INIT,
	BY_CHECK_CHECK_INIT,
	BY_CHECK_CREATE_ONLY_WIFI,
	BY_CHECK_CHECK_ONLY_WIFI,
	BY_CHECK_CHECK_KEY,
	BY_CHECK_CHECK_WAIT_KEY,
	BY_CHECK_CHECK_BACKLIGHT,
	BY_CHECK_CHECK_AUDIO,
	BY_CHECK_CHECK_WAITAUDIO,
	BY_CHECK_CHARGE_CURRENT,
	BY_CHECK_SLEEP_CURRENT,
	BY_CHECK_SCUCESS,
};
enum {
	BY_CHECK_INIT_FLAG_SN = 0,
	BY_CHECK_INIT_FLAG_WIFI,
	BY_CHECK_INIT_FLAG_BT,
	BY_CHECK_INIT_FLAG_DMIC,
	BY_CHECK_INIT_FLAG_PA,
	BY_CHECK_INIT_FLAG_KEYUP,
	BY_CHECK_INIT_FLAG_KEYDOWN,
	BY_CHECK_INIT_FLAG_KEYPOWER,
	BY_CHECK_INIT_FLAG_AUDIO,
	BY_CHECK_INIT_FLAG_USBERR,
};
enum {
	BY_DISPLAY_CURRENT_VALUE_NORMAL = 0,
	BY_DISPLAY_CURRENT_VALUE_CHARGE,
	BY_DISPLAY_CURRENT_VALUE_BACKLIGHT,
	BY_DISPLAY_CURRENT_VALUE_SLEEP,
};
// software_check_by_with_wifibt 对话框

IMPLEMENT_DYNAMIC(software_check_by_with_wifibt, CDialogEx)

software_check_by_with_wifibt::software_check_by_with_wifibt(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_check_by_with_wifibt::IDD, pParent)
{

}

software_check_by_with_wifibt::~software_check_by_with_wifibt()
{
}

void software_check_by_with_wifibt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listEvt);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_STATIC6, m_readSn);
	DDX_Control(pDX, IDC_EDIT_FIRMVER, m_fs_version);
	DDX_Control(pDX, IDC_STATIC10, m_Epass);

	DDX_Control(pDX, IDC_STC_SN_READ, m_snRead);
	DDX_Control(pDX, IDC_STC_BOOT, m_btnBoot);
	DDX_Control(pDX, IDC_STC_WIFI, m_btnWifi);
	DDX_Control(pDX, IDC_STC_BT, m_btnBt);
	DDX_Control(pDX, IDC_STC_DMIC, m_btnDmic);
	DDX_Control(pDX, IDC_STC_PA, m_btnPA);
	//DDX_Control(pDX, IDC_STC_TKEYUP, m_btnKeyU);
	//DDX_Control(pDX, IDC_STC_TKEYDOWN, m_btnKeyD);
	DDX_Control(pDX, IDC_STC_KEYPOWER, m_btnKeyPower);
	DDX_Control(pDX, IDC_STC_AUDIO, m_btnAud);
	DDX_Control(pDX, IDC_EDIT_NORMAL_CURRENT2, m_normalCurrect);
	DDX_Control(pDX, IDC_EDIT_CUR_CURRENT, m_chargeCurrect);
	DDX_Control(pDX, IDC_EDIT_SLEEP_CURRENT3, m_sleepCurrect);
	DDX_Control(pDX, IDC_EDIT_POWEROFF_CURRENT, m_backlightCurrect);

	DDX_Control(pDX, IDC_CHECK1, current_checkBox);
	DDX_Control(pDX, IDC_CHECK3, sleep_current_checkBox);
	DDX_Control(pDX, IDC_CHECK5, backlight_current_checkBox);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_kernel_version);
	DDX_Control(pDX, IDC_AUDIO_SEC, m_rec_sec);
	DDX_Control(pDX, IDC_AUDIO_SEC2, m_rec_db);
}


BEGIN_MESSAGE_MAP(software_check_by_with_wifibt, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_check_by_with_wifibt::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON13, &software_check_by_with_wifibt::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_EDIT2, &software_check_by_with_wifibt::OnEnChangeEdit2)
	ON_WM_CTLCOLOR()
	//ON_BN_CLICKED(IDC_RADIO_MUSIC1, &software_check_by::OnBnClickedRadioMusic1)
	//ON_BN_CLICKED(IDC_RADIO_MUSIC2, &software_check_by::OnBnClickedRadioMusic2)
	//ON_BN_CLICKED(IDC_RADIO_MUSIC3, &software_check_by::OnBnClickedRadioMusic3)
	//ON_BN_CLICKED(IDC_RADIO_MUSIC4, &software_check_by::OnBnClickedRadioMusic4)
	//ON_BN_CLICKED(IDC_RADIO_MUSIC5, &software_check_by::OnBnClickedRadioMusic5)
	//ON_BN_CLICKED(IDC_RADIO_MUSIC6, &software_check_by::OnBnClickedRadioMusic6)
	ON_BN_CLICKED(IDC_BUTTON21, &software_check_by_with_wifibt::OnBnClickedButton21)
END_MESSAGE_MAP()

void software_check_by_with_wifibt::AddEvent2List(CString sText)
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

void software_check_by_with_wifibt::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	check_by_option_instance = g_p->check_by_option_instance;
}

// software_check_by_with_wifibt 消息处理程序

void software_check_by_with_wifibt::init_err_process(unsigned int flag)
{
	CString sText;
	if(flag & (1<<BY_CHECK_INIT_FLAG_USBERR)){
		AddEvent2List("usb失去连接! ");
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_SN)){
		sText.Format("%s",sn_read);
		m_readSn.SetWindowTextA(sText);
		AddEvent2List("sn号检测失败! ");
		m_snRead.SetWindowTextA("err");
		m_snRead.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_WIFI)){
		AddEvent2List("wifi检测失败! ");
		m_btnWifi.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_BT)){
		AddEvent2List("蓝牙检测失败! ");
		m_btnBt.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_DMIC)){
		AddEvent2List("dmic检测失败! ");
		m_btnDmic.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_PA)){
		AddEvent2List("PA检测失败! ");
		m_btnPA.SetColor(STATIC_WND_WARNING);
	}
}

void software_check_by_with_wifibt::init_part_process(unsigned int flag)
{
	CString sText;
	if(flag & (1<<BY_CHECK_INIT_FLAG_SN)){
		sText.Format("%s",fs_version);
		m_fs_version.SetWindowTextA(sText);
		sText.Format("%s",kernel_version);
		m_kernel_version.SetWindowTextA(sText);
		sText.Format("%s",sn_read);
		m_readSn.SetWindowTextA(sText);
		m_snRead.SetWindowTextA("OK");
		m_snRead.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_WIFI)){
		m_btnWifi.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_BT)){
		m_btnBt.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_DMIC)){
		m_btnDmic.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_PA)){
		m_btnPA.SetColor(STATIC_WND_SUCCESS);
	}
	//if(flag & (1<<BY_CHECK_INIT_FLAG_KEYUP)){
	//	m_btnKeyU.SetColor(STATIC_WND_SUCCESS);
	//}
	//if(flag & (1<<BY_CHECK_INIT_FLAG_KEYDOWN)){
	//	m_btnKeyD.SetColor(STATIC_WND_SUCCESS);
	//}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYPOWER)){
		m_btnKeyPower.SetColor(STATIC_WND_SUCCESS);
	}
}

LRESULT software_check_by_with_wifibt::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	float time_ms;
	CString sText;
	msg_by_check_info *pmsg = (msg_by_check_info*)lParam;
	switch(pmsg->msg_type) {
		case BY_CHECK_MSG_TYPE_COM_OPENERR:
			if(pmsg->data1 == 1){
				AfxMessageBox("电流板串口打开失败! ");
				AddEvent2List("电流板串口打开失败! ");
			}else{
				AfxMessageBox("测试机串口打开失败! ");
				AddEvent2List("测试机串口打开失败! ");			
			}
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_COM_OPENSUC:
			AddEvent2List("电流板串口打开成功，开始检测电流! ");
			break;
		case BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR:
			if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_NORMAL){
				sText.Format("%d",pmsg->data2);
				m_normalCurrect.SetWindowTextA(sText);
				m_normalCurrect.SetColor(STATIC_WND_WARNING);
				AfxMessageBox("板卡电流过大! ");
				AddEvent2List("板卡电流过大! ");
			}else if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_CHARGE){
				sText.Format("%d",pmsg->data2);
				m_chargeCurrect.SetWindowTextA(sText);
				m_chargeCurrect.SetColor(STATIC_WND_WARNING);
				AfxMessageBox("充电电流检测失败! ");
				AddEvent2List("充电电流检测失败! ");
			}else if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_BACKLIGHT){
				sText.Format("%d",pmsg->data2);
				m_backlightCurrect.SetWindowTextA(sText);
				m_backlightCurrect.SetColor(STATIC_WND_WARNING);
				AfxMessageBox("背光芯片检测失败! ");
				AddEvent2List("背光芯片检测失败! ");
			}else if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_SLEEP){
				sText.Format("%d",pmsg->data2);
				m_sleepCurrect.SetWindowTextA(sText);
				m_sleepCurrect.SetColor(STATIC_WND_WARNING);
				AfxMessageBox("休眠电流检测失败! ");
				AddEvent2List("休眠电流检测失败! ");
			}
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_CURRENT_VALUE:
			if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_NORMAL){
				sText.Format("%d",pmsg->data2);
				m_normalCurrect.SetWindowTextA(sText);
			}
			break;
		case BY_CHECK_MSG_TYPE_CURRENT_SUC:
			if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_NORMAL){
				AddEvent2List("电流检测成功，开始连接usb! ");
				sText.Format("%d",pmsg->data2);
				m_normalCurrect.SetWindowTextA(sText);
				m_normalCurrect.SetColor(STATIC_WND_SUCCESS);
			}else if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_CHARGE){
				AddEvent2List("充电电流检测成功! ");
				sText.Format("%d",pmsg->data2);
				m_chargeCurrect.SetWindowTextA(sText);
				m_chargeCurrect.SetColor(STATIC_WND_SUCCESS);
			}else if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_BACKLIGHT){
				AddEvent2List("背光芯片检测成功! ");
				sText.Format("%d",pmsg->data2);
				m_backlightCurrect.SetWindowTextA(sText);
				m_backlightCurrect.SetColor(STATIC_WND_SUCCESS);
			}else if(pmsg->data1 == BY_DISPLAY_CURRENT_VALUE_SLEEP){
				AddEvent2List("休眠电流检测成功! ");
				sText.Format("%d",pmsg->data2);
				m_sleepCurrect.SetWindowTextA(sText);
				m_sleepCurrect.SetColor(STATIC_WND_SUCCESS);
			}
			break;
		case BY_CHECK_MSG_TYPE_USB_TIMEOUT:
			AfxMessageBox("usb连接超时! ");
			AddEvent2List("usb连接超时! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_USBDEVICE_DET:
			AddEvent2List("usb连接成功! ");
			m_btnBoot.SetColor(STATIC_WND_SUCCESS);
			((CButton*)GetDlgItem(IDC_BUTTON21))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_CREATE_INIT:
			AddEvent2List("准备启动测试! ");
			break;
		case BY_CHECK_MSG_TYPE_INIT_ERR:
			init_err_process(pmsg->data1);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_INIT_PART:
			init_part_process(pmsg->data1);
			break;
		case BY_CHECK_MSG_TYPE_INIT_SUC:
			AddEvent2List("预备测试按键，等待用户按键! ");
			break;
		case BY_CHECK_MSG_TYPE_WIFI_CONNECT_TIMEOUT:
			AfxMessageBox("wifi连接超时! ");
			AddEvent2List("wifi连接超时!");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_MES_ERR:
			//AfxMessageBox("mes连不上服务器，或者机器已过站! ");
			AddEvent2List("mes连不上服务器，或者机器已过站! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_MES_SUC:
			AddEvent2List("mes包已发送! ");
			break;
		case BY_CHECK_MSG_TYPE_KEYALL_SUC:
			AddEvent2List("按键测试完成! ");
			break;
		case BY_CHECK_MSG_TYPE_KEYALL_ERR:
			AfxMessageBox("按键测试错误! ");
			AddEvent2List("按键测试错误! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_START_AUDIO:
			//for(int id=IDC_RADIO_MUSIC1; id <= IDC_RADIO_MUSIC6; id++) {
			//	((CButton*)GetDlgItem(id))->SetCheck(0);
			//	((CButton*)GetDlgItem(id))->EnableWindow(TRUE);
			//}
			AddEvent2List("开始测试音频，开始录音! ");
			break;
		case BY_CHECK_MSG_TYPE_AUDIO_ERR:
			AddEvent2List("测试音频发生错误! ");
			m_btnAud.SetColor(STATIC_WND_WARNING);
			sText.Format("%d",aud_db);
			m_rec_db.SetWindowTextA(sText);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case BY_CHECK_MSG_TYPE_AUDIO_TIME:
			time_ms = (float)pmsg->data1;
			time_ms = time_ms/1000;
			sText.Format("%f",time_ms);
			sText = sText.Left(3);
			sText += "秒";
			m_rec_sec.SetWindowTextA(sText);
			break;
		case BY_CHECK_MSG_TYPE_AUDIO_SUC:
			AddEvent2List("测试音频成功! ");
			sText.Format("%d",aud_db);
			m_rec_db.SetWindowTextA(sText);
			m_btnAud.SetColor(STATIC_WND_SUCCESS);
			break;
		case BY_CHECK_MSG_TYPE_ALL_SUC:
			AddEvent2List("测试结束，等待用户复位! ");
			pass_green_color = 1;
			m_Epass.SetWindowTextA("PASS");
			Invalidate();
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		default:
			break;
	}
	delete pmsg;
	return 0;
}

CString software_check_by_with_wifibt::ExecuteCmd_iperf(CString cmd_name, CString cmd_param)
{
    STARTUPINFO startupInfo;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
    ::ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.wShowWindow = SW_HIDE;
    startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    if(::CreateProcess(name, (LPTSTR)param, 
                       NULL,  // process security
                       NULL,  // thread security
                       FALSE, // no inheritance
                       0,     // no startup flags
                       NULL,  // no special environment
                       NULL,  // default startup directory
                       &startupInfo,
                       &iperf))
		return "success";
	else
		return "failed";
}
int software_check_by_with_wifibt::create_app_iperf(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\net\\iperf.exe";
	CString cmd_param;
	cmd_param = " -s";
	response = p->ExecuteCmd_iperf(cmd_str, cmd_param);
	return 0;
}

#define WIFI_AFTER_BT
extern int getAvgPcmDB(INT16  *pcmdata,  long nmemb);
int software_check_by_with_wifibt::WorkerThreadProc(void *param)
{
	int status = BY_CHECK_DEVICES;
	int init_pass_flag = 1<<BY_CHECK_INIT_FLAG_SN | //1<<BY_CHECK_INIT_FLAG_WIFI | 
		1<<BY_CHECK_INIT_FLAG_BT | 1<<BY_CHECK_INIT_FLAG_DMIC | 1<<BY_CHECK_INIT_FLAG_PA;
	int init_only_wifi_pass_flag = 1<<BY_CHECK_INIT_FLAG_WIFI;
	CTime tNow = CTime::GetCurrentTime();
	CTime tsav = CTime::GetCurrentTime();
	CTime tsav_wifi = CTime::GetCurrentTime();
	int time_over, time_over_times = 0;
	CTime tLastPlay;
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	CContrlComm *pComBoard = &(p->comInfo_board);
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CCurlCtrl *mesptr = &p->mesInfo;
	int mes_start = 0;
#ifndef WIFI_AFTER_BT
	init_pass_flag |= 1<<BY_CHECK_INIT_FLAG_WIFI;
#endif
	unsigned int init_part_flag = 0;
	unsigned int  init_flag_sav;
	p->init_flag = 0;
	p->init_err_flag = 0;
	msg_by_check_info *msg_p;
	if(p->check_by_option_instance.g_current_test){
		if(FALSE == pComCheck->InitComm(p->check_by_option_instance.g_currentcom_port, DEIGHT, SONE, PNONE, 
			p->check_by_option_instance.g_currentcom_baud)) {
			msg_p = new msg_by_check_info;
			msg_p->msg_type = BY_CHECK_MSG_TYPE_COM_OPENERR;
			msg_p->data1 = 1;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			return -1;
		}else {
			msg_p = new msg_by_check_info;
			msg_p->msg_type = BY_CHECK_MSG_TYPE_COM_OPENSUC;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			pComCheck->SendComand(CHARG_SHUT_COM);      // 关闭充电
			p->com_thread_quit = false;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::COMThreadProc, param, 0, NULL);
		}
		status = BY_CHECK_NORMAL_CURRENT;
	}
	if(FALSE == pComBoard->InitComm(p->general_option_instance.g_ttycom_port, DEIGHT, SONE, PNONE, 
		p->general_option_instance.g_ttycom_baud)) {
		msg_p = new msg_by_check_info;
		msg_p->msg_type = BY_CHECK_MSG_TYPE_COM_OPENERR;
		msg_p->data1 = 2;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		return -1;
	}
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_iperf, param, 0, NULL);
	while(1){
		tNow = CTime::GetCurrentTime();
		//if(p->check_by_option_instance.g_chargCurrent){
		//	pComCheck->Process();
		//}
		switch (status){
			case BY_CHECK_NORMAL_CURRENT:
				p->current = pComCheck->GetCurrent();
				time_over = (int)(tNow.GetTime() - tsav.GetTime());
				if( time_over > 1){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_VALUE;
					msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_NORMAL;
					msg_p->data2 = p->current;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
					tsav = tNow;
				}
				if(p->current > p->check_by_option_instance.g_normal_current_max){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR;
					msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_NORMAL;
					msg_p->data2 = p->current;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if( (p->current < p->check_by_option_instance.g_normal_current_max) &&
					(p->current > p->check_by_option_instance.g_normal_current_min)){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_SUC;
					msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_NORMAL;
					msg_p->data2 = p->current;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					pComCheck->SendComand(CHARG_OPEN_COM);      // 打开充电，同时打开usb
					status = BY_CHECK_CHARGE_CURRENT;
					Sleep(1500);//等1500ms 使得充电稳定
				}else{
					if(time_over > 1){
						p->AddEvent2List("未检测到板卡！");
						time_over_times ++;
					}
					if(time_over_times == p->check_by_option_instance.g_startcheck_times){
						p->AddEvent2List("检测板卡超时，请连接板卡后重新测试！");
						((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
						goto out;
					}
				}
				break;
			case BY_CHECK_CHARGE_CURRENT:
				p->charge_current = pComCheck->GetChargCurrent();
				if(p->charge_current > p->check_by_option_instance.g_chargCurrent){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_SUC;
					msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_CHARGE;
					msg_p->data2 = p->charge_current;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					status = BY_CHECK_DEVICES;
					pComCheck->SendComand(CHARG_SHUT_COM);
					Sleep(1000);
				}else{
					Sleep(1000);
					p->charge_current = pComCheck->GetChargCurrent();
					if(p->charge_current > p->check_by_option_instance.g_chargCurrent){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_SUC;
						msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_CHARGE;
						msg_p->data2 = p->charge_current;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						status = BY_CHECK_DEVICES;
						pComCheck->SendComand(CHARG_SHUT_COM);
						Sleep(1000);
					}else{
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR;
						msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_CHARGE;
						msg_p->data2 = p->charge_current;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						pComCheck->SendComand(CHARG_SHUT_COM);
						goto out;
					}
				}
				break;
			case BY_CHECK_DEVICES:
				cmd_param = " devices";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(tNow.GetTime() - tsav.GetTime() > p->check_by_option_instance.g_usb_timeout){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_USB_TIMEOUT;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
					tsav = tNow; 
					goto out;
				}
				if(p->process_response_devices(response)){
					pComBoard->SendComand(USB_ON);
					Sleep(1000);
				}else{
					cmd_param = " shell /etc/init.d/S50servermanager stop";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
						msg_p->data1 = p->init_err_flag;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
					cmd_param = " shell echo 0 > /sys/class/backlight/gpio-backlight.0/brightness";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
						msg_p->data1 = p->init_err_flag;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_USBDEVICE_DET;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					status = BY_CHECK_CREATE_INIT;
					pComCheck->SendComand(NOPATH_TEST_R);//不过测试电阻，使电流板能供电正常。
					Sleep(100);//等一小会，确保电流板接收到了命令
				}
				break;
			case BY_CHECK_CREATE_INIT:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_sn_init, param, 0, NULL);
#ifndef WIFI_AFTER_BT
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_wifi_init, param, 0, NULL);
#endif
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_bt_init, param, 0, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_start_play_init, param, 0, NULL);
				msg_p = new msg_by_check_info;
				msg_p->msg_type = BY_CHECK_MSG_TYPE_CREATE_INIT;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				status = BY_CHECK_CHECK_INIT;
				break;
			case BY_CHECK_CHECK_INIT:
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if(init_flag_sav != init_part_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_PART;
					msg_p->data1 = init_flag_sav - init_part_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
					init_part_flag = init_flag_sav;
				}else if(p->init_flag == init_pass_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_SUC;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
#ifndef WIFI_AFTER_BT
					status = BY_CHECK_CHECK_KEY;
#else
					status = BY_CHECK_CREATE_ONLY_WIFI;
#endif
				}
				if(p->general_option_instance.g_check_mes  == 1){
					if( p->init_flag & (1<<BY_CHECK_INIT_FLAG_SN) &&
						mes_start == 0){
						mes_start = 1;
						if(p->general_option_instance.g_check_mes_type == 1){
							mesptr->clear_flag();
							mesptr->g_mes_ip = p->general_option_instance.g_mes_ip;
							mesptr->g_mes_port = p->general_option_instance.g_mes_port;
							if(mesptr->SendChecks((LPCSTR)p->sn_read) == 0)   {                       //上传MES流程检查接口    check模式第一个MES
							}else{
								AfxMessageBox("mes连不上服务器! ");
								msg_p = new msg_by_check_info;
								msg_p->msg_type = BY_CHECK_MSG_TYPE_MES_ERR;
								::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
								goto out;
							}
							if(mesptr->Messtatus  == 1){                                     //mes check判断
								msg_p = new msg_by_check_info;
								msg_p->msg_type = BY_CHECK_MSG_TYPE_MES_SUC;
								::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							}else{
								AfxMessageBox("机器已过站! ");
								msg_p = new msg_by_check_info;
								msg_p->msg_type = BY_CHECK_MSG_TYPE_MES_ERR;
								::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
								goto out;
							}
						}
					}
				}
				break;
			case BY_CHECK_CREATE_ONLY_WIFI:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_wifi_init, param, 0, NULL);
				tsav_wifi = tNow;
				status = BY_CHECK_CHECK_ONLY_WIFI;
				break;
			case BY_CHECK_CHECK_ONLY_WIFI:
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if(init_flag_sav != init_part_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_PART;
					msg_p->data1 = init_flag_sav - init_part_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
					init_part_flag = init_flag_sav;
				}else if( (p->init_flag & init_only_wifi_pass_flag) == init_only_wifi_pass_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_SUC;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					status = BY_CHECK_CHECK_KEY;
				}
				if( (tNow.GetTime() - tsav_wifi.GetTime() > p->check_by_option_instance.g_wifi_timeout) &&
					((init_flag_sav & BY_CHECK_INIT_FLAG_WIFI) == 0) ){
					TerminateProcess(p->wifi_trans_pi.hProcess,0);
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_WIFI_CONNECT_TIMEOUT;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				break;
			case BY_CHECK_CHECK_KEY:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_key_init, param, 0, NULL);
				pComCheck->SendComand(PATH_TEST_R);
				cmd_param = " push firmware/check_wifibt/1KHZ4s.wav /tmp";//push wav放这里节省时间
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				if(p->check_by_option_instance.g_current_test && p->check_by_option_instance.g_backlight_current_test){
					Sleep(2000);
					p->backlight_current = pComCheck->GetCurrent();
					cmd_param = " shell echo 16 > /sys/class/backlight/gpio-backlight.0/brightness";//打开背光到最亮
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
						msg_p->data1 = p->init_err_flag;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
					tsav = CTime::GetCurrentTime();
				}
				status = BY_CHECK_CHECK_WAIT_KEY;
				//p->key_init();
				break;
			case BY_CHECK_CHECK_WAIT_KEY:
				if( //(p->init_flag & 1<<BY_CHECK_INIT_FLAG_KEYUP) && 
					//(p->init_flag & 1<<BY_CHECK_INIT_FLAG_KEYDOWN) &&
					(p->init_flag & 1<<BY_CHECK_INIT_FLAG_KEYPOWER) ){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_PART;
					msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_KEYPOWER;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_KEYALL_SUC;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					if(p->check_by_option_instance.g_current_test && p->check_by_option_instance.g_backlight_current_test){
						status = BY_CHECK_CHECK_BACKLIGHT;
					}else{
						status = BY_CHECK_CHECK_AUDIO;
					}
				}else if(p->init_err_flag){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_KEYALL_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				break;
			case BY_CHECK_CHECK_BACKLIGHT:
				if(tNow.GetTime() - tsav.GetTime() > 2){//打开背光是否超过两秒
					unsigned int current_value = pComCheck->GetCurrent();
					if( current_value - p->backlight_current > 
						p->check_by_option_instance.g_backlight_Current ){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_SUC;
						msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_BACKLIGHT;
						msg_p->data2 = current_value;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						status = BY_CHECK_CHECK_AUDIO;
					}else{
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR;
						msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_BACKLIGHT;
						msg_p->data2 = current_value;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
				}
				break;
			case BY_CHECK_CHECK_AUDIO:
				pComCheck->SendComand(NOPATH_TEST_R);//不过测试电阻，使电流板能供电正常。
				Sleep(20);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::create_app_aplay_init, param, 0, NULL);
				msg_p = new msg_by_check_info;
				msg_p->msg_type = BY_CHECK_MSG_TYPE_START_AUDIO;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				Sleep(20);
				if(p->pc_record()){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_AUDIO_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				status = BY_CHECK_CHECK_WAITAUDIO;
				break;
			case BY_CHECK_CHECK_WAITAUDIO:
				int g_size;
				char* g_buf;
				if(p->rec_file.Open(p->rec_file_path, CFile::modeRead) == TRUE) {
					p->rec_file.SeekToBegin();
					g_size = (int)p->rec_file.GetLength();
					g_buf = (char*)malloc(g_size);
					p->rec_file.Read(g_buf,g_size);
					p->rec_file.Close();
					p->aud_db = getAvgPcmDB((INT16*)g_buf, g_size / 2);
					free(g_buf);
					if(p->aud_db > p->check_by_option_instance.g_recode_passdb)
						p->init_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO;
				}
				if( (p->init_flag & 1<<BY_CHECK_INIT_FLAG_AUDIO) ){
					cmd_param = " shell killall aplay";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
						msg_p->data1 = p->init_err_flag;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						pComCheck->SendComand(PATH_TEST_R);
						goto out;
					}
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_AUDIO_SUC;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					if(p->check_by_option_instance.g_current_test && p->check_by_option_instance.g_sleep_current_test){
						status = BY_CHECK_SLEEP_CURRENT;
					}else{
						status = BY_CHECK_SCUCESS;
					}
					pComCheck->SendComand(PATH_TEST_R);
				}else{
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_AUDIO_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					pComCheck->SendComand(PATH_TEST_R);
					goto out;
				}
				break;
			case BY_CHECK_SLEEP_CURRENT:
				pComCheck->SendComand(CURRENT_UART_CLOSE);
				cmd_param = " shell rfkill block all";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				Sleep(10);
				cmd_param = " shell \"echo mem > /sys/power/state\"";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				//Sleep(2500);
				Sleep(p->check_by_option_instance.g_sleep_waittime*1000);
				p->sleep_current = pComCheck->GetSleepCurrent();
				if( p->sleep_current < p->check_by_option_instance.g_sleepCurrent ){
					msg_p = new msg_by_check_info;
					msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_SUC;
					msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_SLEEP;
					msg_p->data2 = p->sleep_current;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					status = BY_CHECK_SCUCESS;
				}else{
					CString str_outcur;
					for(int i=0; i<5; i++){
						str_outcur.Format("err value = %d", p->sleep_current);
						p->AddEvent2List(str_outcur);
						p->sleep_current = pComCheck->GetSleepCurrent();
						Sleep(1000);
					}
					if(p->sleep_current < p->check_by_option_instance.g_sleepCurrent){
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_SUC;
						msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_SLEEP;
						msg_p->data2 = p->sleep_current;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						status = BY_CHECK_SCUCESS;					
					}else{
						msg_p = new msg_by_check_info;
						msg_p->msg_type = BY_CHECK_MSG_TYPE_CURRENT_LARGE_ERR;
						msg_p->data1 = BY_DISPLAY_CURRENT_VALUE_SLEEP;
						msg_p->data2 = p->sleep_current;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
				}
				break;
			case BY_CHECK_SCUCESS:
				if(p->general_option_instance.g_check_mes  == 1){
					if(p->general_option_instance.g_check_mes_type == 1){
						if(mesptr->SendChecke((LPCSTR)p->sn_read) == 0){            //数据上报接口    烧录第二次MES上传
						}else{
							AfxMessageBox("mes连不上服务器! ");
							msg_p = new msg_by_check_info;
							msg_p->msg_type = BY_CHECK_MSG_TYPE_MES_ERR;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						if(mesptr->Messtatus == 3){
							msg_p = new msg_by_check_info;
							msg_p->msg_type = BY_CHECK_MSG_TYPE_MES_SUC;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						}else{
							AfxMessageBox("机器已过站! ");
							msg_p = new msg_by_check_info;
							msg_p->msg_type = BY_CHECK_MSG_TYPE_MES_ERR;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
					}else if(p->general_option_instance.g_check_mes_type == 2){
						struct ch_mes_data ch_mes_data; 
						ch_mes_data.data_flag = 2;
						mesptr->changh_mes_int(p->sn_read, ch_mes_data);					
					}
					Sleep(10);
				}
				msg_p = new msg_by_check_info;
				msg_p->msg_type = BY_CHECK_MSG_TYPE_ALL_SUC;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				goto out;
				break;
		}
		Sleep(1);
	}
out:
	//if(p->check_by_option_instance.g_current_test){
	//	pComCheck->EndComm();
	//}
	TerminateProcess(p->iperf.hProcess,0);
	pComCheck->SendComand(PATH_TEST_R);
	Sleep(10);
	pComCheck->SendComand(CHARG_SHUT_COM);
	p->com_thread_quit = true;
	pComBoard->EndComm();
	return 0;
}

int software_check_by_with_wifibt::COMThreadProc(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	//msg_by_check_info *msg_p;
	while( !p->com_thread_quit ){
		pComCheck->Process();
		Sleep(1);
	}
	pComCheck->ResetComm();
	pComCheck->EndComm();
	return 0;
}

DWORD software_check_by_with_wifibt::MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    // 所有的这些录音缓冲区都是由录音函数自动触发的，开发这不需要自己触发
	software_check_by_with_wifibt *pwnd = (software_check_by_with_wifibt*)dwInstance;     // 表示录音的窗体
    PWAVEHDR whd = (PWAVEHDR)dwParam1; // 录音的头结构体对象
    switch(uMsg){
		case WIM_OPEN: // 打开录音设备，这里不做处理
			break;
		case WIM_DATA: // 表示缓冲区已满，我们将信息写入一个pcm文件
			if (pwnd->isGetSound){
				fwrite(whd->lpData, 1, whd->dwBufferLength, pwnd->pf);
				waveInAddBuffer(hWaveIn, whd, sizeof(WAVEHDR));
            }
			break;
		case WIM_CLOSE: // 停止录音
			waveInStop(hWaveIn);
			waveInReset(hWaveIn);    
			waveInClose(hWaveIn);
			break;
		default:
			break;
   }
   return 0;
}
int software_check_by_with_wifibt::pc_record()
{
	msg_by_check_info *msg_p;
	waveFormat.nSamplesPerSec = 16000;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.cbSize = 0;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample * waveFormat.nChannels)/8;
	waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;

	MMRESULT mRet=waveInOpen(&hWaveIn, WAVE_MAPPER, (LPCWAVEFORMATEX)&waveFormat,(DWORD)MicCallBack, (DWORD)this, CALLBACK_FUNCTION);
	if (mRet != MMSYSERR_NOERROR){
		AfxMessageBox("麦克风打开失败，检查是否插好");
		return -1;
	}

	INT bufSize = waveFormat.nSamplesPerSec * waveFormat.nChannels * waveFormat.wBitsPerSample /8;

	BYTE *pBuffer1 = new BYTE[bufSize];
	if (pBuffer1 == NULL) return -1;
	memset(pBuffer1, 0, bufSize);
	WAVEHDR wHdr1;
	wHdr1.lpData = (LPSTR)pBuffer1;
	wHdr1.dwBufferLength = bufSize;
	wHdr1.dwBytesRecorded = 0;
	wHdr1.dwUser = 0;
	wHdr1.dwFlags = 0;
	wHdr1.dwLoops = 1;
 
	BYTE *pBuffer2 = new BYTE[bufSize];
	if (pBuffer2 == NULL) return -1;
	memset(pBuffer2,0, bufSize);
	WAVEHDR wHdr2;
	wHdr2.lpData = (LPSTR)pBuffer2;
	wHdr2.dwBufferLength = bufSize;
	wHdr2.dwBytesRecorded = 0;
	wHdr2.dwUser = 0;
	wHdr2.dwFlags = 0;
	wHdr2.dwLoops = 1;

	waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // 准备第一个波形数据块用于录音
	waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // 准备第二个数据块用于录音

	waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // 指定波形数据块为录音输入缓存
	waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // 指定波形数据块为录音缓存
	
	CString sText;
	CTime tNow = CTime::GetCurrentTime();
	sText.Format("audio\\check_by_wifibt\\Aud_%s_%02d%02d%02d.pcm", sn_read,
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	rec_file_path = sText;
	fopen_s(&pf, sText, "ab+");// 一定要以二进制数据写入，否则录音的音频会出现杂音

	isGetSound = 1;
	DWORD dwStart, dwStop, dwStart_sav; 
	dwStart = GetTickCount(); 
	dwStart_sav = dwStart;
	waveInStart(hWaveIn); // 开始录音

	while(1){
		dwStop = GetTickCount(); 
		if(dwStop - dwStart > 200){
			msg_p = new msg_by_check_info;
			msg_p->msg_type = BY_CHECK_MSG_TYPE_AUDIO_TIME;
			msg_p->data1 = dwStop - dwStart_sav;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
			dwStart = dwStop;
		}
		if(dwStop - dwStart_sav > check_by_option_instance.g_record_time*1000){
			msg_p = new msg_by_check_info;
			msg_p->msg_type = BY_CHECK_MSG_TYPE_AUDIO_TIME;
			msg_p->data1 = dwStop - dwStart_sav;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
			break;
		}
		if(init_err_flag){
			msg_p = new msg_by_check_info;
			msg_p->msg_type = BY_CHECK_MSG_TYPE_AUDIO_ERR;
			msg_p->data1 = init_err_flag;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			isGetSound = 0;
			waveInReset(hWaveIn);
			waveInClose(hWaveIn); // 停止录音
			delete []pBuffer1;
			delete []pBuffer2;
			fclose(pf);
			return -1;
		}
		Sleep(1);
	}
    isGetSound = 0;
	waveInReset(hWaveIn);
	waveInClose(hWaveIn); // 停止录音
	delete []pBuffer1;
	delete []pBuffer2;
	fclose(pf);
	return 0;
}

int software_check_by_with_wifibt::create_app_aplay_init(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	//cmd_param = " shell amixer cset numid=5 255";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}
	cmd_param = " shell aplay /tmp/1KHZ4s.wav";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}
	return 0;
}
int software_check_by_with_wifibt::PlaysoundThreadProc(void *param)
{
	static BYTE _audfile[6][2] = {{1,2}, {1,3}, {1,4}, {2,3}, {2,4}, {3,4}};
	UINT fNo;
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;

	while(0 == p->PlaysoundThreadProc_exit) {
		::waveOutSetVolume(0, 0xFFFF);          // LEFT
		fNo = IDR_WAVE7 + _audfile[p->audNo][0] - 1;
		PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);

		if(0 != p->PlaysoundThreadProc_exit) break;

		::waveOutSetVolume(0, 0xFFFF<<16);      // RIGHT
		fNo = IDR_WAVE7 + _audfile[p->audNo][1] - 1;
		PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);
		Sleep(1);
	}
	
	return 0;
}
TCHAR* software_check_by_with_wifibt::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
CString software_check_by_with_wifibt::ExecuteCmd(CString cmd_name, CString cmd_param)
{
	SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)){
        AfxMessageBox("Error on CreatePipe()!");
        return NULL;
    }
	STARTUPINFO si={sizeof(si)};
    PROCESS_INFORMATION pi;
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
	if (!CreateProcess(name,param,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)){
        AfxMessageBox("Error on CreateProcess()!");
        return NULL;
    }
    CloseHandle(hWrite);
 
    char buffer[4096];
    memset(buffer,0,4096);
	CString output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
    }
	CloseHandle(hRead);
	return output;
}

int software_check_by_with_wifibt::process_response_devices(CString response)
{
	CString cut_str;
	int n=response.Find("List of devices attached");
	if(n<0){
		return -1;
	}
	int length=response.Delete(0,n); //找出"List of devices attached"右边的字符串位数
	cut_str = response.Right(length - sizeof("List of devices attached"));   
	if(cut_str.Find("device")<0){
		return -1;
	}
	return 0;
}

int software_check_by_with_wifibt::create_app_sn_init(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/check_wifibt/sn_init /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/sn_init";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/sn_init";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	CString cut_str;
	int n=response.Find("sn_init OK");
	if(n<0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_SN;
		return -1;
	}
	int length=response.Delete(0,n); //找出""sn_init OK\r\n""右边的字符串位数
	cut_str = response.Right(length - sizeof("sn_init OK"));   
	if( (length=cut_str.Find("SN:")) < 0 ){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_SN;
		return -1;
	}else{
		CString sn;
		cut_str.Delete(0,length);
		sn = cut_str.Mid(3,20);
		memset(p->sn_read, 0, 20);
		if(sn.Find("-") < 0){
			memcpy(p->sn_read, sn, 18);
			if(memcmp(p->sSetSN, p->sn_read, 18) == 0){
				p->init_flag |= 1<<BY_CHECK_INIT_FLAG_SN;
			}else{
				p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_SN;
			}
		}else{
			memcpy(p->sn_read, sn, 19);
			if(memcmp(p->sSetSN, p->sn_read, 19) == 0){
				p->init_flag |= 1<<BY_CHECK_INIT_FLAG_SN;
			}else{
				p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_SN;
			}
		}
	}

	memset(p->kernel_version, 0, sizeof(p->kernel_version));
	if( (length=cut_str.Find("KERNEL_VERSION:")) >= 0 ){
		memcpy(p->kernel_version, 
			cut_str.Mid( length+sizeof("KERNEL_VERSION"), cut_str.Find("rootfs_version")-length-sizeof("KERNEL_VERSION")), 
			cut_str.Find("rootfs_version")-length-sizeof("KERNEL_VERSION"));		
	}
	
	memset(p->fs_version, 0, sizeof(p->fs_version));
	if( (length=cut_str.Find("ROOTFS_VERSION:")) >= 0 ){
		memcpy(p->fs_version,
			cut_str.Mid( length+sizeof("ROOTFS_VERSION"), 8 ), 
			8);
	}
	return 0;
}
CString software_check_by_with_wifibt::ExecuteCmd_wifi_trans(CString cmd_name, CString cmd_param)
{
	SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
	//sa.bInheritHandle = NULL;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)){
        AfxMessageBox("Error on CreatePipe()!");
        return NULL;
    }
	//TRACE("%s################################################cmd:0x%x,0x%x\r\n",cmd_param,hRead,hWrite);
	
    //PROCESS_INFORMATION pi;
	//TRACE("#############################pi:0x%x\r\n",&pi);
	STARTUPINFO si={0};
    si.cb = sizeof( STARTUPINFO ); //结构体大小 
    GetStartupInfo( &si );
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
	if (!CreateProcess(name,param,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&wifi_trans_pi)){
        AfxMessageBox("Error on CreateProcess()!");
        return NULL;
    }
	CloseHandle(hWrite);    
 
    char buffer[4096];
    memset(buffer,0,4096);
	CString output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
    }
	CloseHandle(hRead);
	return output;
}
int software_check_by_with_wifibt::create_app_wifi_init(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response,sText;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;

	cmd_param = " push firmware/check_wifibt/wifi_trans/wifi_device/iperf /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/iperf";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " push firmware/check_wifibt/wifi_trans/wifi_device/test_wifi /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/test_wifi";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " push firmware/check_wifibt/wifi_trans/wifi_device/wifi_trans /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/wifi_trans";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/wifi_trans ";
	cmd_param += p->check_by_option_instance.g_wifi_ssid;
	cmd_param += " ";
	cmd_param += p->check_by_option_instance.g_wifi_passwd;
	cmd_param += " ";
	cmd_param += p->check_by_option_instance.g_wifi_ip;
	sText.Format(" %d",p->check_by_option_instance.g_wifi_speed);
	cmd_param += sText;
	p->AddEvent2List("开始测试连接wifi！");
	response = p->ExecuteCmd_wifi_trans(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	int n=response.Find("wifi_trans OK");
	if(n<0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_WIFI;
		return -1;
	}else{
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_WIFI;
	}

	return 0;
}
int software_check_by_with_wifibt::create_app_bt_init(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;

	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/brcm_patchram_plus /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/brcm_patchram_plus";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/Comparison.txt /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/hciconfig /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/hciconfig";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/libbluetooth.so.1 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/rfcomm /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/rfcomm";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/ble_read /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/ble_read";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " push firmware/check_wifibt/bt_trans/bt_device/bt_trans /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/bt_trans";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/bt_trans ";
	cmd_param += p->check_by_option_instance.g_bluetooth_board_mac;
	p->AddEvent2List("开始测试连接蓝牙！");
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	int n=response.Find("bt_trans OK");
	if(n<0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_BT;
		return -1;
	}else{
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_BT;
	}

	return 0;
}

int software_check_by_with_wifibt::create_app_start_play_init(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;

	cmd_param = " push firmware/check_wifibt/start_play /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/start_play";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " shell /tmp/start_play 2 1";//2返回校准值， 1为强制校准
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	int n=response.Find("CALI OK");
	if(n<0){
		cmd_param = " shell /tmp/start_play 2 1";//2返回校准值， 1为强制校准
		response = p->ExecuteCmd(cmd_str, cmd_param);
		if(p->if_response_err(response)){
			return -1;
		}
		n=response.Find("CALI OK");
		if(n<0){
			p->AddEvent2List("喇叭校准失败！");
			p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_PA;
		}else{
			p->AddEvent2List("喇叭校准成功！");	
		}
	}else{
		p->AddEvent2List("喇叭校准成功！");
	}

	cmd_param = " shell amixer cset numid=5 255";
	//cmd_param = " shell amixer cset numid=5 180";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/check_wifibt/warning.wav /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell aplay /tmp/warning.wav";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " shell /tmp/start_play 1";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	n=response.Find("DMIC OK");
	if(n<0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_DMIC;
		return -1;
	}else{
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_DMIC;
	}

	n=response.Find("PA OK");
	if(n<0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_PA;
		return -1;
	}else{
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_PA;
	}
	return 0;
}
int software_check_by_with_wifibt::create_app_key_init(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/check_wifibt/button_check /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/button_check";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/button_check 3";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	int n=response.Find("KEYPOWER OK");
	if(n<0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_KEYPOWER;
		p->AddEvent2List("未检测到gpio按键！");
		return -1;
	}else{
		n=response.Find("KEYPMU OK");
		if(n<0){
			p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_KEYPOWER;
			p->AddEvent2List("未检测到PMU按键！");
			return -1;
		}else{
			p->init_flag |= 1<<BY_CHECK_INIT_FLAG_KEYPOWER;
		}
	}

	return 0;
}
int software_check_by_with_wifibt::key_process(CString response)
{
	msg_by_check_info *msg_p;
	unsigned int init_flag_sav = init_flag;

	if(response.Find("KEYUP OK") >= 0){
		init_flag |= 1<<BY_CHECK_INIT_FLAG_KEYUP;
	}
	if(response.Find("KEYDOWN OK") >= 0){
		init_flag |= 1<<BY_CHECK_INIT_FLAG_KEYDOWN;
	}
	if(response.Find("KEYPOWER OK") >= 0){
		init_flag |= 1<<BY_CHECK_INIT_FLAG_KEYPOWER;
	}

	if(init_flag != init_flag_sav){
		msg_p = new msg_by_check_info;
		msg_p->msg_type = BY_CHECK_MSG_TYPE_INIT_PART;
		msg_p->data1 = init_flag - init_flag_sav;
		::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
	}
	return 0;
}
CString software_check_by_with_wifibt::ExecuteCmd_keyprocess(CString cmd_name, CString cmd_param)
{
	SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead,&hWrite,&sa,0))
    {
        AfxMessageBox("Error on CreatePipe()!");
        return NULL;
    }
	STARTUPINFO si={sizeof(si)};
    PROCESS_INFORMATION pi;
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
	if (!CreateProcess(name,param,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)){
        AfxMessageBox("Error on CreateProcess()!");
        return NULL;
    }
    CloseHandle(hWrite);
 
    char buffer[4096];
    memset(buffer,0,4096);
	CString output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
		key_process(output);
    }
	CloseHandle(hRead);
	return output;
}
int software_check_by_with_wifibt::key_init()
{
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/check/button_check /tmp";
	response = ExecuteCmd(cmd_str, cmd_param);
	if(if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/button_check";
	response = ExecuteCmd(cmd_str, cmd_param);
	if(if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/button_check";
	response = ExecuteCmd_keyprocess(cmd_str, cmd_param);
	if(if_response_err(response)){
		return -1;
	}
	return 0;
}
int software_check_by_with_wifibt::create_app_start_play(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	//cmd_param = " push firmware/check/start_play /tmp";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	//cmd_param = " shell chmod +x /tmp/start_play";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	cmd_param = " shell /tmp/start_play 2";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	return 0;
}

int software_check_by_with_wifibt::if_response_err(CString response)
{
	if(response.Find("error:") >= 0){
		init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("adb设备失去连接");
		return -1;
	}
	if(response.Find("error: device not found") < 0){
		return 0;
	}else{
		init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("usb设备失去连接");
		return -1;
	}
}

int software_check_by_with_wifibt::need_file_ifexit()
{
	if(PathFileExists("thirdapp\\adb\\adb.exe") != TRUE){
		AfxMessageBox("错误：程序路径下未找到adb.exe");
		return 1;
	}
	if(PathFileExists("firmware\\check_wifibt\\sn_init") != TRUE){
		AfxMessageBox("错误：程序路径下未找到sn_init");
		return 1;
	}
	//if(PathFileExists("firmware\\check\\wifi_init") != TRUE){
	//	AfxMessageBox("错误：程序路径下未找到wifi_init");
	//	return 1;
	//}
	//if(PathFileExists("firmware\\check\\bt_init") != TRUE){
	//	AfxMessageBox("错误：程序路径下未找到bt_init");
	//	return 1;
	//}
	if(PathFileExists("firmware\\check_wifibt\\start_play") != TRUE){
		AfxMessageBox("错误：程序路径下未找到start_play");
		return 1;
	}
	if(PathFileExists("firmware\\check_wifibt\\button_check") != TRUE){
		AfxMessageBox("错误：程序路径下未找到button_check");
		return 1;
	}
	if(PathFileExists("firmware\\check_wifibt\\1KHZ4s.wav") != TRUE){
		AfxMessageBox("错误：程序路径下未找到1KHZ4s.wav");
		return 1;
	}
	return 0;
}

void software_check_by_with_wifibt::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_fs_version.SetWindowTextA("V0.00.00");
	m_kernel_version.SetWindowTextA("");
	m_readSn.SetWindowTextA("");
	pass_green_color = 0;
	m_Epass.SetWindowTextA("");
	m_rec_db.SetWindowTextA("");
	m_rec_sec.SetWindowTextA("0秒");

	m_snRead.SetWindowTextA("Wait");
	m_snRead.SetColor(STATIC_WND_DISABLE);
	m_btnBoot.SetColor(STATIC_WND_DISABLE);
	m_btnWifi.SetColor(STATIC_WND_DISABLE);
	m_btnBt.SetColor(STATIC_WND_DISABLE);
	m_btnDmic.SetColor(STATIC_WND_DISABLE);
	m_btnPA.SetColor(STATIC_WND_DISABLE);
	//m_btnKeyU.SetColor(STATIC_WND_DISABLE);
	//m_btnKeyD.SetColor(STATIC_WND_DISABLE);
	m_btnKeyPower.SetColor(STATIC_WND_DISABLE);
	m_btnAud.SetColor(STATIC_WND_DISABLE);
	m_normalCurrect.SetColor(STATIC_WND_DISABLE);
	m_chargeCurrect.SetColor(STATIC_WND_DISABLE);
	m_sleepCurrect.SetColor(STATIC_WND_DISABLE);
	m_backlightCurrect.SetColor(STATIC_WND_DISABLE);

	m_normalCurrect.SetWindowTextA("");
	m_chargeCurrect.SetWindowTextA("");
	m_sleepCurrect.SetWindowTextA("");
	m_backlightCurrect.SetWindowTextA("");

	//for(int id=IDC_RADIO_MUSIC1; id <= IDC_RADIO_MUSIC6; id++) {
	//	((CButton*)GetDlgItem(id))->SetCheck(0);
	//	((CButton*)GetDlgItem(id))->EnableWindow(FALSE);
	//}
	//audSelect = 6;
	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	((CButton*)GetDlgItem(IDC_BUTTON21))->EnableWindow(FALSE);
	AddEvent2List("#####等待用户输入sn号#####");
}

BOOL software_check_by_with_wifibt::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	CString dir_path;
	dir_path = LOG_PATH_CHECK_by_wifibt;

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);
	CreateDirectory("audio", 0);
	CreateDirectory("audio\\check_by_wifibt", 0);

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

	need_file_ifexit();

	m_editFont.CreatePointFont(700, "宋体");
	m_Epass.SetFont(&m_editFont);

	SetAllItemRestart();

	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;

	current_checkBox.EnableWindow(FALSE);
	current_checkBox.SetCheck(check_by_option_instance.g_current_test);
	sleep_current_checkBox.EnableWindow(FALSE);
	sleep_current_checkBox.SetCheck(check_by_option_instance.g_sleep_current_test);
	backlight_current_checkBox.EnableWindow(FALSE);
	backlight_current_checkBox.SetCheck(check_by_option_instance.g_backlight_current_test);
	if(check_by_option_instance.g_current_test){
		if(!check_by_option_instance.g_sleep_current_test){
			((CButton*)GetDlgItem(IDC_STATIC_sleep))->EnableWindow(FALSE);	
		}
		if(!check_by_option_instance.g_backlight_current_test){
			((CButton*)GetDlgItem(IDC_STATIC_backlight))->EnableWindow(FALSE);	
		}
	}else{
		((CButton*)GetDlgItem(IDC_STATIC_charge))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STATIC_normal))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STATIC_sleep))->EnableWindow(FALSE);	
		((CButton*)GetDlgItem(IDC_STATIC_backlight))->EnableWindow(FALSE);	
	}
	((CButton*)GetDlgItem(IDC_STATIC_rec))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_STATIC_db))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_AUDIO_SEC))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_AUDIO_SEC2))->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void software_check_by_with_wifibt::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	SetAllItemRestart();
}
int software_check_by_with_wifibt::kill_adbd(void *param)
{
	software_check_by_with_wifibt *p = (software_check_by_with_wifibt*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " shell killall adbd";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	return 0;
}
void software_check_by_with_wifibt::OnBnClickedButton21()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::kill_adbd, this, 0, NULL);
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_check_by_with_wifibt::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	UpdateData(TRUE);
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
			}else{
				memset((void*)sSetSN, 0, sizeof(sSetSN));
				memcpy((void*)sSetSN, sText, 18);
				((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::WorkerThreadProc, this, 0, NULL);
				AddEvent2List("sn号输入成功，等待上电");
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
		}else{
			memset((void*)sSetSN, 0, sizeof(sSetSN));
			memcpy((void*)sSetSN, sText, 19);
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_check_by_with_wifibt::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn号输入成功，等待上电");
		}

	}
	// TODO:  在此添加控件通知处理程序代码
}

BOOL software_check_by_with_wifibt::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH software_check_by_with_wifibt::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC10)//如果是静态编辑框
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