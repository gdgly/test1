// software_air_fz.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_air_fz.h"
#include "afxdialogex.h"


// software_air_fz �Ի���

IMPLEMENT_DYNAMIC(software_air_fz, CDialogEx)

software_air_fz::software_air_fz(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_air_fz::IDD, pParent)
{

}

software_air_fz::~software_air_fz()
{
}

void software_air_fz::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC10, m_ECpass);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
	DDX_Control(pDX, IDC_SET_SN, m_Esetsn);
	DDX_Control(pDX, IDC_MAC_ADDR, m_sMacAddr);
	DDX_Control(pDX, IDC_SN_VALUE, m_snValue);
	//DDX_Control(pDX, IDC_TFA9896_VAL, m_iTFAVal);
	//DDX_Control(pDX, IDC_CYCLE, m_Ecycle);
	DDX_Control(pDX, IDC_AUDIO_DB_RIGHT, m_audiodb_right);
	DDX_Control(pDX, IDC_AUDIO_DB_LEFT, m_audiodb_left);
	DDX_Control(pDX, IDC_LIST1, list_audio);
	DDX_Control(pDX, IDC_CHAVALUE, m_Echavalue);
}

BEGIN_MESSAGE_MAP(software_air_fz, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_air_fz::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON13, &software_air_fz::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_SET_SN, &software_air_fz::OnEnChangeSetSn)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &software_air_fz::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &software_air_fz::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON17, &software_air_fz::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON14, &software_air_fz::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &software_air_fz::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON20, &software_air_fz::OnBnClickedButton20)
END_MESSAGE_MAP()

void software_air_fz::AddEvent2List(CString sText)
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

UINT software_air_fz::Tightess(CString sn,UINT dbvalue)
{
	if(oldTightSn != TightSn){
		oldTightSn.Format("%s",TightSn);
		TightSn.Format("%s",sn);

		oldstauts = dbstauts;
		dbstauts = dbvalue;
	}else{							        //SN����ȣ����������
		oldTightSn.Format("%s",TightSn);
		TightSn.Format("%s",sn);

		oldstauts = dbstauts;
		dbstauts = dbvalue;
	}

	if(oldTightSn == TightSn)        //�ж�DBֵ��־�ǲ���һ��
	{
		if(dbstauts + oldstauts == 2 || dbstauts + oldstauts == 4)
			AddEvent2List("�����һ���ף�");
		if(dbstauts + oldstauts == 3)
			return 1;
		else
			return -1;
	}
	return 0;
}

LRESULT software_air_fz::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	int ispass;
	int count;
	CString sText;
	switch((INT)wParam) {
		case WPARAM_START:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("���ڲɼ��С�����");
			AddEvent2List("��ʼ�ɼ���");
			break;
		case WPARAM_UART_ERR:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("��λ�ɼ�");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			AfxMessageBox("���ڴ�ʧ��");
			AddEvent2List("���ڴ�ʧ��");
			break;
		case WPARAM_START_TIMEOUT:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("��λ�ɼ�");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			AfxMessageBox("���տ�ʼ������ʱ"); 
			AddEvent2List("���տ�ʼ������ʱ");
			break;
		case WPARAM_STOP_TIMEOUT:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("��λ�ɼ�");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			AfxMessageBox("����ֹͣ������ʱ"); 
			AddEvent2List("����ֹͣ������ʱ");
			break;
		case WPARAM_AUDIO_LENGTH:
			sText.Format("%d.%d ��", audMsec_right/1000, (audMsec_right%1000)/100);
			GetDlgItem(IDC_AUDIO_SEC)->SetWindowTextA(sText);
			break;
		case  WPARAM_MAC_SN:
			if( m_sSN.Find("-") < 0 ){
				m_sSN = m_sSN.Right(18);
			}
			m_sMacAddr.SetWindowTextA(m_sMac);
			m_snValue.SetWindowTextA(m_sSN);
			AddEvent2List("mac sn�Ż�ȡ�ɹ�");
			break;
		case WPARAM_GET_RIGHTAUD_FILE:
			sText = aud_right_file_path;
			count = list_audio.GetCount();
			list_audio.InsertString(count, sText);
			list_audio.SetCurSel(count);
			break;
		case WPARAM_GET_LEFTAUD_FILE:
			sText = aud_left_file_path;
			count = list_audio.GetCount();
			list_audio.InsertString(count, sText);
			list_audio.SetCurSel(count);
			break;
		case WPARAM_THREAD_EXITED:
			sText.Format("%d", aud_right_db);
			m_audiodb_right.SetWindowTextA(sText);
			sText.Format("%d", aud_left_db);
			m_audiodb_left.SetWindowTextA(sText);
			sText.Format("%d", abs(aud_right_db - aud_left_db));
			m_Echavalue.SetWindowTextA(sText);

			if(aud_right_db - aud_left_db >= (int)bluetooth_option_instance.g_sub_value_air){
				seconds = 1;
				AddEvent2List("��һ�β���PASS��");
			}else if(aud_left_db - aud_right_db >= (int)bluetooth_option_instance.g_sub_value_air){
				seconds = 2;
				AddEvent2List("�ڶ��β���PASS��");
			}else
				seconds = 0;

			ispass = Tightess(m_sSN, seconds);
			if(ispass == 1){
				pass_green_color = 1;
				m_ECpass.SetWindowTextA("PASS");
				AddEvent2List("#####���ɹ�######");
			}else if(ispass == -1){
				pass_green_color = 0;
				m_ECpass.SetWindowTextA("FAIL");	
				AddEvent2List("#####���ʧ��######");
			}
			Invalidate();
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("��λ�ɼ�");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;

		default:
			break;
	}
	return 0;
}

extern int getAvgPcmDB(INT16  *pcmdata,  long nmemb);

int software_air_fz::WorkerThreadProc(void *param)
{
	CString sText;
	int timeout = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	int m_start_resp = 0;
	int m_stop_resp = 0;
	NetProto send_pkt,cmdPkt;
	unsigned char cmd_dat[26];
	CTime tNow,tSend;
	CTimeSpan timeSpan;
	software_air_fz *p = (software_air_fz*)param;
	CContrlComm_bluetooth *comInfo = &(p->m_ctlCom);

	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//����buf
	int audio_right_len = 0;
	int audio_left_len = 0;
	BYTE *audio_right_buf = new BYTE[1<<20];//������buf
	BYTE *audio_left_buf = new BYTE[1<<20];//������buf
	p->audMsec_right = 0;
	p->audMsec_left = 0;

	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	comInfo->baudCom = p->general_option_instance.g_ttycom_baud;

	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, comInfo->baudCom)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
		goto out;
	}else{
		sText.Format("�򿪶˿�%d�ɹ�",comInfo->portCom);
		p->AddEvent2List(sText);
	}

	memset(cmd_dat, 0, sizeof(cmd_dat));
	cmd_dat[0] = 0x03;
	if(p->m_setSN.GetLength() == 18){
		memcpy(&cmd_dat[2], p->m_setSN, 18);
		cmd_dat[20] = 0x1;//������
	}else{
		memcpy(&cmd_dat[2], p->m_setSN, 19);
		cmd_dat[21] = 0x1;//������
	}
	if(p->m_setSN.GetLength() == 18)
		pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 21);//�ٴ�һλsn��
	else
		pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 22);

	//tSend = tNow + CTimeSpan(0,0,0,1);
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START, 0);
	p->WorkerThreadProc_exit = 0;
	while(p->WorkerThreadProc_exit == 0){
		if(0 == m_start_resp) {
			if(timeout >= 5){
				SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START_TIMEOUT, 0);
				goto out;
			}
			tNow = CTime::GetCurrentTime();
			timeSpan = tNow - tSend;
			if( timeSpan.GetTotalSeconds() >= 1){
				comInfo->WriteCommBlock((LPSTR)&send_pkt, send_pkt.cmdlen);
				tSend = tNow;
				timeout ++;
			}
		}

		rdsize = comInfo->ReadCommBlock((LPSTR)&pRdbuf[readed], COM_MAX_RDSZIZE-readed);
		if(rdsize > 0) {
			readed += rdsize;
		}

ReParse:
		if(readed > 8) {
			olen = 0;
			memset(&cmdPkt, 0, sizeof(cmdPkt));
			result = pto_Parse(pRdbuf, readed, &cmdPkt, &olen);
			if(olen > 0) {
				if(readed > olen) {
					readed -= olen;
					memcpy(&pRdbuf[0], &pRdbuf[olen], readed);
				}
				else readed = 0;
			}

			if(result == NPTO_PARSE_RESULT_SUCC) {
				switch(cmdPkt.command) {
				case CMD_RESP_SET_AUDIO:
					if(cmdPkt.u_dat[2] == 0x01) {
						m_start_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x00) {
						m_stop_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x02) {
						m_start_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x03) {
						m_start_resp = 1;
						timeout = 0;
					}
					break;

				case CMD_BLE_AUD_DAT:
					pktdatlen = cmdPkt.cmdlen-NPTO_HEAD_SIZE;
					memcpy(&audio_right_buf[audio_right_len], cmdPkt.u_dat, pktdatlen);
					audio_right_len += pktdatlen;					
					p->audMsec_right = (audio_right_len * 1000) / (16000 * 2);
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_AUDIO_LENGTH, 0);
					break;
				case CMD_BLE_AUD_DAT2:
					pktdatlen = cmdPkt.cmdlen-NPTO_HEAD_SIZE;
					memcpy(&audio_left_buf[audio_left_len], cmdPkt.u_dat, pktdatlen);
					audio_left_len += pktdatlen;					
					p->audMsec_left = (audio_left_len * 1000) / (16000 * 2);
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_AUDIO_LENGTH, 0);
					break;

				case CMD_RESP_GET_TFA9896_VALUE:     // 9896У��ֵ
					//p->m_iTFA8686val = cmdPkt.u_dat16[1];
					//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_TFA9896_VALUE, 0);
					break;

				case CMD_RESP_MAC_SN:           // �����忨SNֵ
					p->m_sSN.Format("%s", (char*)&cmdPkt.u_dat[8]);
					p->m_sMac.Format("%02x:%02x:%02x:%02x:%02x:%02x", 
						cmdPkt.u_dat[2],cmdPkt.u_dat[3],cmdPkt.u_dat[4],
						cmdPkt.u_dat[5],cmdPkt.u_dat[6],cmdPkt.u_dat[7]);
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MAC_SN, 0);    //check  SN�ź�������ַ
					break;
				}//switch
				goto ReParse;
			}
		}//if readed
		if((p->audMsec_right + p->audMsec_left) >= 8*1000)   {//������������ʱ�����������8���˳�
			if(0 == m_stop_resp) {
				if(timeout >= 5){
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_STOP_TIMEOUT, 0);
					goto out;
				}
				tNow = CTime::GetCurrentTime();
				timeSpan = tNow - tSend;
				if( timeSpan.GetTotalSeconds() >= 1){
					cmd_dat[0] = 0x00; cmd_dat[1] = 0;
					pto_GenerateRaw(&cmdPkt, CMD_SET_AUDIO_START, 1235, cmd_dat, 2);
					comInfo->WriteCommBlock((LPSTR)&cmdPkt, cmdPkt.cmdlen);
					tSend = CTime::GetCurrentTime();
					timeout ++;
				}
			}else{
				break;
			}
		}
		Sleep(1);
	}//while

	tNow = CTime::GetCurrentTime();
	sText.Format("audio\\air_fz\\Aud_%s_%2d%02d%02d", p->m_sSN,
		tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	if(p->WorkerThreadProc_exit == 1){
		p->aud_left_db = 0;
		p->aud_right_db = 0;
		goto disconnect_out;
	}
	if(audio_right_len > 0){
		p->aud_right_db = getAvgPcmDB((INT16*)audio_right_buf, audio_right_len / 2);
		p->aud_right_file_path.Format("%s_%dDB.pcm", sText, p->aud_right_db);
		if(p->aud_right_f.Open(p->aud_right_file_path, CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite) == TRUE) {
			p->aud_right_f.Write(audio_right_buf, audio_right_len);
			p->aud_right_f.Close();
		}
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_GET_RIGHTAUD_FILE, 0);
	}
	if(audio_left_len > 0){
		p->aud_left_db = getAvgPcmDB((INT16*)audio_left_buf, audio_left_len / 2);
		p->aud_left_file_path.Format("%s_2_%dDB.pcm", sText, p->aud_left_db);
		if(p->aud_left_f.Open(p->aud_left_file_path, CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite) == TRUE) {
			p->aud_left_f.Write(audio_left_buf, audio_left_len);
			p->aud_left_f.Close();
		}
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_GET_LEFTAUD_FILE, 0);
	}
disconnect_out:
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);

	cmd_dat[0] = 0x00; cmd_dat[1] = 0;
	pto_GenerateRaw(&cmdPkt, CMD_SET_AUDIO_START, 1235, cmd_dat, 2);
	comInfo->WriteCommBlock((LPSTR)&cmdPkt, cmdPkt.cmdlen);
out:
	delete pRdbuf;
	delete audio_right_buf;
	delete audio_left_buf;

	return 0;
}

void software_air_fz::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	bluetooth_option_instance = g_p->bluetooth_option_instance;
}

void software_air_fz::dlg_pram_init()
{
	pass_green_color = 1;
	calibration_flag = 0;
	m_audiodb_right.SetWindowTextA("");
	m_audiodb_left.SetWindowTextA("");
	m_Echavalue.SetWindowTextA("");
	m_ECpass.SetWindowTextA("");
	m_Esetsn.SetWindowTextA("");
	m_sMacAddr.SetWindowTextA("");
	m_snValue.SetWindowTextA("");
	//m_iTFAVal.SetWindowTextA("");
	//m_Ecycle.SetWindowTextA("");

	GetDlgItem(IDC_AUDIO_SEC)->SetWindowTextA("");
	m_listEvt.ResetContent();
	m_Esetsn.SetFocus();
	AddEvent2List("#####�ȴ��û�������");
}
// software_bluetooth_fz ��Ϣ�������

void software_air_fz::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	dlg_pram_init();

}
extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_air_fz::OnEnChangeSetSn()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	DWORD idThread;
	CString sText;
	UpdateData(TRUE);
	m_Esetsn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("��ǰ��ƷSN��(v2)����ȷ");
			}else{
				m_setSN = sText;
				((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
				hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_fz::WorkerThreadProc, this, 0, &idThread);
				idThread  = idThread;
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("��ǰ��ƷSN��(v1)����ȷ");
		}else{
			m_setSN = sText;
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
			hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_air_fz::WorkerThreadProc, this, 0, &idThread);
			idThread  = idThread;
		}
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


BOOL software_air_fz::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CreateDirectory("audio", NULL);
	CreateDirectory("audio\\air_fz", NULL);

	CString dir_path;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	dir_path = LOG_PATH_AIR_fz;

	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

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
	m_ECpass.SetFont(&m_editFont);

	dlg_pram_init();

	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH software_air_fz::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void software_air_fz::OnBnClickedButton2()
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


void software_air_fz::OnBnClickedButton1()
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


void software_air_fz::OnBnClickedButton17()
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


void software_air_fz::OnBnClickedButton14()
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


void software_air_fz::OnBnClickedButton15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	list_audio.ResetContent();
}


BOOL software_air_fz::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


void software_air_fz::OnBnClickedButton20()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WorkerThreadProc_exit = 1;
}
