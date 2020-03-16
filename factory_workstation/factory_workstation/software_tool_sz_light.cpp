// software_tool_sz_light.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_sz_light.h"
#include "afxdialogex.h"


// software_tool_sz_light �Ի���

IMPLEMENT_DYNAMIC(software_tool_sz_light, CDialogEx)

software_tool_sz_light::software_tool_sz_light(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_sz_light::IDD, pParent)
{

}

software_tool_sz_light::~software_tool_sz_light()
{
}

void software_tool_sz_light::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST17, m_listEvt);
}


BEGIN_MESSAGE_MAP(software_tool_sz_light, CDialogEx)
	ON_BN_CLICKED(IDOK, &software_tool_sz_light::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &software_tool_sz_light::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &software_tool_sz_light::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &software_tool_sz_light::OnBnClickedButton1)
END_MESSAGE_MAP()


void software_tool_sz_light::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);
}

// software_tool_sz_light ��Ϣ�������
int software_tool_sz_light::WorkerThreadProc(void *param)
{
	CString sText;
	software_tool_sz_light *p = (software_tool_sz_light*)param;
	p->thread_exit_flag = 0;
	((CButton*)p->GetDlgItem(IDOK))->EnableWindow(FALSE);
	while(p->thread_exit == 0){
		p->m_devCtrl.ClearMsgNo();
		p->m_devCtrl.CheckSensorRead(p->value);
		sText.Format("value = %d", p->value);
		p->AddEvent2List(sText);
		Sleep(400);
	}
	p->m_devCtrl.CloseEngine();
	p->thread_exit_flag = 1;
	return 0;
}

void software_tool_sz_light::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	thread_exit = 0;
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_tool_sz_light::WorkerThreadProc, this, 0, NULL);
}


void software_tool_sz_light::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_devCtrl.Stop();
	((CButton*)GetDlgItem(IDOK))->EnableWindow(TRUE);
	thread_exit = 1;
	if(thread_exit_flag == 0){
		AddEvent2List("����ֹͣ�����˳���");
		return;
	}
	CDialogEx::OnCancel();
}


BOOL software_tool_sz_light::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString soft_name;
	soft_name += "˫�� �ӽ�����Թ���";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);
	thread_exit_flag = 1;

	AddEvent2List("�����ʼ���ԣ�");
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL software_tool_sz_light::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::PreTranslateMessage(pMsg);
}


void software_tool_sz_light::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_listEvt.ResetContent();
	AddEvent2List("�����ʼ���ԣ�");
}


void software_tool_sz_light::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(thread_exit_flag == 0){
		AddEvent2List("����ֹͣ�����˳���");
		return;
	}
	CDialogEx::OnCancel();
}
