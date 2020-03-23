// software_tool_psensor_calc.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_tool_psensor_calc.h"
#include "afxdialogex.h"


// software_tool_psensor_calc 对话框

IMPLEMENT_DYNAMIC(software_tool_psensor_calc, CDialogEx)

software_tool_psensor_calc::software_tool_psensor_calc(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_tool_psensor_calc::IDD, pParent)
	, m_iValue(0)
	, m_iValue0(0)
	, m_iValue1(0)
	, m_iValue5CM(0)
	, m_iValue3CM(0)
{
	m_iComPort = 1;
}

software_tool_psensor_calc::~software_tool_psensor_calc()
{
	
	m_hCom.EndComm();
}

void software_tool_psensor_calc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_BASE, m_iValue);
	DDX_Text(pDX, IDC_EDIT_BASE1, m_iValue0);
	DDX_Text(pDX, IDC_EDIT_BASE2, m_iValue1);
	DDX_Text(pDX, IDC_EDIT_VALUE1, m_iValue5CM);
	DDX_Text(pDX, IDC_EDIT_VALUE2, m_iValue3CM);
}


BEGIN_MESSAGE_MAP(software_tool_psensor_calc, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_START_1, &software_tool_psensor_calc::OnBnClickedBtnStart1)
	ON_BN_CLICKED(IDC_BTN_READ_1, &software_tool_psensor_calc::OnBnClickedBtnRead1)
	ON_BN_CLICKED(IDC_BTN_START_2, &software_tool_psensor_calc::OnBnClickedBtnStart2)
	ON_BN_CLICKED(IDC_BTN_READ_2, &software_tool_psensor_calc::OnBnClickedBtnRead2)
	ON_BN_CLICKED(IDC_BTN_START_3, &software_tool_psensor_calc::OnBnClickedBtnStart3)
	ON_BN_CLICKED(IDC_BTN_READ_3, &software_tool_psensor_calc::OnBnClickedBtnRead3)
	ON_BN_CLICKED(ID_BTN_SAVE, &software_tool_psensor_calc::OnBnClickedBtnSave)
	ON_BN_CLICKED(ID_BTN_READ, &software_tool_psensor_calc::OnBnClickedBtnRead)
	ON_BN_CLICKED(ID_BTN_SENSOR_CHECK, &software_tool_psensor_calc::OnBnClickedBtnSensorCheck)
	ON_BN_CLICKED(ID_BTN_EAR_RESTART, &software_tool_psensor_calc::OnBnClickedBtnEarRestart)
END_MESSAGE_MAP()


// software_tool_psensor_calc 消息处理程序
void software_tool_psensor_calc::get_param(struct glob_param *g_p)
{
	ini_path = g_p->sFName;
	tools_option_instance = g_p->tools_option_instance;
	m_iComPort = g_p->general_option_instance.g_ttycom_port;
}


int software_tool_psensor_calc::SendAndWait(CString sCmd, int timeout )
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

// 返回值最长为 value+value+value 18 字节
CString software_tool_psensor_calc::GetPromptString(CString sPrompt1, CString sPrompt2, int outMax)
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

int software_tool_psensor_calc::GetPromptValue(CString sPrompt1, CString sPrompt2)
{
	int value = -1;
	CString sText = GetPromptString(sPrompt1, sPrompt2);

	if(sText.GetLength() < 1)
		return -3;

	value = _ttoi(sText);
	return value;
}


BOOL software_tool_psensor_calc::StartOpenSensor(int step, int value)
{
	CString cmd;

	cmd.Format("check open_sensor %d %d\r\n", step, value);
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return FALSE;

	CString str(m_comBuff);
	if(str.Find("check open_sensor left succ") || str.Find("check open_sensor right succ")) {
		return TRUE;
	}

	return FALSE;
}

void software_tool_psensor_calc::OnBnClickedBtnStart1()
{
	int iPort = m_iComPort;

	if(!m_hCom.fConnected) {
		BOOL bRet = m_hCom.InitComm(iPort, DEIGHT, NOPARITY, ONESTOPBIT, 115200);
		if(bRet == FALSE) {
			AfxMessageBox("OpenComm Error");
			return;
		}
	}

	if(StartOpenSensor(1, 0) == TRUE) {
		return;
	}

	AfxMessageBox("OpenSensorCmd Fail");
}


void software_tool_psensor_calc::OnBnClickedBtnRead1()
{
	int readed = SendAndWait("check sensor_talk\r\n");
	if(readed <= 0)
		return;

	int value = GetPromptValue("check sensor_talk left=", "check sensor_talk right=");

	m_iValue = m_iValue0 = m_iValue1 = value;
	UpdateData(FALSE);
}


void software_tool_psensor_calc::OnBnClickedBtnStart2()
{
	CString cmd;

	UpdateData(TRUE);

	if(0 == m_iValue0) {
		AfxMessageBox("请先读取本底值");
		return;
	}

	if(StartOpenSensor(2, m_iValue0) == TRUE) {
		m_iValue5CM = 0;
		UpdateData(FALSE);
		return;
	}
	AfxMessageBox("OpenSensor5CM Fail");
}


void software_tool_psensor_calc::OnBnClickedBtnRead2()
{
	CString cmd;

	UpdateData(TRUE);

	if(0 == m_iValue0) {
		AfxMessageBox("请先读取本底值");
		return;
	}

	cmd.Format("check sensor_val\r\n");
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return;

	int value = GetPromptValue("check sensor_val left=", "check sensor_val right=");
	if(value >= 0) {
		m_iValue5CM = value;
		UpdateData(FALSE);
		return;
	}
	AfxMessageBox("ReadSensor5CM Fail");
}


void software_tool_psensor_calc::OnBnClickedBtnStart3()
{
	CString cmd;

	UpdateData(TRUE);

	if(0 == m_iValue0) {
		AfxMessageBox("请先读取本底值");
		return;
	}

	if(StartOpenSensor(3, m_iValue0) == TRUE) {
		m_iValue3CM = 0;
		UpdateData(FALSE);
		return;
	}
	AfxMessageBox("OpenSensor3CM Fail");
}


void software_tool_psensor_calc::OnBnClickedBtnRead3()
{
	CString cmd;

	UpdateData(TRUE);

	if(0 == m_iValue0) {
		AfxMessageBox("请先读取本底值");
		return;
	}

	cmd.Format("check sensor_val\r\n");
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return;

	int value = GetPromptValue("check sensor_val left=", "check sensor_val right=");
	if(value >= 0) {
		m_iValue3CM = value;
		UpdateData(FALSE);
		return;
	}
	AfxMessageBox("ReadSensor3CM Fail");
}


void software_tool_psensor_calc::OnBnClickedBtnSave()
{
	CString cmd;

	UpdateData(TRUE);

	if(0 == m_iValue0 || 0 == m_iValue5CM || 0 == m_iValue3CM) {
		AfxMessageBox("请先读取本底等值");
		return;
	}

	cmd.Format("check wr_sensor=%d %d %d\r\n", m_iValue5CM, m_iValue3CM, m_iValue0);
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return;
	
	CString str(m_comBuff);
	if((str.Find("check wr_sensor left succ") >= 0) || str.Find("check wr_sensor right succ") >= 0) {
		AfxMessageBox("SavaeSensor Succ");
		return;
	}
	AfxMessageBox("SavaeSensor Fail");
}

// Left/right=xxx xxx xxx
void software_tool_psensor_calc::OnBnClickedBtnRead()
{
	CString cmd;
	int value[3];

	UpdateData(TRUE);

	cmd.Format("check rd_sensor\r\n");
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return;

	CString sText = GetPromptString("check rd_sensor left=", "check rd_sensor right=", 18);
	if(sText.GetLength() <= 0) {
		AfxMessageBox("ReadSensorCalc Fail");
		return;
	}

	value[0] = _ttoi(sText);
	int index = sText.Find(' ');
	CString temp = sText.Mid(index+1, sText.GetLength()-index);
	value[1] = _ttoi(temp);

	int index1 = temp.Find(' ');
	CString temp1 = temp.Mid(index1+1, temp.GetLength()-index1);
	value[2] = _ttoi(temp1);

	CString msg;
	msg.Format("ValueL=%d ValueH=%d Talk=%d", value[0], value[1], value[2]);
	AfxMessageBox(msg);
}

void software_tool_psensor_calc::OnBnClickedBtnEarRestart()
{
	int readed = SendAndWait("check volreset\r\n");
	if(readed <= 0)
		return;
}

void software_tool_psensor_calc::OnBnClickedBtnSensorCheck()
{
	CString cmd;

	UpdateData(TRUE);

	cmd.Format("check stat_sensor\r\n");
	int readed = SendAndWait(cmd);
	if(readed <= 0)
		return;

	CString str(m_comBuff);
	if(str.Find("check stat_sensor left in") >= 0) {
		AfxMessageBox("Left IN");
	}
	else if(str.Find("check stat_sensor left out") >= 0) {
		AfxMessageBox("Left OUT");
	}
	
	if(str.Find("check stat_sensor right in") >= 0) {
		AfxMessageBox("Right IN");
	}
	else if(str.Find("check stat_sensor right out") >= 0) {
		AfxMessageBox("Right OUT");
	}
//	AfxMessageBox("CheckSensor Fail");
}


