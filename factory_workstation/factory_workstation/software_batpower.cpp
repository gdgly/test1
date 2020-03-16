// software_batpower.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_batpower.h"
#include "afxdialogex.h"


// software_batpower 对话框

IMPLEMENT_DYNAMIC(software_batpower, CDialogEx)

software_batpower::software_batpower(CWnd* pParent /*=NULL*/)
	: CDialogEx(software_batpower::IDD, pParent)
{

}

software_batpower::~software_batpower()
{
}

void software_batpower::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, list);
	DDX_Control(pDX, IDC_LIST3, m_listEvt);
	DDX_Control(pDX, IDC_SET_SN, m_Esetsn);
	DDX_Control(pDX, IDC_STATIC_CURRENT_NUM, m_current_num);
	DDX_Control(pDX, IDC_STATIC_CURRENT_SN, m_current_sn);
}

BEGIN_MESSAGE_MAP(software_batpower, CDialogEx)
	ON_MESSAGE(WM_USERMSG_MASSPRODUCT, &software_batpower::OnMSGMassProduct)
	ON_BN_CLICKED(IDC_BUTTON3, &software_batpower::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_SET_SN, &software_batpower::OnEnChangeSetSn)
	ON_BN_CLICKED(IDC_BUTTON16, &software_batpower::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON2, &software_batpower::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON15, &software_batpower::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON14, &software_batpower::OnBnClickedButton14)
END_MESSAGE_MAP()

void software_batpower::AddEvent2List(CString sText)
{
	CTime tNow = CTime::GetCurrentTime();
	CString sTemp;
	int count = m_listEvt.GetCount();
	sTemp.Format("%02d:%02d:%02d %s", tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond(), sText);
	m_listEvt.InsertString(count, sTemp);
	m_listEvt.SetCurSel( m_listEvt.GetCount() - 1);
	//m_listEvt.SetHorizontalExtent(174); 
	if(m_hLogFile.m_hFile != INVALID_HANDLE_VALUE) {
		m_hLogFile.Write(sTemp, sTemp.GetLength());
		m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	}
}

INT software_batpower::MesPass(int idata)   //每一块成品测完以后，点击PASS上传MES
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1,sData;
	UINT a;
	sData.Format("%d",idata);
	if(mesIptr->Messtatus == 1){
		if(mesIptr->power_SendDatae(m_chkSN) == 0){
			sText.Format(_T("mes返回数据:%s"),mesIptr->line);
			sText1.Format(_T("mes数据判断结果:%d"),mesIptr->Messtatus);
			a = sText.Find("}");
			if(a >= 0)
				sText = sText.Left(a+1);

			AddEvent2List(sText);
			AddEvent2List(sText1);
		}else
			AddEvent2List("mes发送失败！");

		if(mesIptr->Messtatus == 3){
			return 1;
		}else{
			AfxMessageBox("mes返回数据错误，不可以继续check-2！ ");
			mesIptr->log = 0;
			mesIptr->Messtatus = 0;
		}
	}
	return 0;
}

INT software_batpower::MesSend(int idata,BYTE *ifirver,BYTE *isouver)   //站位上传以后，上传电池数据
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1,sData,sfirver,Ssouver;
	UINT a;
	sData.Format("%d",idata);
	sfirver.Format("%s",ifirver);
	Ssouver.Format("%s",isouver);
	if(mesIptr->Messtatus == 3){
		if(mesIptr->power_SendChecke(m_chkSN,sData,sfirver,Ssouver) == 0){
			sText.Format(_T("mes返回数据:%s"),mesIptr->line);
			sText1.Format(_T("mes数据判断结果:%d"),mesIptr->Messtatus);
			a = sText.Find("}");
			if(a >= 0)
				sText = sText.Left(a+1);

			AddEvent2List(sText);
			AddEvent2List(sText1);
		}else
			AddEvent2List("mes发送失败！");
	}

	AddEvent2List("\r\n");
	if(mesIptr->Messtatus == 7){
		mesIptr->log = 0;
		mesIptr->Messtatus = 0;
		return 1;
	}else{
		AfxMessageBox("mes返回数据错误，不可以继续check-3！ ");
		mesIptr->log = 0;
		mesIptr->Messtatus = 0;
	}
	return 0;
}

int software_batpower::twice_current_check()
{
	CString sText;
	SqlInfo sql_info;
	CString now_time,last_time;
	CTime sql_time;
	CTimeSpan span;

	sql_time = CTime::GetCurrentTime();
	now_time=sql_time.Format(_T("%Y-%m-%d %H:%M:%S"));
	last_time.Format("%d",batpower_option_instance.g_twotc_lasttime);
	if(m_sqlite.ReadSqlite_with_time(m_chkSN, now_time, last_time, &sql_info) != 0){
		if(sql_info.Status == "currentfail" || sql_info.Status == "hign_power_fail"
			|| sql_info.Status == "twice_time_pass" || sql_info.Status == "twice_time_fail"){
			if(sql_info.Status == "currentfail" && sql_info.Current == "0uA"){
				;
			}else{
				StrToIntEx(sql_info.SOCF, STIF_SUPPORT_HEX, (int*)&last_power);
				int nYear, nMonth, nDate, nHour, nMin, nSec;
				sscanf_s(sql_info.Time, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);   
				CTime old_time(nYear, nMonth, nDate, nHour, nMin, nSec);
			
				span = sql_time - old_time;
				int find_hours = (int)span.GetHours();
				int find_day = (int)span.GetDays();
				need_power = last_power - (find_day*24+find_hours)*batpower_option_instance.g_twotc_consume_power/100;
				sText.Format("find sn %s time : %s find_hours = %d day = %d",m_chkSN,sql_info.Time,find_hours,find_day);
				AddEvent2List(sText);
				return 1;
			}
		}
	}else if(m_sqlite.ReadSqlite_with_time(m_chkSN, now_time, "0", &sql_info) != 0){//1小时与last_time之间有一条记录
		if(sql_info.Status == "currentfail" || sql_info.Status == "hign_power_fail"
			|| sql_info.Status == "twice_time_fail" || sql_info.Status == "twice_time_pass"){
			if(sql_info.Status == "currentfail" && sql_info.Current == "0uA"){
				;
			}else{
				int nYear, nMonth, nDate, nHour, nMin, nSec;
				sscanf_s(sql_info.Time, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDate, &nHour, &nMin, &nSec);   
				CTime old_time(nYear, nMonth, nDate, nHour, nMin, nSec);
				span = sql_time - old_time;
				int find_hours = (int)span.GetHours();
				int find_day = (int)span.GetDays();
				sText.Format("找到sn:%s 上次检测时间 %s\r\n\r\n还需等待%d小时进行第二次检测", m_chkSN, sql_info.Time, 
							batpower_option_instance.g_twotc_lasttime - (find_day*24+find_hours));
				AfxMessageBox(sText);
				//AddEvent2List(sText);
				return 2;//二次检测时间未到
			}
		}
	}

	return 0;
}

void software_batpower::bat_insert_sql(int iTimes, CString status, CTime tNow)
{
	CString sTime,sResult,sReboot,sVoltage,sCurrent,sFirver;
	SqlInfo info;
	FLOAT fVoltage;
	fVoltage = (FLOAT)(voltage /1000.0);

	sResult.Format("%d%%",iBatVal);
	sReboot.Format("%d",rebootnum);
	sVoltage.Format("%.3fV",fVoltage);
	sCurrent.Format("%duA",current);
	sFirver.Format("%s",firversion);

	sTime = tNow.Format(_T("%Y-%m-%d %H:%M:%S"));
	info.Time = sTime;
	info.SN = m_chkSN;
	info.Result = iTimes;
	info.Current = sCurrent;
	info.RebootEnt = sReboot;
	info.SOCF.Format("%d",iBatVal);
	info.Voltage = sVoltage;
	info.Version = sFirver;
	info.Status = status;
	m_sqlite.InsertSqlite(&info);
}

CString software_batpower::old_version_check()
{
	CString sItem,sn, sTime, sResult, sTemp,sFirver,sSouver,sCurrent,sPass,sFail,
		sReboot,sVoltage,sRebootFail,sCurrentFail,twice_time_sPass,twice_time_Fail,hign_power_fail;
	int iTimes = 0;
	sn = m_chkSN;

	if(sn.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		sn = sn + " ";//18位sn号补一位空格
	}
	if(m_chkString.GetLength() >= (STR_SN_LEN)) {
		sTemp = m_chkString.Mid(STR_SN_LEN+STR_SPLIT_LEN+STR_TIME_LEN+STR_SPLIT_LEN, STR_CKCNT_LEN);
		if(StrToIntEx(sTemp, STIF_SUPPORT_HEX, (int*)&iTimes) != TRUE)
				iTimes = 0;
	}
	iTimes += 1;

	CTime tNow = CTime::GetCurrentTime();
	sTime.Format("%04d%02d%02d-%02d:%02d", tNow.GetYear(), tNow.GetMonth(),tNow.GetDay(), tNow.GetHour(), tNow.GetMinute());
	sResult.Format("%d%%",iBatVal);
	sFirver.Format("%s",firversion);
	sSouver.Format("%s",souversion);
	sPass.Format("%s","pass");
	sFail.Format("%s","fail");
	sRebootFail.Format("%s","rebootfail");
	sCurrentFail.Format("%s","currentfail");
	twice_time_sPass.Format("%s","twice_time_pass");
	twice_time_Fail.Format("%s","twice_time_fail");
	hign_power_fail.Format("%s","hign_power_fail");

	if(iBatVal >= 0 && iBatVal <= 100){
		if(iBatVal >= batpower_option_instance.g_power_percent)
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s", sn, sTime, iTimes, sResult, sFirver, sSouver,sPass);
		else
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s", sn, sTime, iTimes, sResult, sFirver, sSouver,sFail);
	}else{
		sItem.Format("%s    %s    %04d    ERROR",sn, sTime, iTimes);
	}

	m_hLogFile.Write(sItem, sItem.GetLength());
	m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();

	return sItem;
}
CString software_batpower::first_time_check_without_current()
{
	CString sItem,sn, sTime, sResult, sTemp,sFirver,sCurrent,sPass,sFail,
		sReboot,sVoltage,sRebootFail,sCurrentFail,twice_time_sPass,twice_time_Fail,hign_power_fail;
	FLOAT fVoltage;
	int iTimes = 0;
	fVoltage = (FLOAT)(voltage /1000.0);
	sn = m_chkSN;

	if(sn.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		sn = sn + " ";//18位sn号补一位空格
	}
	if(m_chkString.GetLength() >= (STR_SN_LEN)) {
		sTemp = m_chkString.Mid(STR_SN_LEN+STR_SPLIT_LEN+STR_TIME_LEN+STR_SPLIT_LEN, STR_CKCNT_LEN);
		if(StrToIntEx(sTemp, STIF_SUPPORT_HEX, (int*)&iTimes) != TRUE)
				iTimes = 0;
	}
	iTimes += 1;

	CTime tNow = CTime::GetCurrentTime();
	sTime.Format("%04d%02d%02d-%02d:%02d", tNow.GetYear(), tNow.GetMonth(),tNow.GetDay(), tNow.GetHour(), tNow.GetMinute());
	sResult.Format("%d%%",iBatVal);
	sReboot.Format("%d",rebootnum);
	sVoltage.Format("%.3fV",fVoltage);
	sCurrent.Format("%duA",current);
	sFirver.Format("%s",firversion);
	sPass.Format("%s","pass");
	sFail.Format("%s","fail");
	sRebootFail.Format("%s","rebootfail");
	sCurrentFail.Format("%s","currentfail");
	twice_time_sPass.Format("%s","twice_time_pass");
	twice_time_Fail.Format("%s","twice_time_fail");
	hign_power_fail.Format("%s","hign_power_fail");

	if(iBatVal >= 0 && iBatVal <= 100){
		if(iBatVal >= batpower_option_instance.g_power_percent &&
			rebootnum <= batpower_option_instance.g_reboot_num){
			sItem.Format("%s    %s    %04d    %s   %s   %s   %s    %s   %s", 
				sn, sTime, iTimes, sResult,sReboot,sVoltage,sCurrent,sFirver,sPass);		
		}else if(iBatVal < batpower_option_instance.g_power_percent){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sFail);		
		}else if(rebootnum > batpower_option_instance.g_reboot_num){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sRebootFail);		
		}
	}else{
		sItem.Format("%s    %s    %04d    ERROR",sn, sTime, iTimes);
	}

	m_hLogFile.Write(sItem, sItem.GetLength());
	m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();

	return sItem;
}

CString software_batpower::first_time_check()
{
	CString sItem,sn, sTime, sResult, sTemp,sFirver,sCurrent,sPass,sFail,
		sReboot,sVoltage,sRebootFail,sCurrentFail,twice_time_sPass,twice_time_Fail,hign_power_fail;
	FLOAT fVoltage;
	int iTimes = 0;
	fVoltage = (FLOAT)(voltage /1000.0);
	sn = m_chkSN;

	if(sn.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		sn = sn + " ";//18位sn号补一位空格
	}
	if(m_chkString.GetLength() >= (STR_SN_LEN)) {
		sTemp = m_chkString.Mid(STR_SN_LEN+STR_SPLIT_LEN+STR_TIME_LEN+STR_SPLIT_LEN, STR_CKCNT_LEN);
		if(StrToIntEx(sTemp, STIF_SUPPORT_HEX, (int*)&iTimes) != TRUE)
				iTimes = 0;
	}
	iTimes += 1;

	CTime tNow = CTime::GetCurrentTime();
	sTime.Format("%04d%02d%02d-%02d:%02d", tNow.GetYear(), tNow.GetMonth(),tNow.GetDay(), tNow.GetHour(), tNow.GetMinute());
	sResult.Format("%d%%",iBatVal);
	sReboot.Format("%d",rebootnum);
	sVoltage.Format("%.3fV",fVoltage);
	sCurrent.Format("%duA",current);
	sFirver.Format("%s",firversion);
	sPass.Format("%s","pass");
	sFail.Format("%s","fail");
	sRebootFail.Format("%s","rebootfail");
	sCurrentFail.Format("%s","currentfail");
	twice_time_sPass.Format("%s","twice_time_pass");
	twice_time_Fail.Format("%s","twice_time_fail");
	hign_power_fail.Format("%s","hign_power_fail");

	if(iBatVal >= 0 && iBatVal <= 100){
		if(iBatVal >= batpower_option_instance.g_power_percent	&& 
			rebootnum <= batpower_option_instance.g_reboot_num	&& 
			current <= batpower_option_instance.g_current_value && 
			current != 0){
			sItem.Format("%s    %s    %04d    %s   %s   %s   %s    %s   %s", 
				sn, sTime, iTimes, sResult,sReboot,sVoltage,sCurrent,sFirver,sPass);
		}else if(iBatVal < batpower_option_instance.g_power_percent){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sFail);
		}else if(rebootnum > batpower_option_instance.g_reboot_num){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sRebootFail);
		}else if(current > batpower_option_instance.g_current_value || current == 0){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sCurrentFail);
			bat_insert_sql(iTimes, sCurrentFail, tNow);
		}
	}else{
		sItem.Format("%s    %s    %04d    ERROR",sn, sTime, iTimes);
	}

	m_hLogFile.Write(sItem, sItem.GetLength());
	m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	return sItem;
}
CString software_batpower::second_time_check()
{
	CString sItem,sn, sTime, sResult, sTemp,sFirver,sCurrent,sPass,sFail,status,
		sReboot,sVoltage,sRebootFail,sCurrentFail,twice_time_sPass,twice_time_Fail,hign_power_fail;
	FLOAT fVoltage;
	int iTimes = 0;
	fVoltage = (FLOAT)(voltage /1000.0);
	sn = m_chkSN;

	if(sn.GetLength() == 18){//v2 sn eg:A01A0A180801000001
		sn = sn + " ";//18位sn号补一位空格
	}
	if(m_chkString.GetLength() >= (STR_SN_LEN)) {
		sTemp = m_chkString.Mid(STR_SN_LEN+STR_SPLIT_LEN+STR_TIME_LEN+STR_SPLIT_LEN, STR_CKCNT_LEN);
		if(StrToIntEx(sTemp, STIF_SUPPORT_HEX, (int*)&iTimes) != TRUE)
				iTimes = 0;
	}
	iTimes += 1;

	CTime tNow = CTime::GetCurrentTime();
	sTime.Format("%04d%02d%02d-%02d:%02d", tNow.GetYear(), tNow.GetMonth(),tNow.GetDay(), tNow.GetHour(), tNow.GetMinute());
	sResult.Format("%d%%",iBatVal);
	sReboot.Format("%d",rebootnum);
	sVoltage.Format("%.3fV",fVoltage);
	sCurrent.Format("%duA",current);
	sFirver.Format("%s",firversion);
	sPass.Format("%s","pass");
	sFail.Format("%s","fail");
	sRebootFail.Format("%s","rebootfail");
	sCurrentFail.Format("%s","currentfail");
	twice_time_sPass.Format("%s","twice_time_pass");
	twice_time_Fail.Format("%s","twice_time_fail");
	hign_power_fail.Format("%s","hign_power_fail");

	if(iBatVal >= 0 && iBatVal <= 100){
		if(iBatVal > (UINT)last_power){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,hign_power_fail);
			status = hign_power_fail;
		}else if(iBatVal >= batpower_option_instance.g_power_percent && 
					rebootnum <= batpower_option_instance.g_reboot_num &&
					iBatVal >= (UINT)need_power && 
					current != 0 ){
			sItem.Format("%s    %s    %04d    %s   %s   %s   %s    %s   %s", 
				sn, sTime, iTimes, sResult,sReboot,sVoltage,sCurrent,sFirver,twice_time_sPass);
			status = twice_time_sPass;
		}else if(current == 0){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sCurrentFail);
			status = sCurrentFail;
		}else if(iBatVal < (UINT)need_power){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s",
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,twice_time_Fail);
			status = twice_time_Fail;
		}else if(iBatVal < batpower_option_instance.g_power_percent){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sFail);
			status = sFail;
		}else if(rebootnum > batpower_option_instance.g_reboot_num){
			sItem.Format("%s    %s    %04d    %s   %s   %s    %s   %s   %s", 
				sn, sTime, iTimes, sResult, sReboot,sVoltage,sCurrent,sFirver,sRebootFail);
			status = sRebootFail;
		}
		bat_insert_sql(iTimes, status, tNow);
	}else{
		sItem.Format("%s    %s    %04d    ERROR",sn, sTime, iTimes);
		SqlInfo info;
		sTime = tNow.Format(_T("%Y-%m-%d %H:%M:%S"));
		info.Time = sTime;
		info.SN = m_chkSN;
		info.Result = iTimes;
		info.Current = "";
		info.RebootEnt = "";
		info.SOCF = "0";
		info.Voltage = "";
		info.Version = "";
		info.Status = "error";
		m_sqlite.InsertSqlite(&info);
	}

	m_hLogFile.Write(sItem, sItem.GetLength());
	m_hLogFile.Write("\r\n", 2); m_hLogFile.Flush();
	return sItem;
}
void software_batpower::power_process()
{
	CString sItem;
	int ret;
	if(old0_3_08_version == 0){//0.3.08以前固件，或者测试板返回错误
		//AfxMessageBox("机器固件为0.3.08前版本，或测试板返回错误，请升级！ ");
		sItem = old_version_check();
	}else{
		if(batpower_option_instance.g_check_current){
			ret = twice_current_check();
			if(ret == 1){//二次检测
				sItem = second_time_check();
			}else if(ret == 2){//二次检测时间未到 去接下来的操作
				return;
			}else{//首次检测
				sItem = first_time_check();
			}
		}else{
			sItem = first_time_check_without_current();
		}
	}
	if(sItem.Find("pass") >= 0 && general_option_instance.g_check_mes) {
		if(general_option_instance.g_check_mes_type == 1){
			if(MesPass(iBatVal) == TRUE){
				if(MesSend(iBatVal,firversion,souversion) != TRUE){
					return;
				}
			}else{
				return;
			}
		}else if(general_option_instance.g_check_mes_type == 2){
			struct ch_mes_data ch_mes_data; 
			ch_mes_data.data_flag = 4;
			ch_mes_data.iBatVal = iBatVal;
			memcpy(ch_mes_data.firversion, firversion, sizeof(firversion));
			memcpy(ch_mes_data.souversion, souversion, sizeof(souversion));
			mesInfo.changh_mes_int(m_chkSN, ch_mes_data);
		}
	}
	list.DeleteString(m_chkNo);
	list.InsertString(m_chkNo, sItem);
}

LRESULT software_batpower::OnMSGMassProduct(WPARAM wParam, LPARAM lParam)
{
	UINT a;
	CString sText;
	switch((INT)wParam) {
		case WPARAM_START:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("正在采集中。。。");
			AddEvent2List("开始采集！");
			break;
		case WPARAM_UART_ERR:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("复位采集");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			AfxMessageBox("串口打开失败");
			AddEvent2List("串口打开失败");
			break;
		case WPARAM_START_TIMEOUT:
			AfxMessageBox("接收开始反馈超时，检查串口 测试板是否接好"); 
			AddEvent2List("接收开始反馈超时,检查串口 测试板是否接好");
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("复位采集");
			process_end = TRUE;
			endisable_input(process_end);
			break;
		//case WPARAM_STOP_TIMEOUT:
		//	((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("复位采集");
		//	((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
		//	AfxMessageBox("接收停止反馈超时"); 
		//	AddEvent2List("接收停止反馈超时");
		//	break;
		case WPARAM_THREAD_EXITED:
			power_process();
			if(m_bChkAllItem){
				while(1){
					if( m_chkNo == list.GetCount()-1 ){//一次循环结束
						loop_times++;
					}
					if(loop_times == 4){//循环四次
						((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("复位采集");
						process_end = TRUE;
						endisable_input(process_end);
						break;//此处真正跳出list循环
					}
					m_chkNo += 1;
					if(m_chkNo > list.GetCount()-1){
						m_chkNo = 0;
					}
					list.GetText(m_chkNo, sText);
					if(sText.Find("pass") < 0  && sText.Find("fail") < 0){//如果没跑过或者error的就开始进程
						Sleep(10);//确保线程已经退出
						open_process_thread(m_chkNo);
						break;
					}
				}
			}else{
				((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("复位采集");
				process_end = TRUE;
				endisable_input(process_end);
			}
			break;
		case WPARAM_MES_STRING:
			sText.Format(_T("mes返回数据:%s"),Mesresring);
			a = sText.Find("}");
			if(sText.Find("}") >= 0)
				sText = sText.Left(a + 1);
			AddEvent2List(sText);
			break;
		case WPARAM_MES_ERR:
			AddEvent2List("mes发送失败！");
			break;
		case WPARAM_SN:
			sText.Format(_T("SN号:%s mes数据判断结果:%d"), m_chkSN, mesInfo.Messtatus);
			AddEvent2List(sText);
			break;
		default:
			break;
	}
	return 0;
}

int software_batpower::WorkerThreadProc(void *param)
{
	CString sText;
	unsigned int timeout = 0;
	int getboard_end = 0;
	INT rdsize = 0, pktdatlen = 0, readed = 0;
	int olen = 0, result = 0;
	int m_start_resp = 0;
	int m_stop_resp = 0;
	NetProto send_pkt,cmdPkt;
	unsigned char cmd_dat[26];
	CTime tNow,tSend;
	CTimeSpan timeSpan;
	software_batpower *p = (software_batpower*)param;
	CContrlComm_bluetooth *comInfo = &(p->m_ctlCom);
	CCurlCtrl *mesptr = &p->mesInfo;
	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//串口buf

	//if(p->com_open_flag == FALSE){
	//	AfxMessageBox("串口打开失败，不可以继续测试！ ");
	//	goto out;
	//}
	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	comInfo->baudCom = p->general_option_instance.g_ttycom_baud;

	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, comInfo->baudCom)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
	}else{
		sText.Format("打开端口%d成功",comInfo->portCom);
		p->AddEvent2List(sText);
	}

	memset(cmd_dat, 0, sizeof(cmd_dat));
	cmd_dat[0] = 0x06;
	if(p->m_chkSN.GetLength() == 18){
		memcpy(&cmd_dat[1], p->m_chkSN, 18);
	}else{
		memcpy(&cmd_dat[1], p->m_chkSN, 19);
	}
	if(p->m_chkSN.GetLength() == 18)
		pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 19);//少传一位sn号
	else
		pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 20);

	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START, 0);
	while(1){
		if(0 == m_start_resp) {
			if(timeout >= p->batpower_option_instance.g_com_timeout_times){
				comInfo->EndComm();
				SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_START_TIMEOUT, 0);
				goto out;
			}
			tNow = CTime::GetCurrentTime();
			timeSpan = tNow - tSend;
			if( timeSpan.GetTotalSeconds() >= 1){
				comInfo->WriteCommBlock((LPSTR)&send_pkt, send_pkt.cmdlen);
				tSend = tNow;
				timeout ++;
			}
		}

		rdsize = comInfo->ReadCommBlock((LPSTR)&pRdbuf[readed], COM_MAX_RDSZIZE-readed);
		if(rdsize > 0) {
			readed += rdsize;
		}

ReParse:
		if(readed > 8) {
			olen = 0;
			memset(&cmdPkt, 0, sizeof(cmdPkt));
			result = pto_Parse(pRdbuf, readed, &cmdPkt, &olen);
			if(olen > 0) {
				if(readed > olen) {
					readed -= olen;
					memcpy(&pRdbuf[0], &pRdbuf[olen], readed);
				}
				else readed = 0;
			}

			if(result == NPTO_PARSE_RESULT_SUCC) {
				switch(cmdPkt.command) {
				case CMD_RESP_SET_AUDIO:
					if(cmdPkt.u_dat[2] == 0x01) {
						m_start_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x00) {
						m_stop_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x02) {
						m_start_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x03) {
						m_start_resp = 1;
						timeout = 0;
					}else if(cmdPkt.u_dat[2] == 0x06) {
						m_start_resp = 1;
						timeout = 0;
					}
					break;

				case CMD_GET_BOARDINFO:
					p->old0_3_08_version = cmdPkt.u_dat16[0];
					if(0 == cmdPkt.u_dat16[0] || 1 == cmdPkt.u_dat16[0]){ //1表示0.03.08以后的版本
						if(0 == cmdPkt.u_dat16[0]) {      // 成功
							p->iBatVal = cmdPkt.u_dat16[1];
							memcpy(p->firversion,&cmdPkt.u_dat[4],7);
							p->firversion[7] = '\0';
							memcpy(p->souversion,&cmdPkt.u_dat[11],7);
							p->souversion[7] = '\0';	
						}else if(1 == cmdPkt.u_dat16[0]){//0.03.08以后的版本
							p->iBatVal = cmdPkt.u_dat[2];    //电量
							p->rebootnum = cmdPkt.u_dat[3];  //重启次数
							memcpy(&p->voltage, &cmdPkt.u_dat[4], 2);  //电压
							memcpy(&p->current, &cmdPkt.u_dat[6], 2);  //电流
							memcpy(p->firversion, &cmdPkt.u_dat[8], 7);   //固件版本号
							p->firversion[7] = '\0';
							memcpy(p->souversion, &cmdPkt.u_dat[15], 7);   //资源版本号
							p->souversion[7] = '\0';
						}
						if(p->general_option_instance.g_check_mes){
							if(p->general_option_instance.g_check_mes_type == 1){
								if(mesptr->power_SendChecks((LPCSTR)p->m_chkSN) == 0){            //数据上报接口    烧录第二次MES上传
									p->Mesvalue = mesptr->Messtatus;
									memcpy(p->Mesresring,mesptr->line,strlen(mesptr->line));
									SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_STRING, 0); 
									SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_SN, 0);
								}else
									SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_ERR, 0);
								if(p->Mesvalue != 1){
									AfxMessageBox("mes返回数据错误，不可以继续测试-1！ ");
									mesptr->log = 0;
									mesptr->Messtatus = 0;
								}
							}
						}
					}else{
						p->iBatVal = 1000;
					}
					getboard_end = 1;
					break;
				}//switch
				goto ReParse;
			}
		}//if readed
		if(getboard_end == 1) {
			cmd_dat[0] = 0x00; cmd_dat[1] = 0;
			pto_GenerateRaw(&cmdPkt, CMD_SET_AUDIO_START, 1235, cmd_dat, 2);
			comInfo->WriteCommBlock((LPSTR)&cmdPkt, cmdPkt.cmdlen);
			break;
		}
		Sleep(1);
	}//while
	comInfo->EndComm();
	Sleep(20);
	SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	
out:
	//SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_THREAD_EXITED, 0);
	delete pRdbuf;
	return 0;
}

void software_batpower::get_param(struct glob_param *g_p)
{
	last_sel_soft = g_p->last_sel_soft;
	general_option_instance = g_p->general_option_instance;
	batpower_option_instance = g_p->batpower_option_instance;
}

// software_batpower 消息处理程序

void software_batpower::open_process_thread(INT list_num)
{
	CString sText;
	m_chkNo = list_num;
	list.GetText(list_num, m_chkString);
	list.SetCurSel( m_chkNo );
	m_chkSN = m_chkString.Left(STR_SN_LEN);
	if( m_chkSN.Find("-") < 0 ){
		m_chkSN = m_chkString.Left(18);
	}
	sText.Format("%d", m_chkNo+1);
	m_current_num.SetWindowTextA(sText);
	m_current_sn.SetWindowTextA(m_chkSN);
	DWORD idThread;
	HANDLE        hThread;
	hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)software_batpower::WorkerThreadProc, this, 0, &idThread);
}

void software_batpower::dlg_pram_init()
{
	CString sText;
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
	m_current_num.SetWindowTextA("");
	m_current_sn.SetWindowTextA("");

	m_listEvt.ResetContent();
	m_Esetsn.SetFocus();
}
void software_batpower::endisable_input(BOOL process_end)
{
	((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(process_end);
	((CButton*)GetDlgItem(IDC_BUTTON2))->EnableWindow(process_end);
	((CButton*)GetDlgItem(IDC_BUTTON16))->EnableWindow(process_end);
	((CButton*)GetDlgItem(IDC_BUTTON14))->EnableWindow(process_end);
	((CButton*)GetDlgItem(IDC_BUTTON15))->EnableWindow(process_end);
}
void software_batpower::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	INT list_num;
	INT count = list.GetCount();
	dlg_pram_init();
	m_bChkAllItem = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if(m_bChkAllItem){
		if(count < 1) {
			AfxMessageBox("列表为空");
			return;
		}
		loop_times = 0;
		process_end = FALSE;
		endisable_input(process_end);
		open_process_thread(0);
	}else{
		list_num = list.GetCurSel();
		if(list_num < 0) {
			AfxMessageBox("请选择需要检测的SN号");
			return;
		}
		process_end = FALSE;
		endisable_input(process_end);
		open_process_thread(list_num);
	}
}

BOOL software_batpower::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);
	// TODO:  在此添加额外的初始化
	CString dir_path;
	if(last_sel_soft.Find("方舟") >= 0){
		dir_path = LOG_PATH_POWER_fz;
	}else if (last_sel_soft.Find("金牛") >= 0){
		dir_path = LOG_PATH_POWER_jn;
	}
	CreateDirectory("log", 0);
	CreateDirectory(dir_path, 0);

	CString soft_name;
	soft_name += last_sel_soft;
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	CString sFname;
	CTime tNow = CTime::GetCurrentTime();
	sFname.Format("LOG-CH%d-%04d%02d%02d-%dh.txt", 0,
		tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour());
	sFname = dir_path + sFname;

	if(m_hLogFile.Open(sFname, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite) == TRUE) {
		m_hLogFile.SeekToEnd();
		sFname.Format("===============================================\r\n");
		m_hLogFile.Write(sFname, sFname.GetLength());
	}
	
	mesInfo.g_mes_ip = general_option_instance.g_mes_ip;
	mesInfo.g_mes_port = general_option_instance.g_mes_port;
	m_sqlite.OpenSqliteInit();

	process_end = TRUE;
	dlg_pram_init();

	//CString sText;
	//m_ctlCom.portCom = general_option_instance.g_ttycom_port;
	//m_ctlCom.baudCom = general_option_instance.g_ttycom_baud;

	//if(FALSE == m_ctlCom.InitComm(m_ctlCom.portCom, DEIGHT, SONE, PNONE, m_ctlCom.baudCom)) {
	//	com_open_flag = FALSE;
	//	SEND_USER_MSG(m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
	//}else{
	//	sText.Format("打开端口%d成功",m_ctlCom.portCom);
	//	AddEvent2List(sText);
	//	com_open_flag = TRUE;
	//}
	return TRUE;  // return TRUE unless you set the focus to a control
}

extern BOOL CheckSN(CString sn, CString sPrompt, UINT iStart, UINT count);
extern BOOL CheckSNv2(CString sn, CString sPrompt, UINT iStart, UINT count);
BOOL software_batpower::CompareSameName(CString sItem)
{
	BOOL bRet = FALSE;
	CString sText;
	int count = list.GetCount();
	for(int i = 0; i < count; i++) {
		list.GetText(i, sText);
		if(sText.Find(sItem) >=0) {
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}
void software_batpower::OnEnChangeSetSn()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString sText;
	UpdateData(TRUE);
	if(process_end){//表示现在程序没有跑，可以添加列表
		m_Esetsn.GetWindowTextA(sText);
		if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
			if( sText.Find("-") < 0 ){
				if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
								general_option_instance.g_sn_start, 
								general_option_instance.g_sn_count) != TRUE) {
					AfxMessageBox("当前产品SN号(v2)不正确");
				}else{
					if(CompareSameName(sText) == FALSE){
						list.AddString(sText);
						m_Esetsn.SetWindowTextA("");
						sText.Format("%d", list.GetCount());
						GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
						list.SetCurSel( list.GetCount() - 1);
					}else {
						AfxMessageBox("已经存在相同的SN号");
						m_Esetsn.SetWindowTextA("");
					}
				}
			}
		}
		if(sText.GetLength() == 19){
			if(CheckSN(sText, general_option_instance.g_sn_prompt, 
								general_option_instance.g_sn_start, 
								general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("当前产品SN号(v1)不正确");
			}else{
					if(CompareSameName(sText) == FALSE){
						list.AddString(sText);
						m_Esetsn.SetWindowTextA("");
						sText.Format("%d", list.GetCount());
						GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
						list.SetCurSel( list.GetCount() - 1);
					}else {
						AfxMessageBox("已经存在相同的SN号");
						m_Esetsn.SetWindowTextA("");
					}
			}
		}
	}
	// TODO:  在此添加控件通知处理程序代码
}


void software_batpower::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	list.ResetContent();
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
}


void software_batpower::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sText;
	int list_num = list.GetCurSel();
	if(list_num < 0) {
		AfxMessageBox("请选择需要检测的SN号");
		return;
	}
	list.DeleteString(list_num);
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
}


void software_batpower::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	int count = list.GetCount();
	if(count < 1) {
		AfxMessageBox("列表为空");
		return;
	}

	char fliter[]="列表文件(*.txt;)\0*.txt;\0"
		"所有文件(*.*)\0*.*\0\0";

	CFileDialog fdlg(FALSE);
	fdlg.m_ofn.lpstrFilter=fliter;
	if(IDOK!=fdlg.DoModal())
		return;

	CString path=fdlg.GetPathName();
	CString file=fdlg.GetFileName();
	path = path+".txt";

	if(TRUE == PathFileExists(path)) {
		if(AfxMessageBox("文件已经存在，确定覆盖？", MB_YESNO) != IDYES)
			return;
	}

	CFile hFile;

	if(hFile.Open(path, CFile::modeCreate | CFile::modeReadWrite) != TRUE) {
		AfxMessageBox("打开文件失败");
		return;
	}

	CString sText;
	for(int i = 0; i < count; i++) {
		list.GetText(i, sText);
		hFile.Write(sText, sText.GetLength());
		hFile.Write("\r\n", 2);
	}

	hFile.Close();
}


void software_batpower::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	if(list.GetCount() > 0) {
		if(AfxMessageBox("载入文件会清空原来的列表，确定载入？", MB_YESNO) != IDYES)
			return;
	}
	
	char fliter[]="列表文件(*.txt;)\0*.txt;\0"
		"所有文件(*.*)\0*.*\0\0";
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrFilter=fliter;
	if(IDOK!=fdlg.DoModal())
		return;

	CString path=fdlg.GetPathName();

	TRACE("开始载入文件数据\n");

	CStdioFile hFile;
	if(hFile.Open(path, CFile::modeRead) != TRUE) {
		AfxMessageBox("打开文件失败");
		return;
	}
	
	list.ResetContent();

	CString sText;
	for(int i = 0; i < 999999; i++) {
		if(hFile.ReadString(sText) != TRUE)
			break;
		
		//if(sText.GetLength() >= STR_SN_LEN)  {
		if(sText.GetLength() >= 18)  {
			if(CompareSameName(sText) == FALSE)
				list.AddString(sText);
			else {
				AfxMessageBox("文件中有相同的SN号，自动跳过");
			}
		}

	}
	hFile.Close();
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
}


BOOL software_batpower::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
