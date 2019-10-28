
// ProductDevToolsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProductDevTools.h"
#include "ProductDevToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProductDevToolsDlg �Ի���



CProductDevToolsDlg::CProductDevToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRODUCTDEVTOOLS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_devHandle = 0;
}

void CProductDevToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDITS_SEND, m_edSend);
	DDX_Control(pDX, IDC_EDIT_RECV, m_edRecv);
	DDX_Control(pDX, IDC_EDITS_BTADDR, m_edBtAddr);
	DDX_Control(pDX, IDC_EDIT_FIRM_NAME, m_edFirmName);
	DDX_Control(pDX, IDC_LIST_COMMU, m_ListCtrl);
	DDX_Control(pDX, IDC_EDIT5, m_edAddr);
}

BEGIN_MESSAGE_MAP(CProductDevToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CProductDevToolsDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CProductDevToolsDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECV, &CProductDevToolsDlg::OnBnClickedButtonRecv)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CProductDevToolsDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BTN_START, &CProductDevToolsDlg::OnBnClickedBtnStart)
	ON_MESSAGE(WM_DEV_ERROR, &CProductDevToolsDlg::OnDevCtrlError)
	ON_MESSAGE(WM_DEV_REPORT, &CProductDevToolsDlg::OnDevCtrlReport)
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, &CProductDevToolsDlg::OnBnClickedBtnOpenFile)
END_MESSAGE_MAP()


// CProductDevToolsDlg ��Ϣ�������

BOOL CProductDevToolsDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_edAddr.SetWindowTextA("{0x00ff09, 0x5b, 0x02}");
	m_edSend.SetWindowTextA("abcdefg��������1234567");
	m_edFirmName.SetWindowTextA("C:\\Users\\dannywang\\Documents\\QCC5126.git\\Earbuds1\\apps\\applications\\earbud\\qcc512x_rom_v21\\CF376_CG814\\flash_image.xuv");

	int nCol = 0;
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;       //
	dwStyle |= LVS_EX_GRIDLINES;           //
	m_ListCtrl.SetExtendedStyle(dwStyle);  //

	m_ListCtrl.InsertColumn(nCol++, _T("���"), LVCFMT_CENTER, 60);
	m_ListCtrl.InsertColumn(nCol++, _T("��Ŀ"), LVCFMT_CENTER, 100);
	m_ListCtrl.InsertColumn(nCol++, _T("����"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(nCol++, _T("����"), LVCFMT_LEFT, 300);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CProductDevToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProductDevToolsDlg::OnPaint()
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
HCURSOR CProductDevToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

INT flmGetPorts(void)
{
	uint16 maxLen = 256;
	uint16 count(0);
	char* portsStr = new char[maxLen];
	char* transStr = new char[maxLen];
//	vector<string> ports; // The human readable port strings (e.g. "USBDBG(100)")
//	vector<string> trans; // The transport option strings (e.g. "SPITRANS=USBDBG SPIPORT=1")

	int32 status = flmGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	if (status != TFL_OK && maxLen != 0)
	{
		// Not enough space - resize the storage
		delete[] portsStr;
		portsStr = new char[maxLen];
		delete[] transStr;
		transStr = new char[maxLen];
		status = flmGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	}

	if (status != TFL_OK || count == 0)
	{
		TRACE("Error getting ports, or none found\n");
		delete[] portsStr;
		delete[] transStr;
	}

	TRACE("PORT:%s   |%s\n", portsStr, transStr);

	char newTransStr[256];
	uint32 device = 0xFF;

#if 10
	status = flmConvertPort(transStr, newTransStr, &device);
	if (status != TFL_OK)
	{
		TRACE("ConvertPort error\n");
	}

	TRACE("DEVICE=0x%x  new=%s\n", device, newTransStr);
#endif

	delete portsStr, delete transStr;




	UINT devMask = 112;
	char sVer[32];


	//	atlTraceGeneral - PORT:USBDBG(112) | SPITRANS = USBDBG SPIPORT = 1
	if (flmOpen(devMask, 10, TFL_USBDBG | TFL_SPI_LPT) != TFL_OK) {
		TRACE("Failed to open devices %d err:%d\n", flmGetBitErrorField(),
			flmGetLastError());
	}

	devMask = 1;
	if (flmOpenTrans(devMask, "SPITRANS=USBDBG", 26) != TFL_OK) {

		TRACE("Failed to flmOpenTrans devices %d\n", flmGetBitErrorField());
	}

	status = flmGetChipId(0);   // 0 is :ERROR
	TRACE("chipID=%x\n", status);

	status = flmGetVersion(sVer);
	TRACE("VER:%s\n", sVer);

	status = flmReadProgramFiles("C:\\Users\\dannywang\\Documents\\QCC5126.git\\Earbuds1\\apps\\applications\\earbud\\qcc512x_rom_v21\\CF376_CG814\\flash_image.xuv");
	if (status != TFL_OK)
	{
		TRACE("Failed to read flmash program files\n");
	}

	if (flmSetFlashType(devMask, TFL_TYPE_SQIF) != TFL_OK)
	{
		TRACE("Failed to set flmash type\n");
	}

	uint16 sectors, sizeMbits; uint32 manid, devid;
	if (flmGetDeviceFlashInfo(devMask, &sectors, &sizeMbits, &manid, &devid) == TFL_OK) {
		TRACE("FLASH: %d %dMB %d %d\n", sectors, sizeMbits, manid, devid);
	}

	if (flmEraseBlock(devMask) == TFL_OK) {
		while (status >= 0 && status < 100) {
			status = flmGetDeviceProgress(0);
			TRACE("ERASE %d percent\n", status);
			Sleep(1000);
		}
	}

	if (flmSetSubsysBank(devMask, 4, 0) != TFL_OK)
	{
		TRACE("Failed to set subsystem and bank\n");
	}

#if 10
	if (flmProgramSpawn(devMask, 1, 1, 0) != TFL_OK)
	{
		TRACE("Failed to spawn flmash program thread\n");
	}
#else

	if (flmProgramBlock() != TFL_OK)
		TRACE("ERROR flmProgramBlock\n");
#endif

	uint16 devicesRunning = 0;
	int32 progress;
	do
	{
		devicesRunning = 0;
		progress = flmGetDeviceProgress(0);
		if (progress < 100)
		{
			++devicesRunning;
		}
		TRACE("progress = %d\n", progress);

		Sleep(1000);
	} while (devicesRunning > 0);

	TRACE("Completed\n");

	int32 error = flmGetLastError();
	if (error != TFL_OK)
	{
		TRACE("Programming failed with error: %d\n", error);
		TRACE("Failed devices mask = %d\n", flmGetBitErrorField());
	}

	flmClose(devMask);



	return 0;
}

INT flGetPorts(void)
{
	uint16 maxLen = 256;
	uint16 count(0);
	char* portsStr = new char[maxLen];
	char* transStr = new char[maxLen];
	//	vector<string> ports; // The human readable port strings (e.g. "USBDBG(100)")
	//	vector<string> trans; // The transport option strings (e.g. "SPITRANS=USBDBG SPIPORT=1")

	int32 status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	if (status != TFL_OK && maxLen != 0)
	{
		// Not enough space - resize the storage
		delete[] portsStr;
		portsStr = new char[maxLen];
		delete[] transStr;
		transStr = new char[maxLen];
		status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	}

	if (status != TFL_OK || count == 0)
	{
		TRACE("Error getting ports, or none found\n");
		delete[] portsStr;
		delete[] transStr;
	}

	TRACE("PORT:%s   |%s\n", portsStr, transStr);

	delete portsStr, delete transStr;




	char sVer[32];
	
	//	atlTraceGeneral - PORT:USBDBG(112) | SPITRANS = USBDBG SPIPORT = 1

#if 0
	if (flOpen(devMask, 10, 26, TFL_USBDBG) != TFL_OK) {
		TRACE("Failed to open devices %d err:%d\n", flGetBitErrorField(),
			flGetLastError());
	}

#else
	if (flOpenTrans("SPITRANS=USBDBG", 10, 26) != TFL_OK) {

		TRACE("Failed to flmOpenTrans devices %d\n", flmGetBitErrorField());
	}
#endif

	status = flGetChipId();   // 0 is :ERROR
	TRACE("chipID=%x\n", status);

	status = flGetVersion(sVer);
	TRACE("VER:%s\n", sVer);

	status = flReadProgramFiles("C:\\Users\\dannywang\\Documents\\QCC5126.git\\Earbuds1\\apps\\applications\\earbud\\qcc512x_rom_v21\\CF376_CG814\\flash_image.xuv");
	if (status != TFL_OK)
	{
		TRACE("Failed to read flash program files:%d\n", flGetLastError());
	}

	if (flSetFlashType(TFL_TYPE_SQIF) != TFL_OK)
	{
		TRACE("Failed to set flash type\n");
	}

	uint16 sectors, sizeMbits; uint32 manid, devid;
	if (flGetFlashInfo(&sectors, &sizeMbits, &manid, &devid) == TFL_OK) {
		TRACE("FLASH: %d %dMB %d %d\n", sectors, sizeMbits, manid, devid);
	}

	if (flErase() == TFL_OK) {
		while (status >= 0 && status < 100) {
			status = flGetProgress();
			TRACE("ERASE %d percent\n", status);
			Sleep(1000);
		}
	}
	else
		TRACE("ERASE ERROR\n");

	if (flSetSubsysBank(4, 0) != TFL_OK)
	{
		TRACE("Failed to set subsystem and bank\n");
	}

#if 10
	if (flProgramSpawn() != TFL_OK)
	{
		TRACE("Failed to spawn flmash program thread\n");
	}
#else

	if (flmProgramBlock() != TFL_OK)
		TRACE("ERROR flmProgramBlock\n");
#endif

	uint16 devicesRunning = 0;
	int32 progress;
	do
	{
		devicesRunning = 0;
		progress = flGetProgress();
		if (progress < 100)
		{
			++devicesRunning;
		}
		TRACE("progress = %d\n", progress);

		Sleep(1000);
	} while (devicesRunning > 0);

	TRACE("Completed\n");

	int32 error = flGetLastError();
	if (error != TFL_OK)
	{
		TRACE("Programming failed with error: %d\n", error);
		TRACE("Failed devices mask = %d\n", flGetBitErrorField());
	}

	flClose();

	return 0;
}


#define KEY_READ_BUFFER_LEN        (256)
// ע�ⲻ�ܿ����ظ�����������������������teConfigCacheInit ����
void CProductDevToolsDlg::OnBnClickedOk()
{
//	flGetPorts();
//	flmGetPorts();

	char data[KEY_READ_BUFFER_LEN]; uint32 datalen; uint16 datalen16;
	int ret, ret2;
	CString sText;

	m_edBtAddr.SetWindowTextA("��ʼ��...");
	
#if 10
//	m_devHandle = openTestEngine(USBDBG, "SPIPORT=1", 0, 5000, 0);
	m_devHandle = openTestEngine(USBDBG, "1", 0, 5000, 0);
	if (m_devHandle == 0) {
		TRACE("Error openTestEngine\n");
		return;
	}
#else
	m_devHandle = openTestEngineDebugTrans("SPITRANS=USBDBG", 0);
	if (handle == 0) {
		TRACE("Error openTestEngineDebugTrans\n");
	}
#endif

	//  ע����Ҫ D:\QCC512X\BlueSuite 3.2.2
	//  ���е�DLL�ļ� ��hpm�ļ�

	const char* const CFG_DB_PARAM = "hydracore_config.sdb:QCC512X_CONFIG";
	if ((ret=teConfigCacheInit(m_devHandle, CFG_DB_PARAM)) != 1)
		AfxMessageBox((LPCTSTR)"ERROR CACHE INIT", MB_OK, 0);

	if (TE_OK != (ret2=teConfigCacheRead(m_devHandle, NULL, 0)))
		AfxMessageBox((LPCTSTR)"ERRO Cache Read\n", MB_OK, 0);

	sText.Format("RET:%d %d", ret, ret2);
//	AfxMessageBox(sText);

	datalen = KEY_READ_BUFFER_LEN;
	if((ret=teConfigCacheReadItem(m_devHandle, "bt2:pskey_bdaddr", data, &datalen)) == 1) {
		TRACE("BTADDR teConfigCacheReadItem:%s LEN=%d\n", data, datalen);
		m_edBtAddr.SetWindowTextA(data);
		// {0xff09, 0x5b, 0x02}
	}
	else {
		TRACE("Error teConfigCacheReadItem\n");
	}
	// psReadBdAddr  ��һ���ֲ���ȷ,��֪��Ϊʲô����a5a5  // 0x00a5a5 5b 0002

#if 0
	/* �޸�������ַ OK*/
	data[6] = 'a';
	ret = teConfigCacheWriteItem(m_devHandle, "bt2:pskey_bdaddr", data);
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);

	ret = teConfigCacheWrite(m_devHandle, NULL, 0);
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);
#endif

	// ���ɹ�
	datalen16 = 64;
	ret = tePsRead(m_devHandle, 10192, sizeof(FixParam)/2, (uint16*)data, &datalen16);
	TRACE("LINE:%d ret=%d ERROR=%d\n", __LINE__, ret, teGetLastError(m_devHandle));


	// Ҳ�ǲ��ɹ� 
	datalen16 = 64;
	ret = psRead(m_devHandle, 10192, PS_STORES_IFAR, sizeof(FixParam) / 2, (uint16*)data, &datalen16);
	TRACE("LINE:%d ret=%d ERROR=%d\n", __LINE__, ret, teGetLastError(m_devHandle));
	// PS_STORES_R = 8��Other Error)

	//ret=2 unsupport
	//ret = psReadUsrValue(handle, 10192, sizeof(FixParam) / 2, &datalen16, (uint16*)data);
	//TRACE("LINE:%d ret=%d\n", __LINE__, ret);

}


void CProductDevToolsDlg::OnBnClickedButtonSend()
{
	int32 ret;
	CString sText;
	uint16 data[128], datlen;

	m_edRecv.SetWindowTextA("");
	UpdateData(TRUE);

	if (m_devHandle < 1) {
		m_edRecv.SetWindowText("Please Open First");
		return;
	}

	m_edSend.GetWindowText(sText);
	datlen = sText.GetLength();
	memcpy(data, sText.GetBuffer(), sText.GetLength());
	sText.ReleaseBuffer();


	ret = teAppWrite(m_devHandle, 0, data, datlen/2);
	if (ret == TE_OK) {
		sText.Format("���ͳɹ�");
	}
	else
		sText.Format("����ʧ��:%d", ret);

	m_edRecv.SetWindowTextA(sText);
}


void CProductDevToolsDlg::OnBnClickedButtonRecv()
{
	if (m_devHandle < 1) {
		m_edRecv.SetWindowText("Please Open First");
		return;
	}

	CString sText;
	int32 ret;
	uint8 channel;
	uint16 data[128], datlen = 128;
	ret = teAppRead(m_devHandle, &channel, data, 128, &datlen, 2000);
	if (ret == TE_OK)
	{
		data[datlen] = 0;
		sText.Format("RECV[%d]:%s", datlen, (char*)data);
	}
	else {
		sText.Format("����ʧ��:%d", ret);
	}
	m_edRecv.SetWindowTextA(sText);
}


void CProductDevToolsDlg::OnBnClickedButtonClose()
{
	int ret = -11;

	if (m_devHandle > 0) {
		ret = closeTestEngine(m_devHandle);
		m_devHandle = 0;
	}

	m_edRecv.SetWindowTextA("");
	m_edBtAddr.SetWindowTextA("");
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);
}


void CProductDevToolsDlg::OnBnClickedBtnStart()
{
	CString sText;

	UpdateData(TRUE);
	m_edFirmName.GetWindowText(sText);	m_devCtrl.SetFlashImage(sText);
	m_edAddr.GetWindowText(sText); m_devCtrl.SetBtAddr(sText);	

	m_devCtrl.Start(this->m_hWnd);
}




LRESULT CProductDevToolsDlg::OnDevCtrlError(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	int count = m_ListCtrl.GetItemCount(), colum = 1;

	sText.Format("%02d", count + 1);
	m_ListCtrl.InsertItem(count, sText);

	sText.Format("Error");
	m_ListCtrl.SetItemText(count, colum, sText); colum += 1;

	sText.Format("%02d", (int)wParam);
	m_ListCtrl.SetItemText(count, colum, sText); colum += 1;


	return 0;
}

LRESULT CProductDevToolsDlg::OnDevCtrlReport(WPARAM wParam, LPARAM lParam)
{
	CString sText;
	int count = m_ListCtrl.GetItemCount(), colum = 1;

	sText.Format("%02d", count + 1);
	m_ListCtrl.InsertItem(count, sText);

	sText.Format("Report");
	m_ListCtrl.SetItemText(count, colum, sText); colum += 1;

	sText.Format("%03d--%s", (int)wParam, m_devCtrl.Report2String((int)wParam));
	m_ListCtrl.SetItemText(count, colum, sText); colum += 1;

	switch ((int)wParam) {
	case REPORT_PROGRAM_PERCENT:
		sText.Format("%02d%%", (int)lParam);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
		break;
	case REPORT_COMMU_READ:
		sText.Format("%s", (char*)lParam);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
		break;
	}

	return 0;
}


void CProductDevToolsDlg::OnBnClickedBtnOpenFile()
{
	CFileDialog dlg(TRUE, "*.txt", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"�̼��ļ�(*.xuv)|*.xuv|�����ļ�(*.*)|*.*||", this);

	if (IDOK != dlg.DoModal())
		return;

	CString sPath = dlg.GetPathName();

	m_edFirmName.SetWindowTextA(sPath);
}
