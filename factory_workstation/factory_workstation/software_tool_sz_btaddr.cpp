// software_tool_sz_btaddr.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_sz_btaddr.h"
#include "afxdialogex.h"


// software_tool_sz_btaddr 对话框

IMPLEMENT_DYNAMIC(software_tool_sz_btaddr, CDialogEx)

software_tool_sz_btaddr::software_tool_sz_btaddr(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_sz_btaddr::IDD, pParent)
{

}

software_tool_sz_btaddr::~software_tool_sz_btaddr()
{
}

void software_tool_sz_btaddr::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listEvt);
	DDX_Control(pDX, IDC_EDIT15, m_btaddr);
	DDX_Control(pDX, IDC_EDIT45, m_btname);
	DDX_Control(pDX, IDC_EDIT46, m_hw_version);
}


BEGIN_MESSAGE_MAP(software_tool_sz_btaddr, CDialogEx)
	ON_BN_CLICKED(IDOK, &software_tool_sz_btaddr::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &software_tool_sz_btaddr::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON14, &software_tool_sz_btaddr::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON2, &software_tool_sz_btaddr::OnBnClickedButton2)
	ON_MESSAGE(WM_DEV_ERROR, &software_tool_sz_btaddr::OnDevCtrlError)
	ON_MESSAGE(WM_DEV_REPORT, &software_tool_sz_btaddr::OnDevCtrlReport)
END_MESSAGE_MAP()


void software_tool_sz_btaddr::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);
}
// software_tool_sz_btaddr 消息处理程序


void software_tool_sz_btaddr::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listEvt.ResetContent();
	AddEvent2List("可读取，可写入蓝牙地址！");
}


void software_tool_sz_btaddr::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_devCtrl.Stop();
	CDialogEx::OnCancel();
}

LRESULT software_tool_sz_btaddr::OnDevCtrlError(WPARAM wParam, LPARAM lParam)
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
	thread_exit_flag = 0;

	return 0;
}

LRESULT software_tool_sz_btaddr::OnDevCtrlReport(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	int length;
	CString value_str;
	switch ((int)wParam) {
	case REPORT_THREAD_START:
		AddEvent2List("启动进程！");
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
		thread_exit_flag = 0;
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
		if( (length = sText.Find("check BTADDR")) >= 0){
			value_str = " ";
			value_str += sText.Mid( length+sizeof("check BTADDR") );
			AddEvent2List(value_str);
		}
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

#include "Ini.h"
struct bt_lic_ins_str_burn{
	UINT uap;
	UINT nap;
	UINT lap_min;
	UINT lap_max;
	CString license;
};
int software_tool_sz_btaddr::WorkerThreadProc(void *param)
{
	CString sText;
	CIni ini;
	CString sFName, license = "",mac_addr;
	int i, lic_num;
	UINT uap, nap, lap;
	software_tool_sz_btaddr *p = (software_tool_sz_btaddr*)param;
	p->thread_exit_flag = 1;
	if(p->rw_flag == 1){
		p->m_btname.GetWindowText(sText); p->m_devCtrl.SetBtName(sText);
		p->m_btaddr.GetWindowText(sText); p->m_devCtrl.SetBtAddr(sText); mac_addr = sText;
		p->m_hw_version.GetWindowText(sText); p->m_devCtrl.SetHwVersion(sText);

		sText = mac_addr.Mid(3,6);
		lap = strtol(sText,NULL,16);
		sText = mac_addr.Mid(13,2);
		uap = strtol(sText,NULL,16);
		sText = mac_addr.Mid(19,4);
		nap = strtol(sText,NULL,16);
		sFName = "bluesite_lib\\bt_license.ini";
		ini.SetPathName(sFName);
		lic_num = ini.GetUInt("BT_LIC", "bt_lic_num", 0x11, 10);
		for(i=0; i<lic_num; i++){
			struct bt_lic_ins_str_burn *lic_p = new struct bt_lic_ins_str_burn;
			sText.Format("BT_LIC_INS%d", i+1);
			lic_p->uap = ini.GetUInt(sText, "uap", 0x11, 16);
			lic_p->nap = ini.GetUInt(sText, "nap", 0x11, 16);
			lic_p->lap_min = ini.GetUInt(sText, "lap_min", 0x11, 16);
			lic_p->lap_max = ini.GetUInt(sText, "lap_max", 0x11, 16);
			lic_p->license = ini.GetString(sText, "license");
			if( (lic_p->uap == uap) && (lic_p->nap == nap) &&
				(lic_p->lap_min <= lap) && (lic_p->lap_max >= lap) ){
				license = lic_p->license;
				delete lic_p;
				break;
			}
			delete lic_p;
		}
		if(license != ""){
			p->m_devCtrl.SetLicenseText(license);
			p->AddEvent2List("本地 license 获取成功！");
		}else{
			license.Empty();
			p->m_devCtrl.SetLicenseText(license);
			p->AddEvent2List("本地 license 获取失败！");
			return -1;
		}

		p->m_devCtrl.SetBtWrite(TRUE);
		p->m_devCtrl.SetThreadFunc(THREAD_BT_ADDR);
		p->m_devCtrl.Start(p->m_hWnd);
	}else{
		p->m_devCtrl.SetThreadFunc(THREAD_CHECK);
		p->m_devCtrl.Start(p->m_hWnd);
	}
	return 0;
}

void software_tool_sz_btaddr::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	if(thread_exit_flag != 1){
		rw_flag = 1;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_sz_btaddr::WorkerThreadProc, this, 0, NULL);
	}else{
		AddEvent2List("程序运行中！");
	}
}


void software_tool_sz_btaddr::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(thread_exit_flag != 1){
		rw_flag = 2;
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_sz_btaddr::WorkerThreadProc, this, 0, NULL);
	}else{
		AddEvent2List("程序运行中！");
	}
}


#define SZ_DEFAULT_BTADDR "{0x4d3064, 0xd5, 0x70b3}"//"00:ff:01:5b:00:02"
#define SZ_DEFAULT_BTNAME "\"iFLYBUDS\""
#define SZ_DEFAULT_HWVERSION "000102"
BOOL software_tool_sz_btaddr::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "双子 蓝牙地址本地读写工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	m_btaddr.SetWindowTextA(SZ_DEFAULT_BTADDR);
	m_btname.SetWindowTextA(SZ_DEFAULT_BTNAME);
	m_hw_version.SetWindowTextA(SZ_DEFAULT_HWVERSION);
	thread_exit_flag = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
}
