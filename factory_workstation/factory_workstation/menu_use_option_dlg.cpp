// menu_use_option_dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "menu_use_option_dlg.h"
#include "afxdialogex.h"


// menu_use_option_dlg �Ի���

IMPLEMENT_DYNAMIC(menu_use_option_dlg, CDialogEx)

menu_use_option_dlg::menu_use_option_dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(menu_use_option_dlg::IDD, pParent)
{

}

menu_use_option_dlg::~menu_use_option_dlg()
{
}

void menu_use_option_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(menu_use_option_dlg, CDialogEx)
END_MESSAGE_MAP()


// menu_use_option_dlg ��Ϣ�������


BOOL menu_use_option_dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString sText;
	m_editFont.CreatePointFont(100, "����");
	GetDlgItem(IDC_STATIC)->SetFont(&m_editFont);
	sText = "��վʹ�÷���:\n\n";
	sText += "  1������������ȵ���������һ����Ҫ�Ĳ�����Ȼ�����Ӧ�Ĺ�վ���롣\n";
	sText += "  2���򿪹�վ��ɨ����������кż���ʼ������ֱ����pass����fail���֡�\n";
	sText += "      ע�⣺��δ���ʱ�벻Ҫ����ɨ��,�����������ر���������ԣ�\n";
	sText += "  3��Ĭ�ϵ���ҳ���а�ťֱ�Ӵ��ϴδ򿪵Ĺ�վ��\n";
	sText += "  4�����ֹ�վ�����˵��������������֧�ֵĲ��Ǻܺã����·���в�Ҫ�������ġ�\n";
	sText += "\nע������:\n\n";
	sText += "  С���������õ��Ĵ��ڶ���ͨ�������������á�\n";
	GetDlgItem(IDC_STATIC)->SetWindowTextA(sText);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
