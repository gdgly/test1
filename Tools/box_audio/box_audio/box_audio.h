
// box_audio.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cbox_audioApp:
// �йش����ʵ�֣������ box_audio.cpp
//

class Cbox_audioApp : public CWinApp
{
public:
	Cbox_audioApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cbox_audioApp theApp;