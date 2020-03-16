// software_tool_gener_barcode.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_gener_barcode.h"
#include "afxdialogex.h"


// software_tool_gener_barcode �Ի���

IMPLEMENT_DYNAMIC(software_tool_gener_barcode, CDialogEx)

software_tool_gener_barcode::software_tool_gener_barcode(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_gener_barcode::IDD, pParent)
{

}

software_tool_gener_barcode::~software_tool_gener_barcode()
{
}

void software_tool_gener_barcode::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
}


BEGIN_MESSAGE_MAP(software_tool_gener_barcode, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON3, &software_tool_gener_barcode::OnBnClickedButton3)
END_MESSAGE_MAP()


// software_tool_gener_barcode ��Ϣ�������


void software_tool_gener_barcode::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sText;
	CRect rect;
	CDC *pDC;
	CWnd *pWnd;
	m_edit.GetWindowTextA(sText);
	pWnd=GetDlgItem(IDC_STATIC_barcode); //���pictrue�ؼ����ڵľ��
	pWnd->GetClientRect(&rect); //���pictrue�ؼ����ڵľ�������
	pDC=pWnd->GetDC(); //���pictrue�ؼ���DC
	pDC->FillSolidRect(rect, RGB(255, 255, 255));
	m_barcode.out_draw_bar(pDC, rect, sText);
}


BOOL software_tool_gener_barcode::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}


BOOL software_tool_gener_barcode::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString soft_name;
	soft_name += "�������ɹ���";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	return TRUE;  // return TRUE unless you set the focus to a control
}
