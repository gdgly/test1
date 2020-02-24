#include "stdafx.h"
#include "CtrlMain.h"


CCtrlMain::CCtrlMain(void)
{
	m_pListCtrl  = NULL;
	m_hWnd       = 0;
}


CCtrlMain::~CCtrlMain(void)
{
}


int CCtrlMain::Init(UINT uIDListCtrl)
{
//	CListCtrl *pList = ::GetWindow(

	return 0;
}

int CCtrlMain::Init(CListCtrl *pListCtrl)
{
	if(!pListCtrl)
		return -1;

	m_pListCtrl = pListCtrl;
	m_CtrlItem.InitListCtrl(pListCtrl);

	return 0;
}


int CCtrlMain::Start(HWND hWnd)
{
	m_hWnd = hWnd;

	if(m_CtrlDev.IsRuning()) {
		AfxMessageBox("Current Runing ...");
		return -1;
	}

	m_CtrlItem.ResetChekitem(m_pListCtrl);
	m_CtrlDev.SetThreadFunc(THREAD_COM);
	m_CtrlDev.SetCheckItemCount(m_CtrlItem.GetCheckItemCount());

	return m_CtrlDev.Start(hWnd); 
}

int CCtrlMain::OnRecvText(UINT type, CString sText)
{
	if(MSG_T_DEV == type)
		m_CtrlItem.OnRecvTextDevice(sText);
	else if(MSG_T_REPORT) {
		if(sText.Find("FailOpenCom") >= 0) {
			AfxMessageBox("´ò¿ª´®¿ÚÊ§°Ü");
			return -1;
		}
		m_CtrlItem.OnRecvTextReport(sText);
	}

	return 0;
}

