// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"


// CColorStatic

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

CColorStatic::CColorStatic()
{
	SetColor(::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNTEXT), FALSE);

	m_crSelect = 0;
	m_crColors[STATIC_WND_DISABLE][0] = ::GetSysColor(COLOR_BTNFACE);
	m_crColors[STATIC_WND_DISABLE][1] = ::GetSysColor(COLOR_GRAYTEXT);

	m_crColors[STATIC_WND_WARNING][0] = RGB(255, 0, 0);
	m_crColors[STATIC_WND_WARNING][1] = RGB(0, 0, 0);

	m_crColors[STATIC_WND_SUCCESS][0] = RGB(0, 255, 0);
	m_crColors[STATIC_WND_SUCCESS][1] = RGB(255, 255, 255);
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CColorStatic::SetColor(UINT type, COLORREF crBK, COLORREF crFont, BOOL bDraw)
{
	if(type >= STATIC_WND_LAST)
		return -1;

	m_crColors[type][0]  = crBK;
	m_crColors[type][1]  = crFont;

	if(bDraw)
		Invalidate(TRUE);
	return 0;
}
int CColorStatic::SetColor(UINT type, BOOL bDraw)
{
	if(type >= STATIC_WND_LAST)
		return -1;

	m_crSelect = type;
	if(bDraw)
		Invalidate(TRUE);
	return 0;
}

BOOL CColorStatic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return FALSE;
	return CStatic::OnEraseBkgnd(pDC);
}


void CColorStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CDC*	pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	DrawColorStatic(pDC);
}


int CColorStatic::DrawColorStatic(CDC *pDC)
{
	CRect rect;
	GetWindowRect(&rect);

	DWORD dwStyle = GetStyle();
	pDC->SetBkMode(TRANSPARENT);

	if((dwStyle & WS_BORDER)) {
		pDC->Draw3dRect(rect, RGB(0,0,0), RGB(128,128,128));
	}

	GetClientRect(&rect);
	CBrush brush(m_crColors[m_crSelect][0]);
	pDC->FillRect(rect, &brush);
	
	CString sText;
	GetWindowText(sText);

	pDC->SetTextColor(m_crColors[m_crSelect][1]);

	if(dwStyle & WS_EX_LEFT)
		dwStyle = DT_LEFT|DT_WORDBREAK|DT_VCENTER|DT_SINGLELINE;
	else
		dwStyle = DT_CENTER|DT_WORDBREAK|DT_VCENTER|DT_SINGLELINE;	

	pDC->DrawText(sText, -1, rect, dwStyle);

	return 0;
}

void CColorStatic::PreSubclassWindow()
{
	ModifyStyle(0, SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}