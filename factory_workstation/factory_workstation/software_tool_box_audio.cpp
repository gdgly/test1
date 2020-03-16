// software_tool_box_audio.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_box_audio.h"
#include "afxdialogex.h"


// software_tool_box_audio �Ի���

IMPLEMENT_DYNAMIC(software_tool_box_audio, CDialogEx)

software_tool_box_audio::software_tool_box_audio(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_box_audio::IDD, pParent)
{

}

software_tool_box_audio::~software_tool_box_audio()
{
}

void software_tool_box_audio::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SET_SN, m_Esetsn);
	DDX_Control(pDX, IDC_SET_SN2, m_command);
}


BEGIN_MESSAGE_MAP(software_tool_box_audio, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_tool_box_audio::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON3, &software_tool_box_audio::OnBnClickedButton3)
END_MESSAGE_MAP()
LRESULT software_tool_box_audio::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	switch((INT)wParam) {
		case WPARAM_START:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("���ڲɼ��С�����");
			break;
		case WPARAM_UART_ERR:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("��λ�ɼ�");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			AfxMessageBox("���ڴ�ʧ��");
			break;
		case WPARAM_START_TIMEOUT:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("��ʱ���������");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			break;
		case WPARAM_THREAD_EXITED:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("�ɹ����������");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			break;

		default:
			break;
	}
	return 0;
}

int software_tool_box_audio::WorkerThreadProc(void *param)
{
	CString sText;
	int timeout = 0;
	int getboard_end = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	NetProto cmdPkt;
	unsigned char cmd_dat[26];
	CTime tNow;
	CTime tEnd;
	CTimeSpan timeSpan;
	software_tool_box_audio *p = (software_tool_box_audio*)param;
	CContrlComm_bluetooth *comInfo = &(p->m_ctlCom);
	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//����buf
	unsigned short command = p->g_command;
	unsigned char *p_temp;

	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	comInfo->baudCom = p->general_option_instance.g_ttycom_baud;
	
	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, comInfo->baudCom)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
		goto out;
	}else{
		sText.Format("�򿪶˿�%d�ɹ�",comInfo->portCom);
	}
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START, 0);

	memset(cmd_dat, 0, sizeof(cmd_dat));
	if(p->m_chkSN.GetLength() == 18){
		memcpy(&cmd_dat[0], p->m_chkSN, 18);
		p_temp = (unsigned char *)&command;
		cmd_dat[18] = *(p_temp+1);
		cmd_dat[19] = *p_temp;
		//pto_GenerateRaw(&cmdPkt, 0x7, 1234, cmd_dat, 21);
		pto_GenerateRaw(&cmdPkt, 0x10, 1234, cmd_dat, 20);
	}else{
		memcpy(&cmd_dat[0], p->m_chkSN, 19);
		p_temp = (unsigned char *)&command;
		cmd_dat[19] = *(p_temp+1);
		cmd_dat[20] = *p_temp;
		//pto_GenerateRaw(&cmdPkt, 0x7, 1234, cmd_dat, 21);
		pto_GenerateRaw(&cmdPkt, 0x10, 1234, cmd_dat, 21);
	}

	comInfo->WriteCommBlock((LPSTR)&cmdPkt, cmdPkt.cmdlen);

	tNow = CTime::GetCurrentTime();
	while(1){
		rdsize = comInfo->ReadCommBlock((LPSTR)&pRdbuf[readed], COM_MAX_RDSZIZE-readed);
		if(rdsize > 0) {
			readed += rdsize;
		}
		if(readed > 8) {
			olen = 0;
			memset(&cmdPkt, 0, sizeof(cmdPkt));
			result = pto_Parse(pRdbuf, readed, &cmdPkt, &olen);
			//if(cmdPkt.command == 0x87){
			//	break;//common ����ɹ�
			//}
			if(cmdPkt.command == 0x5 || cmdPkt.command == 0x90){
				break;//common ����ɹ�
			}
		}
		tEnd = CTime::GetCurrentTime();
		timeSpan = tEnd - tNow;
		int nTSeconds = (int)( timeSpan.GetTotalSeconds() );
		if(nTSeconds > 2){
			SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START_TIMEOUT, 0);
			goto out;//common ���ճ�ʱ��
		}
	}

	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	comInfo->EndComm();
out:
	//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	delete pRdbuf;
	return 0;
}
// software_tool_box_audio ��Ϣ�������

void software_tool_box_audio::get_param(struct glob_param *g_p)
{
	general_option_instance = g_p->general_option_instance;
}

void software_tool_box_audio::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned int temp;
	CString s_command;
	DWORD idThread;
	HANDLE        hThread;
	m_Esetsn.GetWindowTextA(m_chkSN);
	if(m_chkSN.GetLength() != 18 && m_chkSN.GetLength() != 19){
		AfxMessageBox("sn�����벻��ȷ");
		return;
	}
	m_command.GetWindowTextA(s_command);
	StrToIntEx(s_command, STIF_SUPPORT_HEX, (int*)&temp);
	g_command = (unsigned short)temp;

	((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE);
	hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_box_audio::WorkerThreadProc, this, 0, &idThread);
}

BOOL software_tool_box_audio::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString soft_name;
	soft_name += "box ɨƵ����";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);
	
	m_command.SetWindowTextA("0xa290");

	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_tool_box_audio::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
