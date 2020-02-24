
// ProductCaseToolsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProductCaseTools.h"
#include "ProductCaseToolsDlg.h"
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


// CProductCaseToolsDlg �Ի���



CProductCaseToolsDlg::CProductCaseToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductCaseToolsDlg::IDD, pParent)
	, m_iPort(15)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProductCaseToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CHECKDEV, m_ListCtrl);
	DDX_Text(pDX, IDC_EDIT_COM_PORT, m_iPort);
}

BEGIN_MESSAGE_MAP(CProductCaseToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DEV_STRING, &CProductCaseToolsDlg::OnCtrlDevice)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CHECKDEV, &CProductCaseToolsDlg::OnNMDblclkListCheckdev)
	ON_BN_CLICKED(IDC_BTN_START, &CProductCaseToolsDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CProductCaseToolsDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CProductCaseToolsDlg ��Ϣ�������

BOOL CProductCaseToolsDlg::OnInitDialog()
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
	m_iPort = ::AfxGetApp()->GetProfileInt("PRODUCT_CASE", "COM_PORT", 0);
	UpdateData(FALSE);

	m_CtrlMain.Init(&m_ListCtrl);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CProductCaseToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CProductCaseToolsDlg::OnPaint()
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
HCURSOR CProductCaseToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CProductCaseToolsDlg::OnCtrlDevice(WPARAM wParam, LPARAM lParam)
{
	CString sText((char*)lParam);
	int result = m_CtrlMain.OnRecvText((UINT)wParam, (CString)sText);

	return 0;
}


void CProductCaseToolsDlg::OnNMDblclkListCheckdev(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	int nRow = m_ListCtrl.GetNextSelectedItem(pos);

	if(nRow >= 0)
		m_CtrlMain.SetItemChecked(nRow);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CProductCaseToolsDlg::OnBnClickedBtnStart()
{
	UpdateData(TRUE);
	::AfxGetApp()->WriteProfileInt("PRODUCT_CASE", "COM_PORT", m_iPort);

	m_CtrlMain.SetComPort(m_iPort, "Test");
	m_CtrlMain.Start(this->m_hWnd);
}


void CProductCaseToolsDlg::OnBnClickedBtnStop()
{
	m_CtrlMain.Stop();
}
