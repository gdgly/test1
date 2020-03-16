// software_by_before_press_current.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_by_before_press_current.h"
#include "afxdialogex.h"


// software_by_before_press_current 对话框

IMPLEMENT_DYNAMIC(software_by_before_press_current, CDialogEx)

software_by_before_press_current::software_by_before_press_current(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_by_before_press_current::IDD, pParent)
{

}

software_by_before_press_current::~software_by_before_press_current()
{
}

void software_by_before_press_current::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listEvt);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_STATIC10, m_Epass);
	DDX_Control(pDX, IDC_EDIT_SLEEP_CURRENT3, m_current_value);
}

BEGIN_MESSAGE_MAP(software_by_before_press_current, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT2, &software_by_before_press_current::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON13, &software_by_before_press_current::OnBnClickedButton13)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void software_by_before_press_current::AddEvent2List(CString sText)
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
// software_by_before_press_current 消息处理程序

int software_by_before_press_current::WorkerThreadProc(void *param)
{
	CString sText;
	software_by_before_press_current *p = (software_by_before_press_current*)param;
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(FALSE);
	CContrlComm *pComCheck = &(p->comInfo);
	if(FALSE == pComCheck->InitComm(p->bp_current_by_option_instance.g_currentcom_port, DEIGHT, SONE, PNONE, 
			p->bp_current_by_option_instance.g_currentcom_baud)) {
			AfxMessageBox("串口打开失败！");
			p->AddEvent2List("串口打开失败！");
			goto out;
	}else {
			pComCheck->SendComand(CHARG_SHUT_COM);      // 关闭充电
			p->com_thread_quit = false;
			p->AddEvent2List("串口打开成功！");
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_by_before_press_current::COMThreadProc, param, 0, NULL);
	}

	pComCheck->SendComand(CURRENT_UART_CLOSE);
	Sleep(1000);
	p->sleep_current = pComCheck->GetSleepCurrent();
	sText.Format("%d",p->sleep_current);
	p->m_current_value.SetWindowTextA(sText);
	if(p->sleep_current > p->bp_current_by_option_instance.g_sleep_current_min &&
		p->sleep_current < p->bp_current_by_option_instance.g_sleep_current_max){
			p->AddEvent2List("测试结束，等待用户复位! ");
			p->pass_green_color = 1;
			p->m_Epass.SetWindowTextA("PASS");
			p->Invalidate();
	}else{
		for(int i=0; i<3; i++){
			CString str_outcur;
			str_outcur.Format("err value = %d", p->sleep_current);
			p->AddEvent2List(str_outcur);
			p->sleep_current = pComCheck->GetSleepCurrent();
			sText.Format("%d",p->sleep_current);
			p->m_current_value.SetWindowTextA(sText);
			Sleep(1000);
		}
		if(p->sleep_current > p->bp_current_by_option_instance.g_sleep_current_min &&
			p->sleep_current < p->bp_current_by_option_instance.g_sleep_current_max){
			p->AddEvent2List("测试结束，等待用户复位! ");
			p->pass_green_color = 1;
			p->m_Epass.SetWindowTextA("PASS");
			p->Invalidate();		
		}else{
			p->AddEvent2List("测试结束，等待用户复位! ");
			p->pass_green_color = 0;
			p->m_Epass.SetWindowTextA("FAIL");
			p->Invalidate();		
		}
	}

out:
	((CButton*)p->GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
	p->com_thread_quit = true;
	return 0;
}

int software_by_before_press_current::COMThreadProc(void *param)
{
	software_by_before_press_current *p = (software_by_before_press_current*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	//msg_by_check_info *msg_p;
	while( !p->com_thread_quit ){
		pComCheck->Process();
		Sleep(1);
	}
	pComCheck->EndComm();
	return 0;
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_by_before_press_current::OnEnChangeEdit2()
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
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_by_before_press_current::WorkerThreadProc, this, 0, NULL);
				AddEvent2List("sn号输入成功，准备连接");
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
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_by_before_press_current::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn号输入成功，准备连接");
		}

	}
	// TODO:  在此添加控件通知处理程序代码
}

void software_by_before_press_current::all_init()
{
	m_EditSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	pass_green_color = 0;

	m_EditSn.SetFocus();
	m_listEvt.ResetContent();
	AddEvent2List("#####机器进入休眠后输入sn号开始测试#####");
}

void software_by_before_press_current::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	all_init();
}

void software_by_before_press_current::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	bp_current_by_option_instance = g_p->bp_current_by_option_instance;
}

BOOL software_by_before_press_current::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	CString dir_path;
	dir_path = LOG_PATH_BP_CURRENT_by;

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

	m_editFont.CreatePointFont(700, "宋体");
	m_Epass.SetFont(&m_editFont);

	all_init();
	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL software_by_before_press_current::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH software_by_before_press_current::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
