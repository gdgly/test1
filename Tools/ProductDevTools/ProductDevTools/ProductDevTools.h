
// ProductDevTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProductDevToolsApp: 
// �йش����ʵ�֣������ ProductDevTools.cpp
//

class CProductDevToolsApp : public CWinApp
{
public:
	CProductDevToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProductDevToolsApp theApp;