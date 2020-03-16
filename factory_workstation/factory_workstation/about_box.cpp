// about_box.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "about_box.h"
#include "afxdialogex.h"


// about_box 对话框

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


// about_box 消息处理程序
