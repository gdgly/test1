// software_bluetooth_jn.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_bluetooth_jn.h"
#include "afxdialogex.h"


// software_bluetooth_jn 对话框

IMPLEMENT_DYNAMIC(software_bluetooth_jn, CDialogEx)

software_bluetooth_jn::software_bluetooth_jn(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_bluetooth_jn::IDD, pParent)
{

}

software_bluetooth_jn::~software_bluetooth_jn()
{
}

void software_bluetooth_jn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC10, m_ECpass);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
	DDX_Control(pDX, IDC_SET_SN, m_Esetsn);
	DDX_Control(pDX, IDC_MAC_ADDR, m_sMacAddr);
	DDX_Control(pDX, IDC_SN_VALUE, m_snValue);
	//DDX_Control(pDX, IDC_TFA9896_VAL, m_iTFAVal);
	DDX_Control(pDX, IDC_AUDIO_DB_RIGHT, m_audiodb_right);
	//DDX_Control(pDX, IDC_AUDIO_DB_LEFT, m_audiodb_left);
	DDX_Control(pDX, IDC_LIST1, list_audio);
	DDX_Control(pDX, IDC_AUDIO_CYCLE, m_Ecycle);
}

BEGIN_MESSAGE_MAP(software_bluetooth_jn, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_bluetooth_jn::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON13, &software_bluetooth_jn::OnBnClickedButton13)
	ON_EN_CHANGE(IDC_SET_SN, &software_bluetooth_jn::OnEnChangeSetSn)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON2, &software_bluetooth_jn::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &software_bluetooth_jn::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON17, &software_bluetooth_jn::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON14, &software_bluetooth_jn::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &software_bluetooth_jn::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON20, &software_bluetooth_jn::OnBnClickedButton20)
END_MESSAGE_MAP()

void software_bluetooth_jn::AddEvent2List(CString sText)
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

INT software_bluetooth_jn::MesPass()   //每一块成品测完以后，点击PASS上传MES
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1;
	UINT a;
	if(mesIptr->Messtatus == 1){
		if(mesIptr->bluetooth_SendChecke(m_sSN) == 0){
			sText.Format(_T("mes返回数据:%s"),mesIptr->line);
			sText1.Format(_T("mes数据判断结果:%d"),mesIptr->Messtatus);
			a = sText.Find("}");
			if(a >= 0)
				sText = sText.Left(a+1);

			AddEvent2List(sText);
			AddEvent2List(sText1);
		}else
			AddEvent2List("mes发送失败！");
	}

	AddEvent2List("\r\n");
	if(mesIptr->Messtatus == 3){
		return 1;
	}else{
		AfxMessageBox("mes返回数据错误，不可以继续check！ ");
	}
	return 0;
}

LRESULT software_bluetooth_jn::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	int cycle_flag;
	UINT a;
	int count;
	CString sText;
	switch((INT)wParam) {
		case WPARAM_START:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("正在采集中。。。");
			AddEvent2List("开始采集！");
			break;
		case WPARAM_UART_ERR:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			AfxMessageBox("串口打开失败");
			AddEvent2List("串口打开失败");
			break;
		case WPARAM_START_TIMEOUT:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			AfxMessageBox("接收开始反馈超时"); 
			AddEvent2List("接收开始反馈超时");
			break;
		case WPARAM_STOP_TIMEOUT:
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			AfxMessageBox("接收停止反馈超时"); 
			AddEvent2List("接收停止反馈超时");
			break;
		case WPARAM_AUDIO_LENGTH:
			sText.Format("%d.%d 秒", audMsec_right/1000, (audMsec_right%1000)/100);
			GetDlgItem(IDC_AUDIO_SEC)->SetWindowTextA(sText);
			break;
		case  WPARAM_MAC_SN:
			if( m_sSN.Find("-") < 0 ){
				m_sSN = m_sSN.Right(18);
			}
			m_sMacAddr.SetWindowTextA(m_sMac);
			m_snValue.SetWindowTextA(m_sSN);
			AddEvent2List("mac sn号获取成功");
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
			sText.Format("%d", cycle);
			m_Ecycle.SetWindowTextA(sText);
			if(bluetooth_option_instance.g_check_cycle_jn){
				if(cycle >= (int)bluetooth_option_instance.g_cycle_min_jn &&
					cycle <= (int)bluetooth_option_instance.g_cycle_max_jn){
					cycle_flag = 1;
				}else{
					cycle_flag = 0;
				}
			}else{
				cycle_flag = 1;
			}
			if(aud_right_db >= (int)bluetooth_option_instance.g_db_value_jn &&
					cycle_flag == 1){
				if( general_option_instance.g_check_mes ){
					if(general_option_instance.g_check_mes_type == 1){
						if(MesPass() == 1){
							pass_green_color = 1;
							m_ECpass.SetWindowTextA("PASS");
							AddEvent2List("#####检测成功######");
						}else{
							pass_green_color = 0;
							m_ECpass.SetWindowTextA("FAIL");	
							AddEvent2List("#####检测失败######");
						}
					}else if(general_option_instance.g_check_mes_type == 2){
						struct ch_mes_data ch_mes_data; 
						ch_mes_data.data_flag = 3;
						mesInfo.changh_mes_int(m_sSN, ch_mes_data);
						pass_green_color = 1;
						m_ECpass.SetWindowTextA("PASS");
						AddEvent2List("#####检测成功######");
					}
				}else{
					pass_green_color = 1;
					m_ECpass.SetWindowTextA("PASS");
					AddEvent2List("#####检测成功######");
				}
			}else{
				pass_green_color = 0;
				m_ECpass.SetWindowTextA("FAIL");	
				AddEvent2List("#####检测失败######");
			}
			Invalidate();
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case WPARAM_MES_STRING:
			sText.Format(_T("mes返回数据:%s"),Mesresring);
			a = sText.Find("}");
			if(sText.Find("}") >= 0)
				sText = sText.Left(a + 1);
			AddEvent2List(sText);
			break;
		case WPARAM_MES_ERR:
			AddEvent2List("mes发送失败！");
			pass_green_color = 0;
			m_ECpass.SetWindowTextA("FAIL");	
			AddEvent2List("#####检测失败######");
			Invalidate();
			((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
			break;
		case WPARAM_SN:
			sText.Format(_T("SN号:%s mes数据判断结果:%d"),m_sSN,mesInfo.Messtatus);
			AddEvent2List(sText);
			break;
		default:
			break;
	}
	return 0;
}

extern int getAvgPcmDB(INT16  *pcmdata,  long nmemb);
int max_count(INT16 *pcmdata,  long nmemb)
{	
	int i, count = 0;
	int state = -1;
	for(i = 1; i < nmemb-1; i++){
		if(pcmdata[i] >0){
			if(pcmdata[i]>pcmdata[i-1] && pcmdata[i]>pcmdata[i+1]){
				if(state == -1){
					state = 1;
					//fprintf(fp , "%d\n", pcmdata[i]);
				}else{
					continue;
				}
			}	
		}
		if(pcmdata[i] < 0){
			if(pcmdata[i]<pcmdata[i-1] && pcmdata[i]<pcmdata[i+1]){
				if(state == 1){
					//fprintf(fp , "%d\n", pcmdata[i]);
					count++;
					state = -1;
				}else{
					continue;
				}
			}	
		}
	}
	return count;
}

int software_bluetooth_jn::WorkerThreadProc(void *param)
{
	CString sText;
	int timeout = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	int m_start_resp = 0;
	int m_stop_resp = 0;
	unsigned short outSize;
	NetProto send_pkt,cmdPkt;
	unsigned char cmd_dat[26];
	CTime tNow,tSend;
	CTimeSpan timeSpan;
	software_bluetooth_jn *p = (software_bluetooth_jn*)param;
	CContrlComm_bluetooth *comInfo = &(p->m_ctlCom);

	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//串口buf
	int audio_right_len = 0;
	int audio_decode_len = 0;
	BYTE *audio_right_buf = new BYTE[1<<20];//右声道buf
	BYTE *audio_decode_buf = new BYTE[1<<20];//解码buf
	p->audMsec_right = 0;
	p->audMsec_left = 0;

	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	comInfo->baudCom = p->general_option_instance.g_ttycom_baud;

	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, comInfo->baudCom)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
		goto out;
	}else{
		sText.Format("打开端口%d成功",comInfo->portCom);
		p->AddEvent2List(sText);
	}

	memset(cmd_dat, 0, sizeof(cmd_dat));
	cmd_dat[0] = 0x01;
	if(p->m_setSN.GetLength() == 18){
		memcpy(&cmd_dat[2], p->m_setSN, 18);
	}else{
		memcpy(&cmd_dat[2], p->m_setSN, 19);
	}
	if(p->m_setSN.GetLength() == 18)
		pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 21);//少传一位sn号
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
					p->audMsec_right = (audio_right_len * 1000) / (16000 * 2 /16);
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_AUDIO_LENGTH, 0);
					break;
				case CMD_BLE_AUD_DAT2:
					//pktdatlen = cmdPkt.cmdlen-NPTO_HEAD_SIZE;
					//memcpy(&audio_left_buf[audio_left_len], cmdPkt.u_dat, pktdatlen);
					//audio_left_len += pktdatlen;					
					//p->audMsec_left = (audio_left_len * 1000) / (16000 * 2);
					//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_AUDIO_LENGTH, 0);
					break;

				case CMD_RESP_GET_TFA9896_VALUE:     // 9896校正值
					//p->m_iTFA8686val = cmdPkt.u_dat16[1];
					//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_TFA9896_VALUE, 0);
					break;

				case CMD_RESP_MAC_SN:           // 反馈板卡SN值
					p->m_sSN.Format("%s", (char*)&cmdPkt.u_dat[8]);
					p->m_sMac.Format("%02x:%02x:%02x:%02x:%02x:%02x", 
						cmdPkt.u_dat[2],cmdPkt.u_dat[3],cmdPkt.u_dat[4],
						cmdPkt.u_dat[5],cmdPkt.u_dat[6],cmdPkt.u_dat[7]);
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MAC_SN, 0);    //check  SN号和蓝牙地址
					if(p->general_option_instance.g_check_mes){
						if(p->general_option_instance.g_check_mes_type == 1){
							if(p->mesInfo.bluetooth_SendChecks(p->m_sSN) == 0){
								int a = strlen(p->mesInfo.line);
								strncpy_s(p->Mesresring,p->mesInfo.line,strlen(p->mesInfo.line));
								SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_STRING, 0);
								SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_SN, 0);
							}else{
								SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_ERR, 0);
								goto mes_out;
							}
							if(p->mesInfo.Messtatus ==1){
							}else{
								SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_ERR, 0);
								AfxMessageBox("mes返回数据错误，不可以继续check！ ");
								goto mes_out;
							}
						}
					}
					break;
				}//switch
				goto ReParse;
			}
		}//if readed
		if((p->audMsec_right) >= 4*1000)   {//左右声道接收时间加起来超过8秒退出
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
	sText.Format("audio\\bluetooth_jn\\Aud_%s_%02d%02d%02d", p->m_sSN,
		tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
	if(p->WorkerThreadProc_exit == 1){
		p->cycle = 0;
		p->aud_right_db = 0;
		goto disconnect_out;
	}
	if(audio_right_len > 0){
		p->aud_right_file_path.Format("%s_%dDB.g722", sText, p->aud_right_db);
		if(p->aud_right_f.Open(p->aud_right_file_path, CFile::modeCreate | CFile::typeBinary | CFile::modeReadWrite) == TRUE) {
			p->aud_right_f.Write(audio_right_buf, audio_right_len);
			p->aud_right_f.Close();
		}
		audio_decode_len = 0;
		while(audio_decode_len != audio_right_len*16){
			 acodec_decoder((audio_right_buf+(audio_decode_len/16)), 40,(audio_decode_buf+audio_decode_len), &outSize);
			 audio_decode_len += outSize;
		}
		p->cycle = max_count((INT16 *)audio_decode_buf, audio_decode_len / 2);
		p->aud_right_db = getAvgPcmDB((INT16 *)audio_decode_buf, audio_decode_len / 2);
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_GET_RIGHTAUD_FILE, 0);
	}

disconnect_out:
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);

mes_out:
	cmd_dat[0] = 0x00; cmd_dat[1] = 0;
	pto_GenerateRaw(&cmdPkt, CMD_SET_AUDIO_START, 1235, cmd_dat, 2);
	comInfo->WriteCommBlock((LPSTR)&cmdPkt, cmdPkt.cmdlen);
out:
	delete pRdbuf;
	delete audio_right_buf;
	delete audio_decode_buf;

	return 0;
}

void software_bluetooth_jn::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	bluetooth_option_instance = g_p->bluetooth_option_instance;
}

void software_bluetooth_jn::dlg_pram_init()
{
	pass_green_color = 1;
	//calibration_flag = 0;
	m_audiodb_right.SetWindowTextA("");
	//m_audiodb_left.SetWindowTextA("");
	m_ECpass.SetWindowTextA("");
	m_Esetsn.SetWindowTextA("");
	m_sMacAddr.SetWindowTextA("");
	m_snValue.SetWindowTextA("");
	//m_iTFAVal.SetWindowTextA("");
	m_Ecycle.SetWindowTextA("");

	GetDlgItem(IDC_AUDIO_SEC)->SetWindowTextA("");
	m_listEvt.ResetContent();
	m_Esetsn.SetFocus();
	AddEvent2List("#####等待用户操作！");
}
// software_bluetooth_fz 消息处理程序

void software_bluetooth_jn::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	dlg_pram_init();

}
extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_bluetooth_jn::OnEnChangeSetSn()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	DWORD idThread;
	CString sText;
	UpdateData(TRUE);
	m_Esetsn.GetWindowTextA(sText);
	if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		if( sText.Find("-") < 0 ){
			if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v2)不正确");
			}else{
				m_setSN = sText;
				((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
				hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_bluetooth_jn::WorkerThreadProc, this, 0, &idThread);
				idThread  = idThread;
			}
		}
	}
	if(sText.GetLength() >= 19){
		if(CheckSN(sText, general_option_instance.g_sn_prompt, 
							general_option_instance.g_sn_start, 
							general_option_instance.g_sn_count) != TRUE) {
			AfxMessageBox("当前产品SN号(v1)不正确");
		}else{
			m_setSN = sText;
			((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
			hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_bluetooth_jn::WorkerThreadProc, this, 0, &idThread);
			idThread  = idThread;
		}
	}
	// TODO:  在此添加控件通知处理程序代码
}

BOOL software_bluetooth_jn::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	CreateDirectory("audio", NULL);
	CreateDirectory("audio\\bluetooth_jn", NULL);

	CString dir_path;
	// TODO:  在此添加额外的初始化
	if(last_sel_soft.Find("方舟") >= 0){
		dir_path = LOG_PATH_BLUETOOTH_fz;
	}else if (last_sel_soft.Find("金牛") >= 0){
		dir_path = LOG_PATH_BLUETOOTH_jn;
	}
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

	//sFname.Format("LOG-calibration.txt");
	//sFname = dir_path + sFname;

	//if(m_hLog_calibration_file.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
	//	m_hLog_calibration_file.SeekToEnd();
	//	sFname.Format("===============================================\r\n");
	//	m_hLog_calibration_file.Write(sFname, sFname.GetLength());
	//}

	m_editFont.CreatePointFont(700, "宋体");
	m_ECpass.SetFont(&m_editFont);

	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;
	dlg_pram_init();

	return TRUE;  // return TRUE unless you set the focus to a control
}

HBRUSH software_bluetooth_jn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void software_bluetooth_jn::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString temp;
	m_sFileName.Empty();
	int index = list_audio.GetCurSel();
	if(index < 0){
		AfxMessageBox("没有音乐或者没选中...请添加或选择");
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


void software_bluetooth_jn::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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


void software_bluetooth_jn::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	char fliter[]="音乐文件(*.g722;*.pcm)\0*.g722;*.pcm\0"
		"所有文件(*.*)\0*.*\0\0";
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


void software_bluetooth_jn::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	int list_num = list_audio.GetCurSel();
	if(list_num < 0) {
		AfxMessageBox("请选择需要删除的音频");
		return;
	}
	list_audio.DeleteString(list_num);
}


void software_bluetooth_jn::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	list_audio.ResetContent();
}


BOOL software_bluetooth_jn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


void software_bluetooth_jn::OnBnClickedButton20()
{
	// TODO: 在此添加控件通知处理程序代码
	WorkerThreadProc_exit = 1;
}
