// software_tool_barcode.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_barcode.h"
#include "afxdialogex.h"


// software_tool_barcode 对话框

IMPLEMENT_DYNAMIC(software_tool_barcode, CDialogEx)

software_tool_barcode::software_tool_barcode(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_barcode::IDD, pParent)
{

}

software_tool_barcode::~software_tool_barcode()
{
}

void software_tool_barcode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BLUETOOTH, m_sMacAddr);
	DDX_Control(pDX, IDC_STATIC_SN, m_snValue);
}


BEGIN_MESSAGE_MAP(software_tool_barcode, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_tool_barcode::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON3, &software_tool_barcode::OnBnClickedButton3)
END_MESSAGE_MAP()

LRESULT software_tool_barcode::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	CRect rect;
	CDC *pDC;
	CWnd *pWnd;
	switch((INT)wParam) {
		case WPARAM_START:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("正在采集中。。。");
			break;
		case WPARAM_UART_ERR:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			AfxMessageBox("串口打开失败");
			break;
		case WPARAM_START_TIMEOUT:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("超时，点击继续");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			break;
		case WPARAM_MAC_SN:
			m_sMacAddr.SetWindowTextA(m_sMac);
			m_snValue.SetWindowTextA(m_sSN);
			break;
		case WPARAM_THREAD_EXITED:
			pWnd=GetDlgItem(IDC_STATIC_barcode); //获得pictrue控件窗口的句柄
			pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域
			pDC=pWnd->GetDC(); //获得pictrue控件的DC
			pDC->FillSolidRect(rect, RGB(255, 255, 255));
			m_barcode.out_draw_bar(pDC, rect, m_sSN);
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("成功，点击继续");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			break;

		default:
			break;
	}
	return 0;
}

int software_tool_barcode::WorkerThreadProc(void *param)
{
	CString sText;
	int timeout = 0;
	int getboard_end = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	NetProto send_pkt,cmdPkt;
	int m_start_resp = 0;
	int m_stop_resp = 0;
	unsigned char cmd_dat[26];
	CTime tNow;
	CTime tSend;
	CTimeSpan timeSpan;
	software_tool_barcode *p = (software_tool_barcode*)param;
	CContrlComm_bluetooth *comInfo = &(p->m_ctlCom);
	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//串口buf
	int getsn_succ_end = 0;

	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	comInfo->baudCom = p->general_option_instance.g_ttycom_baud;
	
	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, comInfo->baudCom)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
		goto out;
	}else{
		sText.Format("打开端口%d成功",comInfo->portCom);
	}
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START, 0);

	memset(cmd_dat, 0, sizeof(cmd_dat));
	cmd_dat[0] = 0x02;
	pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 2);

	//tSend = tNow + CTimeSpan(0,0,0,1);
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START, 0);
	while(1){
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
				case CMD_RESP_MAC_SN:           // 反馈板卡SN值
					p->m_sSN.Format("%s", (char*)&cmdPkt.u_dat[8]);
					p->m_sMac.Format("%02x:%02x:%02x:%02x:%02x:%02x", 
						cmdPkt.u_dat[2],cmdPkt.u_dat[3],cmdPkt.u_dat[4],
						cmdPkt.u_dat[5],cmdPkt.u_dat[6],cmdPkt.u_dat[7]);
					if(p->m_sSN.Find("-") < 0){
						p->m_sSN = p->m_sSN.Right(18);
					}
					SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MAC_SN, 0);    //check  SN号和蓝牙地址
					getsn_succ_end = 1;
					break;
				}//switch
				goto ReParse;
			}
		}//if readed
		if(getsn_succ_end == 1) {
			cmd_dat[0] = 0x00; cmd_dat[1] = 0;
			pto_GenerateRaw(&cmdPkt, CMD_SET_AUDIO_START, 1235, cmd_dat, 2);
			comInfo->WriteCommBlock((LPSTR)&cmdPkt, cmdPkt.cmdlen);
			break;
		}
		Sleep(1);
	}//while


	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	comInfo->EndComm();
out:
	//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	delete pRdbuf;
	return 0;
}
// software_tool_box_audio 消息处理程序

void software_tool_barcode::get_param(struct glob_param *g_p)
{
	general_option_instance = g_p->general_option_instance;
}

void software_tool_barcode::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s_command;
	DWORD idThread;
	HANDLE        hThread;

	((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE);
	hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_barcode::WorkerThreadProc, this, 0, &idThread);
			//CWnd *pWnd=GetDlgItem(IDC_STATIC_barcode); //获得pictrue控件窗口的句柄
			//CRect rect;
			//pWnd->GetClientRect(&rect); //获得pictrue控件所在的矩形区域
			//CDC *pDC=pWnd->GetDC(); //获得pictrue控件的DC
			//pDC->FillSolidRect(rect, RGB(255, 255, 255));
			//m_barcode.out_draw_bar(pDC, rect, "A01A0A180801000001");
}

BOOL software_tool_barcode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "sn号条码获取工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}
