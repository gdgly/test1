
// factory_workstation.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cfactory_workstationApp:
// �йش����ʵ�֣������ factory_workstation.cpp
//

class Cfactory_workstationApp : public CWinApp
{
public:
	Cfactory_workstationApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cfactory_workstationApp theApp;