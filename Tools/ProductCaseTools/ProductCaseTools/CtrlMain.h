#pragma once

#include "CtrlItem.h"
#include "CtrlDevice.h"

class CCtrlMain
{
public:
	CCtrlMain(void);
	~CCtrlMain(void);

public:  // CCtrlDevice	
	void    SetComPort(int port, CString sName) { m_CtrlDev.SetComPort(port, sName);}
	int Start(HWND hWnd);
	int Stop(void) {if(m_CtrlDev.IsRuning()) m_CtrlDev.Stop(); return 0;}

public:
	int Init(CListCtrl *pListCtrl);
	int Init(UINT uIDListCtrl);

	int OnRecvText(UINT type, CString sText);
	int SetItemChecked(int nRow) { return m_CtrlItem.SetItemChecked(nRow); }


public:


private:
	HWND m_hWnd;
	CListCtrl *m_pListCtrl;
	CCtrlItem  m_CtrlItem;
	CCtrlDevice m_CtrlDev;
};

