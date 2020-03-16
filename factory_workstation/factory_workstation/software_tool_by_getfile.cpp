// software_tool_by_getfile.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_by_getfile.h"
#include "afxdialogex.h"

// software_tool_by_getfile 对话框

IMPLEMENT_DYNAMIC(software_tool_by_getfile, CDialogEx)

software_tool_by_getfile::software_tool_by_getfile(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_by_getfile::IDD, pParent)
{

}

software_tool_by_getfile::~software_tool_by_getfile()
{
}

void software_tool_by_getfile::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
}

BEGIN_MESSAGE_MAP(software_tool_by_getfile, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON13, &software_tool_by_getfile::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON22, &software_tool_by_getfile::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON23, &software_tool_by_getfile::OnBnClickedButton23)
END_MESSAGE_MAP()


void software_tool_by_getfile::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);

	//if(m_hLogFile.m_hFile != INVALID_HANDLE_VALUE) {
	//	m_hLogFile.Write(sTemp, sTemp.GetLength());
	//	m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	//}
}

void software_tool_by_getfile::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	tools_option_instance = g_p->tools_option_instance;
}
// software_tool_by_getfile 消息处理程序

void software_tool_by_getfile::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	CIni ini;
	ini.SetPathName(ini_path);
	ini.WriteString("TOOL_BY_GETFILE" ,"g_by_get_left_filename",tools_option_instance.g_by_get_left_filename);
	ini.WriteString("TOOL_BY_GETFILE" ,"g_by_get_right_filename",tools_option_instance.g_by_get_right_filename);
	ini.WriteString("TOOL_BY_GETFILE" ,"g_by_savfilename",tools_option_instance.g_by_savfilename);
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(tools_option_instance.g_by_get_left_filename);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(tools_option_instance.g_by_get_right_filename);
	GetDlgItem(IDC_EDIT44)->GetWindowTextA(tools_option_instance.g_by_savfilename);
	AddEvent2List("配置已保存");
}

TCHAR* software_tool_by_getfile::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
CString software_tool_by_getfile::ExecuteCmd(CString cmd_name, CString cmd_param)
{
	SECURITY_ATTRIBUTES sa;
    HANDLE hRead,hWrite;
 
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;
    if (!CreatePipe(&hRead,&hWrite,&sa,0)){
        AfxMessageBox("Error on CreatePipe()!");
        return NULL;
    }
	STARTUPINFO si={sizeof(si)};
    PROCESS_INFORMATION pi;
    si.hStdError = hWrite;
    si.hStdOutput = hWrite;
    si.wShowWindow = SW_HIDE;
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* name=StringToChar(cmd_name);
	TCHAR* param=StringToChar(cmd_param);
	if (!CreateProcess(name,param,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi)){
        AfxMessageBox("Error on CreateProcess()!");
        return NULL;
    }
    CloseHandle(hWrite);
 
    char buffer[4096];
    memset(buffer,0,4096);
	CString output;
    DWORD byteRead;
    while(true){
		memset(buffer,0,4096);
        if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL){
            break;
        }
		TRACE("%s",buffer);
        output += buffer;
    }
	CloseHandle(hRead);
	return output;
}

int software_tool_by_getfile::if_response_err(CString response)
{
	if(response.Find("error: device not found") < 0){
		if(response.Find("remote object") < 0){
			if(response.Find("error:") >= 0){
				AfxMessageBox("adb设备失去连接");
				return -1;
			}
			return 0;
		}else{
			return -2;
		}
	}else{
		//init_err_flag = 1<<BY_CHECK_INIT_FLAG_USBERR;
		AfxMessageBox("usb设备失去连接");
		return -1;
	}
}
int software_tool_by_getfile::process_response_devices(CString response)
{
	CString cut_str;
	int n=response.Find("List of devices attached");
	if(n<0){
		return -1;
	}
	int length=response.Delete(0,n); //找出"List of devices attached"右边的字符串位数
	cut_str = response.Right(length - sizeof("List of devices attached"));   
	if(cut_str.Find("device")<0){
		return -1;
	}
	return 0;
}
int software_tool_by_getfile::WorkerThreadProc(void *param)
{
	CString sText;
	CTime tNow = CTime::GetCurrentTime();
	software_tool_by_getfile *p = (software_tool_by_getfile*)param;
	((CButton*)p->GetDlgItem(IDC_BUTTON22))->EnableWindow(FALSE);
	CString response;
	CString cmd_str = "thirdapp\\adb\\adb.exe";
	CString cmd_param;
	cmd_param = " devices";
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(p->process_response_devices(response)){
		p->AddEvent2List("adb连接失败");
		goto out;
	}

	cmd_param = " pull ";
	cmd_param += p->tools_option_instance.g_by_get_left_filename;
	cmd_param += " ";
	sText.Format("audio/getfile_by/%04d%02d%02d-%02d%02d%02d_1_", 
					tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	sText += p->tools_option_instance.g_by_savfilename;
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	int ret;
	if(ret = p->if_response_err(response)){
		if(ret == -2){
			sText.Format("远端不存在");
			sText += p->tools_option_instance.g_by_get_left_filename;
			sText += "的文件";
			p->AddEvent2List(sText);
		}
		p->AddEvent2List("文件1获取失败");
		goto out;
	}
	p->AddEvent2List("文件1获取成功");

	cmd_param = " pull ";
	cmd_param += p->tools_option_instance.g_by_get_right_filename;
	cmd_param += " ";
	sText.Format("audio/getfile_by/%04d%02d%02d-%02d%02d%02d_2_", 
					tNow.GetYear(),tNow.GetMonth(),tNow.GetDay(),
					tNow.GetHour(),tNow.GetMinute(), tNow.GetSecond());
	sText += p->tools_option_instance.g_by_savfilename;
	cmd_param += sText;
	response = p->ExecuteCmd(cmd_str, cmd_param);
	if(ret = p->if_response_err(response)){
		if(ret == -2){
			sText.Format("远端不存在");
			sText += p->tools_option_instance.g_by_get_right_filename;
			sText += "的文件";
			p->AddEvent2List(sText);
		}
		p->AddEvent2List("文件2获取失败");
		goto out;
	}
	p->AddEvent2List("文件2获取成功");

out:
	((CButton*)p->GetDlgItem(IDC_BUTTON22))->EnableWindow(TRUE);
	return 0;
}

void software_tool_by_getfile::OnBnClickedButton22()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT3)->GetWindowTextA(tools_option_instance.g_by_get_left_filename);
	GetDlgItem(IDC_EDIT4)->GetWindowTextA(tools_option_instance.g_by_get_right_filename);
	GetDlgItem(IDC_EDIT44)->GetWindowTextA(tools_option_instance.g_by_savfilename);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_by_getfile::WorkerThreadProc, this, 0, NULL);
	AddEvent2List("开始传输文件");
}

void software_tool_by_getfile::OnBnClickedButton23()
{
	// TODO: 在此添加控件通知处理程序代码
	m_listEvt.ResetContent();
	AddEvent2List("##点击获取文件到audio/getfile_by下##");
}

int software_tool_by_getfile::need_file_ifexit()
{
	if(PathFileExists("thirdapp\\adb\\adb.exe") != TRUE){
		AfxMessageBox("错误：程序路径下未找到adb.exe");
		return 1;
	}
	return 0;
}

BOOL software_tool_by_getfile::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString soft_name;
	soft_name += "白羊文件获取工具";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CreateDirectory("audio", 0);
	CreateDirectory("audio\\getfile_by", 0);

	GetDlgItem(IDC_EDIT3)->SetWindowTextA(tools_option_instance.g_by_get_left_filename);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(tools_option_instance.g_by_get_right_filename);
	GetDlgItem(IDC_EDIT44)->SetWindowTextA(tools_option_instance.g_by_savfilename);

	need_file_ifexit();
	OnBnClickedButton23();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL software_tool_by_getfile::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
