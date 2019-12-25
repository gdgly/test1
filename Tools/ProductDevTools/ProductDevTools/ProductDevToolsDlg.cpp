
// ProductDevToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProductDevTools.h"
#include "ProductDevToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CProductDevToolsDlg 对话框



CProductDevToolsDlg::CProductDevToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PRODUCTDEVTOOLS_DIALOG, pParent)
	, m_bEraseAll(FALSE)
	, m_edCap(7)
	, m_edTrim(6)
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
	DDX_Control(pDX, IDC_EDIT_HWVER, m_edHWver);
	DDX_Check(pDX, IDC_CHECK_ERASE, m_bEraseAll);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edName);
	DDX_Control(pDX, IDC_LIST_CHECK, m_ListCheck);
	DDX_Text(pDX, IDC_EDIT_CAP, m_edCap);
	DDX_Text(pDX, IDC_EDIT_TRIM, m_edTrim);
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
	ON_BN_CLICKED(IDC_BTN_STOP, &CProductDevToolsDlg::OnBnClickedBtnStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_PSWRITE, &CProductDevToolsDlg::OnBnClickedButtonPswrite)
	ON_BN_CLICKED(IDC_BTN_RESET, &CProductDevToolsDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_BURN_APO, &CProductDevToolsDlg::OnBnClickedBtnBurnApo)
	ON_BN_CLICKED(IDC_BTN_BURN_ADDR, &CProductDevToolsDlg::OnBnClickedBtnBurnAddr)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CProductDevToolsDlg::OnBnClickedBtnCheck)
	ON_BN_CLICKED(IDC_BTN_RECORD, &CProductDevToolsDlg::OnBnClickedBtnRecord)
	ON_BN_CLICKED(IDC_BTN_CRITRIM, &CProductDevToolsDlg::OnBnClickedBtnCritrim)
	ON_BN_CLICKED(IDC_BTN_RECORD_1, &CProductDevToolsDlg::OnBnClickedBtnRecord1)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CProductDevToolsDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_WAKEUP, &CProductDevToolsDlg::OnBnClickedBtnWakeup)
	ON_BN_CLICKED(IDC_BTN_SENSOR, &CProductDevToolsDlg::OnBnClickedBtnSensor)
	ON_BN_CLICKED(IDC_BTN_CASEPLC, &CProductDevToolsDlg::OnBnClickedBtnCaseplc)
	ON_BN_CLICKED(IDC_BTN_TAP, &CProductDevToolsDlg::OnBnClickedBtnTap)
	ON_BN_CLICKED(IDC_BTN_DUT_MODE, &CProductDevToolsDlg::OnBnClickedBtnDutMode)
	ON_BN_CLICKED(IDC_BTN_PLAY, &CProductDevToolsDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_ALL, &CProductDevToolsDlg::OnBnClickedBtnAll)
	ON_BN_CLICKED(IDC_BTN_CRITRIM_READ, &CProductDevToolsDlg::OnBnClickedBtnCritrimRead)
	ON_BN_CLICKED(IDC_BTN_CRITRIM_WRITE, &CProductDevToolsDlg::OnBnClickedBtnCritrimWrite)
END_MESSAGE_MAP()


// CProductDevToolsDlg 消息处理程序

BOOL CProductDevToolsDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	CString sText;
	sText = ::AfxGetApp()->GetProfileString("PRODUCT_CONFIG", "IMAGENAME", "");
	m_edFirmName.SetWindowTextA(sText);
	CDeviceCtrl::LoadIniParam("./flash_image.ini", &m_Param);
	CDeviceCtrl::LoadIniParam(sText, &m_Param);
	m_devCtrl.LoadIniParam(sText);


	m_edHWver.SetWindowText(m_Param.hwVer);
	m_edAddr.SetWindowTextA("{0x00ff0F, 0x5b, 0x02}");
	if (m_Param.btName.IsEmpty())
		m_Param.btName.Format("iFlyBuds");
	sText.Format("\"%s\"", m_Param.btName);
	m_edName.SetWindowTextA(sText);
	m_edSend.SetWindowTextA("abcdefg测试数据1234567");

	int nCol = 0;
	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;           //
	m_ListCtrl.SetExtendedStyle(dwStyle);  //

	m_ListCtrl.InsertColumn(nCol++, _T("序号"), LVCFMT_CENTER, 60);
	m_ListCtrl.InsertColumn(nCol++, _T("项目"), LVCFMT_CENTER, 100);
	m_ListCtrl.InsertColumn(nCol++, _T("测试"), LVCFMT_LEFT, 200);
	m_ListCtrl.InsertColumn(nCol++, _T("其它"), LVCFMT_LEFT, 300);

	dwStyle = m_ListCheck.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;           //
	m_ListCheck.SetExtendedStyle(dwStyle);  //

	m_ListCheck.InsertColumn(nCol++, _T("序号"), LVCFMT_CENTER, 60);
	m_ListCheck.InsertColumn(nCol++, _T("测试项目"), LVCFMT_CENTER, 100);
	m_ListCheck.InsertColumn(nCol++, _T("结果"), LVCFMT_LEFT, 200);
	m_ListCheck.InsertColumn(nCol++, _T("其它"), LVCFMT_LEFT, 300);
	CheckItemInit();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CProductDevToolsDlg::OnPaint()
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
HCURSOR CProductDevToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef struct tagCHTECKITEMINFO {
	unsigned short result;       // CHECK result, 1 success
	unsigned short uID;          // 通过 ID 或IDString 来查找对应的条目
	char           *sIDStr;

	char           *sPrompt;
}CheckItem, *CheckIPtr;

CheckItem gItems[] = {
	{ 0,  1, "BurnApo",     "Apo烧写"},
	{ 0,  2, "BurnQCC",     "主芯片烧写"},

	{ 0,  3, "FirmVer",     "固件版本"},
	{ 0,  4, "ApoVer",      "Apo固件版本" },


	{ 0,  51, "MIC1",       "主MIC测试" },
	{ 0,  52, "MIC2",       "附MIC测试" },

	{ 0,  0, NULL, NULL}, // Last
};

void CProductDevToolsDlg::CheckItemInit(void)
{
	int index = 0;
	CString sText;
	CheckIPtr pItem;

	m_ListCheck.DeleteAllItems();
	while (TRUE) {
		pItem = &gItems[index];
		if (pItem->result != 0 || pItem->uID == 0)
			break;

		sText.Format("%d", index + 1);
		m_ListCheck.InsertItem(index, sText);

		m_ListCheck.SetItemText(index, 1, pItem->sPrompt);

		index += 1;
	}
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
// 注意不能快速重复调用这个函数，否则可能在teConfigCacheInit 出错
void CProductDevToolsDlg::OnBnClickedOk()
{
//	flGetPorts();
//	flmGetPorts();

	char data[KEY_READ_BUFFER_LEN]; uint32 datalen; uint16 datalen16;
	int ret, ret2;
	CString sText;

	m_edBtAddr.SetWindowTextA("开始打开...");
	
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

	//  注意需要 D:\QCC512X\BlueSuite 3.2.2
	//  所有的DLL文件 及hpm文件

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
	// psReadBdAddr  有一部分不正确,不知道为什么出现a5a5  // 0x00a5a5 5b 0002

#if 0
	/* 修改蓝牙地址 OK*/
	data[6] = 'a';
	ret = teConfigCacheWriteItem(m_devHandle, "bt2:pskey_bdaddr", data);
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);

	ret = teConfigCacheWrite(m_devHandle, NULL, 0);
	TRACE("LINE:%d ret=%d\n", __LINE__, ret);
#endif

	// 不成功
	datalen16 = 64;
	ret = tePsRead(m_devHandle, 10192, sizeof(FixParam)/2, (uint16*)data, &datalen16);
	TRACE("LINE:%d ret=%d ERROR=%d\n", __LINE__, ret, teGetLastError(m_devHandle));


	// 也是不成功 
	datalen16 = 64;
	ret = psRead(m_devHandle, 10192, PS_STORES_IFAR, sizeof(FixParam) / 2, (uint16*)data, &datalen16);
	TRACE("LINE:%d ret=%d ERROR=%d\n", __LINE__, ret, teGetLastError(m_devHandle));
	// PS_STORES_R = 8（Other Error)

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
		sText.Format("发送成功");
	}
	else
		sText.Format("发送失败:%d", ret);

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
		sText.Format("接收失败:%d", ret);
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


void CProductDevToolsDlg::OnBnClickedButtonPswrite()
{
	unsigned char buf[24];

	memset(buf, 0xFF, sizeof(buf));
	m_devCtrl.PsWrite(9742 + 141, buf, sizeof(buf));
}

void CProductDevToolsDlg::StartDevContrl(UINT type)
{
	CString sText;
	UpdateData(TRUE);

	m_edFirmName.GetWindowText(sText);	m_devCtrl.SetFlashImage(sText);
	if (!sText.IsEmpty()) {
		DWORD dwAttrib = GetFileAttributes(sText);
		if (INVALID_FILE_ATTRIBUTES != dwAttrib) {
			BOOL ret = ::AfxGetApp()->WriteProfileString("PRODUCT_CONFIG", "IMAGENAME", sText);
			TRACE("Write FlashImage%d:%s\n", ret, sText);
		}
	}
	m_edName.GetWindowText(sText); m_devCtrl.SetBtName(sText);
	m_edAddr.GetWindowText(sText); m_devCtrl.SetBtAddr(sText);
	m_edHWver.GetWindowTextA(sText); m_devCtrl.SetHwVersion(sText);
	m_devCtrl.SetEraseAll(m_bEraseAll);

	m_devCtrl.SetThreadFunc(type);
	m_devCtrl.Start(this->m_hWnd);


}

void CProductDevToolsDlg::OnBnClickedBtnStart()
{
	StartDevContrl(THREAD_BURN);
}

void CProductDevToolsDlg::OnBnClickedBtnBurnApo()
{
	StartDevContrl(THREAD_BURN_APO);
}


void CProductDevToolsDlg::OnBnClickedBtnBurnAddr()
{
	StartDevContrl(THREAD_BT_ADDR);
}


void CProductDevToolsDlg::OnBnClickedBtnCheck()
{
	StartDevContrl(THREAD_CHECK);
}


void CProductDevToolsDlg::OnBnClickedBtnRecord()
{
	StartDevContrl(THREAD_RECORD_0);
}

void CProductDevToolsDlg::OnBnClickedBtnRecord1()
{
	StartDevContrl(THREAD_RECORD_1);
}

void CProductDevToolsDlg::OnBnClickedBtnCritrim()
{
	StartDevContrl(THREAD_CRYSTGALTRIM);
}

void CProductDevToolsDlg::OnBnClickedBtnWakeup()
{
	StartDevContrl(THREAD_WAKEUP);
}


void CProductDevToolsDlg::OnBnClickedBtnSensor()
{
	StartDevContrl(THREAD_SENSOR);
}


void CProductDevToolsDlg::OnBnClickedBtnCaseplc()
{
	StartDevContrl(THREAD_PLC);
}

void CProductDevToolsDlg::OnBnClickedBtnTap()
{
	StartDevContrl(THREAD_TAP);
}

void CProductDevToolsDlg::OnBnClickedBtnPlay()
{
	StartDevContrl(THREAD_PLAY);
}

void CProductDevToolsDlg::OnBnClickedBtnCritrimRead()
{
	StartDevContrl(THREAD_CRYSTGALTRIM_READ);
}

void CProductDevToolsDlg::OnBnClickedBtnCritrimWrite()
{
	UpdateData(TRUE);

	m_devCtrl.SetXtalTrim(m_edCap, m_edTrim);
	StartDevContrl(THREAD_CRYSTGALTRIM_WRITE);
}


void CProductDevToolsDlg::OnBnClickedBtnAll()
{
//	StartDevContrl(THREAD_CHECK | THREAD_RECORD_0 | THREAD_RECORD_1| THREAD_PLAY);
//	StartDevContrl(THREAD_CHECK | THREAD_RECORD_0 | THREAD_RECORD_1| THREAD_PLAY| THREAD_WAKEUP| THREAD_SENSOR| THREAD_PLC| THREAD_TAP);
//	StartDevContrl(THREAD_CHECK| THREAD_SENSOR);
	StartDevContrl(THREAD_CHECK | THREAD_PLC| THREAD_TAP);
}



void CProductDevToolsDlg::OnBnClickedBtnDutMode()
{
	m_devCtrl.EnterDUTMode();
}


void CProductDevToolsDlg::OnBnClickedBtnStop()
{
	m_devCtrl.Stop();
}

void CProductDevToolsDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	MSG msg;
	m_devCtrl.Stop();
	for (int i = 0; i < 1000; i++) {
		if (FALSE == m_devCtrl.IsRuning()) {
			TRACE("DevCtrl Exit i=%d\n", i);
			break;
		}

		if (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		Sleep(3);
	}
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

	if (0 != lParam) {
		sText.Format("%s", (int)lParam);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
	}

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

	case REPORT_APOLLO:
	case REPORT_READ_XTALTRIM:
	case REPORT_READ_XTALCAP:
	case REPORT_WRITE_XTALTRIM:
	case REPORT_WRITE_XTALCAP:

	case REPORT_RDBD_NAME:
	case REPORT_WRBD_NAME:
	case REPORT_RDBD_ADDR:
	case REPORT_WRBD_ADDR:
	case REPORT_COMMU_SUCC:
		sText.Format("%s", (char*)lParam);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
		break;
	case REPORT_COMMU_READ:
	case REPORT_READ_SENSOR:
	case REPORT_WRITE_SENSOR:
	case REPORT_PLAY_TONE:
	case REPORT_WAKEUP:
	case REPORT_SENSOR:
	case REPORT_PLC:
	case REPORT_TAP:
		sText.Format("%s", (char*)lParam);
		OnReportCheck(sText, count);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
		break;
	case REPORT_READ_RECORD:
		sText.Format("Recv=%d", (int)lParam);
		OnReportCheck(sText, count);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
		break;
	case REPORT_END_ALL:
		sText.Format("result=%d", (int)lParam);
		m_ListCtrl.SetItemText(count, colum, sText); colum += 1;
		break;
	}

	m_ListCtrl.EnsureVisible(count, FALSE);

	return 0;
}

CString _sCheckString[] = { "DEVICEINFO", "VERSION", "BTADDR", "BTNAM",
	"LIS25", "EM20168","LIS2DW12", "MAX20340", "APOLLO",
	"",
};


int CProductDevToolsDlg::OnReportCheck(CString sText, int rows)
{	
	int ret, index;
	CString sTmp;
	CString sResult[] = { "PASS", "fail" };

	if (sText.IsEmpty())
		return -1;

	ret = sText.Find("check");
	if(ret < 0 || ret > 2)     // maybe blank
		return -2;

	index = 0;
	while (1) {
		if (_sCheckString[index].IsEmpty())
			break;

		if (sText.Find(_sCheckString[index]) < 0) {
			index += 1;
			continue;
		}

		ret = -1;
		switch (index) {
		case 0:           // 开始
			break;
		case 1:           // 版本
			sTmp.Format("%s.%s.%s-%s.%s.%s.%s",
				m_Param.hwVer.Mid(0, 2), m_Param.hwVer.Mid(2, 2), m_Param.hwVer.Mid(4, 2),
				m_Param.swVer.Mid(0, 2), m_Param.swVer.Mid(2, 2), m_Param.swVer.Mid(4, 2), m_Param.swVer.Mid(6, 2));
			ret = (sText.Find(sTmp) > 0 ) ? 0 : 1;
			break;
		case 2:           // 地址 [{0x00ff0F, 0x5b, 0x02}     00:02:5B:00:ff:0F
			ret = 0;
			break;
		case 3:
			break;

		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			ret = (sText.Find("PASS") > 0) ? 0 : 1;
			break;
		}

		if(ret >= 0 && ret < 2)
			m_ListCtrl.SetItemText(rows, 2, sResult[ret]);

		break;
	}



	return 0;
}


void CProductDevToolsDlg::OnBnClickedBtnOpenFile()
{
	CFileDialog dlg(TRUE, "*.txt", 0, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"固件文件(*.xuv)|*.xuv|所有文件(*.*)|*.*||", this);

	if (IDOK != dlg.DoModal())
		return;

	CString sPath = dlg.GetPathName();

	m_edFirmName.SetWindowTextA(sPath);
}

void CProductDevToolsDlg::OnBnClickedBtnReset()
{
	if (m_devCtrl.OpenEngine() >= 0) {
		m_devCtrl.CloseEngine();
		AfxMessageBox("ResetOK");
	}	
}




void CProductDevToolsDlg::OnBnClickedBtnClear()
{
	m_ListCtrl.DeleteAllItems();
}






