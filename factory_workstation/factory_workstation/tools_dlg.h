#pragma once

#include "software_tool_box_audio.h"
#include "software_tool_version.h"
#include "software_tool_barcode.h"
#include "software_tool_current_board.h"
#include "software_tool_gener_barcode.h"
#include "software_tool_vnc.h"
#include "software_tool_by_getfile.h"
#include "software_tool_sz_btaddr.h"
// tools_dlg 对话框
#include <windows.h>
#include <Mmsystem.h>
class tools_dlg : public CPropertyPage
{
	DECLARE_DYNAMIC(tools_dlg)

public:
	tools_dlg();
	virtual ~tools_dlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_tools };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	bool isGetSound;
	static DWORD CALLBACK MicCallBack(HWAVEIN hWaveIn, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
	void *glob_param_p;
	void set_param(void *p);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedBtnSensorCalc();
	afx_msg void OnBnClickedBtnTapCalc();
};
