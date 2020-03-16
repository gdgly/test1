// software_total_machine_by.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_total_machine_by.h"
#include "afxdialogex.h"


// software_total_machine_by �Ի���

IMPLEMENT_DYNAMIC(software_total_machine_by, CDialogEx)

software_total_machine_by::software_total_machine_by(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_total_machine_by::IDD, pParent)
{

}

software_total_machine_by::~software_total_machine_by()
{
}

void software_total_machine_by::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_SN, m_EditSn);
	DDX_Control(pDX, IDC_STATIC6, m_readSn);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_kernel_version);
	DDX_Control(pDX, IDC_EDIT_FIRMVER, m_fs_version);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
	DDX_Control(pDX, IDC_STATIC10, m_Epass);
	DDX_Control(pDX, IDC_STC_BOOT, m_btnBoot);
	DDX_Control(pDX, IDC_STC_WIFI, m_btnWifi);
	DDX_Control(pDX, IDC_STC_BT, m_btnBt);
	DDX_Control(pDX, IDC_STC_DMIC, m_btnDmic);
	DDX_Control(pDX, IDC_STC_PA, m_btnPA);
	DDX_Control(pDX, IDC_STC_TKEYUP, m_btnKeyU);
	DDX_Control(pDX, IDC_STC_TKEYDOWN, m_btnKeyD);
	DDX_Control(pDX, IDC_STC_TKEYPOWER, m_btnKeyPower);
	DDX_Control(pDX, IDC_STC_TOUCHSCREEN, m_ts_event);
	DDX_Control(pDX, IDC_LIST1, list_audio);
	DDX_Control(pDX, IDC_TFA9896_VAL, m_calibration);
	DDX_Control(pDX, IDC_AUDIO_SEC3, m_play_db);
	DDX_Control(pDX, IDC_AUDIO_DB_RIGHT, m_rec_right_db);
	DDX_Control(pDX, IDC_AUDIO_DB_LEFT, m_rec_left_db);
	DDX_Control(pDX, IDC_RECPLAY_SECOND, m_recplay_sec);
	DDX_Control(pDX, IDC_STC_SN_READ, m_snRead);
	DDX_Control(pDX, IDC_wifi_name, m_wifihot_name);
	DDX_Control(pDX, IDC_wifi_status, m_wifihot_status);
	DDX_Control(pDX, IDC_PLAYER_SECOND, m_player_sec);
	DDX_Control(pDX, IDC_bat_power, m_bat_power);
}

BEGIN_MESSAGE_MAP(software_total_machine_by, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_total_machine_by::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON13, &software_total_machine_by::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_SET_SN, &software_total_machine_by::OnEnChangeSetSn)
	ON_BN_CLICKED(IDC_BUTTON2, &software_total_machine_by::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &software_total_machine_by::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON17, &software_total_machine_by::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON14, &software_total_machine_by::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &software_total_machine_by::OnBnClickedButton15)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON20, &software_total_machine_by::OnBnClickedButton20)
END_MESSAGE_MAP()

INT software_total_machine_by::MesPass(int idata)   //ÿһ���Ʒ�����Ժ󣬵��PASS�ϴ�MES
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1;
	msg_by_total_info *msg_p;
	sText.Format("%s",sn_read);
	if(mesIptr->Messtatus == 1){
		if(mesIptr->function_SendChecke(sText) == 0){
		}else{
			AfxMessageBox("mes�����Ϸ�����! ");
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_ERR;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		}
	}

	if(mesIptr->Messtatus == 3){
		msg_p = new msg_by_total_info;
		msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_SUC;
		::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		return 1;
	}else{
		AfxMessageBox("�����ѹ�վ! ");
		msg_p = new msg_by_total_info;
		msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_ERR;
		::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		return 0;
	}
	//CCurlCtrl *mesIptr =  &mesInfo;
	//CString sText,sText1,sData;
	//UINT a;
	//sData.Format("%d",idata);
	//sText.Format("%s",sn_read);
	//if(mesIptr->Messtatus == 1){
	//	if(mesIptr->power_SendDatae(sText) == 0){
	//		sText.Format(_T("mes��������:%s"),mesIptr->line);
	//		sText1.Format(_T("mes�����жϽ��:%d"),mesIptr->Messtatus);
	//		a = sText.Find("}");
	//		if(a >= 0)
	//			sText = sText.Left(a+1);

	//		AddEvent2List(sText);
	//		AddEvent2List(sText1);
	//	}else
	//		AddEvent2List("mes����ʧ�ܣ�");

	//	if(mesIptr->Messtatus == 3){
	//		return 1;
	//	}else{
	//		AfxMessageBox("mes�������ݴ��󣬲����Լ���check-2�� ");
	//		mesIptr->log = 0;
	//		mesIptr->Messtatus = 0;
	//	}
	//}
	//return 0;
}

INT software_total_machine_by::MesSend(int idata,BYTE *ifirver,BYTE *isouver)   //վλ�ϴ��Ժ��ϴ��������
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1,sData,sfirver,Ssouver;
	UINT a;
	sData.Format("%d",idata);
	sfirver.Format("%s",ifirver);
	Ssouver.Format("%s",isouver);
	sText.Format("%s",sn_read);
	if(mesIptr->Messtatus == 3){
		if(mesIptr->power_SendChecke(sText,sData,sfirver,Ssouver) == 0){
			sText.Format(_T("mes��������:%s"),mesIptr->line);
			sText1.Format(_T("mes�����жϽ��:%d"),mesIptr->Messtatus);
			a = sText.Find("}");
			if(a >= 0)
				sText = sText.Left(a+1);

			AddEvent2List(sText);
			AddEvent2List(sText1);
		}else
			AddEvent2List("mes����ʧ�ܣ�");
	}

	AddEvent2List("\r\n");
	if(mesIptr->Messtatus == 7){
		mesIptr->log = 0;
		mesIptr->Messtatus = 0;
		return 1;
	}else{
		AfxMessageBox("mes�������ݴ��󣬲����Լ���check-3�� ");
		mesIptr->log = 0;
		mesIptr->Messtatus = 0;
	}
	return 0;
}
void software_total_machine_by::AddEvent2List(CString sText)
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

void software_total_machine_by::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	total_machine_by_option_instance = g_p->total_machine_by_option_instance;
}
enum {
	BY_CHECK_INIT_FLAG_SN = 0,
	BY_CHECK_INIT_FLAG_WIFI,
	BY_CHECK_INIT_FLAG_BT,
	BY_CHECK_INIT_FLAG_KEYUP,
	BY_CHECK_INIT_FLAG_KEYDOWN,
	BY_CHECK_INIT_FLAG_KEYPOWER,
	BY_CHECK_INIT_FLAG_TS,
	BY_CHECK_INIT_FLAG_CALI,
	BY_CHECK_INIT_FLAG_AUDIO_REC,
	BY_CHECK_INIT_FLAG_AUDIO_PLAY,
	BY_CHECK_INIT_FLAG_BAT_POWER,
	BY_CHECK_INIT_FLAG_USBERR,
};
enum {
	BY_TOTAL_WAIT_WIFI=0,
	BY_TOTAL_DEVICES,
	BY_TOTAL_CREATE_SN_KEY_TS,
	BY_TOTAL_WAIT_SN_KEY_TS,
	BY_TOTAL_CREATE_WIFI_BT,
	BY_TOTAL_WAIT_WIFI_BT,
	BY_TOTAL_CREATE_ONLY_WIFI,
	BY_TOTAL_WAIT_ONLY_WIFI,
	BY_TOTAL_CREATE_RECORD,
	BY_TOTAL_START_RECORD,
	BY_TOTAL_CREATE_PLAY,
	BY_TOTAL_START_PLAY,
	BY_TOTAL_SCUCESS,
};
enum {
	BY_TOTAL_CONNECT_USB=1,
	BY_TOTAL_CONNECT_WIFI,
	BY_TOTAL_CONNECT_UNKNOW,
};
extern int getAvgPcmDB(INT16  *pcmdata,  long nmemb);

void software_total_machine_by::init_part_process(unsigned int flag)
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
	if(flag & (1<<BY_CHECK_INIT_FLAG_WIFI)){
		AddEvent2List("wifi���Գɹ���");
		m_btnWifi.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_BT)){
		m_btnBt.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYUP)){
		m_btnKeyU.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYDOWN)){
		m_btnKeyD.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYPOWER)){
		m_btnKeyPower.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_TS)){
		m_ts_event.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_CALI)){
		sText.Format("%d",calibration_value);
		m_calibration.SetWindowTextA(sText);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_AUDIO_REC)){
		AddEvent2List("¼�����! ");
		m_btnDmic.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY)){
		AddEvent2List("�������! ");
		m_btnPA.SetColor(STATIC_WND_SUCCESS);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_BAT_POWER)){
		AddEvent2List("��������������! ");
	}
}

void software_total_machine_by::init_err_process(unsigned int flag)
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
	if(flag & (1<<BY_CHECK_INIT_FLAG_WIFI)){
		AddEvent2List("wifi���ʧ��! ");
		m_btnWifi.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_BT)){
		AddEvent2List("�������ʧ��! ");
		m_btnBt.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYUP)){
		AddEvent2List("�ϼ����ʧ��! ");
		m_btnKeyU.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYDOWN)){
		AddEvent2List("�¼����ʧ��! ");
		m_btnKeyD.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_KEYPOWER)){
		AddEvent2List("��Դ�����ʧ��! ");
		m_btnKeyPower.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_TS)){
		AddEvent2List("���������ʧ��! ");
		m_ts_event.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_CALI)){
		AddEvent2List("����У׼ֵ����! ");
		sText.Format("%d",calibration_value);
		m_calibration.SetWindowTextA(sText);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_AUDIO_REC)){
		AddEvent2List("¼�����ʧ��! ");
		m_btnDmic.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY)){
		AddEvent2List("�������ʧ��! ");
		m_btnPA.SetColor(STATIC_WND_WARNING);
	}
	if(flag & (1<<BY_CHECK_INIT_FLAG_BAT_POWER)){
		AddEvent2List("�������ʧ��! ");
	}
}

LRESULT software_total_machine_by::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	int count;
	float time_ms;
	CString sText;
	msg_by_total_info *pmsg = (msg_by_total_info*)lParam;
	switch(pmsg->msg_type) {
		case BY_TOTAL_MSG_TYPE_WIFI_MODE:
			AddEvent2List("��ʼ���ԣ�ϵͳ����Ϊwifiģʽ! ");
			break;
		case BY_TOTAL_MSG_TYPE_USB_MODE:
			AddEvent2List("��ʼ���ԣ�ϵͳ����Ϊusbģʽ! ");
			break;
		case BY_TOTAL_MSG_TYPE_UNKNOW_MODE:
			AddEvent2List("����δѡ������ģʽ! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_TOTAL_MSG_TYPE_WIFI_CONNECT_ERR:
			AddEvent2List("wifi���Ӵ���! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_TOTAL_MSG_TYPE_WIFI_GET_NAME:
			AddEvent2List("�������豸�ȵ㣡 ");
			sText.Format("%s",sSetSN);
			m_wifihot_name.SetWindowTextA(sText);
			m_wifihot_status.SetWindowTextA("������..");
			break;
		case BY_TOTAL_MSG_TYPE_WIFI_CONNECT_SUC:
			m_btnWifi.SetColor(STATIC_WND_SUCCESS);
			AddEvent2List("wifi���ӳɹ�! ");
			m_wifihot_status.SetWindowTextA("������");
			break;
		case BY_TOTAL_MSG_TYPE_WIFI_CONNECT_TIMEOUT:
			AddEvent2List("wifi���ӳ�ʱ! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_TOTAL_MSG_TYPE_WIFI_ADB_TIMEOUT:
			AddEvent2List("wifiģʽ��adb���ӳ�ʱ! ");
			m_btnBoot.SetColor(STATIC_WND_WARNING);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_TOTAL_MSG_TYPE_USB_ADB_TIMEOUT:
			AddEvent2List("usbģʽ��adb���ӳ�ʱ! ");
			m_btnBoot.SetColor(STATIC_WND_WARNING);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_TOTAL_MSG_TYPE_ADB_SEC:
			AddEvent2List("adb���ӳɹ�! ");
			m_btnBoot.SetColor(STATIC_WND_SUCCESS);
			((CButton*)GetDlgItem(IDC_BUTTON20))->EnableWindow(TRUE);
			break;
		case BY_TOTAL_MSG_TYPE_PART_ERR:
			init_err_process(pmsg->data1);
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();	
			break;
		case BY_TOTAL_MSG_TYPE_PART_SUC:
			init_part_process(pmsg->data1);
			break;
		case BY_TOTAL_MSG_TYPE_KEY_TS_SUC:
			AddEvent2List("�������������Գɹ�! ");
			break;
		case BY_TOTAL_MSG_TYPE_WIFI_BT_SUC:
			AddEvent2List("�������Գɹ�! ");
			break;
		case BY_TOTAL_MSG_TYPE_RECORD_DB_VALUE:
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
			break;
		case BY_TOTAL_MSG_TYPE_RECORD_START:
			AddEvent2List("��ʼ¼����������! ");
			break;
		case BY_TOTAL_MSG_TYPE_RECORD_TIME:
			time_ms = (float)pmsg->data1;
			time_ms = time_ms/1000;
			sText.Format("%f",time_ms);
			sText = sText.Left(3);
			sText += "��";
			m_recplay_sec.SetWindowTextA(sText);
			break;
		case BY_TOTAL_MSG_TYPE_PLAY_TIME:
			time_ms = (float)pmsg->data1;
			time_ms = time_ms/1000;
			sText.Format("%f",time_ms);
			sText = sText.Left(3);
			sText += "��";
			m_player_sec.SetWindowTextA(sText);
			break;
		case BY_TOTAL_MSG_TYPE_PLAY_DB_VALUE:
			sText.Format("%d",aud_db);
			m_play_db.SetWindowTextA(sText);
			sText = rec_file_path;
			count = list_audio.GetCount();
			list_audio.InsertString(count, sText);
			list_audio.SetCurSel(count);
			break;
		case BY_TOTAL_MSG_TYPE_ALL_SEC:
			AddEvent2List("������ɣ��ȴ��û���λ! ");
			pass_green_color = 1;
			m_Epass.SetWindowTextA("PASS");
			Invalidate();
			break;
		case BY_TOTAL_MSG_TYPE_BAT_POWER_VALUE:
			sText.Format("%d",read_bat_power);
			m_bat_power.SetWindowTextA(sText);
			break;
		case BY_TOTAL_MSG_TYPE_MES_ERR:
			//AfxMessageBox("mes�����Ϸ����������߻����ѹ�վ! ");
			AddEvent2List("mes�����Ϸ����������߻����ѹ�վ! ");
			pass_green_color = 0;
			m_Epass.SetWindowTextA("FAIL");
			Invalidate();
			break;
		case BY_TOTAL_MSG_TYPE_MES_SUC:
			AddEvent2List("mes���ѷ���! ");
			break;
		case BY_TOTAL_MSG_TYPE_PIXEL_ERR:
			sText.Format("����λ�õ��x=%d,y=%d",pmsg->data1,pmsg->data2);
			AddEvent2List(sText);
			break;
		default:
			break;
	}
	delete pmsg;
	return 0;
}
CString ping_test(CString ip);
//������û�����ʾ��bt֮ǰ��wifi
//#define WIFI_WITH_BT
#define WIFI_AFTER_BT
int software_total_machine_by::WorkerThreadProc(void *param)
{
	DWORD dwStart, dwnow, dwsav; 
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString sText;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CTime tNow = CTime::GetCurrentTime();
	CTime tsav = CTime::GetCurrentTime();
	CCurlCtrl *mesptr = &p->mesInfo;
	int mes_start = 0;
	CTime board_time;
	CString sboard_time;
	unsigned int  init_flag_sav;
	unsigned int init_part_flag = 0;
	p->init_flag = 0;
	p->init_err_flag = 0;
	msg_by_total_info *msg_p;
	unsigned int init_pass_flag,init_wifi_bt_pass_flag,init_only_wifi_pass_flag,status;
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
	if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_iperf, param, 0, NULL);
		init_pass_flag = 1<<BY_CHECK_INIT_FLAG_SN | 1<<BY_CHECK_INIT_FLAG_KEYUP | 
			1<<BY_CHECK_INIT_FLAG_KEYDOWN | 1<<BY_CHECK_INIT_FLAG_KEYPOWER | 
			1<<BY_CHECK_INIT_FLAG_TS | 1<<BY_CHECK_INIT_FLAG_CALI;
#ifdef WIFI_WITH_BT
		init_wifi_bt_pass_flag = 1<<BY_CHECK_INIT_FLAG_WIFI | 1<<BY_CHECK_INIT_FLAG_BT;
#else
#ifndef WIFI_AFTER_BT
		init_pass_flag |= 1<<BY_CHECK_INIT_FLAG_WIFI;
#endif
		init_wifi_bt_pass_flag = 1<<BY_CHECK_INIT_FLAG_BT;
		init_only_wifi_pass_flag = 1<<BY_CHECK_INIT_FLAG_WIFI;
#endif
		status = BY_TOTAL_DEVICES;
		msg_p = new msg_by_total_info;
		msg_p->msg_type = BY_TOTAL_MSG_TYPE_USB_MODE;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
	}else if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_WIFI){
		sText.Format("%s",p->sSetSN);
		p->target = sText;
		p->targetKey = p->total_machine_by_option_instance.g_wifihot_passwd;
		if(p->SearchNet()!=true){
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_CONNECT_ERR;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			goto out;
		}
		CString netsh_cmd;
		netsh_cmd = "netsh interface ip set address ";
		netsh_cmd += p->total_machine_by_option_instance.g_local_net_name;
		netsh_cmd += " static ";
		netsh_cmd += p->total_machine_by_option_instance.g_local_ip;
		netsh_cmd += " 255.255.255.0";
		//system(netsh_cmd);
		WinExec(netsh_cmd,SW_HIDE);
		//system("netsh interface ip set address ������������ static 192.168.1.23 255.255.255.0");
		init_pass_flag = 1<<BY_CHECK_INIT_FLAG_SN | 1<<BY_CHECK_INIT_FLAG_KEYUP | 
			1<<BY_CHECK_INIT_FLAG_KEYDOWN | 1<<BY_CHECK_INIT_FLAG_KEYPOWER | 
			1<<BY_CHECK_INIT_FLAG_TS | 1<<BY_CHECK_INIT_FLAG_CALI;
		init_wifi_bt_pass_flag = 1<<BY_CHECK_INIT_FLAG_BT;
		status = BY_TOTAL_WAIT_WIFI;
		msg_p = new msg_by_total_info;
		msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_MODE;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
	}else{
		AfxMessageBox("δѡ��ϵͳ���ӷ�ʽ�� ");
		msg_p = new msg_by_total_info;
		msg_p->msg_type = BY_TOTAL_MSG_TYPE_UNKNOW_MODE;
		::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		goto out;
	}

	while(1){
		tNow = CTime::GetCurrentTime();
		switch (status){
			case BY_TOTAL_WAIT_WIFI:
				if(p->get_wifi_status() && p->get_connect_msg()){
					int ping_right_times = 0;
					for(int i=0; i<4; i++){
						response = ping_test(p->total_machine_by_option_instance.g_wifihot_ip);
						if(response.Find("Reply from") >= 0 &&
							response.Find("bytes=") >= 0 &&
							response.Find("time=") >= 0 &&
							response.Find("TTL=") >= 0){
							ping_right_times ++;
						}
					}
					if(ping_right_times >= 4){
						tsav = tNow;
						status = BY_TOTAL_DEVICES;
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_CONNECT_SUC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);						
					}
				}
				if(tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifihot_timeout){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_CONNECT_TIMEOUT;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				break;
			case BY_TOTAL_DEVICES:
				if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_WIFI){
					if(tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifihot_adb_timeout){
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_ADB_TIMEOUT;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
					cmd_param = " connect ";
					cmd_param += p->total_machine_by_option_instance.g_wifihot_ip;
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->process_response_connect(response)){
						Sleep(1000);
					}else{
						cmd_param = " shell /etc/init.d/S50servermanager stop";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo on > /sys/devices/platform/jz-fb/debug/power_updown";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo 1 > /sys/class/backlight/gpio-backlight.0/brightness";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						status = BY_TOTAL_CREATE_SN_KEY_TS;
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_ADB_SEC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					}
				}else{
					if(tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_usb_timeout){
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_USB_ADB_TIMEOUT;
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
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo on > /sys/devices/platform/jz-fb/debug/power_updown";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						cmd_param = " shell echo 1 > /sys/class/backlight/gpio-backlight.0/brightness";
						response = p->ExecuteCmd(cmd_str, cmd_param);
						if(p->if_response_err(response)){
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = p->init_err_flag;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
						status = BY_TOTAL_CREATE_SN_KEY_TS;
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_ADB_SEC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					}
				}
				break;
			case BY_TOTAL_CREATE_SN_KEY_TS:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_start_play_init, param, 0, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_sn_init, param, 0, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_key_init, param, 0, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_ts_init, param, 0, NULL);
#ifndef WIFI_WITH_BT
#ifndef WIFI_AFTER_BT
				if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){//wifi���Է��������ֹ������һ����Գ��ִ���
					CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_wifi_trans_init, param, 0, NULL);
					tsav = tNow;
				}
#endif
#endif
				cmd_param = " push firmware/total/1KHZ4s.wav /tmp";//push wav�������ʡʱ��
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				status = BY_TOTAL_WAIT_SN_KEY_TS;
				break;
			case BY_TOTAL_WAIT_SN_KEY_TS:
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if(init_flag_sav != init_part_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_SUC;
					msg_p->data1 = init_flag_sav - init_part_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					init_part_flag = init_flag_sav;
				}else if(init_flag_sav >= init_pass_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_KEY_TS_SUC;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					status = BY_TOTAL_CREATE_WIFI_BT;
				}
#ifndef WIFI_WITH_BT
#ifndef WIFI_AFTER_BT
				if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){
					if( (tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifi_timeout) &&
						((init_flag_sav & BY_CHECK_INIT_FLAG_WIFI) == 0) ){
						TerminateProcess(p->wifi_trans_pi.hProcess,0);
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_CONNECT_TIMEOUT;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
				}
#endif
#endif
				if(p->general_option_instance.g_check_mes){
					if( p->init_flag & (1<<BY_CHECK_INIT_FLAG_SN) &&
						mes_start == 0){
						mes_start = 1;
						if(p->general_option_instance.g_check_mes_type == 1){
							mesptr->clear_flag();
							if(mesptr->function_SendChecks((LPCSTR)p->sn_read) == 0)   {                       //�ϴ�MES���̼��ӿ�    checkģʽ��һ��MES
							}else{
								AfxMessageBox("mes�����Ϸ�����! ");
								msg_p = new msg_by_total_info;
								msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_ERR;
								::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
								goto out;
							}
							if(mesptr->Messtatus  == 1){                                     //mes check�ж�
								msg_p = new msg_by_total_info;
								msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_SUC;
								::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							}else{
								AfxMessageBox("�����ѹ�վ! ");
								msg_p = new msg_by_total_info;
								msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_ERR;
								::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
								goto out;
							}
						}
					}
				}
				break;
			case BY_TOTAL_CREATE_WIFI_BT:
#ifdef WIFI_WITH_BT
				if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){
					CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_wifi_trans_init, param, 0, NULL);
					tsav = tNow;
				}
#endif
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_bt_trans_init, param, 0, NULL);
				status = BY_TOTAL_WAIT_WIFI_BT;
				break;
			case BY_TOTAL_WAIT_WIFI_BT:
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if(init_flag_sav != init_part_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_SUC;
					msg_p->data1 = init_flag_sav - init_part_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					init_part_flag = init_flag_sav;
				}else if( (init_flag_sav & init_wifi_bt_pass_flag) == init_wifi_bt_pass_flag ){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_BT_SUC;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
#ifdef WIFI_WITH_BT
					status = BY_TOTAL_CREATE_RECORD;
				}
				if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){
					if( (tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifi_timeout) &&
						((init_flag_sav & BY_CHECK_INIT_FLAG_WIFI) == 0) ){
						TerminateProcess(p->wifi_trans_pi.hProcess,0);
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_CONNECT_TIMEOUT;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
				}
#else
#ifndef WIFI_AFTER_BT
					status = BY_TOTAL_CREATE_RECORD;
				}
#else
					if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){
						status = BY_TOTAL_CREATE_ONLY_WIFI;
					}else{
						status = BY_TOTAL_CREATE_RECORD;
					}
				}
#endif
#endif
				break;
			case BY_TOTAL_CREATE_ONLY_WIFI:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_wifi_trans_init, param, 0, NULL);
				tsav = tNow;
				status = BY_TOTAL_WAIT_ONLY_WIFI;
				break;
			case BY_TOTAL_WAIT_ONLY_WIFI:
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if(init_flag_sav != init_part_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_SUC;
					msg_p->data1 = init_flag_sav - init_part_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					init_part_flag = init_flag_sav;
				}else if( (init_flag_sav & init_only_wifi_pass_flag) == init_only_wifi_pass_flag ){
					status = BY_TOTAL_CREATE_RECORD;
				}
				if( (tNow.GetTime() - tsav.GetTime() > p->total_machine_by_option_instance.g_wifi_timeout) &&
					((init_flag_sav & BY_CHECK_INIT_FLAG_WIFI) == 0) ){
					TerminateProcess(p->wifi_trans_pi.hProcess,0);
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_CONNECT_TIMEOUT;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				break;
			case BY_TOTAL_CREATE_RECORD:
				p->PlaysoundThreadProc_exit = 1;
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::PlaysoundThreadProc, param, 0, NULL);
				Sleep(20);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_start_record, param, 0, NULL);
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::get_bat_power, param, 0, NULL);
				dwStart = GetTickCount(); 
				dwnow = GetTickCount(); 
				dwsav = dwnow;
				msg_p = new msg_by_total_info;
				msg_p->msg_type = BY_TOTAL_MSG_TYPE_RECORD_START;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				status = BY_TOTAL_START_RECORD;
				break;
			case BY_TOTAL_START_RECORD:
				dwnow = GetTickCount(); 
				if(dwnow - dwsav > 100 &&
					dwnow - dwStart < p->total_machine_by_option_instance.g_record_time_sec * 1000 + 100){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_RECORD_TIME;
					msg_p->data1 = dwnow - dwStart;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					dwsav = dwnow;
				}
				init_flag_sav = p->init_flag;
				if(p->init_err_flag){
					::waveOutSetVolume(0, 0);
					p->PlaysoundThreadProc_exit = 0;
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if( (init_flag_sav & (1<<BY_CHECK_INIT_FLAG_AUDIO_REC)) ){
					::waveOutSetVolume(0, 0);
					p->PlaysoundThreadProc_exit = 0;
					if(p->rec_aud_db_left >= p->total_machine_by_option_instance.g_record_passdb &&
						p->rec_aud_db_right >= p->total_machine_by_option_instance.g_record_passdb &&
						abs((int)p->rec_aud_db_left - (int)p->rec_aud_db_right) <= (int)p->total_machine_by_option_instance.g_record_subdb){
						status = BY_TOTAL_CREATE_PLAY;
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_SUC;
						msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					}else{
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
						msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						goto out;
					}
				}
				break;
			case BY_TOTAL_CREATE_PLAY:
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::create_app_start_play, param, 0, NULL);
				Sleep(20);
				if(p->pc_record()){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				status = BY_TOTAL_START_PLAY;
				break;
			case BY_TOTAL_START_PLAY:
				init_flag_sav = p->init_flag;
				cmd_param = " shell killall aplay";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->init_err_flag){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}else if( (init_flag_sav & (1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY)) &&
						(init_flag_sav & (1<<BY_CHECK_INIT_FLAG_BAT_POWER)) ){
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
						msg_p = new msg_by_total_info;
						msg_p->msg_type = BY_TOTAL_MSG_TYPE_PLAY_DB_VALUE;
						::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						if(p->aud_db > p->total_machine_by_option_instance.g_play_passdb){
							status = BY_TOTAL_SCUCESS;
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_SUC;
							msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY | 1<<BY_CHECK_INIT_FLAG_BAT_POWER;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
						}else{
							msg_p = new msg_by_total_info;
							msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
							msg_p->data1 = 1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY;
							::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							goto out;
						}
					}
				}
				break;
			case BY_TOTAL_SCUCESS:
				board_time = CTime::GetCurrentTime();
				sboard_time.Format("%04d-%02d-%02d %02d:%02d:%02d", 
					board_time.GetYear(), board_time.GetMonth(), board_time.GetDay(),
					board_time.GetHour(), board_time.GetMinute(), board_time.GetSecond() );
				cmd_param = " shell date -s \"";
				cmd_param += sboard_time;
				cmd_param += "\"";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				cmd_param = " shell hwclock -w -u";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					msg_p = new msg_by_total_info;
					msg_p->msg_type = BY_TOTAL_MSG_TYPE_PART_ERR;
					msg_p->data1 = p->init_err_flag;
					::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
					goto out;
				}
				p->AddEvent2List("ϵͳʱ��������ɣ�");
				if(p->general_option_instance.g_check_mes) {
					if(p->general_option_instance.g_check_mes_type == 1){
						if(p->MesPass(p->read_bat_power) == TRUE){
							//if(p->MesSend(p->read_bat_power,p->fs_version,p->kernel_version) != TRUE){
							//	msg_p = new msg_by_total_info;
							//	msg_p->msg_type = BY_TOTAL_MSG_TYPE_MES_ERR;
							//	::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
							//	goto out;
							//}
						}else{
							goto out;
						}
					}else if(p->general_option_instance.g_check_mes_type == 2){
						struct ch_mes_data ch_mes_data; 
						ch_mes_data.data_flag = 4;
						ch_mes_data.iBatVal = p->read_bat_power;
						memcpy(ch_mes_data.firversion, p->fs_version, sizeof(p->fs_version));
						memcpy(ch_mes_data.souversion, p->kernel_version, sizeof(p->kernel_version));
						mesptr->changh_mes_int(p->sn_read, ch_mes_data);
					}
				}
				msg_p = new msg_by_total_info;
				msg_p->msg_type = BY_TOTAL_MSG_TYPE_ALL_SEC;
				::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				goto out;
				break;
			default:
				break;
		}
	}
out:
	if(status >= BY_TOTAL_DEVICES){
		cmd_param = " shell reboot";
		response = p->ExecuteCmd(cmd_str, cmd_param);
	}
	if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_USB){
		TerminateProcess(p->iperf.hProcess,0);
	}else if(p->total_machine_by_option_instance.g_connect_type == BY_TOTAL_CONNECT_WIFI){
		p->WLANDisconnect();
		p->CloseWLANService();	
	}
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
	return 0;
}
DWORD software_total_machine_by::MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
    // ���е���Щ¼��������������¼�������Զ������ģ������ⲻ��Ҫ�Լ�����
	software_total_machine_by *pwnd = (software_total_machine_by*)dwInstance;     // ��ʾ¼���Ĵ���
    PWAVEHDR whd = (PWAVEHDR)dwParam1; // ¼����ͷ�ṹ�����
    switch(uMsg){
		case WIM_OPEN: // ��¼���豸�����ﲻ������
			break;
		case WIM_DATA: // ��ʾ���������������ǽ���Ϣд��һ��pcm�ļ�
			if (pwnd->isGetSound){
				fwrite(whd->lpData, 1, whd->dwBufferLength, pwnd->pf);
				waveInAddBuffer(hWaveIn, whd, sizeof(WAVEHDR));
            }
			break;
		case WIM_CLOSE: // ֹͣ¼��
			waveInStop(hWaveIn);
			waveInReset(hWaveIn);    
			waveInClose(hWaveIn);
			break;
		default:
			break;
   }
   return 0;
}
int software_total_machine_by::pc_record()
{
	msg_by_total_info *msg_p;
	waveFormat.nSamplesPerSec = 16000;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.cbSize = 0;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample * waveFormat.nChannels)/8;
	waveFormat.nAvgBytesPerSec = waveFormat.nBlockAlign * waveFormat.nSamplesPerSec;

	MMRESULT mRet=waveInOpen(&hWaveIn, WAVE_MAPPER, (LPCWAVEFORMATEX)&waveFormat,(DWORD)MicCallBack, (DWORD)this, CALLBACK_FUNCTION);
	if (mRet != MMSYSERR_NOERROR){
		AfxMessageBox("��˷��ʧ�ܣ�����Ƿ���");
		init_err_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY;
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

	waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // ׼����һ���������ݿ�����¼��
	waveInPrepareHeader(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // ׼���ڶ������ݿ�����¼��

	waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr1, sizeof(WAVEHDR)); // ָ���������ݿ�Ϊ¼�����뻺��
	waveInAddBuffer(hWaveIn, (LPWAVEHDR)&wHdr2, sizeof(WAVEHDR)); // ָ���������ݿ�Ϊ¼������
	
	CString sText;
	CTime tNow = CTime::GetCurrentTime();
	sText.Format("audio\\total_by\\play\\Aud_%s_%02d%02d%02d.pcm", sn_read,
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	rec_file_path = sText;
	fopen_s(&pf, sText, "ab+");// һ��Ҫ�Զ���������д�룬����¼������Ƶ���������

	isGetSound = 1;
	DWORD dwStart, dwStop, dwStart_sav; 
	dwStart = GetTickCount(); 
	dwStart_sav = dwStart;
	waveInStart(hWaveIn); // ��ʼ¼��

	while(1){
		dwStop = GetTickCount(); 
		if(dwStop - dwStart > 150){
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_PLAY_TIME;
			msg_p->data1 = dwStop - dwStart_sav;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
			dwStart = dwStop;
		}
		if(dwStop - dwStart_sav > total_machine_by_option_instance.g_play_time_sec*1000){
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_PLAY_TIME;
			msg_p->data1 = dwStop - dwStart_sav;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
			break;
		}
		Sleep(1);
	}
    isGetSound = 0;
	waveInReset(hWaveIn);
	waveInClose(hWaveIn); // ֹͣ¼��
	delete []pBuffer1;
	delete []pBuffer2;
	fclose(pf);
	init_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_PLAY;
	return 0;
}
int software_total_machine_by::get_bat_power(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	p->read_bat_power = 0;
	cmd_param = " shell cat /sys/class/power_supply/sm5007-fuelgauge/capacity";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	StrToIntEx(response, STIF_SUPPORT_HEX, (int*)&p->read_bat_power);
	msg_by_total_info *msg_p;
	msg_p = new msg_by_total_info;
	msg_p->msg_type = BY_TOTAL_MSG_TYPE_BAT_POWER_VALUE;
	::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
	if(p->read_bat_power >= p->total_machine_by_option_instance.g_pass_bat_power){
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_BAT_POWER;
	}else{
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_BAT_POWER;	
	}
	return 0;
}
int software_total_machine_by::PlaysoundThreadProc(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	UINT fNo;
	while(p->PlaysoundThreadProc_exit) {
		::waveOutSetVolume(0, 0xFFFFFFFF);
		fNo = IDR_WAVE12;
		PlaySound((LPCTSTR)fNo, NULL, SND_NODEFAULT|SND_SYNC|SND_RESOURCE);
	}
	return 0;
}
int software_total_machine_by::create_app_wifi_trans_init(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString sText;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/total/iperf /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/iperf";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " push firmware/total/test_wifi /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/test_wifi";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " push firmware/total/wifi_trans /tmp";
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
	cmd_param += p->total_machine_by_option_instance.g_wifi_ssid;
	cmd_param += " ";
	cmd_param += p->total_machine_by_option_instance.g_wifi_passwd;
	cmd_param += " ";
	cmd_param += p->total_machine_by_option_instance.g_wifi_ip;
	sText.Format(" %d",p->total_machine_by_option_instance.g_wifi_speed);
	cmd_param += sText;
	p->AddEvent2List("��ʼ��������wifi��");
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
int software_total_machine_by::create_app_bt_trans_init(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/total/brcm_patchram_plus /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/brcm_patchram_plus";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/total/Comparison.txt /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/total/hciconfig /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/hciconfig";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/total/libbluetooth.so.1 /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/total/rfcomm /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/rfcomm";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/total/ble_read /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/ble_read";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	cmd_param = " push firmware/total/bt_trans /tmp";
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
	cmd_param += p->total_machine_by_option_instance.g_bluetooth_board_mac;
	p->AddEvent2List("��ʼ��������������");
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

int software_total_machine_by::create_app_start_record(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString sText;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CFile record_file;
	msg_by_total_info *msg_p;
	sText.Format("%d",p->total_machine_by_option_instance.g_record_time_sec);
	cmd_param = " shell /tmp/start_play 3 ";//3��ʼ¼���� ����Ϊ¼��ʱ��
	cmd_param += sText;
	p->AddEvent2List("��ʼ����¼����");
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	int n=response.Find("RECORD OK");
	
	CTime tNow = CTime::GetCurrentTime();
	sText.Format("audio/total_by/record/%s_%02d%02d%02d_left.pcm", p->sn_read,
					tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
	p->rec_left_file_path = sText;
	cmd_param = " pull /tmp/left.pcm ";
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	

	sText.Format("audio/total_by/record/%s_%02d%02d%02d_right.pcm", p->sn_read,
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	p->rec_right_file_path = sText;
	cmd_param = " pull /tmp/right.pcm ";
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	if(n<0){
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
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_RECORD_DB_VALUE;
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
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_RECORD_DB_VALUE;
			msg_p->data1 = 2;
			::PostMessage(p->m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		}
		//if(p->rec_aud_db_left >= p->total_machine_by_option_instance.g_record_passdb &&
		//	p->rec_aud_db_right >= p->total_machine_by_option_instance.g_record_passdb &&
		//	abs((int)p->rec_aud_db_left - (int)p->rec_aud_db_right) <= (int)p->total_machine_by_option_instance.g_record_subdb){
		//	p->init_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
		//}else{
		//	p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
		//}
	}
	p->init_flag |= 1<<BY_CHECK_INIT_FLAG_AUDIO_REC;
	return 0;
}
int software_total_machine_by::create_app_start_play(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	//cmd_param = " shell amixer cset numid=5 255";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}
	p->AddEvent2List("��ʼ�����������ȣ�");
	cmd_param = " shell aplay /tmp/1KHZ4s.wav";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}

	return 0;
}
int software_total_machine_by::create_app_start_play_init(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
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
	cmd_param = " shell /tmp/start_play 2 1";//2����У׼ֵ�� 1Ϊǿ��У׼
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	int n=response.Find("CALI OK");
	if(n<0){
		cmd_param = " shell /tmp/start_play 2 1";//2����У׼ֵ�� 1Ϊǿ��У׼
		response = p->ExecuteCmd(cmd_str, cmd_param);
		if(p->if_response_err(response)){
			return -1;
		}
		n=response.Find("CALI OK");
		if(n<0){
			p->calibration_value = 0;
			p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_CALI;
			return -1;
		}else{
			p->init_flag |= 1<<BY_CHECK_INIT_FLAG_CALI;		
		}
	}else{
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_CALI;
	}

	CString sText;
	n=response.Find("calibration_value:");
	sText = response.Mid(n+sizeof("calibration_value"), 4);
	StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&p->calibration_value);

	cmd_param = " shell amixer cset numid=5 255";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " push firmware/total/warning.wav /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell aplay /tmp/warning.wav";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}

	return 0;
}
int software_total_machine_by::create_app_ts_init(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString sText;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/total/ts_init /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/ts_init";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/ts_init";
	sText.Format(" %d",p->total_machine_by_option_instance.g_ts_pix_off);
	cmd_param += sText;
	sText.Format(" %d",p->total_machine_by_option_instance.g_ts_err_times);
	cmd_param += sText;
	p->AddEvent2List("�ȴ��û����Դ�������");
	response = p->ExecuteCmd_tsprocess(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	if(response.Find("ts_init OK") >= 0){
		p->init_flag |= 1<<BY_CHECK_INIT_FLAG_TS;
	}else{
	//if(response.Find("ts_init ERR") >= 0){
		p->init_err_flag |= 1<<BY_CHECK_INIT_FLAG_TS;
	}
	return 0;
}
int software_total_machine_by::ts_process(CString *response)
{
	int pos,x,y;
	msg_by_total_info *msg_p;
	CString sText;
	while(1){
		if( (pos=response->Find("pixel ERR:")) >= 0){
			sText = response->Mid(pos+sizeof("pixel ERR:X"),3);
			StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&x);
			sText = response->Mid(pos+sizeof("pixel ERR:X=000,Y"),3);
			StrToIntEx(sText, STIF_SUPPORT_HEX, (int*)&y);
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_PIXEL_ERR;
			msg_p->data1 = x;
			msg_p->data2 = y;
			::PostMessage(m_hWnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			//TRACE("###################x=%d,y=%d\r\n",x, y);
			response->Delete(0,pos+sizeof("pixel ERR:X=000,Y=000"));
		}else{
			break;
		}
	}
	return 0;
}
CString software_total_machine_by::ExecuteCmd_tsprocess(CString cmd_name, CString cmd_param)
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
	CString ts_output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
		ts_output += buffer;
		ts_process(&ts_output);
    }
	CloseHandle(hRead);
	return output;
}
int software_total_machine_by::key_process(CString response)
{
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

	}
	return 0;
}
CString software_total_machine_by::ExecuteCmd_keyprocess(CString cmd_name, CString cmd_param)
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
int software_total_machine_by::create_app_key_init(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " push firmware/total/button_check /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/button_check";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	cmd_param = " shell /tmp/button_check 2";
	p->AddEvent2List("�ȴ��û�������");
	response = p->ExecuteCmd_keyprocess(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		return -1;
	}
	if( (p->init_flag & 1<<BY_CHECK_INIT_FLAG_KEYUP) == 0 ){
		p->init_err_flag = 1<<BY_CHECK_INIT_FLAG_KEYUP;
		return -1;
	}
	if( (p->init_flag & 1<<BY_CHECK_INIT_FLAG_KEYDOWN) == 0 ){
		p->init_err_flag = 1<<BY_CHECK_INIT_FLAG_KEYDOWN;
		return -1;
	}
	if( (p->init_flag & 1<<BY_CHECK_INIT_FLAG_KEYPOWER) == 0 ){
		p->init_err_flag = 1<<BY_CHECK_INIT_FLAG_KEYPOWER;
		return -1;
	}
	return 0;
}
int software_total_machine_by::if_response_err(CString response)
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
int software_total_machine_by::create_app_sn_init(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
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
TCHAR* software_total_machine_by::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
CString software_total_machine_by::ExecuteCmd_wifi_trans(CString cmd_name, CString cmd_param)
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
    si.cb = sizeof( STARTUPINFO ); //�ṹ���С 
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
CString software_total_machine_by::ExecuteCmd(CString cmd_name, CString cmd_param)
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
int software_total_machine_by::process_response_connect(CString response)
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
int software_total_machine_by::process_response_devices(CString response)
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

// software_total_machine_by ��Ϣ�������
int software_total_machine_by::need_file_ifexit()
{
	if(PathFileExists("thirdapp\\adb\\adb.exe") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�adb.exe");
		return 1;
	}
	if(PathFileExists("thirdapp\\net\\iperf.exe") != TRUE){
		AfxMessageBox("���棺����·����δ�ҵ�iperf.exe");
		return 1;
	}
	if(PathFileExists("firmware\\total\\sn_init") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�sn_init");
		return 1;
	}
	if(PathFileExists("firmware\\total\\bt_trans") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�bt_trans");
		return 1;
	}
	if(PathFileExists("firmware\\total\\button_check") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�button_check");
		return 1;
	}
	if(PathFileExists("firmware\\total\\start_play") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�start_play");
		return 1;
	}
	if(PathFileExists("firmware\\total\\ts_init") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�ts_init");
		return 1;
	}
	if(PathFileExists("firmware\\total\\wifi_trans") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�wifi_trans");
		return 1;
	}
	if(PathFileExists("firmware\\total\\1KHZ4s.wav") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�1KHZ4s.wav");
		return 1;
	}

	if(PathFileExists("firmware\\total\\test_wifi") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�test_wifi");
		return 1;
	}
	if(PathFileExists("firmware\\total\\iperf") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�iperf");
		return 1;
	}

	if(PathFileExists("firmware\\total\\brcm_patchram_plus") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�brcm_patchram_plus");
		return 1;
	}
	if(PathFileExists("firmware\\total\\Comparison.txt") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�Comparison.txt");
		return 1;
	}
	if(PathFileExists("firmware\\total\\hciconfig") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�hciconfig");
		return 1;
	}
	if(PathFileExists("firmware\\total\\libbluetooth.so.1") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�libbluetooth.so.1");
		return 1;
	}
	if(PathFileExists("firmware\\total\\rfcomm") != TRUE){
		AfxMessageBox("���󣺳���·����δ�ҵ�rfcomm");
		return 1;
	}
	return 0;
}
void software_total_machine_by::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_fs_version.SetWindowTextA("V0.00.00");
	m_kernel_version.SetWindowTextA("");
	m_readSn.SetWindowTextA("");
	pass_green_color = 0;
	m_Epass.SetWindowTextA("");

	m_wifihot_name.SetWindowTextA("");
	//m_wifihot_status.SetWindowTextA("������..");
	m_wifihot_status.SetWindowTextA("");
	m_calibration.SetWindowTextA("");
	m_play_db.SetWindowTextA("");
	m_rec_right_db.SetWindowTextA("");
	m_rec_left_db.SetWindowTextA("");
	m_recplay_sec.SetWindowTextA("0��");
	m_player_sec.SetWindowTextA("0��");
	m_bat_power.SetWindowTextA("0");

	m_snRead.SetColor(STATIC_WND_DISABLE);
	m_btnBoot.SetColor(STATIC_WND_DISABLE);
	m_btnWifi.SetColor(STATIC_WND_DISABLE);
	m_btnBt.SetColor(STATIC_WND_DISABLE);
	m_btnDmic.SetColor(STATIC_WND_DISABLE);
	m_btnPA.SetColor(STATIC_WND_DISABLE);
	m_btnKeyU.SetColor(STATIC_WND_DISABLE);
	m_btnKeyD.SetColor(STATIC_WND_DISABLE);
	m_btnKeyPower.SetColor(STATIC_WND_DISABLE);
	m_ts_event.SetColor(STATIC_WND_DISABLE);

	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	((CButton*)GetDlgItem(IDC_BUTTON20))->EnableWindow(FALSE);
	AddEvent2List("#####�ȴ��û�����sn��#####");
}
BOOL software_total_machine_by::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString dir_path;
	dir_path = LOG_PATH_TOTAL_by;

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);
	CreateDirectory("audio", 0);
	CreateDirectory("audio\\total_by", 0);
	CreateDirectory("audio\\total_by\\record", 0);
	CreateDirectory("audio\\total_by\\play", 0);

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
	
	need_file_ifexit();

	m_editFont.CreatePointFont(700, "����");
	m_Epass.SetFont(&m_editFont);

	SetAllItemRestart();
	return TRUE;  // return TRUE unless you set the focus to a control
}

CString software_total_machine_by::ExecuteCmd_iperf(CString cmd_name, CString cmd_param)
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
int software_total_machine_by::create_app_iperf(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
	CString response;
	CString cmd_str = "thirdapp\\net\\iperf.exe";
	CString cmd_param;
	cmd_param = " -s";
	response = p->ExecuteCmd_iperf(cmd_str, cmd_param);

	//software_total_machine_by *p = (software_total_machine_by*)param;
	//CString response;
	//CString cmd_str = "thirdapp\\adb\\adb.exe";
	//CString cmd_param;
	//cmd_param = " push firmware/total/while1 /tmp";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}
	//cmd_param = " shell chmod +x /tmp/while1";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}
	//cmd_param = " shell /tmp/while1";
	//response = p->ExecuteCmd(cmd_str, cmd_param);
	//if(p->if_response_err(response)){
	//	return -1;
	//}
	return 0;
}

bool software_total_machine_by::OpenWLANService()
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
bool software_total_machine_by::WLANNetwork()
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

LPCWSTR software_total_machine_by::StringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length()-1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
	return wcstring;
}

bool software_total_machine_by::SetProfile()
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

bool software_total_machine_by::WLANConnect()
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

bool software_total_machine_by::SearchNet()
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
			msg_by_total_info *msg_p;
			msg_p = new msg_by_total_info;
			msg_p->msg_type = BY_TOTAL_MSG_TYPE_WIFI_GET_NAME;
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

bool software_total_machine_by::get_wifi_status()
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
bool software_total_machine_by::get_connect_msg()
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
bool software_total_machine_by::WLANDisconnect()
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

bool software_total_machine_by::CloseWLANService()
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
void software_total_machine_by::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAllItemRestart();
	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::WorkerThreadProc, this, 0, NULL);
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_total_machine_by::OnEnChangeSetSn()
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
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::WorkerThreadProc, this, 0, NULL);
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
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn������ɹ���׼������");
		}

	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

int software_total_machine_by::kill_adbd(void *param)
{
	software_total_machine_by *p = (software_total_machine_by*)param;
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
void software_total_machine_by::OnBnClickedButton20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_total_machine_by::kill_adbd, this, 0, NULL);
}

void software_total_machine_by::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TerminateProcess(iperf.hProcess,0);
	//::waveOutSetVolume(0, 0);
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

void software_total_machine_by::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//::waveOutSetVolume(0, 0xffffffff);
	//CString response;
	//CString cmd_str = "thirdapp\\adb\\adb.exe";
	//CString cmd_param;
	//cmd_param = " shell killall adbd";
	//response = ExecuteCmd(cmd_str, cmd_param);
	//if(if_response_err(response)){
	//	return;
	//}
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

void software_total_machine_by::OnBnClickedButton17()
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

void software_total_machine_by::OnBnClickedButton14()
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

void software_total_machine_by::OnBnClickedButton15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	list_audio.ResetContent();
}

HBRUSH software_total_machine_by::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL software_total_machine_by::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

