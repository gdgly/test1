// software_tool_tap_calc.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_tap_calc.h"
#include "afxdialogex.h"


// software_tool_tap_calc 对话框

IMPLEMENT_DYNAMIC(software_tool_tap_calc, CDialogEx)

software_tool_tap_calc::software_tool_tap_calc(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_tap_calc::IDD, pParent)
	, m_sCalcVal(_T(""))
	, m_sCalcOrign(_T(""))
	, m_sTapParamW(_T(""))
	, m_sTapParamR(_T(""))
{

}

software_tool_tap_calc::~software_tool_tap_calc()
{
}

void software_tool_tap_calc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TAP_READ, m_sCalcVal);
	DDX_Text(pDX, IDC_EDIT_TAP_ORIGN, m_sCalcOrign);
	DDX_Text(pDX, IDC_EDIT_TAP_SAVE, m_sTapParamW);
	DDX_Text(pDX, IDC_EDIT_READ_VALUE, m_sTapParamR);
}


BEGIN_MESSAGE_MAP(software_tool_tap_calc, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_TAP, &software_tool_tap_calc::OnBnClickedBtnStartTap)
	ON_BN_CLICKED(IDC_BTN_READ_TAP, &software_tool_tap_calc::OnBnClickedBtnReadTap)
	ON_BN_CLICKED(ID_BTN_TAP_SAVE_PARAM, &software_tool_tap_calc::OnBnClickedBtnTapSaveParam)
	ON_BN_CLICKED(ID_BTN_READ_TAP_PARAM, &software_tool_tap_calc::OnBnClickedBtnReadTapParam)
	ON_BN_CLICKED(IDC_BTN_READ_ORIGN, &software_tool_tap_calc::OnBnClickedBtnReadOrign)
END_MESSAGE_MAP()


// software_tool_tap_calc 消息处理程序

void software_tool_tap_calc::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	tools_option_instance = g_p->tools_option_instance;
	m_iComPort = g_p->general_option_instance.g_ttycom_port;
}


int software_tool_tap_calc::SendAndWait(CString sCmd, int timeout )
{
	BOOL bRet;
	unsigned short wMax = sCmd.GetLength();

	if(!m_hCom.fConnected)
		return -1;

	memset(m_comBuff, 0, BUFFER_SIZE);
	m_hCom.ClearReceiveBuffer();
	bRet = m_hCom.WriteCommBlock(sCmd.GetBuffer(), sCmd.GetLength());
	sCmd.ReleaseBuffer();
	if(bRet == FALSE)
		return -2;

	Sleep(timeout);

	int readed = m_hCom.ReadCommBlock(m_comBuff, BUFFER_SIZE);
	if(readed  <= 0) {
		return -3;
	}

	GetDlgItem(IDC_EDIT_COMSTR)->SetWindowText(m_comBuff);

	return readed;
}

CString software_tool_tap_calc::GetPromptString(CString sPrompt1, CString sPrompt2, int outMax)
{
	int index, value = -5;
	CString str(m_comBuff), sText;

	sText.Empty();
	if((index = str.Find(sPrompt1)) >= 0) {
		sText = str.Mid(index+sPrompt1.GetLength(), outMax);

	}
	else if((index = str.Find(sPrompt2)) >= 0) {
		sText = str.Mid(index+sPrompt2.GetLength(), outMax);
	}

	return sText;
}

int software_tool_tap_calc::Get3ValueFromString(CString sText, short &value0, short &value1, short &value2)
{
	int ret = -1;
	CString tmp = sText;

	if(sText.GetLength() > 1)
		value0 = _ttoi(sText);
	else return -1;

	int index = sText.Find(' ');
	if(index < 0)
		return -2;

	CString temp = sText.Mid(index+1, sText.GetLength()-index);
	value1 = _ttoi(temp);
	
	int index2 = temp.Find(' ');
	if(index2 < 0)
		return -3;

	CString temp2 = temp.Mid(index+1, temp.GetLength()-index);
	value2 = _ttoi(temp2);
	return 0;
}

int software_tool_tap_calc::GetPromptValue(CString sPrompt1, CString sPrompt2)
{
	int value = -1;
	CString sText = GetPromptString(sPrompt1, sPrompt2);

	if(sText.GetLength() < 1)
		return -3;

	value = _ttoi(sText);
	return value;
}

void software_tool_tap_calc::OnBnClickedBtnStartTap()
{
	int iPort = m_iComPort;

	if(!m_hCom.fConnected) {
		BOOL bRet = m_hCom.InitComm(iPort, DEIGHT, NOPARITY, ONESTOPBIT, 115200);
		if(bRet == FALSE) {
			AfxMessageBox("OpenComm Error");
			return;
		}
	}

	int readed = SendAndWait("check tapopen\r\n");
	if(readed <= 0)
		return;

	CString str(m_comBuff);
	if(str.Find("check tapopen left succ") || str.Find("check tapopen right succ")) {
		return;
	}

	AfxMessageBox("启动失败");
}


void software_tool_tap_calc::OnBnClickedBtnReadTap()
{
	int readed = SendAndWait("check tapcalc\r\n");
	if(readed <= 0)
		return;

	CString sText = GetPromptString("check tapcalc left=", "check tapcalc right=", 12);
	if(sText.GetLength() > 4) {
		short value[3];

		if(Get3ValueFromString(sText, value[0], value[1], value[2]) == 0) {
			m_sCalcVal.Format("%d %d %d", value[0], value[1], value[2]);
			m_sTapParamW = m_sCalcVal;
			UpdateData(FALSE);
		}
		else
			AfxMessageBox("Error Get Calc Value");
	}
}


void software_tool_tap_calc::OnBnClickedBtnReadOrign()
{
	int readed = SendAndWait("check tapload\r\n");
	if(readed <= 0)
		return;

	CString sText = GetPromptString("check tapload left=", "check tapload right=", 32);
	if(sText.GetLength() > 4) {
		short value[3];

		if(Get3ValueFromString(sText, value[0], value[1], value[2]) == 0) {
			m_sCalcOrign.Format("%d %d %d", value[0], value[1], value[2]);
			UpdateData(FALSE);
		}
		else
			AfxMessageBox("Error Get CalcOrign Value");
	}
}


void software_tool_tap_calc::OnBnClickedBtnTapSaveParam()
{
	CString cmd;
	short value[3];

	UpdateData(TRUE);
	if(Get3ValueFromString(m_sTapParamW, value[0], value[1], value[2]) != 0) {
		AfxMessageBox("unknow param");
		return;
	}

	cmd.Format("check tapwrite %d %d %d", value[0], value[1], value[2]);
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return;

	CString str(m_comBuff);
	if(str.Find("check tapload left succ") || str.Find("check tapload right succ")) {
		return;
	}

	AfxMessageBox("WriteParam Fail");
}


void software_tool_tap_calc::OnBnClickedBtnReadTapParam()
{
	int readed = SendAndWait("check tapread\r\n");
	if(readed <= 0)
		return;

	CString sText = GetPromptString("check tapread left=", "check tapread right=", 12);
	if(sText.GetLength() > 4) {
		short value[3];

		if(Get3ValueFromString(sText, value[0], value[1], value[2]) == 0) {
			m_sTapParamR.Format("%d %d %d", value[0], value[1], value[2]);
		}
		else {
			m_sTapParamR.Empty();
			AfxMessageBox("Error Get CalcOrign Value");
		}
		UpdateData(FALSE);
	}
}


