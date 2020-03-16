// software_tool_current_board.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_current_board.h"
#include "afxdialogex.h"


// software_tool_current_board 对话框

IMPLEMENT_DYNAMIC(software_tool_current_board, CDialogEx)

software_tool_current_board::software_tool_current_board(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_current_board::IDD, pParent)
{

}

software_tool_current_board::~software_tool_current_board()
{
}

void software_tool_current_board::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, edit_out);
}


BEGIN_MESSAGE_MAP(software_tool_current_board, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_tool_current_board::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON1, &software_tool_current_board::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON18, &software_tool_current_board::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON8, &software_tool_current_board::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &software_tool_current_board::OnBnClickedButton9)
END_MESSAGE_MAP()

LRESULT software_tool_current_board::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	char buffer[4096];
	switch((INT)wParam) {
		case WPARAM_START:
			break;
		case WPARAM_UART_ERR:
			AfxMessageBox("串口打开失败");
			break;
		case WPARAM_START_TIMEOUT:
			break;
		case WPARAM_THREAD_EXITED:
			memset(buffer,0,sizeof(buffer));
			memcpy(buffer, buf, buf_len);
			edit_out.ReplaceSel(buffer);
			display_finish_flag = TRUE;
			break;

		default:
			break;
	}
	return 0;
}

int software_tool_current_board::WorkerThreadProc(void *param)
{
	CString sText;
	int timeout = 0;
	int getboard_end = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	CTime tNow;
	CTime tEnd;
	CTimeSpan timeSpan;
	software_tool_current_board *p = (software_tool_current_board*)param;
	CContrlComm *comInfo = &(p->m_ctlCom);
	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//串口buf

	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	//comInfo->baudCom = p->general_option_instance.g_ttycom_baud;
	
	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, p->general_option_instance.g_ttycom_baud)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
		goto out;
	}else{
		sText.Format("打开端口%d成功",comInfo->portCom);
	}
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START, 0);
	p->display_finish_flag = TRUE;
	while(1){
		rdsize = comInfo->ReadCommBlock((LPSTR)&pRdbuf[readed], COM_MAX_RDSZIZE-readed);
		if(rdsize > 0) {
			readed += rdsize;
		}
		if(readed > 8 && p->display_finish_flag) {
			memcpy(p->buf, pRdbuf, readed);
			p->buf_len = readed;
			readed = 0;
			p->display_finish_flag = FALSE;
			SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
		}
		Sleep(1);
	}

out:
	//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	delete pRdbuf;
	return 0;
}
// software_tool_box_audio 消息处理程序

void software_tool_current_board::get_param(struct glob_param *g_p)
{
	general_option_instance = g_p->general_option_instance;
}

// software_tool_current_board 消息处理程序


BOOL software_tool_current_board::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "电流板测试工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	DWORD idThread;
	HANDLE        hThread;
	hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_current_board::WorkerThreadProc, this, 0, &idThread);

	return TRUE;  // return TRUE unless you set the focus to a control
}


void software_tool_current_board::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ctlCom.idComDev != INVALID_HANDLE_VALUE){
		m_ctlCom.SendComand(CHARG_OPEN_COM);
	}
}


void software_tool_current_board::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ctlCom.idComDev != INVALID_HANDLE_VALUE){
		m_ctlCom.SendComand(CHARG_SHUT_COM);
	}
}


void software_tool_current_board::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ctlCom.idComDev != INVALID_HANDLE_VALUE){
		m_ctlCom.SendComand(CURRENT_UART_OPEN);
	}
}


void software_tool_current_board::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_ctlCom.idComDev != INVALID_HANDLE_VALUE){
		m_ctlCom.SendComand(CURRENT_UART_CLOSE);
	}
}
