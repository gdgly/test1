#pragma once

#include "CtrlDevice.h"

enum {IMAGE_DEFAULT=0, IMAGE_RUN, IMAGE_REV, IMAGE_WAIT, IMAGE_OK, IMAGE_OK_HALF, IAMGE_FAIL};
enum {COL_No=0, COL_SHOWNAME, COL_COM, COL_IMAGE, COL_COMMENT};
enum {ITEM_CHECK_NONE=0, ITEM_CHECK_1=0x01, ITEM_CHECK_2=0x02, ITEM_CHECK_1AND2 = 0x03};
typedef struct tagCTRLITEMINFO {
	const char       *showName;                        // 显示名称
	const char       *succPtr1, *succPtr2;             // 如果两个都非空，则需要同时检测到两个
	const char       *failPtr;                         // 出错检测
	
	const char       *command;                         // 检测时需要发送命令
	unsigned char    waitsend;                         // 命令发送后，需要等待时间SEC，不能连续快速发送

	unsigned short   itemType;
	unsigned short   itemID;

	unsigned char    cmdsended;                        // 命令是否已经发送 0:没有发送
	unsigned char    checked;                          // ITEM_CHECK_XX 本条目是否检测完成
	unsigned short   nRow;                             // 在列表框中的行号

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

