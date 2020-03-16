// software_tool_sz_freq.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_sz_freq.h"
#include "afxdialogex.h"


// software_tool_sz_freq 对话框

IMPLEMENT_DYNAMIC(software_tool_sz_freq, CDialogEx)

software_tool_sz_freq::software_tool_sz_freq(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_sz_freq::IDD, pParent)
{

}

software_tool_sz_freq::~software_tool_sz_freq()
{
}

void software_tool_sz_freq::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT47, m_cap);
	DDX_Control(pDX, IDC_EDIT48, m_trim);
}


BEGIN_MESSAGE_MAP(software_tool_sz_freq, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &software_tool_sz_freq::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &software_tool_sz_freq::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON14, &software_tool_sz_freq::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &software_tool_sz_freq::OnBnClickedButton15)
	ON_MESSAGE(WM_DEV_ERROR, &software_tool_sz_freq::OnDevCtrlError)
	ON_MESSAGE(WM_DEV_REPORT, &software_tool_sz_freq::OnDevCtrlReport)
END_MESSAGE_MAP()


// software_tool_sz_freq 消息处理程序
void software_tool_sz_freq::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);
}


LRESULT software_tool_sz_freq::OnDevCtrlError(WPARAM wParam, LPARAM lParam)
{
	CString sText,stmp;
	switch(wParam){
		case ERROR_GETPORT:
			sText = "ERROR_GETPORT";
			break;
		case ERROR_GETPORT_NOPORT:
			sText = "ERROR_GETPORT_NOPORT";
			break;
		case ERROR_OPENTRANS:
			sText = "ERROR_OPENTRANS";
			break;
		case ERROR_OPEN_FLASHFILE:
			sText = "ERROR_OPEN_FLASHFILE";
			break;
		case ERROR_ERASE_START:
			sText = "ERROR_ERASE_START";
			break;
		case ERROR_ERASE_SETBANK:
			sText = "ERROR_ERASE_SETBANK";
			break;
		case ERROR_PROGRAM_START:
			sText = "ERROR_PROGRAM_START";
			break;
		case ERROR_PROGRAM_FAIL:
			sText = "ERROR_PROGRAM_FAIL";
			break;
		case ERROR_FLASH_TIMEOUT:
			sText = "ERROR_FLASH_TIMEOUT";
			break;
		case ERROR_OPEN_ENGINE:
			sText = "ERROR_OPEN_ENGINE";
			break;
		case ERROR_CACHE_INIT:
			sText = "ERROR_CACHE_INIT";
			break;
		case ERROR_CACHE_READ:
			sText = "ERROR_CACHE_READ";
			break;
		case ERROR_READ_DEVNAME:
			sText = "ERROR_READ_DEVNAME";
			break;
		case ERROR_READ_BDADDR:
			sText = "ERROR_READ_BDADDR";
			break;
		case ERROR_WRITE_BTADDR:
			sText = "ERROR_WRITE_BTADDR";
			break;
		case ERROR_WRITE_DEVNAME:
			sText = "ERROR_WRITE_DEVNAME";
			break;
		case ERROR_WRITE_CACHE:
			sText = "ERROR_WRITE_CACHE";
			break;
		case ERROR_WRITE_FIXPARAM:
			sText = "ERROR_WRITE_FIXPARAM";
			break;
		case ERROR_APOLLO:
			sText = "ERROR_APOLLO";
			break;
		case ERROR_READ_RECORD:
			sText = "ERROR_READ_RECORD";
			break;
		case ERROR_COMMU_FAIL:
			sText = "ERROR_COMMU_FAIL";
			break;
        case ERROR_READ_XTALTRIM:
			sText = "ERROR_READ_XTALTRIM";
			break;
        case ERROR_READ_XTALCAP:
			sText = "ERROR_READ_XTALCAP";
			break;
        case ERROR_WRITE_XTALTRIM:
			sText = "ERROR_WRITE_XTALTRIM";
			break;
        case ERROR_WRITE_XTALCAP:
			sText = "ERROR_WRITE_XTALCAP";
			break;
	}
	sText = "ERROR: err msg = " + sText;
	AddEvent2List(sText);

	return 0;
}

LRESULT software_tool_sz_freq::OnDevCtrlReport(WPARAM wParam, LPARAM lParam)
{
	CString sText;

	switch ((int)wParam) {
	case REPORT_THREAD_START:
		AddEvent2List("启动烧写进程！");
		break;
	case REPORT_CHIPID:
		AddEvent2List("找到芯片id！");
		break;
	case REPORT_ERASE_START:
		AddEvent2List("开始擦除flash！");
		break;
	case REPORT_ERASE_PROGRESS:
		AddEvent2List("正在擦除flash！");
		break;
	case REPORT_PROGRAM_START:
		AddEvent2List("开始下载固件！");
		break;
	case REPORT_PROGRAM_PERCENT:
		break;
	case REPORT_PROGRAM_END:
		AddEvent2List("固件下载完成！");
		break;
	case REPORT_OPEN_ENGINE:
		break;
	//case REPORT_RDBD_ADDR:
	//	break;
	//case REPORT_WRBD_ADDR:
	//	break;
	//case REPORT_RDBD_NAME:
	//	break;
	//case REPORT_WRBD_NAME:
	//	break;
	case REPORT_WRITE_FIXPARAM:
		break;
	case REPORT_CLOSE_ENGINE:
		break;
	case REPORT_WAIT_RESTART:
		break;
	case REPORT_COMMU_START:
		break;
	//case REPORT_COMMU_SUCC:
	//	break;
	//case REPORT_COMMU_READ:
	//	break;
	case REPORT_COMMU_OPEN:
		break;
	case RERROT_COMMU_TIMEOUT:
		break;
	//case REPORT_APOLLO:
	//	break;
	//case REPORT_READ_RECORD:
	//	break;
	case REPORT_USER_EXIT:
		break;
	case REPORT_END_ALL:
		break;
	case REPORT_LAST:
		break;

	case REPORT_APOLLO:
		sText.Format("%s", (char*)lParam);
		break;
	case REPORT_RDBD_NAME:
	case REPORT_WRBD_NAME:
	case REPORT_RDBD_ADDR:
	case REPORT_WRBD_ADDR:
	case REPORT_COMMU_SUCC:
		sText.Format("%s", (char*)lParam);
		AddEvent2List(sText);
		break;
	case REPORT_COMMU_READ:
		sText.Format("%s", (char*)lParam);
		AddEvent2List(sText);
		break;
	case REPORT_READ_RECORD:
		sText.Format("Recv=%d", (int)lParam);
		AddEvent2List(sText);
		break;
    case REPORT_READ_XTALTRIM:
		sText.Format("读取频偏trim值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
    case REPORT_READ_XTALCAP:
		sText.Format("读取频偏cap值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
    case REPORT_WRITE_XTALTRIM:
		sText.Format("写入频偏trim值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
    case REPORT_WRITE_XTALCAP:
		sText.Format("写入频偏cap值=%s 成功", lParam);
		AddEvent2List(sText);
		break;
	}

	return 0;
}
int software_tool_sz_freq::WorkerThreadProc(void *param)
{
	CString sText;int cap = 0, trim = 0;
	software_tool_sz_freq *p = (software_tool_sz_freq*)param;

	p->thread_exit_flag = 1;
	((CButton*)p->GetDlgItem(IDC_BUTTON2))->EnableWindow(FALSE);
	((CButton*)p->GetDlgItem(IDC_BUTTON14))->EnableWindow(FALSE);

	p->m_devCtrl.SethWnd(p->m_hWnd);
	if(p->rw_flag == 1){
		if (p->m_devCtrl.CrystalTrimmingRead(trim, cap, 1) == 0) {
			sText.Format("cap=%d, trim=%d", cap, trim);
			sText = "频偏值读取成功！" + sText;
			p->AddEvent2List(sText);
			sText.Format("%d", cap);
			p->m_cap.SetWindowText(sText);
			sText.Format("%d", trim);
			p->m_trim.SetWindowText(sText);			
		}else{
			sText.Format("cap=%d, trim=%d", cap, trim);
			sText = "频偏值读取失败！" + sText;
			p->AddEvent2List(sText);
			return -1;
		}
	}else if(p->rw_flag == 2){
		p->m_trim.GetWindowTextA(sText);
		trim = _tstoi(sText);
		p->m_cap.GetWindowTextA(sText);
		cap = _tstoi(sText);
		if (p->m_devCtrl.CrystalTrimmingWrite(trim, cap, 1) == 0) {
			sText = "频偏值写入成功！";
			p->AddEvent2List(sText);
		}else{
			sText = "频偏值读取失败！";
			p->AddEvent2List(sText);
			return -1;
		}
	}

	((CButton*)p->GetDlgItem(IDC_BUTTON2))->EnableWindow(TRUE);
	((CButton*)p->GetDlgItem(IDC_BUTTON14))->EnableWindow(TRUE);
	p->thread_exit_flag = 0;
	return 0;
}
void software_tool_sz_freq::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	rw_flag = 1;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_sz_freq::WorkerThreadProc, this, 0, NULL);
}


void software_tool_sz_freq::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	rw_flag = 2;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_sz_freq::WorkerThreadProc, this, 0, NULL);
}


void software_tool_sz_freq::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listEvt.ResetContent();
	AddEvent2List("可读取，可写入晶振校准值！");
}

void software_tool_sz_freq::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	while(thread_exit_flag == 1){;}
	CDialogEx::OnCancel();
}

BOOL software_tool_sz_freq::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "双子 晶振校准值读取写入工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);
	thread_exit_flag = 0;
	m_cap.SetWindowText("11");
	m_trim.SetWindowText("0");

	AddEvent2List("可读取，可写入晶振校准值！");
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
