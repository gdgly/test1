#pragma once

#include "CtrlDevice.h"

enum {IMAGE_DEFAULT=0, IMAGE_RUN, IMAGE_REV, IMAGE_WAIT, IMAGE_OK, IMAGE_OK_HALF, IAMGE_FAIL};
enum {COL_No=0, COL_SHOWNAME, COL_COM, COL_IMAGE, COL_COMMENT};
enum {ITEM_CHECK_NONE=0, ITEM_CHECK_1=0x01, ITEM_CHECK_2=0x02, ITEM_CHECK_1AND2 = 0x03};
typedef struct tagCTRLITEMINFO {
	const char       *showName;                        // ��ʾ����
	const char       *succPtr1, *succPtr2;             // ����������ǿգ�����Ҫͬʱ��⵽����
	const char       *failPtr;                         // ������
	
	const char       *command;                         // ���ʱ��Ҫ��������
	unsigned char    waitsend;                         // ����ͺ���Ҫ�ȴ�ʱ��SEC�������������ٷ���

	unsigned short   itemType;
	unsigned short   itemID;

	unsigned char    cmdsended;                        // �����Ƿ��Ѿ����� 0:û�з���
	unsigned char    checked;                          // ITEM_CHECK_XX ����Ŀ�Ƿ������
	unsigned short   nRow;                             // ���б���е��к�

}CtrlItemInfo, *CtrlIPtr;

extern CtrlItemInfo gItemArr[];
class CCtrlItem
{
public:
	CCtrlItem(void);
	~CCtrlItem(void);

public:
	int SetImageListCtrl(CListCtrl *pListCtrl, int nRow, int nCol, int image);
	int SetRunListCtrl(CListCtrl *pListCtrl, int nRow, int image);
	int SetStateListCtrl(CListCtrl *pListCtrl, int nRow, int image);
	int InitListCtrl(CListCtrl *pListCtrl);
	UINT GetCheckItemCount(void);
	void ResetChekitem(CListCtrl *pListCtrl);

public:
	int OnRecvTextReport(CString sText);
	int OnRecvTextDevice(CString sText);

	int SetItemChecked(int nRow);

private:
	CImageList m_ImageState;
	CListCtrl  *m_pListCtrl;

};

