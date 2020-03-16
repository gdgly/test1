// software_tool_version.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_version.h"
#include "afxdialogex.h"


// software_tool_version 对话框

IMPLEMENT_DYNAMIC(software_tool_version, CDialogEx)

software_tool_version::software_tool_version(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_version::IDD, pParent)
{

}

software_tool_version::~software_tool_version()
{
}

void software_tool_version::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_version, m_static_version);
}


BEGIN_MESSAGE_MAP(software_tool_version, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_tool_version::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON3, &software_tool_version::OnBnClickedButton3)
END_MESSAGE_MAP()

LRESULT software_tool_version::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	CString sText;
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
		case WPARAM_THREAD_EXITED:
			m_static_version.SetWindowTextA(s_version);
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("成功，点击继续");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			break;

		default:
			break;
	}
	return 0;
}

int software_tool_version::WorkerThreadProc(void *param)
{
	CString sText;
	int timeout = 0;
	int getboard_end = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	NetProto cmdPkt;
	unsigned char cmd_dat[26];
	char buf[9];
	CTime tNow;
	CTime tEnd;
	CTimeSpan timeSpan;
	software_tool_version *p = (software_tool_version*)param;
	CContrlComm_bluetooth *comInfo = &(p->m_ctlCom);
	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//串口buf

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

	pto_GenerateRaw(&cmdPkt, 0x8, 1234, cmd_dat, 0);

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
			if(cmdPkt.command == 0x88){
				memset(buf, 0, sizeof(buf));
				memcpy(buf, cmdPkt.u._dat, 8);
				p->s_version.Format("%s",buf);
				break;//common 传输成功
			}
		}
		tEnd = CTime::GetCurrentTime();
		timeSpan = tEnd - tNow;
		int nTSeconds = (int)(timeSpan.GetTotalSeconds());
		if(nTSeconds > 2){
			SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START_TIMEOUT, 0);
			goto out;//common 接收超时；
		}
	}

	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	comInfo->EndComm();
out:
	//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	delete pRdbuf;
	return 0;
}
// software_tool_box_audio 消息处理程序

void software_tool_version::get_param(struct glob_param *g_p)
{
	general_option_instance = g_p->general_option_instance;
}

void software_tool_version::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString s_command;
	DWORD idThread;
	HANDLE        hThread;

	((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(FALSE);
	hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_version::WorkerThreadProc, this, 0, &idThread);
}

BOOL software_tool_version::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "测试板固件版本检测工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);
	
	m_static_version.SetWindowTextA("");

	return TRUE;  // return TRUE unless you set the focus to a control
}

