
// box_audioDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "box_audio.h"
#include "box_audioDlg.h"
#include "afxdialogex.h"
#include "MyComm.h"
#include "ContrlComm.h"
#include "Ini.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString g_mes_ip;                  //MES�˿ڵ�����
UINT g_mes_port;
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cbox_audioDlg �Ի���



Cbox_audioDlg::Cbox_audioDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cbox_audioDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cbox_audioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_common_port);
	DDX_Control(pDX, IDC_EDIT4, m_baudrate);
	DDX_Control(pDX, IDC_EDIT8, m_Esetsn);
	DDX_Control(pDX, IDC_EDIT9, m_Esetsn1);
	DDX_Control(pDX, IDC_EDIT15, m_Esetsn8);
	DDX_Control(pDX, IDC_EDIT10, m_Esetsn2);
	DDX_Control(pDX, IDC_EDIT11, m_Esetsn3);
	DDX_Control(pDX, IDC_EDIT12, m_Esetsn4);
	DDX_Control(pDX, IDC_EDIT13, m_Esetsn5);
	DDX_Control(pDX, IDC_EDIT14, m_Esetsn6);
	DDX_Control(pDX, IDC_EDIT16, m_Esetsn7);
	
	
	
}

BEGIN_MESSAGE_MAP(Cbox_audioDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &Cbox_audioDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &Cbox_audioDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK3, &Cbox_audioDlg::OnBnClickedOk3)
	ON_BN_CLICKED(IDOK4, &Cbox_audioDlg::OnBnClickedOk4)
	ON_BN_CLICKED(IDOK6, &Cbox_audioDlg::OnBnClickedOk6)
	ON_BN_CLICKED(IDOK5, &Cbox_audioDlg::OnBnClickedOk5)
	ON_BN_CLICKED(IDOK10, &Cbox_audioDlg::OnBnClickedOk10)
	ON_BN_CLICKED(IDC_BUTTON3, &Cbox_audioDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK7, &Cbox_audioDlg::OnBnClickedOk7)
END_MESSAGE_MAP()

#define DEVICE_CHECK_CONF "config.ini"
// Cbox_audioDlg ��Ϣ�������
unsigned short g_command;
void Cbox_audioDlg::LoadCheckConf()
{
	unsigned int temp;
	CIni ini;
	CString sText, sFName;
	CString s_common_port, s_baudrate, s_command;
	GetModuleFileName(NULL, sText.GetBuffer(MAX_PATH), MAX_PATH);
	sText.ReleaseBuffer();
	sText = sText.Left(sText.ReverseFind('\\'));
	sFName = sText + "\\" + DEVICE_CHECK_CONF;
	if(GetFileAttributes(sFName) == INVALID_FILE_ATTRIBUTES) {
		sText = sText.Left(sText.ReverseFind('\\') + 1);
		sFName = sText + DEVICE_CHECK_CONF;
	}
	ini.SetPathName(sFName);
	g_con_path = sFName;
	s_common_port = ini.GetString("GLOBLE", "common_port");
	s_baudrate = ini.GetString("GLOBLE", "baudrate");

	GetDlgItem(IDC_EDIT3)->SetWindowTextA(s_common_port);
	GetDlgItem(IDC_EDIT4)->SetWindowTextA(s_baudrate);


	StrToIntEx(s_common_port, STIF_SUPPORT_HEX, (int*)&common_port);
	StrToIntEx(s_baudrate, STIF_SUPPORT_HEX, (int*)&baudrate);
	StrToIntEx(s_command, STIF_SUPPORT_HEX, (int*)&temp);
	command = (unsigned short)temp;
	g_command = command;
	//CString lala = "0xf";
	//int a;
	//StrToIntEx(lala, STIF_SUPPORT_HEX, (int*)&a);
	//lala.Format("########## %d",a);
	//AfxMessageBox(lala);
}
BOOL Cbox_audioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	running_flag = 0;
	LoadCheckConf();
	comInfo = &my_com;
	comInfo->portCom = common_port;
	comInfo->baudCom = baudrate;
	if(FALSE == comInfo->InitComm(common_port, DEIGHT, SONE, PNONE, baudrate)) {
		AfxMessageBox("�򿪴��ڳ���,���������ú��������");
		comInfo = NULL;
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cbox_audioDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cbox_audioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cbox_audioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cbox_audioDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	int ret;char *p;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 5;
		memcpy(comInfo->Mesresring, "mic0",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK)->SetWindowTextA("���ڴ���...");
		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check mic0 ");
		if(p != NULL)
			if(strlen(p)>11)
			{
				char*token=strtok((p+11)," ");
				m_Esetsn4.SetWindowTextA(token);
				GetDlgItem(IDOK)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK)->SetWindowTextA("�����ִ���");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CIni ini;
	CString s_common_port, s_baudrate, s_command;
	m_common_port.GetWindowTextA(s_common_port);
	m_baudrate.GetWindowTextA(s_baudrate);
	TRACE(s_common_port);
	TRACE(s_baudrate);
	ini.SetPathName(g_con_path);
	ini.WriteString("GLOBLE" ,"common_port",s_common_port);
	ini.WriteString("GLOBLE" ,"baudrate",s_baudrate);
}





void Cbox_audioDlg::OnBnClickedOk3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		int ret;char * p;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 8;
		memcpy(comInfo->Mesresring, "speaker",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK3)->SetWindowTextA("���ڴ���...");
		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check speaker ");

		if(p != NULL)
			if(strlen(p)>14)
			{
				char*token=strtok((p+14)," ");
				m_Esetsn2.SetWindowTextA(token);
				GetDlgItem(IDOK3)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK3)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK3)->SetWindowTextA("�����ִ���");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){

		}else if(ret == 1){
			GetDlgItem(IDOK3)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedOk4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		int ret;char * p;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 5;
		memcpy(comInfo->Mesresring, "mic1",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK4)->SetWindowTextA("���ڴ���...");
		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check mic1 ");

		if(p != NULL)
			if(strlen(p)>11)
			{
				char*token=strtok((p+11)," ");
				m_Esetsn3.SetWindowTextA(token);
				GetDlgItem(IDOK4)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK4)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK4)->SetWindowTextA("�����ִ���");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK4)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedOk6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
			int ret;
			char *p;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 4;
		memcpy(comInfo->Mesresring, "dut",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK6)->SetWindowTextA("���ڴ���...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check dut ");

		if(p != NULL)
			if(strlen(p)>10)
			{
				char*token=strtok((p+10)," ");
				m_Esetsn5.SetWindowTextA(token);
				GetDlgItem(IDOK6)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK6)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK6)->SetWindowTextA("�����ִ���");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){

			
		}else if(ret == 1){
			GetDlgItem(IDOK6)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedOk5()
{
	 char * p;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 9;
		memcpy(comInfo->Mesresring, "rdsensor",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK5)->SetWindowTextA("���ڴ���...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check rdsensor");

		if(p != NULL)
			if(strlen(p)>15)
			{
				char*token=strtok((p+15)," ");
				m_Esetsn1.SetWindowTextA(token);
				GetDlgItem(IDOK5)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK5)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK5)->SetWindowTextA("�����ִ���");

		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK5)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}




void Cbox_audioDlg::OnBnClickedOk10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;char * p;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 9;
		memcpy(comInfo->Mesresring, "wrsensor=",comInfo->file1db);
		
		m_Esetsn.GetWindowTextA(comInfo->m_setSN);
		if(comInfo->m_setSN.GetLength() < 1){
			AfxMessageBox("���벻��ȷ");
			running_flag = 0;
			return;
		}
		memcpy(&comInfo->Mesresring[comInfo->file1db], comInfo->m_setSN,comInfo->m_setSN.GetLength());
		comInfo->file1db  = 9 + comInfo->m_setSN.GetLength()+1;
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK10)->SetWindowTextA("���ڴ���...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		p=strstr((char *)comInfo->pRdbuf,"check wrsensor ");

		if(p != NULL)
			if(strlen(p)>15)
			{
				char*token=strtok((p+15)," ");
				m_Esetsn7.SetWindowTextA(token);
				GetDlgItem(IDOK10)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK10)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK10)->SetWindowTextA("�����ִ���");
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK10)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDOK)->SetWindowTextA("��MIC����");
	GetDlgItem(IDOK4)->SetWindowTextA("��MIC����");
	GetDlgItem(IDOK3)->SetWindowTextA("SPEAK����");
	GetDlgItem(IDOK6)->SetWindowTextA("��������");
	GetDlgItem(IDOK5)->SetWindowTextA("�ӽ���У׼ ��");
	GetDlgItem(IDOK10)->SetWindowTextA("�ӽ���У׼ д");
	GetDlgItem(IDOK7)->SetWindowTextA("�� device");
	m_Esetsn1.SetWindowTextA("");
	m_Esetsn2.SetWindowTextA("");
	m_Esetsn3.SetWindowTextA("");
	m_Esetsn4.SetWindowTextA("");
	m_Esetsn5.SetWindowTextA("");
	m_Esetsn6.SetWindowTextA("");
	m_Esetsn8.SetWindowTextA("");
}


void Cbox_audioDlg::OnBnClickedOk7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	 char * p;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	if(comInfo == NULL){
		AfxMessageBox("���ڴ�ʧ��,���������ñ�����������");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 9;
		memcpy(comInfo->Mesresring, "device",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK7)->SetWindowTextA("���ڴ���...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check device=");

		if(p != NULL)
			if(strlen(p)>12)
			{
				m_Esetsn8.SetWindowTextA((p+12));
				GetDlgItem(IDOK7)->SetWindowTextA("����ɹ�");
			}
			else
				GetDlgItem(IDOK7)->SetWindowTextA("���س��ȴ���");
		else
			GetDlgItem(IDOK7)->SetWindowTextA("�����ִ���");

		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK7)->SetWindowTextA("��ʱ,����ش�");
		}
		running_flag = 0;
	}
}
