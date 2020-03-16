// software_charge_fzjn.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_charge_fzjn.h"
#include "afxdialogex.h"

// software_charge_fzjn 对话框

IMPLEMENT_DYNAMIC(software_charge_fzjn, CDialogEx)

software_charge_fzjn::software_charge_fzjn(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_charge_fzjn::IDD, pParent)
{
}

software_charge_fzjn::~software_charge_fzjn()
{
}

void software_charge_fzjn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC9, m_Epass);
	DDX_Control(pDX, IDC_EDIT2, m_EditSn);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT_CUR_CURRENT, m_chargeCurrect);
}


BEGIN_MESSAGE_MAP(software_charge_fzjn, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT2, &software_charge_fzjn::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &software_charge_fzjn::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

void software_charge_fzjn::AddEvent2List(CString sText)
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

void software_charge_fzjn::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	charge_option_instance = g_p->charge_option_instance;
}

int software_charge_fzjn::COMThreadProc(void *param)
{
	software_charge_fzjn *p = (software_charge_fzjn*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	//msg_by_check_info *msg_p;
	while( !p->com_thread_quit ){
		pComCheck->Process();
		Sleep(1);
	}
	pComCheck->EndComm();
	return 0;
}
enum {
	CHARGE_CURRENT_GETVALUE = 0,
	CHARGE_CURRENT_GETVALUE_2TIMES,
	CHARGE_CURRENT_SUC,
	CHARGE_CURRENT_ERR,
};
int software_charge_fzjn::WorkerThreadProc(void *param)
{
	CString sText;
	software_charge_fzjn *p = (software_charge_fzjn*)param;
	CContrlComm *pComCheck = &(p->comInfo);
	CTime tNow = CTime::GetCurrentTime();
	SqlInfo_charge info;
	CString sTime = tNow.Format(_T("%Y-%m-%d %H:%M:%S"));
	int status = CHARGE_CURRENT_GETVALUE;
	((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(FALSE);
		if(FALSE == pComCheck->InitComm(p->charge_option_instance.g_currentcom_port, DEIGHT, SONE, PNONE, 
			p->charge_option_instance.g_currentcom_baud)) {
			AfxMessageBox("电流板串口打开失败! ");
			p->AddEvent2List("电流板串口打开失败!");
			goto out;
		}else {
			pComCheck->SendComand(CHARG_OPEN_COM);
			//pComCheck->SendComand(CHARG_SHUT_COM);      // 关闭充电
			p->com_thread_quit = false;
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_charge_fzjn::COMThreadProc, param, 0, NULL);
			p->AddEvent2List("串口打开成功！开始检测电流");
		}
		while(1){
			switch(status){
				case CHARGE_CURRENT_GETVALUE:
					Sleep(1500);
					p->charge_current = pComCheck->GetChargCurrent();
					sText.Format("%d",p->charge_current);
					p->m_chargeCurrect.SetWindowTextA(sText);
					if(p->charge_current >= p->charge_option_instance.g_chargCurrent){
						p->AddEvent2List("电流检测成功！");
						status = CHARGE_CURRENT_SUC;
					}else{
						p->AddEvent2List("电流检测失败，开始第二次检测！");
						status = CHARGE_CURRENT_GETVALUE_2TIMES;
					}
					break;
				case CHARGE_CURRENT_GETVALUE_2TIMES:
					Sleep(1200);
					p->charge_current = pComCheck->GetChargCurrent();
					sText.Format("%d",p->charge_current);
					p->m_chargeCurrect.SetWindowTextA(sText);
					if(p->charge_current >= p->charge_option_instance.g_chargCurrent){
						p->AddEvent2List("电流检测成功！");
						status = CHARGE_CURRENT_SUC;
					}else{
						p->AddEvent2List("第二次电流检测失败！");
						status = CHARGE_CURRENT_ERR;
					}
					break;
				case CHARGE_CURRENT_SUC:
					p->pass_green_color = 1;
					p->m_Epass.SetWindowTextA("PASS");
					p->Invalidate();
					info.Status = "PASS";
					goto quit;
					break;
				case CHARGE_CURRENT_ERR:
					p->pass_green_color = 0;
					p->m_Epass.SetWindowTextA("FAIL");
					p->Invalidate();
					info.Status = "FAIL";
					goto quit;
					break;
				default:
					break;
			}
		}
quit:
	info.Time = sTime;
	info.SN.Format("%s",p->sSetSN);
	info.Current.Format("%d",p->charge_current);
	p->m_sqlite.InsertSqlite_charge(&info);

	pComCheck->SendComand(CHARG_SHUT_COM);
out:
	p->com_thread_quit = true;
	((CButton*)p->GetDlgItem(IDC_BUTTON1))->EnableWindow(TRUE);
	return 0;
}


// software_charge_fzjn 消息处理程序
extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
void software_charge_fzjn::OnEnChangeEdit2()
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
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_charge_fzjn::WorkerThreadProc, this, 0, NULL);
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
			CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_charge_fzjn::WorkerThreadProc, this, 0, NULL);
			AddEvent2List("sn号输入成功，准备连接");
		}

	}
	// TODO:  在此添加控件通知处理程序代码
}

void software_charge_fzjn::SetAllItemRestart(void)
{
	CString sText;
	m_EditSn.SetWindowTextA("");
	m_Epass.SetWindowTextA("");
	m_chargeCurrect.SetWindowTextA("");
	m_listEvt.ResetContent();
	AddEvent2List("###等待用户输入sn号###");
	m_EditSn.SetFocus();
}

void software_charge_fzjn::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetAllItemRestart();
}


BOOL software_charge_fzjn::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	CString dir_path;
	if(last_sel_soft.Find("方舟") >= 0){
		dir_path = LOG_PATH_CHARGE_fz;
		m_sqlite.OpenSqliteInit_charge(SQL_DB_CHARGE_NAME_FZ);
	}else if (last_sel_soft.Find("金牛") >= 0){
		dir_path = LOG_PATH_CHARGE_jn;
		m_sqlite.OpenSqliteInit_charge(SQL_DB_CHARGE_NAME_JN);
	}

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

	SetAllItemRestart();
	return TRUE;  // return TRUE unless you set the focus to a control
}


HBRUSH software_charge_fzjn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if(pWnd->GetDlgCtrlID()==IDC_STATIC9)//如果是静态编辑框
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


BOOL software_charge_fzjn::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
