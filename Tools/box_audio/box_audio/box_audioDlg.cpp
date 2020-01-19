
// box_audioDlg.cpp : 实现文件
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

CString g_mes_ip;                  //MES端口的设置
UINT g_mes_port;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// Cbox_audioDlg 对话框



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
// Cbox_audioDlg 消息处理程序
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

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	running_flag = 0;
	LoadCheckConf();
	comInfo = &my_com;
	comInfo->portCom = common_port;
	comInfo->baudCom = baudrate;
	if(FALSE == comInfo->InitComm(common_port, DEIGHT, SONE, PNONE, baudrate)) {
		AfxMessageBox("打开串口出错,请重新配置后重启软件");
		comInfo = NULL;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cbox_audioDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cbox_audioDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cbox_audioDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	int ret;char *p;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 5;
		memcpy(comInfo->Mesresring, "mic0",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK)->SetWindowTextA("正在传输...");
		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check mic0 ");
		if(p != NULL)
			if(strlen(p)>11)
			{
				char*token=strtok((p+11)," ");
				m_Esetsn4.SetWindowTextA(token);
				GetDlgItem(IDOK)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK)->SetWindowTextA("命令字错误");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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
	// TODO: 在此添加控件通知处理程序代码
		int ret;char * p;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 8;
		memcpy(comInfo->Mesresring, "speaker",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK3)->SetWindowTextA("正在传输...");
		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check speaker ");

		if(p != NULL)
			if(strlen(p)>14)
			{
				char*token=strtok((p+14)," ");
				m_Esetsn2.SetWindowTextA(token);
				GetDlgItem(IDOK3)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK3)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK3)->SetWindowTextA("命令字错误");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){

		}else if(ret == 1){
			GetDlgItem(IDOK3)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedOk4()
{
	// TODO: 在此添加控件通知处理程序代码
		int ret;char * p;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 5;
		memcpy(comInfo->Mesresring, "mic1",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK4)->SetWindowTextA("正在传输...");
		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check mic1 ");

		if(p != NULL)
			if(strlen(p)>11)
			{
				char*token=strtok((p+11)," ");
				m_Esetsn3.SetWindowTextA(token);
				GetDlgItem(IDOK4)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK4)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK4)->SetWindowTextA("命令字错误");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK4)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedOk6()
{
	// TODO: 在此添加控件通知处理程序代码
			int ret;
			char *p;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 4;
		memcpy(comInfo->Mesresring, "dut",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK6)->SetWindowTextA("正在传输...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check dut ");

		if(p != NULL)
			if(strlen(p)>10)
			{
				char*token=strtok((p+10)," ");
				m_Esetsn5.SetWindowTextA(token);
				GetDlgItem(IDOK6)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK6)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK6)->SetWindowTextA("命令字错误");
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){

			
		}else if(ret == 1){
			GetDlgItem(IDOK6)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedOk5()
{
	 char * p;
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 9;
		memcpy(comInfo->Mesresring, "rdsensor",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK5)->SetWindowTextA("正在传输...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check rdsensor");

		if(p != NULL)
			if(strlen(p)>15)
			{
				char*token=strtok((p+15)," ");
				m_Esetsn1.SetWindowTextA(token);
				GetDlgItem(IDOK5)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK5)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK5)->SetWindowTextA("命令字错误");

		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK5)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}




void Cbox_audioDlg::OnBnClickedOk10()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret;char * p;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 9;
		memcpy(comInfo->Mesresring, "wrsensor=",comInfo->file1db);
		
		m_Esetsn.GetWindowTextA(comInfo->m_setSN);
		if(comInfo->m_setSN.GetLength() < 1){
			AfxMessageBox("输入不正确");
			running_flag = 0;
			return;
		}
		memcpy(&comInfo->Mesresring[comInfo->file1db], comInfo->m_setSN,comInfo->m_setSN.GetLength());
		comInfo->file1db  = 9 + comInfo->m_setSN.GetLength()+1;
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK10)->SetWindowTextA("正在传输...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		p=strstr((char *)comInfo->pRdbuf,"check wrsensor ");

		if(p != NULL)
			if(strlen(p)>15)
			{
				char*token=strtok((p+15)," ");
				m_Esetsn7.SetWindowTextA(token);
				GetDlgItem(IDOK10)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK10)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK10)->SetWindowTextA("命令字错误");
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK10)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}


void Cbox_audioDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDOK)->SetWindowTextA("主MIC测试");
	GetDlgItem(IDOK4)->SetWindowTextA("副MIC测试");
	GetDlgItem(IDOK3)->SetWindowTextA("SPEAK测试");
	GetDlgItem(IDOK6)->SetWindowTextA("蓝牙测试");
	GetDlgItem(IDOK5)->SetWindowTextA("接近光校准 读");
	GetDlgItem(IDOK10)->SetWindowTextA("接近光校准 写");
	GetDlgItem(IDOK7)->SetWindowTextA("读 device");
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
	// TODO: 在此添加控件通知处理程序代码
	 char * p;
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	if(comInfo == NULL){
		AfxMessageBox("串口打开失败,请重新配置保存后重启软件");
		return;
	}
	if(running_flag == 0){
		running_flag = 1;
		comInfo->file1db  = 9;
		memcpy(comInfo->Mesresring, "device",comInfo->file1db);
		comInfo->Mesresring[comInfo->file1db-1]='\n';
		GetDlgItem(IDOK7)->SetWindowTextA("正在传输...");

		ret = CContrlComm::WorkerThreadProc(comInfo);
		p=strstr((char *)comInfo->pRdbuf,"check device=");

		if(p != NULL)
			if(strlen(p)>12)
			{
				m_Esetsn8.SetWindowTextA((p+12));
				GetDlgItem(IDOK7)->SetWindowTextA("传输成功");
			}
			else
				GetDlgItem(IDOK7)->SetWindowTextA("返回长度错误");
		else
			GetDlgItem(IDOK7)->SetWindowTextA("命令字错误");

		comInfo->m_setSN.Format("%s",comInfo->pRdbuf);
		m_Esetsn6.SetWindowTextA(comInfo->m_setSN);
		if(ret == 0){
			
		}else if(ret == 1){
			GetDlgItem(IDOK7)->SetWindowTextA("超时,点击重传");
		}
		running_flag = 0;
	}
}
