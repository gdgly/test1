// software_batpower_by.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_batpower_by.h"
#include "afxdialogex.h"


// software_batpower_by �Ի���

IMPLEMENT_DYNAMIC(software_batpower_by, CDialogEx)

software_batpower_by::software_batpower_by(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_batpower_by::IDD, pParent)
{

}

software_batpower_by::~software_batpower_by()
{
}

void software_batpower_by::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_STATIC6, m_readSn);
	DDX_Control(pDX, IDC_STC_SN_READ, m_snRead);
	DDX_Control(pDX, IDC_EDIT_FIRMVER, m_fs_version);
	DDX_Control(pDX, IDC_EDIT_FIRMVER2, m_kernel_version);
	DDX_Control(pDX, IDC_CHECK1, current_checkBox);
	DDX_Control(pDX, IDC_EDIT_NORMAL_CURRENT2, m_normalCurrect);
	DDX_Control(pDX, IDC_EDIT_CUR_CURRENT, m_chargeCurrect);
	DDX_Control(pDX, IDC_STC_BOOT, m_btnBoot);
	DDX_Control(pDX, IDC_LIST2, m_listEvt);
	DDX_Control(pDX, IDC_STATIC10, m_Epass);
	DDX_Control(pDX, IDC_STC_PA, m_reset_setup);
	DDX_Control(pDX, IDC_bat_power, m_bat_power);
}


BEGIN_MESSAGE_MAP(software_batpower_by, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT2, &software_batpower_by::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON13, &software_batpower_by::OnBnClickedButton13)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void software_batpower_by::AddEvent2List(CString sText)
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

void software_batpower_by::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	batpower_by_option_instance = g_p->batpower_by_option_instance;
}
// software_batpower_by ��Ϣ�������
enum {
	BY_CHECK_NORMAL_CURRENT = 0,
	BY_CHECK_DEVICES,
	BY_CHECK_CREATE_INIT,
	BY_CHECK_CREATE_POWER,
	BY_CHECK_CHECK_INIT,
	BY_CHECK_CHARGE_CURRENT,
	BY_CHECK_SCUCESS,
};
int software_batpower_by::WorkerThreadProc(void *param)
{
	CString sText;
	CTime tNow = CTime::GetCurrentTime();
	CTime tsav = CTime::GetCurrentTime();
	int time_over, time_over_times = 0;
	software_batpower_by *p = (software_batpower_by*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CCurlCtrl *mesptr = &p->mesInfo;
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
	p->current = 0;
	p->charge_current = 0;
	int status = BY_CHECK_DEVICES;
	if(p->batpower_by_option_instance.g_current_test){
		if(FALSE == pComCheck->InitComm(p->batpower_by_option_instance.g_currentcom_port, DEIGHT, SONE, PNONE, 
			p->batpower_by_option_instance.g_currentcom_baud)) {
			AfxMessageBox("�����崮�ڴ�ʧ��! ");
			p->AddEvent2List("�����崮�ڴ�ʧ��! ");
			((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			return -1;
		}else {
			p->AddEvent2List("�����崮�ڴ򿪳ɹ�����ʼ������! ");
			pComCheck->SendComand(CHARG_SHUT_COM);      // �رճ��
			p->com_thread_quit = false;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_batpower_by::COMThreadProc, param, 0, NULL);
			status = BY_CHECK_NORMAL_CURRENT;
		}
	}
	while(1){
		tNow = CTime::GetCurrentTime();
		switch (status){
			case BY_CHECK_NORMAL_CURRENT:
				//p->current = pComCheck->GetCurrent();
				p->current = 1;
				time_over = (int)(tNow.GetTime() - tsav.GetTime());
				if( time_over > 1){
					sText.Format("%d",p->current);
					p->m_normalCurrect.SetWindowTextA(sText);
					tsav = tNow;
				}
				if(p->current > p->batpower_by_option_instance.g_normal_current_max){
					sText.Format("%d",p->current);
					p->m_normalCurrect.SetWindowTextA(sText);
					p->m_normalCurrect.SetColor(STATIC_WND_WARNING);
					AfxMessageBox("�忨��������! ");
					p->AddEvent2List("�忨��������! ");
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					goto out;
				}else if( (p->current < p->batpower_by_option_instance.g_normal_current_max) &&
					(p->current > p->batpower_by_option_instance.g_normal_current_min)){
					p->AddEvent2List("�������ɹ�����ʼ��������! ");
					sText.Format("%d",p->current);
					p->m_normalCurrect.SetWindowTextA(sText);
					p->m_normalCurrect.SetColor(STATIC_WND_SUCCESS);
					pComCheck->SendComand(CHARG_OPEN_COM);      // �򿪳�磬ͬʱ��usb
					status = BY_CHECK_CHARGE_CURRENT;
					Sleep(1500);//��1500ms ʹ�ó���ȶ�
				}else{
					if(time_over > 1){
						p->AddEvent2List("δ��⵽�忨��");
						time_over_times ++;
					}
					if(time_over_times == p->batpower_by_option_instance.g_startcheck_times){
						p->AddEvent2List("���忨��ʱ�������Ӱ忨�����²��ԣ�");
						goto out;
					}
				}
				break;
			case BY_CHECK_CHARGE_CURRENT:
				p->charge_current = pComCheck->GetChargCurrent();
				if(p->charge_current > p->batpower_by_option_instance.g_chargCurrent){
					p->AddEvent2List("���������ɹ�! ");
					sText.Format("%d",p->charge_current);
					p->m_chargeCurrect.SetWindowTextA(sText);
					p->m_chargeCurrect.SetColor(STATIC_WND_SUCCESS);
					status = BY_CHECK_DEVICES;
					//pComCheck->SendComand(CHARG_SHUT_COM);
					Sleep(1000);
				}else{
					Sleep(1000);
					p->charge_current = pComCheck->GetChargCurrent();
					if(p->charge_current > p->batpower_by_option_instance.g_chargCurrent){
						p->AddEvent2List("���������ɹ�! ");
						sText.Format("%d",p->charge_current);
						p->m_chargeCurrect.SetWindowTextA(sText);
						p->m_chargeCurrect.SetColor(STATIC_WND_SUCCESS);
						status = BY_CHECK_DEVICES;
						//pComCheck->SendComand(CHARG_SHUT_COM);
						Sleep(1000);
					}else{
						sText.Format("%d",p->charge_current);
						p->m_chargeCurrect.SetWindowTextA(sText);
						p->m_chargeCurrect.SetColor(STATIC_WND_WARNING);
						AfxMessageBox("���������ʧ��! ");
						p->AddEvent2List("���������ʧ��! ");
						p->pass_green_color = 0;
						p->m_Epass.SetWindowTextA("FAIL");
						//pComCheck->SendComand(CHARG_SHUT_COM);
						goto out;
					}
				}
				break;
			case BY_CHECK_DEVICES:
				cmd_param = " devices";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(tNow.GetTime() - tsav.GetTime() > p->batpower_by_option_instance.g_usb_timeout){
					AfxMessageBox("usb���ӳ�ʱ! ");
					p->AddEvent2List("usb���ӳ�ʱ! ");
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					tsav = tNow;
					goto out;
				}
				if(p->process_response_devices(response)){
					Sleep(1000);
				}else{
					cmd_param = " shell /etc/init.d/S50servermanager stop";
					response = p->ExecuteCmd(cmd_str, cmd_param);
					if(p->if_response_err(response)){
						p->pass_green_color = 0;
						p->m_Epass.SetWindowTextA("FAIL");
						goto out;
					}
					p->AddEvent2List("usb���ӳɹ�! ");
					p->m_btnBoot.SetColor(STATIC_WND_SUCCESS);
					status = BY_CHECK_CREATE_INIT;
					//pComCheck->SendComand(NOPATH_TEST_R);//�������Ե��裬ʹ�������ܹ���������
					Sleep(100);//��һС�ᣬȷ����������յ�������
				}
				break;
			case BY_CHECK_CREATE_INIT:
				if( p->sn_init(param) != 0 ){
					goto poweroff_out;
				}else{
					if(p->general_option_instance.g_check_mes){
						if(p->general_option_instance.g_check_mes_type == 1){
							mesptr->clear_flag();
							if(mesptr->power_SendChecks((LPCSTR)p->sn_read) == 0)   {                       //�ϴ�MES���̼��ӿ�    checkģʽ��һ��MES
							}else{
								AfxMessageBox("mes�����Ϸ�����! ");
								p->pass_green_color = 0;
								p->m_Epass.SetWindowTextA("FAIL");
								goto poweroff_out;
							}
							if(mesptr->Messtatus  == 1){                                     //mes check�ж�
								p->AddEvent2List("mes���ѷ���! ");
							}else{
								AfxMessageBox("�����ѹ�վ! ");
								p->pass_green_color = 0;
								p->m_Epass.SetWindowTextA("FAIL");
								goto poweroff_out;
							}
						}
					}
					status = BY_CHECK_CREATE_POWER;
				}
				break;
			case BY_CHECK_CREATE_POWER:
				p->read_bat_power = 0;
				cmd_param = " shell cat /sys/class/power_supply/sm5007-fuelgauge/capacity";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					goto poweroff_out;
				}
				StrToIntEx(response, STIF_SUPPORT_HEX, (int*)&p->read_bat_power);
				sText.Format("%d",p->read_bat_power);
				p->m_bat_power.SetWindowTextA(sText);
				if(p->read_bat_power >= p->batpower_by_option_instance.g_power_percent){
					p->m_bat_power.SetColor(STATIC_WND_SUCCESS);
				}else{
					p->m_bat_power.SetColor(STATIC_WND_WARNING);	
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					goto poweroff_out;
				}
				if(p->batpower_by_option_instance.g_if_setup_reset)
					status = BY_CHECK_CHECK_INIT;
				else 
					status = BY_CHECK_SCUCESS;
				break;
			case BY_CHECK_CHECK_INIT:
				cmd_param = " shell rm /usr/data/starot/config/ui.json";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					p->m_reset_setup.SetColor(STATIC_WND_WARNING);
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					goto poweroff_out;
				}
				cmd_param = " shell sync";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				if(p->if_response_err(response)){
					p->m_reset_setup.SetColor(STATIC_WND_WARNING);
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					goto poweroff_out;
				}
				p->m_reset_setup.SetColor(STATIC_WND_SUCCESS);
				status = BY_CHECK_SCUCESS;
				break;
			case BY_CHECK_SCUCESS:
				if( p->general_option_instance.g_check_mes ) {
					if(p->general_option_instance.g_check_mes_type == 1){
						if(p->MesPass() == TRUE){
							if(p->MesSend() != TRUE){
								p->pass_green_color = 0;
								p->m_Epass.SetWindowTextA("FAIL");
								goto poweroff_out;
							}
						}else{
							p->pass_green_color = 0;
							p->m_Epass.SetWindowTextA("FAIL");
							goto poweroff_out;
						}
					}else if(p->general_option_instance.g_check_mes_type == 2){
						;
					}
				}
				cmd_param = " shell poweroff";
				response = p->ExecuteCmd(cmd_str, cmd_param);
				Sleep(200);
				p->AddEvent2List("������ɣ��ȴ��û���λ! ");
				p->pass_green_color = 1;
				p->m_Epass.SetWindowTextA("PASS");
				goto poweroff_out;
				break;
		}
	}
poweroff_out:
	cmd_param = " shell poweroff";
	response = p->ExecuteCmd(cmd_str, cmd_param);
out:
	if(p->batpower_by_option_instance.g_current_test){
		pComCheck->SendComand(CHARG_SHUT_COM);
	}
	p->com_thread_quit = TRUE;
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
	p->Invalidate();
	return 0;
}

INT software_batpower_by::MesPass()   //ÿһ���Ʒ�����Ժ󣬵��PASS�ϴ�MES
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1;
	sText.Format("%s",sn_read);
	if(mesIptr->Messtatus == 1){
		if(mesIptr->power_SendDatae(sText) == 0){
		}else{
			AfxMessageBox("mes�����Ϸ�����! ");
			AddEvent2List("mes�����Ϸ�������");
			return 0;
		}
	}

	if(mesIptr->Messtatus == 3){
		return 1;
	}else{
		AfxMessageBox("�����ѹ�վ! ");
		AddEvent2List("�����ѹ�վ��");
		return 0;
	}
}

INT software_batpower_by::MesSend()   //վλ�ϴ��Ժ��ϴ��������
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sData,sfirver,Ssouver,snor_cur,schar_cur;
	sData.Format("%d",read_bat_power);
	sfirver.Format("%s",fs_version);
	Ssouver.Format("%s",kernel_version);
	sText.Format("%s",sn_read);
	snor_cur.Format("%d",current);
	schar_cur.Format("%d",charge_current);
	if(mesIptr->Messtatus == 3){
		if(mesIptr->by_power_SendChecke(sText,sData,sfirver,Ssouver,snor_cur,schar_cur) == 0){
		}else{
			AfxMessageBox("mes�����Ϸ�����! ");
			AddEvent2List("mes�����Ϸ�����! ");
			return 0;
		}
	}

	if(mesIptr->Messtatus == 7){
		return 1;
	}else{
		AfxMessageBox("�����ѹ�վ! ");
		AddEvent2List("�����ѹ�վ! ");
		return 0;
	}
}

int software_batpower_by::sn_init(void* param)
{
	CString sText;
	software_batpower_by *p = (software_batpower_by*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	CString cut_str;
	cmd_param = " push firmware/total/sn_init /tmp";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->pass_green_color = 0;
		p->m_Epass.SetWindowTextA("FAIL");
		return -1;
	}
	cmd_param = " shell chmod +x /tmp/sn_init";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->pass_green_color = 0;
		p->m_Epass.SetWindowTextA("FAIL");
		return -1;
	}
	cmd_param = " shell /tmp/sn_init";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->if_response_err(response)){
		p->pass_green_color = 0;
		p->m_Epass.SetWindowTextA("FAIL");
		return -1;
	}
		int n=response.Find("sn_init OK");
	if(n<0){
		p->AddEvent2List("sn�ų���ִ��ʧ��! ");
		p->m_snRead.SetColor(STATIC_WND_WARNING);
		p->pass_green_color = 0;
		p->m_Epass.SetWindowTextA("FAIL");
		return -1;
	}
	int length=response.Delete(0,n); //�ҳ�""sn_init OK\r\n""�ұߵ��ַ���λ��
	cut_str = response.Right(length - sizeof("sn_init OK"));   
	if( (length=cut_str.Find("SN:")) < 0 ){
		p->AddEvent2List("sn��δ�ҵ�! ");
		p->m_snRead.SetColor(STATIC_WND_WARNING);
		p->pass_green_color = 0;
		p->m_Epass.SetWindowTextA("FAIL");
		return -1;
	}else{
		CString sn;
		cut_str.Delete(0,length);
		sn = cut_str.Mid(3,20);
		memset(p->sn_read, 0, 20);
		if(sn.Find("-") < 0){
			memcpy(p->sn_read, sn, 18);
			if(memcmp(p->sSetSN, p->sn_read, 18) == 0){
				sText.Format("%s",p->sn_read);
				p->m_readSn.SetWindowTextA(sText);
				p->m_snRead.SetColor(STATIC_WND_SUCCESS);
			}else{
				sText.Format("%s",p->sn_read);
				p->m_readSn.SetWindowTextA(sText);
				p->AddEvent2List("sn�ż��ʧ��! ");
				p->m_snRead.SetColor(STATIC_WND_WARNING);
				p->pass_green_color = 0;
				p->m_Epass.SetWindowTextA("FAIL");
				return -1;
			}
		}else{
			memcpy(p->sn_read, sn, 19);
			if(memcmp(p->sSetSN, p->sn_read, 19) == 0){
				sText.Format("%s",p->sn_read);
				p->m_readSn.SetWindowTextA(sText);
				p->m_snRead.SetColor(STATIC_WND_SUCCESS);
			}else{
				sText.Format("%s",p->sn_read);
				p->m_readSn.SetWindowTextA(sText);
				p->AddEvent2List("sn�ż��ʧ��! ");
				p->m_snRead.SetColor(STATIC_WND_WARNING);
				p->pass_green_color = 0;
				p->m_Epass.SetWindowTextA("FAIL");
				return -1;
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
	sText.Format("%s",p->fs_version);
	p->m_fs_version.SetWindowTextA(sText);
	sText.Format("%s",p->kernel_version);
	p->m_kernel_version.SetWindowTextA(sText);
	return 0;
}

int software_batpower_by::if_response_err(CString response)
{
	if(response.Find("error:") >= 0){
		//init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("adb�豸ʧȥ����");
		return -1;
	}
	if(response.Find("error: device not found") < 0){
		return 0;
	}else{
		//init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("usb�豸ʧȥ����");
		return -1;
	}
}

int software_batpower_by::process_response_devices(CString response)
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

TCHAR* software_batpower_by::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
CString software_batpower_by::ExecuteCmd(CString cmd_name, CString cmd_param)
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

int software_batpower_by::COMThreadProc(void *param)
{
	software_batpower_by *p = (software_batpower_by*)param;
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

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_batpower_by::OnEnChangeEdit2()
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
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_batpower_by::WorkerThreadProc, this, 0, NULL);
				AddEvent2List("sn������ɹ����ȴ��ϵ�");
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
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_batpower_by::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn������ɹ����ȴ��ϵ�");
		}

	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void software_batpower_by::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_fs_version.SetWindowTextA("V0.00.00");
	m_kernel_version.SetWindowTextA("");
	m_readSn.SetWindowTextA("");
	m_snRead.SetWindowTextA("Wait");
	m_snRead.SetColor(STATIC_WND_DISABLE);

	m_normalCurrect.SetColor(STATIC_WND_DISABLE);
	m_chargeCurrect.SetColor(STATIC_WND_DISABLE);
	m_normalCurrect.SetWindowTextA("");
	m_chargeCurrect.SetWindowTextA("");

	m_btnBoot.SetColor(STATIC_WND_DISABLE);
	m_bat_power.SetColor(STATIC_WND_DISABLE);
	m_bat_power.SetWindowTextA("");
	m_reset_setup.SetColor(STATIC_WND_DISABLE);

	pass_green_color = 0;
	m_Epass.SetWindowTextA("");

	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	AddEvent2List("#####�ȴ��û�����sn��#####");
}

void software_batpower_by::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetAllItemRestart();
}

BOOL software_batpower_by::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString dir_path;
	dir_path = LOG_PATH_BATPOWER_by;

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

	m_editFont.CreatePointFont(700, "����");
	m_Epass.SetFont(&m_editFont);

	SetAllItemRestart();
	current_checkBox.EnableWindow(FALSE);
	current_checkBox.SetCheck(batpower_by_option_instance.g_current_test);
	if(batpower_by_option_instance.g_current_test){
		((CButton*)GetDlgItem(IDC_STATIC_charge))->EnableWindow(TRUE);
		((CButton*)GetDlgItem(IDC_STATIC_normal))->EnableWindow(TRUE);
	}else{
		((CButton*)GetDlgItem(IDC_STATIC_charge))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_STATIC_normal))->EnableWindow(FALSE);
	}

	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;

	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_batpower_by::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH software_batpower_by::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
