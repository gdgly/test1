// software_air_by.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_air_by.h"
#include "afxdialogex.h"


// software_air_by �Ի���

IMPLEMENT_DYNAMIC(software_air_by, CDialogEx)

software_air_by::software_air_by(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_air_by::IDD, pParent)
{

}

software_air_by::~software_air_by()
{
}

void software_air_by::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_SN, m_EditSn);
	DDX_Control(pDX, IDC_STATIC6, m_readSn);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_kernel_version);
	DDX_Control(pDX, IDC_EDIT_FIRMVER, m_fs_version);
	DDX_Control(pDX, IDC_wifi_name, m_wifihot_name);
	DDX_Control(pDX, IDC_wifi_status, m_wifihot_status);
	DDX_Control(pDX, IDC_AUDIO_DB_RIGHT, m_rec_right_db);
	DDX_Control(pDX, IDC_AUDIO_DB_LEFT, m_rec_left_db);
	DDX_Control(pDX, IDC_STATIC10, m_Epass);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
	DDX_Control(pDX, IDC_LIST1, list_audio);
	DDX_Control(pDX, IDC_STC_BOOT, m_btnBoot);
	DDX_Control(pDX, IDC_STC_SN_READ, m_snRead);
	DDX_Control(pDX, IDC_STC_TIME1, m_fstime);
	DDX_Control(pDX, IDC_STC_TIME2, m_sctime);
	DDX_Control(pDX, IDC_AUDIO_DB_RIGHT2, m_rec_right_db2);
	DDX_Control(pDX, IDC_AUDIO_DB_LEFT2, m_rec_left_db2);
	DDX_Control(pDX, IDC_RECPLAY_SECOND, m_recplay_sec);
}

BEGIN_MESSAGE_MAP(software_air_by, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_air_by::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON13, &software_air_by::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_SET_SN, &software_air_by::OnEnChangeSetSn)
	ON_BN_CLICKED(IDC_BUTTON20, &software_air_by::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON2, &software_air_by::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &software_air_by::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON17, &software_air_by::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON14, &software_air_by::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &software_air_by::OnBnClickedButton15)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void software_air_by::AddEvent2List(CString sText)
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
void software_air_by::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	air_by_option_instance = g_p->air_by_option_instance;
}
// software_air_by ��Ϣ�������
enum {
	BY_CHECK_INIT_FLAG_SN = 0,
	BY_CHECK_INIT_FLAG_AUDIO_REC,
	BY_CHECK_INIT_FLAG_USBERR,
};
enum {
	BY_AIR_WAIT_WIFI=0,
	BY_AIR_DEVICES,
	BY_AIR_CREATE_SN,
	BY_AIR_WAIT_SN,
	BY_AIR_CREATE_RECORD,
	BY_AIR_START_RECORD,
	BY_AIR_SCUCESS,
};
enum {
	BY_AIR_CONNECT_USB=1,
	BY_AIR_CONNECT_WIFI,
	BY_AIR_CONNECT_UNKNOW,
};

void software_air_by::init_part_process(unsigned int flag)
{
	CString sText;
	if(flag & (1<<BY_CHECK_INIT_FLAG_SN)){
		sText.Format("%s",fs_version);
		m_fs_version.SetWindowTextA(sText);
		sText.Format("%s",kernel_version);
		m_kernel_version.SetWindowTextA(sText);
		sText.Format("%s",sn_read);
		m_readSn.SetWindowTextA(sText);
		m_snRead.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_AUDIO_REC)){
		AddEvent2List("¼�����! ");
		if(test_times == 1){
			m_fstime.SetColor(STATIC_WND_SUCCESS);
		}else if(test_times == 2){
			m_sctime.SetColor(STATIC_WND_SUCCESS);
		}
	}
}

void software_air_by::init_err_process(unsigned int flag)
{
	CString sText;
	if(flag & (1<<BY_CHECK_INIT_FLAG_USBERR)){
		AddEvent2List("adbʧȥ����! ");
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_SN)){
		sText.Format("%s",sn_read);
		m_readSn.SetWindowTextA(sText);
		AddEvent2List("sn�ż��ʧ��! ");
		m_snRead.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_AUDIO_REC)){
		AddEvent2List("¼�����ʧ��! ");
		if(test_times == 1){
			m_fstime.SetColor(STATIC_WND_WARNING);
		}else if(test_times == 2){
			m_sctime.SetColor(STATIC_WND_WARNING);
		}
	}
}

LRESULT software_air_by::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	int count;
	float time_ms;
	CString sText;
	msg_by_air_info *pmsg = (msg_by_air_info*)lParam;
	switch(pmsg->msg_type) {
		case BY_AIR_MSG_TYPE_WIFI_MODE:
			AddEvent2List("��ʼ���ԣ�ϵͳ����Ϊwifiģʽ! ");
			break;
		case BY_AIR_MSG_TYPE_USB_MODE:
			AddEvent2List("��ʼ���ԣ�ϵͳ����Ϊusbģʽ! ");
			break;
		case BY_AIR_MSG_TYPE_UNKNOW_MODE:
			AddEvent2List("����δѡ������ģʽ! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_AIR_MSG_TYPE_WIFI_CONNECT_ERR:
			AddEvent2List("wifi���Ӵ���! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_AIR_MSG_TYPE_WIFI_GET_NAME:
			AddEvent2List("�������豸�ȵ㣡 ");
			sText.Format("%s",sSetSN);
			m_wifihot_name.SetWindowTextA(sText);
			m_wifihot_status.SetWindowTextA("������..");
			break;
		case BY_AIR_MSG_TYPE_WIFI_CONNECT_SUC:
			AddEvent2List("wifi���ӳɹ�! ");
			m_wifihot_status.SetWindowTextA("������");
			break;
		case BY_AIR_MSG_TYPE_WIFI_CONNECT_TIMEOUT:
			AddEvent2List("wifi���ӳ�ʱ! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_AIR_MSG_TYPE_WIFI_ADB_TIMEOUT:
			AddEvent2List("wifiģʽ��adb���ӳ�ʱ! ");
			m_btnBoot.SetColor(STATIC_WND_WARNING);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_AIR_MSG_TYPE_USB_ADB_TIMEOUT:
			AddEvent2List("usbģʽ��adb���ӳ�ʱ! ");
			m_btnBoot.SetColor(STATIC_WND_WARNING);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_AIR_MSG_TYPE_ADB_SEC:
			AddEvent2List("adb���ӳɹ�! ");
			m_btnBoot.SetColor(STATIC_WND_SUCCESS);
			((CButton*)GetDlgItem(IDC_BUTTON20))->EnableWindow(TRUE);
			break;
		case BY_AIR_MSG_TYPE_PART_ERR:
			init_err_process(pmsg->data1);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_AIR_MSG_TYPE_PART_SUC:
			init_part_process(pmsg->data1);
			break;
		case BY_AIR_MSG_TYPE_RECORD_DB_VALUE:
			if(test_times == 1){
				if(pmsg->data1 == 1){
					sText.Format("%d",rec_aud_db_left);
					m_rec_left_db.SetWindowTextA(sText);
					sText = rec_left_file_path;
					count = list_audio.GetCount();
					list_audio.InsertString(count, sText);
					list_audio.SetCurSel(count);
				}else if(pmsg->data1 == 2){
					sText.Format("%d",rec_aud_db_right);
					m_rec_right_db.SetWindowTextA(sText);
					sText = rec_right_file_path;
					count = list_audio.GetCount();
					list_audio.InsertString(count, sText);
					list_audio.SetCurSel(count);
				}
			}else{
				if(pmsg->data1 == 1){
					sText.Format("%d",rec_aud_db_left);
					m_rec_left_db2.SetWindowTextA(sText);
					sText = rec_left_file_path;
					count = list_audio.GetCount();
					list_audio.InsertString(count, sText);
					list_audio.SetCurSel(count);
				}else if(pmsg->data1 == 2){
					sText.Format("%d",rec_aud_db_right);
					m_rec_right_db2.SetWindowTextA(sText);
					sText = rec_right_file_path;
					count = list_audio.GetCount();
					list_audio.InsertString(count, sText);
					list_audio.SetCurSel(count);
				}			
			}
			break;
		case BY_AIR_MSG_TYPE_RECORD_START:
			AddEvent2List("��ʼ¼����������! ");
			break;
		case BY_AIR_MSG_TYPE_RECORD_TIME:
			time_ms = (float)pmsg->data1;
			time_ms = time_ms/1000;
			sText.Format("%f",time_ms);
			sText = sText.Left(3);
			sText += "��";
			m_recplay_sec.SetWindowTextA(sText);
			break;
		case BY_AIR_MSG_TYPE_ALL_SEC:
			if(test_times == 1){
				AddEvent2List("�״β�����ɣ���׺�����λ���Եڶ���! ");
				memcpy(last_sSetSN, sSetSN, sizeof(sSetSN));
				test_times = 2;
			}else if(test_times == 2){
				AddEvent2List("������ɣ��ȴ��û���λ! ");
				pass_green_color = 1;
				m_Epass.SetWindowTextA("PASS");
				Invalidate();
				test_times = 1;
			}
			break;
		default:
			break;
	}
	delete pmsg;
	return 0;
}

CString ping_test(CString ip);
int software_air_by::WorkerThreadProc(void *param)
{
	DWORD dwStart, dwnow, dwsav; 
	software_air_by *p = (software_air_by*)param;
	CString sText;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CTime tNow = CTime::GetCurrentTime();
	CTime tsav = CTime::GetCurrentTime();
	msg_by_air_info *msg_p;
	p->init_flag = 0;
	p->init_err_flag = 0;
	unsigned int init_flag_sav;
	unsigned int test_times_sav = p->test_times;
	int status;
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
	if(p->air_by_option_instance.g_connect_type == BY_AIR_CONNECT_USB){
		msg_p = new msg_by_air_info;
		msg_p->msg_type = BY_AIR_MSG_TYPE_USB_MODE;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		status = BY_AIR_DEVICES;
	}else if(p->air_by_option_instance.g_connect_type == BY_AIR_CONNECT_WIFI){
		sText.Format("%s",p->sSetSN);
		p->target = sText;
		p->targetKey = p->air_by_option_instance.g_wifihot_passwd;
		if(p->SearchNet()!=true){
			msg_p = new msg_by_air_info;
			msg_p->msg_type = BY_AIR_MSG_TYPE_WIFI_CONNECT_ERR;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			goto out;
		}
		CString netsh_cmd;
		netsh_cmd = "netsh interface ip set address ";
		netsh_cmd += p->air_by_option_instance.g_local_net_name;
		netsh_cmd += " static ";
		netsh_cmd += p->air_by_option_instance.g_local_ip;
		netsh_cmd += " 255.255.255.0";
		//system(netsh_cmd);
		WinExec(netsh_cmd,SW_HIDE);
		//system("netsh interface ip set address ������������ static 192.168.1.23 255.255.255.0");
		status = BY_AIR_WAIT_WIFI;
		msg_p = new msg_by_air_info;
		msg_p->msg_type = BY_AIR_MSG_TYPE_WIFI_MODE;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
	}else{
		msg_p = new msg_by_air_info;
		msg_p->msg_type = BY_AIR_MSG_TYPE_UNKNOW_MODE;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		AfxMessageBox("δѡ��ϵͳ���ӷ�ʽ�� ");
		goto out;
	}
	while(1){
		tNow = CTime::GetCurrentTime();
		switch (status){
			case BY_AIR_WAIT_WIFI:
				if(p->get_wifi_status() && p->get_connect_msg()){
					int ping_right_times = 0;
					for(int i=0; i<4; i++){
						response = ping_test(p->air_by_option_instance.g_wifihot_ip);
						if(response.Find("Reply from") >= 0 &&
							response.Find("bytes=") >= 0 &&
							response.Find("time=") >= 0 &&
							response.Find("TTL=") >= 0){
							ping_right_times ++;
						}
					}
					if(ping_right_times >= 4){
						tsav = tNow;
						status = BY_AIR_DEVICES;
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_WIFI_CONNECT_SUC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);						
					}
				}
				if(tNow.GetTime() - tsav.GetTime() > p->air_by_option_instance.g_wifihot_timeout){
					msg_p = new msg_by_air_info;
					msg_p->msg_type = BY_AIR_MSG_TYPE_WIFI_CONNECT_TIMEOUT;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				break;
			case BY_AIR_DEVICES:
				if(p->air_by_option_instance.g_connect_type == BY_AIR_CONNECT_WIFI){
					if(tNow.GetTime() - tsav.GetTime() > p->air_by_option_instance.g_wifihot_adb_timeout){
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_WIFI_ADB_TIMEOUT;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
					cmd_param = " connect ";
					cmd_param += p->air_by_option_instance.g_wifihot_ip;
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->process_response_connect(response)){
						Sleep(1000);
					}else{
						cmd_param = " shell /etc/init.d/S50servermanager stop";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_air_info;
							msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo on > /sys/devices/platform/jz-fb/debug/power_updown";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_air_info;
							msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo 1 > /sys/class/backlight/gpio-backlight.0/brightness";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_air_info;
							msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						status = BY_AIR_CREATE_SN;
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_ADB_SEC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					}
				}else{
					if(tNow.GetTime() - tsav.GetTime() > p->air_by_option_instance.g_usb_timeout){
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_USB_ADB_TIMEOUT;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
					cmd_param = " devices";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->process_response_devices(response)){
						Sleep(1000);
					}else{
						cmd_param = " shell /etc/init.d/S50servermanager stop";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_air_info;
							msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo on > /sys/devices/platform/jz-fb/debug/power_updown";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_air_info;
							msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo 1 > /sys/class/backlight/gpio-backlight.0/brightness";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_air_info;
							msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						status = BY_AIR_CREATE_SN;
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_ADB_SEC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					}
				}
				break;
			case BY_AIR_CREATE_SN:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_by::create_app_sn_init, param, 0, NULL);
				status = BY_AIR_WAIT_SN;
				break;
			case BY_AIR_WAIT_SN:
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					msg_p = new msg_by_air_info;
					msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if( (init_flag_sav & (1<<BY_CHECK_INIT_FLAG_SN)) == 1<<BY_CHECK_INIT_FLAG_SN ){
					msg_p = new msg_by_air_info;
					msg_p->msg_type = BY_AIR_MSG_TYPE_PART_SUC;
					msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_SN;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					status = BY_AIR_CREATE_RECORD;
				}
				break;
			case BY_AIR_CREATE_RECORD:
				p->PlaysoundThreadProc_exit = 1;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_by::PlaysoundThreadProc, param, 0, NULL);
				Sleep(20);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_by::create_app_start_record, param, 0, NULL);
				dwStart = GetTickCount(); 
				dwnow = GetTickCount(); 
				dwsav = dwnow;
				msg_p = new msg_by_air_info;
				msg_p->msg_type = BY_AIR_MSG_TYPE_RECORD_START;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				status = BY_AIR_START_RECORD;
				break;
			case BY_AIR_START_RECORD:
				dwnow = GetTickCount(); 
				if(dwnow - dwsav > 100 &&
					dwnow - dwStart < p->air_by_option_instance.g_record_time_sec * 1000 + 100){
					msg_p = new msg_by_air_info;
					msg_p->msg_type = BY_AIR_MSG_TYPE_RECORD_TIME;
					msg_p->data1 = dwnow - dwStart;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					dwsav = dwnow;
				}
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					::waveOutSetVolume(0, 0);
					p->PlaysoundThreadProc_exit = 0;
					msg_p = new msg_by_air_info;
					msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if( (init_flag_sav & (1<<BY_CHECK_INIT_FLAG_AUDIO_REC)) ){
					::waveOutSetVolume(0, 0);
					p->PlaysoundThreadProc_exit = 0;
					if(	abs((int)p->rec_aud_db_left - (int)p->rec_aud_db_right) >= (int)p->air_by_option_instance.g_record_subdb){
						status = BY_AIR_SCUCESS;
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_PART_SUC;
						msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					}else{
						msg_p = new msg_by_air_info;
						msg_p->msg_type = BY_AIR_MSG_TYPE_PART_ERR;
						msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						if(p->test_times == 2){
							p->test_times = 1;
						}
						goto out;
					}
				}
				break;
			case BY_AIR_SCUCESS:
				msg_p = new msg_by_air_info;
				msg_p->msg_type = BY_AIR_MSG_TYPE_ALL_SEC;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				goto out;
				break;
			default:
				break;
		}
	}

out:
	if( (status == BY_AIR_SCUCESS && test_times_sav == 2) ||
		(status != BY_AIR_SCUCESS && status >= BY_AIR_DEVICES) ){
		cmd_param = " shell reboot";
		response = p->ExecuteCmd(cmd_str, cmd_param);
	}
	if(p->air_by_option_instance.g_connect_type == BY_AIR_CONNECT_WIFI){
		p->WLANDisconnect();
		p->CloseWLANService();	
	}
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
	return 0;
}

int software_air_by::create_app_sn_init(void *param)
{
	software_air_by *p = (software_air_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/total/sn_init /tmp";
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
	int length=response.Delete(0,n); //�ҳ�""sn_init OK\r\n""�ұߵ��ַ���λ��
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

extern int getAvgPcmDB(INT16  *pcmdata,  long nmemb);
int software_air_by::create_app_start_record(void *param)
{
	software_air_by *p = (software_air_by*)param;
	CString sText;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CFile record_file;
	msg_by_air_info *msg_p;
	
	cmd_param = " push firmware/total/start_play /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/start_play";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	
	sText.Format("%d",p->air_by_option_instance.g_record_time_sec);
	cmd_param = " shell /tmp/start_play 3 ";//3��ʼ¼���� ����Ϊ¼��ʱ��
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	
	int n=response.Find("RECORD OK");

	CTime tNow = CTime::GetCurrentTime();
	sText.Format("audio/air_by/%s_%02d%02d%02d_left_%d.pcm", p->sn_read,
					tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), p->test_times);
	p->rec_left_file_path = sText;
	cmd_param = " pull /tmp/left.pcm ";
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	
	sText.Format("audio/air_by/%s_%02d%02d%02d_right_%d.pcm", p->sn_read,
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond(), p->test_times);
	p->rec_right_file_path = sText;
	cmd_param = " pull /tmp/right.pcm ";
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	if(n<0){
		AfxMessageBox("¼����������ʧ�ܣ�");
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
		return -1;
	}else{
		int g_size;
		char* g_buf;
		if(record_file.Open(p->rec_left_file_path, CFile::modeRead) == TRUE) {
			record_file.SeekToBegin();
			g_size = (int)record_file.GetLength();
			g_buf = (char*)malloc(g_size);
			record_file.Read(g_buf,g_size);
			record_file.Close();
			p->rec_aud_db_left = getAvgPcmDB((INT16*)g_buf, g_size / 2);
			free(g_buf);
			msg_p = new msg_by_air_info;
			msg_p->msg_type = BY_AIR_MSG_TYPE_RECORD_DB_VALUE;
			msg_p->data1 = 1;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		}
		if(record_file.Open(p->rec_right_file_path, CFile::modeRead) == TRUE) {
			record_file.SeekToBegin();
			g_size = (int)record_file.GetLength();
			g_buf = (char*)malloc(g_size);
			record_file.Read(g_buf,g_size);
			record_file.Close();
			p->rec_aud_db_right = getAvgPcmDB((INT16*)g_buf, g_size / 2);
			free(g_buf);
			msg_p = new msg_by_air_info;
			msg_p->msg_type = BY_AIR_MSG_TYPE_RECORD_DB_VALUE;
			msg_p->data1 = 2;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		}
	}
	p->init_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
	return 0;
}

int software_air_by::PlaysoundThreadProc(void *param)
{
	software_air_by *p = (software_air_by*)param;
	UINT fNo;
	while(p->PlaysoundThreadProc_exit) {
		::waveOutSetVolume(0, 0xFFFFFFFF);
		fNo = IDR_WAVE12;
		PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);
	}
	return 0;
}
int software_air_by::process_response_connect(CString response)
{
	CString cut_str;
	int n=response.Find("connected to");
	if(n<0){
		return -1;
	}
	int length=response.Delete(0,n); //�ҳ�"List of devices attached"�ұߵ��ַ���λ��
	cut_str = response.Right(length - sizeof("connected to"));   
	if(cut_str.Find(":5555")<0){
		return -1;
	}
	return 0;
}
int software_air_by::process_response_devices(CString response)
{
	CString cut_str;
	int n=response.Find("List of devices attached");
	if(n<0){
		return -1;
	}
	int length=response.Delete(0,n); //�ҳ�"List of devices attached"�ұߵ��ַ���λ��
	cut_str = response.Right(length - sizeof("List of devices attached"));   
	if(cut_str.Find("device")<0){
		return -1;
	}
	return 0;
}
int software_air_by::if_response_err(CString response)
{
	if(response.Find("error:") >= 0){
		init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("adb�豸ʧȥ����");
		return -1;
	}
	if(response.Find("error: device not found") < 0 &&
		response.Find("error: protocol fault") < 0){
		return 0;
	}else{
		init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("adb�豸ʧȥ����");
		return -1;
	}
}
TCHAR* software_air_by::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
CString software_air_by::ExecuteCmd(CString cmd_name, CString cmd_param)
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
	
    PROCESS_INFORMATION pi;
	//TRACE("#############################pi:0x%x\r\n",&pi);
	STARTUPINFO si={0};
    si.cb = sizeof( STARTUPINFO ); //�ṹ���С 
    GetStartupInfo( &si );
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
bool software_air_by::OpenWLANService()
{
	dwResult = WlanOpenHandle(1    //�ͻ���֧�ֵ�WLAN API����߰汾
			, NULL, &dwCurVersion    //ָ����λỰ�н���ʹ�õİ汾
			, &hClient    //ָ���ͻ�������λỰ��ʹ�õľ������������ᴩ�����Ự����������ʹ�� ��
			);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
            case ERROR_INVALID_PARAMETER:    //����һ����Ϊ�ջ��������Ϊ��
                TRACE(L"Para is NULL\n"); 
                break;
            case ERROR_NOT_ENOUGH_MEMORY:    //û���㹻���ڴ�ռ�
                TRACE(L"Failed to allocate memory \n");
                break; 
            case ERROR_REMOTE_SESSION_LIMIT_EXCEEDED:    //�����������޶��ľ������
                TRACE(L"Server is Busy\n");
                break;
            default:
                dwResult = GetLastError();
                TRACE(L"WlanOpenHandle Fail��%wd\n", dwResult);
                break;
        }
		return false;
    }
	return true;
}
bool software_air_by::WLANNetwork()
{
	dwResult = WlanEnumInterfaces(hClient    //�ͻ��˻Ự������� WlanOpenHandle �õ���
		, NULL, &pInterfaceList    //ָ��������������ӿ���Ϣlist�Ľṹ�� PWLAN_INTERFACE_INFO_LIST ��ָ�롣
		);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:    //����һ����Ϊ�գ����������Ϊ��
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_INVALID_HANDLE:    //��Ч�ľ��
				TRACE(L"Failed to INVALID HANDLE \n");
				break; 
			case ERROR_NOT_ENOUGH_MEMORY:    //û���㹻���ڴ�ռ�
				TRACE(L"Failed to allocate memory \n");
				break; 
			default:
				dwResult = GetLastError();
				TRACE(L"WlanEnumInterfaces Fail�� %wd\n", dwResult);
				break;
        }
		return false;
    }
	TRACE(L"WlanEnumInterfaces Number %wd\n", pInterfaceList->dwNumberOfItems);    //�����������
	return true;
}
LPCWSTR software_air_by::StringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length()-1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

bool software_air_by::SetProfile()
{ 
	string szProfileXML("");  //Profile XML��
	LPCWSTR wscProfileXML = NULL;
	/*��ϲ���XML����*/ 
    string szTemp("");
	// char p[1024];
    /*ͷ*/
    szProfileXML += string("<?xml version=\"1.0\"?><WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\"><name>");
    /*name��һ����SSID��ͬ*/
	szTemp = string((char*)pNet->dot11Ssid.ucSSID);    //�����ļ���
    szProfileXML += szTemp;
    /*SSIDConfig*/
    szProfileXML += string("</name><SSIDConfig><SSID><name>");
	szProfileXML += szTemp;    //���������������Ӧ��SSID
	szProfileXML += string("</name></SSID></SSIDConfig>");
	/*connectionType*/
	szProfileXML += string("<connectionType>");
	switch(pNet->dot11BssType){    //��������
        case dot11_BSS_type_infrastructure:
            szProfileXML += "ESS";
            break;
        case dot11_BSS_type_independent:
            szProfileXML += "IBSS";
            break;
        case dot11_BSS_type_any:
            szProfileXML += "Any";
            break;
        default:
            TRACE(L"Unknown BSS type");
            return false;
    }
	/*MSM*/
	szProfileXML += string("</connectionType><MSM><security><authEncryption><authentication>");
	switch( pNet->dot11DefaultAuthAlgorithm){    //������ܷ�ʽ
        case DOT11_AUTH_ALGO_80211_OPEN:
            szProfileXML += "open";
			TRACE(L"Open 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
			szProfileXML += "shared";
            TRACE(L"Shared 802.11 authentication");
            break;
        case DOT11_AUTH_ALGO_WPA:
			szProfileXML += "WPA";
            TRACE(L"WPA-Enterprise 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_WPA_PSK:
			szProfileXML += "WPAPSK";
            TRACE(L"WPA-Personal 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_WPA_NONE:
			szProfileXML += "none";
            TRACE(L"WPA-NONE,not exist in MSDN\n");
            break;
        case DOT11_AUTH_ALGO_RSNA:
			szProfileXML += "WPA2";
            TRACE(L"WPA2-Enterprise 802.11 authentication\n");
            break;
        case DOT11_AUTH_ALGO_RSNA_PSK:
			szProfileXML += "WPA2PSK";
            TRACE(L"WPA2-Personal 802.11 authentication\n");
            break;
        default:
			TRACE(L"Unknown authentication");
   			return false;
    }
	szProfileXML += string("</authentication><encryption>");
	/*sprintf(p, "%d", pNet->dot11DefaultCipherAlgorithm);
	szProfileXML += string(p);*/
	switch(pNet->dot11DefaultCipherAlgorithm){
        case DOT11_CIPHER_ALGO_NONE:
            szProfileXML += "none";
            break;
        case DOT11_CIPHER_ALGO_WEP40:
            szProfileXML += "WEP";
            break;
        case DOT11_CIPHER_ALGO_TKIP:
            szProfileXML += "TKIP";
            break;
        case DOT11_CIPHER_ALGO_CCMP:
            szProfileXML += "AES";
            break;
        case DOT11_CIPHER_ALGO_WEP104:
            szProfileXML += "WEP";
            break;        
        case DOT11_CIPHER_ALGO_WEP:
            szProfileXML += "WEP";
            break;
		case DOT11_CIPHER_ALGO_WPA_USE_GROUP:
			TRACE(L"USE-GROUP not exist in MSDN");
        default:
            TRACE(L"Unknown encryption");
			return false;
    }
	//szProfileXML += string	("</encryption><useOneX>false</useOneX></authEncryption></security></MSM>");
    //������ܷ�ʽΪWEP��keyType�����ΪnetworkKey
	szProfileXML += string	("</encryption></authEncryption><sharedKey><keyType>passPhrase</keyType><protected>false</protected><keyMaterial>");
	szProfileXML += targetKey;//;
	/*β*/
	szProfileXML += string("</keyMaterial></sharedKey></security></MSM></WLANProfile>");
	/*XML����ת����˫�ֽ�*/
	wscProfileXML = StringToLPCWSTR(szProfileXML);
	if(NULL == wscProfileXML){
		TRACE(L"Change wscProfileXML fail\n");
		return false;
	}
 	/*�����������*/
	DWORD dwReasonCode;
    dwResult = WlanSetProfile(hClient, &pInterface->InterfaceGuid, 
			0x00    //�����û��ļ�����
		, wscProfileXML    //�û��ļ�����
		, NULL    //��XP SP1��SP2�±���ΪNULL
		, FALSE    //�Ƿ񸲸��Ѵ��ڵ��û��ļ�
		, NULL    //��XP SP1��SP2�±���ΪNULL
		, &dwReasonCode);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:    //����һ�������ġ���Ϊ�ջ���XP SP1��SP2�²�������Ϊ0
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_NO_MATCH:    //����ӿڲ�֧�ֵļ�������
				TRACE(L"NIC NOT SUPPORT\n");
				break; 
			case ERROR_NOT_ENOUGH_MEMORY:    //û���㹻���ڴ�ռ�
				TRACE(L"Failed to allocate memory \n");
				break;
			case ERROR_BAD_PROFILE:    //�û��ļ���ʽ����
				TRACE(L"The profile specified by strProfileXml is not valid \n");
				break;
			case ERROR_ALREADY_EXISTS:    //���õ��û��ļ��Ѵ���
				TRACE(L"strProfileXml specifies a network that already exists \n");
				break;
			case ERROR_ACCESS_DENIED:    //�û�û��Ȩ�������û��ļ�
				TRACE(L"The caller does not set the profile. \n");
				break;
			default:
				dwResult = GetLastError();
				TRACE(L"WlanSetProfile Fail�� %wd\n", dwResult); 
				break;
        }
		if (dwResult != 183){
			return false;
		}
    }
	return true;
}

bool software_air_by::WLANConnect()
{
	pConnPara = (PWLAN_CONNECTION_PARAMETERS)calloc(1, sizeof(WLAN_CONNECTION_PARAMETERS));
    /*�����������Ӳ���*/
    if(NULL == pConnPara){
        TRACE(L"pConnPara fail\n");
    }
	pConnPara->wlanConnectionMode = (WLAN_CONNECTION_MODE)0; //XP SP2,SP3 must be 0
	pConnPara->strProfile = StringToLPCWSTR(target);    //ָ�����û��ļ�
	pConnPara->pDot11Ssid = &pNet->dot11Ssid;    //ָ����SSID
	pConnPara->pDesiredBssidList = NULL; //XP SP2,SP3 must be NULL
	pConnPara->dot11BssType = pNet->dot11BssType;    //��������
	pConnPara->dwFlags = 0x00000000; //XP SP2,SP3 must be 0
   
    dwResult = WlanConnect(hClient    //�ͻ��˾��
		, &pInterface->InterfaceGuid    //����ʹ�õĽӿڵ�GUID
		, pConnPara    //ָ��ṹ�� WLAN_CONNECTION_PARAMETERS ������ָ�����������ͣ�ģʽ������ſ���SSID ������������
		, NULL);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_INVALID_HANDLE:
				TRACE(L"Failed to INVALID HANDLE \n");
				break; 
			case ERROR_ACCESS_DENIED:
				TRACE(L"The caller does not have sufficient permissions. \n");
				break;
			default:
				dwResult = GetLastError();
				TRACE(L"WlanConnect Fail�� %wd\n", dwResult);
				break;
        }
		return false;
    }
	return true;
}

bool software_air_by::SearchNet()
{   
	int nInterfaceNumber;
	unsigned int nNetNumber;

    if (!OpenWLANService()){
		AfxMessageBox("������ʧ�ܣ�����Ƿ������������");
        return false;
    }
    if (!WLANNetwork()){
		AfxMessageBox("������ʧ�ܣ�����Ƿ������������");
        return false;
    } 
	if ((int)pInterfaceList->dwNumberOfItems == 0){
		AfxMessageBox("δ��⵽������������");
        return false;
    } 
    for(nInterfaceNumber = 0; nInterfaceNumber < (int)pInterfaceList->dwNumberOfItems; nInterfaceNumber++){
        /*��ȡ������Ϣ*/
        pInterface = (PWLAN_INTERFACE_INFO)&pInterfaceList->InterfaceInfo[nInterfaceNumber];
        /*���������Ϣ�б�*/
        dwResult = WlanGetAvailableNetworkList(hClient    //�ͻ��˵ĻỰ���
			, &pInterface->InterfaceGuid    //Ҫ�����Ľӿڵ�GUID
			, 0x00    //����list�з��ص���������ͣ�XP SP2��SP3Ϊ��
			, NULL, &pNetList    //ָ�򷵻صĿ�������� WLAN_AVAILABLE_NETWORK_LIST ��ָ��
			);
        if(ERROR_SUCCESS != dwResult){
            switch (dwResult){
				case ERROR_INVALID_PARAMETER:    //�����Ĳ�Ϊ�գ�����������Ϊ��
					TRACE(L"Para is NULL\n");
					break;
				case ERROR_INVALID_HANDLE:
					TRACE(L"Failed to INVALID HANDLE \n");
					break; 
				case ERROR_NOT_ENOUGH_MEMORY:
					TRACE(L"Failed to allocate memory \n");
					break;
				case ERROR_NDIS_DOT11_POWER_STATE_INVALID:    //�㲥�ر��޷�����
					TRACE(L"The radio for the interface is turned off \n");
					break;
				default:
					dwResult = GetLastError();
					TRACE(L"WlanGetAvailableNetworkList Fail�� %wd\n", dwResult);
					break;
            }
			return false;
        }
        TRACE(L"WlanGetAvailableNetworkList Number %wd\n", pNetList->dwNumberOfItems);    //�����������

        /*��������*/
		bool resultSearch = true;

        for(nNetNumber = 0; nNetNumber < pNetList->dwNumberOfItems; nNetNumber++){
            /*��ȡ������Ϣ��ȥ�ظ�*/
            pNet = (PWLAN_AVAILABLE_NETWORK)&pNetList->Network[nNetNumber];           
			if (strcmp(target.c_str(),(char*)pNet->dot11Ssid.ucSSID) || !resultSearch){
				continue;
			}
			msg_by_air_info *msg_p;
			msg_p = new msg_by_air_info;
			msg_p->msg_type = BY_AIR_MSG_TYPE_WIFI_GET_NAME;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			resultSearch = false;
			if (!SetProfile()){
				AfxMessageBox("��������ʧ�ܣ������sn���Ƿ���ȷ");
				return false;
			}
			if (!WLANConnect()){
				AfxMessageBox("��������ʧ�ܣ����sn�������Ƿ���ȷ");
				return false;
			} else {
				if (NULL != pNetList){
					WlanFreeMemory(pNetList);
					pNetList = NULL;
				}	
				if(NULL != pInterfaceList){
					WlanFreeMemory(pInterfaceList);
					pInterfaceList = NULL;
				}   
				TRACE("SUCESS\n");
				return true;
			}
		}
		if(resultSearch == true){
			AfxMessageBox("δ��⵽��Ӧwifi���磡");
			return false;
		}
        /*�ͷ�������Ϣ�б�*/
        if (NULL != pNetList){
            WlanFreeMemory(pNetList);
            pNetList = NULL;
        }
    }
    /*�ͷ�������Ϣ�б�*/
    if(NULL != pInterfaceList){
        WlanFreeMemory(pInterfaceList);
        pInterfaceList = NULL;
    }   
    TRACE("SUCESS\n");
    return true;
}

bool software_air_by::get_wifi_status()
{
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    PWLAN_INTERFACE_INFO pIfInfo = NULL;
    dwResult = WlanEnumInterfaces(hClient, NULL, &pIfList); 
    if (dwResult != ERROR_SUCCESS)  {
        TRACE(L"WlanEnumInterfaces failed with error: %u\n", dwResult);
        // FormatMessage can be used to find out why the function failed
		return FALSE;
    }
    pIfInfo = (WLAN_INTERFACE_INFO *) &pIfList->InterfaceInfo[0];
    switch (pIfInfo->isState) {
            case wlan_interface_state_not_ready:
                TRACE(L"Not ready\n");
                break;
            case wlan_interface_state_connected:
                TRACE(L"Connected\n");
				return true;
                break;
            case wlan_interface_state_ad_hoc_network_formed:
                TRACE(L"First node in a ad hoc network\n");
                break;
            case wlan_interface_state_disconnecting:
                TRACE(L"Disconnecting\n");
                break;
            case wlan_interface_state_disconnected:
                TRACE(L"Not connected\n");
                break;
            case wlan_interface_state_associating:
                TRACE(L"Attempting to associate with a network\n");
                break;
            case wlan_interface_state_discovering:
                TRACE(L"Auto configuration is discovering settings for the network\n");
                break;
            case wlan_interface_state_authenticating:
                TRACE(L"In process of authenticating\n");
                break;
            default:
                TRACE(L"Unknown state %ld\n", pIfInfo->isState);
                break;
    }
	return false;
}
bool software_air_by::get_connect_msg()
{
	WLAN_CONNECTION_ATTRIBUTES *p_connect_net;
	DWORD                  pdwDataSize;
	WlanQueryInterface(hClient,
		&pInterface->InterfaceGuid,
		wlan_intf_opcode_current_connection,
		0,
		&pdwDataSize,
		(PVOID*)&p_connect_net,
		NULL);
	if(pdwDataSize != sizeof(WLAN_CONNECTION_ATTRIBUTES)){
		return false;
	}
	if(memcmp(sSetSN, p_connect_net->wlanAssociationAttributes.dot11Ssid.ucSSID, 
		p_connect_net->wlanAssociationAttributes.dot11Ssid.uSSIDLength) == 0){
		return true;
	}
	return false;
}
bool software_air_by::WLANDisconnect()
{ 
    dwResult = WlanDisconnect(hClient, &pInterface->InterfaceGuid, NULL);
    if(ERROR_SUCCESS != dwResult){
        switch (dwResult){
			case ERROR_INVALID_PARAMETER:    //����һ����Ϊ��
				TRACE(L"Para is NULL\n");
				break;
			case ERROR_INVALID_HANDLE:
				TRACE(L"Failed to INVALID HANDLE \n");
				break; 
			case ERROR_NOT_ENOUGH_MEMORY:
				TRACE(L"Failed to allocate memory \n");
				break;
			case ERROR_ACCESS_DENIED:
				TRACE(L"The caller does not have sufficient permissions. \n");
				break;
			default:
				dwResult = GetLastError();
				TRACE(L"WlanConnect Fail�� %wd\n", dwResult);
				break;
        }
		//if (pProfileXml != NULL){
		//	WlanFreeMemory(pProfileXml);	//����API�õ��Ĳ�������Ҫʹ��API�ṩ�ĺ���ȥ�ͷ�
		//	pProfileXml = NULL;
		//}
        free(pConnPara);
		return false;
    }
	return true;
}

bool software_air_by::CloseWLANService()
{
	if(NULL != hClient){
        dwResult = WlanCloseHandle(hClient    //Ҫ�رյ����ӵĿͻ��˾��
			, NULL);
        if(ERROR_SUCCESS != dwResult){
            switch (dwResult){
				case ERROR_INVALID_PARAMETER:    //����һΪ�ջ��������Ϊ��
					TRACE(L"Para is NULL\n");
					break;
				case ERROR_INVALID_HANDLE:
					TRACE(L"Failed to INVALID HANDLE \n");
					break; 
				default:
					dwResult = GetLastError();
					TRACE(L"WlanCloseHandle Fail�� %wd\n", dwResult);
					break;
            }
			return false;
        }
        hClient = NULL;
    }
 	return true;
}
void software_air_by::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_fs_version.SetWindowTextA("V0.00.00");
	m_kernel_version.SetWindowTextA("");
	m_readSn.SetWindowTextA("");
	pass_green_color = 0;
	m_Epass.SetWindowTextA("");

	m_wifihot_name.SetWindowTextA("");
	m_wifihot_status.SetWindowTextA("");
	m_snRead.SetColor(STATIC_WND_DISABLE);
	m_btnBoot.SetColor(STATIC_WND_DISABLE);

	m_recplay_sec.SetWindowTextA("0��");
	m_rec_right_db2.SetWindowTextA("");
	m_rec_left_db2.SetWindowTextA("");
	if(test_times == 1){
		m_rec_right_db.SetWindowTextA("");
		m_rec_left_db.SetWindowTextA("");
		m_fstime.SetColor(STATIC_WND_DISABLE);
		m_listEvt.ResetContent();
	}
	m_sctime.SetColor(STATIC_WND_DISABLE);

	m_EditSn.SetFocus();
	((CButton*)GetDlgItem(IDC_BUTTON20))->EnableWindow(FALSE);
	AddEvent2List("#####�ȴ��û�����sn��#####");
}
void software_air_by::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAllItemRestart();
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_air_by::OnEnChangeSetSn()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	UpdateData(TRUE);
	m_EditSn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("��ǰ��ƷSN��(v2)����ȷ");
			}else{
				memset((void*)sSetSN, 0, sizeof(sSetSN));
				memcpy((void*)sSetSN, sText, 18);
				if(test_times ==  2){
					if(memcmp(sSetSN, last_sSetSN, sizeof(last_sSetSN)) != 0){
						test_times = 1;
					}
				}
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_by::WorkerThreadProc, this, 0, NULL);
				AddEvent2List("sn������ɹ���׼������");
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("��ǰ��ƷSN��(v1)����ȷ");
		}else{
			memset((void*)sSetSN, 0, sizeof(sSetSN));
			memcpy((void*)sSetSN, sText, 19);
			if(test_times ==  2){
				if(memcmp(sSetSN, last_sSetSN, sizeof(last_sSetSN)) != 0){
					test_times = 1;
				}
			}
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_by::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn������ɹ���׼������");
		}

	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

int software_air_by::kill_adbd(void *param)
{
	software_air_by *p = (software_air_by*)param;
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
void software_air_by::OnBnClickedButton20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_by::kill_adbd, this, 0, NULL);
}


void software_air_by::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp;
	m_sFileName.Empty();
	int index = list_audio.GetCurSel();
	if(index < 0){
		AfxMessageBox("û�����ֻ���ûѡ��...����ӻ�ѡ��");
		return;
	}
	list_audio.GetText(index,m_sFileName);
	SetDlgItemText(IDC_STATIC_filename, m_sFileName);

	//GetDlgItem(IDC_BUTTON2)->EnableWindow(FALSE);
	if(m_sFileName.Find("g722") >= 0) {
		m_G722Play.PlayFile(m_sFileName, AUD_T_G722);    //0,1,2
		return;
	}
	else if(m_sFileName.Find("pcm") >= 0) {
		m_G722Play.PlayFile(m_sFileName, AUD_T_SRC_STERO);    //0,1,2
		return;
	}
}


void software_air_by::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_sFileName.Find("g722") >= 0) {
		m_G722Play.m_bExitPlay = TRUE;
		m_sFileName.Empty();
		return;
	}
	else if(m_sFileName.Find(".pcm") >= 0) {
		m_G722Play.m_bExitPlay = TRUE;
		m_sFileName.Empty();
		return;
	}
}


void software_air_by::OnBnClickedButton17()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char fliter[]="�����ļ�(*.g722;*.pcm)\0*.g722;*.pcm\0"
		"�����ļ�(*.*)\0*.*\0\0";
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrFilter=fliter;
	if(IDOK==fdlg.DoModal())
	{
		CString path=fdlg.GetPathName();
		CString file=fdlg.GetFileName();
		list_audio.InsertString(list_audio.GetCount(),path);
		list_audio.SetCurSel(list_audio.GetCount()-1);
	}
}


void software_air_by::OnBnClickedButton14()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sText;
	int list_num = list_audio.GetCurSel();
	if(list_num < 0) {
		AfxMessageBox("��ѡ����Ҫɾ������Ƶ");
		return;
	}
	list_audio.DeleteString(list_num);
}


void software_air_by::OnBnClickedButton15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	list_audio.ResetContent();
}


HBRUSH software_air_by::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if(pWnd->GetDlgCtrlID()==IDC_STATIC10)//����Ǿ�̬�༭��
    {
		if(pass_green_color){
			pDC->SetTextColor(RGB(0,255,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}else{
		    pDC->SetTextColor(RGB(255,0,0));//�޸��������ɫ
			pDC->SetBkMode(TRANSPARENT);//������ı������͸����
		}
    }
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
int software_air_by::need_file_ifexit()
{
	if(PathFileExists("thirdapp\\adb\\adb.exe") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�adb.exe");
		return 1;
	}
	if(PathFileExists("firmware\\total\\sn_init") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�sn_init");
		return 1;
	}
	if(PathFileExists("firmware\\total\\start_play") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�sn_init");
		return 1;
	}

	return 0;
}
BOOL software_air_by::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	test_times = 1;
	CString dir_path;
	dir_path = LOG_PATH_AIR_by;

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);
	CreateDirectory("audio", 0);
	CreateDirectory("audio\\air_by", 0);

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

	m_editFont.CreatePointFont(700, "����");
	m_Epass.SetFont(&m_editFont);

	SetAllItemRestart();
	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_air_by::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
