#include "stdafx.h"
#include "CtrlItem.h"
#include "resource.h"

CtrlItemInfo gItemArr[] = {
//    ÏÔÊ¾Ãû³Æ              ¼ì²â×Ö·û´®Ò»          ¼ì²â×Ö·û´®¶þ       ³ö´í×Ö·û´®     ·¢ËÍÃüÁî     ·¢ËÍÃüÁîµÈ´ýÊ±¼ä
	{"ÏµÍ³¸´Î»",              "Power On",            NULL,            NULL,        "reset case\r\n",  0,},
	{"WatchDog",              "WDOG inited",         NULL,            NULL,        },
	{"MAX77813Ð¾Æ¬",          "max77813 init ok",    NULL,            "max77813 i2c read id error",        },
	{"MAX23040Ð¾Æ¬",          "max20340 init ok",    NULL,            "max20340 read id error",        },
	{"MAX23040×ó¶úÍ¨ÐÅ",      "Left Ear 0 to 1",     NULL,            NULL,        },
	{"MAX23040ÓÒ¶úÍ¨ÐÅ",      "Right Ear 0 to 1",    NULL,            NULL,        },
	{"ºÏ¸Ç¿ª¹Ø¼ì²â",          "hal close",           "hal open",      NULL,        },
	{"°´¼ü¼ì²â",              "key down",            "key up",        NULL,        },
	{"USB³äµç¼ì²â",           "usbdet out",          "usbdet in",     NULL,        },
	{"Ö¸Ê¾µÆ¼ì²â-ºì",         NULL,                  NULL,            NULL,        "ledlight 0\r\n",        },
	{"Ö¸Ê¾µÆ¼ì²â-ÂÌ",         NULL,                  NULL,            NULL,        "ledlight 1\r\n", },
	{"Ö¸Ê¾µÆ¼ì²â-À¶",         NULL,                  NULL,            NULL,        "ledlight 2\r\n", },
	{"Ö¸Ê¾µÆ¼ì²â-°×",         NULL,                  NULL,            NULL,        "ledlight 6\r\n", },
	{"Ö¸Ê¾µÆ¼ì²â-³È",         NULL,                  NULL,            NULL,        "ledlight 3\r\n", },
	
};

CCtrlItem::CCtrlItem(void)
{
	m_pListCtrl    = NULL;
}


CCtrlItem::~CCtrlItem(void)
{
}


UINT CCtrlItem::GetCheckItemCount(void)
{
	return sizeof(gItemArr)/ sizeof(gItemArr[0]);
}


int CCtrlItem::SetImageListCtrl(CListCtrl *pListCtrl, int nRow, int nCol, int image)
{
	char buffer[24];
	LV_ITEM lvitem;

	if(!pListCtrl)
		return -1;
	
	lvitem.iItem = nRow;
	lvitem.iSubItem = nCol;
	
	sprintf_s(buffer, "%02d", nRow+1);
	if(COL_IMAGE == nCol)
		lvitem.mask = LVIF_IMAGE;
	else
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvitem.stateMask = 0;
	lvitem.iImage = image;
	lvitem.pszText = buffer;
	pListCtrl->SetItem(&lvitem);
	return 0;
}


int CCtrlItem::SetRunListCtrl(CListCtrl *pListCtrl, int nRow, int image)
{
	return SetImageListCtrl(pListCtrl, nRow, COL_No, image);
}

int CCtrlItem::SetStateListCtrl(CListCtrl *pListCtrl, int nRow, int image)
{
#if 10
	if(pListCtrl)
		pListCtrl->SetItemData(nRow, image);

	return SetImageListCtrl(pListCtrl, nRow, COL_IMAGE, image);
#else
	LV_ITEM lvitem;

	if(!pListCtrl)
		return -1;
	
	lvitem.mask = LVIF_IMAGE;
	lvitem.stateMask = 0;
	lvitem.iItem = nRow;
	lvitem.iSubItem = COL_IMAGE;
	lvitem.iImage = image;
	pListCtrl->SetItem(&lvitem);

	pListCtrl->SetItemData(nRow, image);
	return 0;
#endif
}

int CCtrlItem::InitListCtrl(CListCtrl *pListCtrl)
{
	if(!pListCtrl)
		return -1;
	
	DWORD dwStyle = pListCtrl->GetExtendedStyle();

	if((dwStyle & LVS_EX_SUBITEMIMAGES) == 0) {
		dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES;
		//	dwStyle |= LVS_EX_CHECKBOXES;
		pListCtrl->SetExtendedStyle(dwStyle);

		if(NULL == m_ImageState.m_hImageList)
			m_ImageState.Create(IDB_BITMAP_CTRL, 48, 48, RGB(255,255,255));
		pListCtrl->SetImageList(&m_ImageState,  LVSIL_SMALL);

		int nCol = 0;
		pListCtrl->InsertColumn(nCol++, _T("ÐòºÅ"), LVCFMT_CENTER, 90);
		pListCtrl->InsertColumn(nCol++, _T("¼ì²âÏîÄ¿"), LVCFMT_LEFT, 200);
		pListCtrl->InsertColumn(nCol++, _T("²âÊÔ"), LVCFMT_LEFT, 200);
		pListCtrl->InsertColumn(nCol++, _T("½á¹û"), LVCFMT_CENTER, 60);
		pListCtrl->InsertColumn(nCol++, _T("ÆäËü"), LVCFMT_LEFT, 500);
	}
	
	pListCtrl->DeleteAllItems();
	CString sText;
	CtrlIPtr pCtrlI;
	int count = sizeof(gItemArr)/ sizeof(gItemArr[0]);
	for(int i = 0; i < count; i++) {
		pCtrlI = &gItemArr[i];
		pCtrlI->checked    = 0;
		pCtrlI->nRow       = i;
		pCtrlI->cmdsended  = 0;
		sText.Format("%02d", i+1);
		pListCtrl->InsertItem(i, sText, IMAGE_DEFAULT);
#if 0
		LV_ITEM lvitem;
		lvitem.mask = LVIF_TEXT;
		lvitem.state = 0;
		lvitem.stateMask = 0;
		lvitem.iItem = i;
		lvitem.iSubItem = COL_No;

		lvitem.pszText = sText.GetBuffer();

		//pListCtrl->InsertItem(&lvitem);
	//	pListCtrl->SetItem(&lvitem);
		sText.ReleaseBuffer();

		pListCtrl->GetItem(&lvitem);
#endif

		pListCtrl->SetItemText(i, COL_SHOWNAME, pCtrlI->showName);
				
		SetStateListCtrl(pListCtrl, i, IMAGE_WAIT);
	}

	m_pListCtrl = pListCtrl;

	return 0;
}


void CCtrlItem::ResetChekitem(CListCtrl *pListCtrl)
{
	if(!pListCtrl)
		pListCtrl = m_pListCtrl;

	CString sText;
	CtrlIPtr pCtrlI;
	int count = sizeof(gItemArr)/ sizeof(gItemArr[0]);
	for(int i = 0; i < count; i++) {
		pCtrlI = &gItemArr[i];
		pCtrlI->checked    = 0;
		pCtrlI->nRow       = i;
		pCtrlI->cmdsended  = 0;
		sText.Format("%02d", i+1);
		SetRunListCtrl(pListCtrl, i, IMAGE_DEFAULT);
		SetStateListCtrl(pListCtrl, i, IMAGE_WAIT);
	}
//	InitListCtrl(pListCtrl);
}

int CCtrlItem::OnRecvTextReport( CString sText)
{
	int offset, row;
	CString tmp;

	//sText.Format("WaitUser row=%d", pCtrlI->nRow);
	if((offset=sText.Find("WaitUser row=")) >= 0) {
		tmp = sText.Mid(offset+13, 3);
		row = _ttoi(tmp);
		SetRunListCtrl(m_pListCtrl, row, IMAGE_RUN);
	}

	return 0;
}

int CCtrlItem::OnRecvTextDevice( CString sText)
{
	int count;
	CtrlIPtr pCtrlI = NULL;
//	TRACE("sText:%s\n", sText);

	count = sizeof(gItemArr)/ sizeof(gItemArr[0]);
	for(int i = 0; i < count; i++) {
		pCtrlI = &gItemArr[i];
		if(ITEM_CHECK_1AND2 == pCtrlI->checked) {
			pCtrlI = NULL;
			continue;
		}

		if(pCtrlI->succPtr1) {
			if(sText.Find(pCtrlI->succPtr1) >= 0) {
				pCtrlI->checked |= ITEM_CHECK_1;
				if(NULL == pCtrlI->succPtr2)
					pCtrlI->checked |= ITEM_CHECK_1AND2;
				break;
			}
		}


		if(pCtrlI->succPtr2) {
			if(sText.Find(pCtrlI->succPtr2) >= 0) {
				pCtrlI->checked |= ITEM_CHECK_2;
				if(NULL == pCtrlI->succPtr1)
					pCtrlI->checked |= ITEM_CHECK_1AND2;
				break;
			}
		}

		pCtrlI = NULL;
	}

	if(pCtrlI && pCtrlI->checked) {
		if(pCtrlI->checked == ITEM_CHECK_1AND2)
			SetStateListCtrl(m_pListCtrl, pCtrlI->nRow, IMAGE_OK);
		else			
			SetStateListCtrl(m_pListCtrl, pCtrlI->nRow, IMAGE_OK_HALF);

		m_pListCtrl->SetItemText(pCtrlI->nRow, COL_COMMENT, sText);
	}

	return 0;
}


int CCtrlItem::SetItemChecked(int nRow)
{
	int count;
	CtrlIPtr pCtrlI = NULL;

	if(!m_pListCtrl)
		return -1;

	count = sizeof(gItemArr)/ sizeof(gItemArr[0]);
	if(nRow >= count)
		return -2;

	pCtrlI = &gItemArr[nRow];
	if(!pCtrlI->command || 0 == pCtrlI->cmdsended)
		return -3;

	pCtrlI->checked = ITEM_CHECK_1AND2;
	m_pListCtrl->SetItemText(pCtrlI->nRow, COL_COMMENT, "CheckByUser");
	SetStateListCtrl(m_pListCtrl, pCtrlI->nRow, IMAGE_OK);

	return 0;
}
