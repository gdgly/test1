#pragma once


// CColorStatic
enum {STATIC_WND_DISABLE=0, STATIC_WND_WARNING, STATIC_WND_SUCCESS, STATIC_WND_LAST};
#define STATIC_MAX_COLORS             (2)
class CColorStatic : public CStatic
{
	DECLARE_DYNAMIC(CColorStatic)

public:
	CColorStatic();
	virtual ~CColorStatic();

public:
	int SetColor(UINT type, COLORREF crBK, COLORREF crFont, BOOL bDraw = TRUE);
	int SetColor(UINT type, BOOL bDraw = TRUE);

private:	
	UINT m_crSelect;
	COLORREF	m_crColors[STATIC_WND_LAST][2];	// Colors to be used

	int DrawColorStatic(CDC *pDC);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
};


