// about_box.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "about_box.h"
#include "afxdialogex.h"


// about_box �Ի���

IMPLEMENT_DYNAMIC(about_box, CDialogEx)

about_box::about_box(CWnd* pParent /*=NULL*/)
	: CDialogEx(about_box::IDD, pParent)
{

}

about_box::~about_box()
{
}

void about_box::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(about_box, CDialogEx)
END_MESSAGE_MAP()


// about_box ��Ϣ�������
