// software_batpower.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "software_batpower.h"
#include "afxdialogex.h"


// software_batpower �Ի���

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

INT software_batpower::MesPass(int idata)   //ÿһ���Ʒ�����Ժ󣬵��PASS�ϴ�MES
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1,sData;
	UINT a;
	sData.Format("%d",idata);
	if(mesIptr->Messtatus == 1){
		if(mesIptr->power_SendDatae(m_chkSN) == 0){
			sText.Format(_T("mes��������:%s"),mesIptr->line);
			sText1.Format(_T("mes�����жϽ��:%d"),mesIptr->Messtatus);
			a = sText.Find("}");
			if(a >= 0)
				sText = sText.Left(a+1);

			AddEvent2List(sText);
			AddEvent2List(sText1);
		}else
			AddEvent2List("mes����ʧ�ܣ�");

		if(mesIptr->Messtatus == 3){
			return 1;
		}else{
			AfxMessageBox("mes�������ݴ��󣬲����Լ���check-2�� ");
			mesIptr->log = 0;
			mesIptr->Messtatus = 0;
		}
	}
	return 0;
}

INT software_batpower::MesSend(int idata,BYTE *ifirver,BYTE *isouver)   //վλ�ϴ��Ժ��ϴ��������
{
	CCurlCtrl *mesIptr =  &mesInfo;
	CString sText,sText1,sData,sfirver,Ssouver;
	UINT a;
	sData.Format("%d",idata);
	sfirver.Format("%s",ifirver);
	Ssouver.Format("%s",isouver);
	if(mesIptr->Messtatus == 3){
		if(mesIptr->power_SendChecke(m_chkSN,sData,sfirver,Ssouver) == 0){
			sText.Format(_T("mes��������:%s"),mesIptr->line);
			sText1.Format(_T("mes�����жϽ��:%d"),mesIptr->Messtatus);
			a = sText.Find("}");
			if(a >= 0)
				sText = sText.Left(a+1);

			AddEvent2List(sText);
			AddEvent2List(sText1);
		}else
			AddEvent2List("mes����ʧ�ܣ�");
	}

	AddEvent2List("\r\n");
	if(mesIptr->Messtatus == 7){
		mesIptr->log = 0;
		mesIptr->Messtatus = 0;
		return 1;
	}else{
		AfxMessageBox("mes�������ݴ��󣬲����Լ���check-3�� ");
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
	}else if(m_sqlite.ReadSqlite_with_time(m_chkSN, now_time, "0", &sql_info) != 0){//1Сʱ��last_time֮����һ����¼
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
				sText.Format("�ҵ�sn:%s �ϴμ��ʱ�� %s\r\n\r\n����ȴ�%dСʱ���еڶ��μ��", m_chkSN, sql_info.Time, 
							batpower_option_instance.g_twotc_lasttime - (find_day*24+find_hours));
				AfxMessageBox(sText);
				//AddEvent2List(sText);
				return 2;//���μ��ʱ��δ��
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
		sn = sn + " ";//18λsn�Ų�һλ�ո�
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
		sn = sn + " ";//18λsn�Ų�һλ�ո�
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
		sn = sn + " ";//18λsn�Ų�һλ�ո�
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
		sn = sn + " ";//18λsn�Ų�һλ�ո�
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
	if(old0_3_08_version == 0){//0.3.08��ǰ�̼������߲��԰巵�ش���
		//AfxMessageBox("�����̼�Ϊ0.3.08ǰ�汾������԰巵�ش����������� ");
		sItem = old_version_check();
	}else{
		if(batpower_option_instance.g_check_current){
			ret = twice_current_check();
			if(ret == 1){//���μ��
				sItem = second_time_check();
			}else if(ret == 2){//���μ��ʱ��δ�� ȥ�������Ĳ���
				return;
			}else{//�״μ��
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
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("���ڲɼ��С�����");
			AddEvent2List("��ʼ�ɼ���");
			break;
		case WPARAM_UART_ERR:
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("��λ�ɼ�");
			((CButton*)GetDlgItem(IDC_BUTTON3))->EnableWindow(TRUE);
			AfxMessageBox("���ڴ�ʧ��");
			AddEvent2List("���ڴ�ʧ��");
			break;
		case WPARAM_START_TIMEOUT:
			AfxMessageBox("���տ�ʼ������ʱ����鴮�� ���԰��Ƿ�Ӻ�"); 
			AddEvent2List("���տ�ʼ������ʱ,��鴮�� ���԰��Ƿ�Ӻ�");
			((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("��λ�ɼ�");
			process_end = TRUE;
			endisable_input(process_end);
			break;
		//case WPARAM_STOP_TIMEOUT:
		//	((CButton*)GetDlgItem(IDC_BUTTON13))->SetWindowTextA("��λ�ɼ�");
		//	((CButton*)GetDlgItem(IDC_BUTTON13))->EnableWindow(TRUE);
		//	AfxMessageBox("����ֹͣ������ʱ"); 
		//	AddEvent2List("����ֹͣ������ʱ");
		//	break;
		case WPARAM_THREAD_EXITED:
			power_process();
			if(m_bChkAllItem){
				while(1){
					if( m_chkNo == list.GetCount()-1 ){//һ��ѭ������
						loop_times++;
					}
					if(loop_times == 4){//ѭ���Ĵ�
						((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("��λ�ɼ�");
						process_end = TRUE;
						endisable_input(process_end);
						break;//�˴���������listѭ��
					}
					m_chkNo += 1;
					if(m_chkNo > list.GetCount()-1){
						m_chkNo = 0;
					}
					list.GetText(m_chkNo, sText);
					if(sText.Find("pass") < 0  && sText.Find("fail") < 0){//���û�ܹ�����error�ľͿ�ʼ����
						Sleep(10);//ȷ���߳��Ѿ��˳�
						open_process_thread(m_chkNo);
						break;
					}
				}
			}else{
				((CButton*)GetDlgItem(IDC_BUTTON3))->SetWindowTextA("��λ�ɼ�");
				process_end = TRUE;
				endisable_input(process_end);
			}
			break;
		case WPARAM_MES_STRING:
			sText.Format(_T("mes��������:%s"),Mesresring);
			a = sText.Find("}");
			if(sText.Find("}") >= 0)
				sText = sText.Left(a + 1);
			AddEvent2List(sText);
			break;
		case WPARAM_MES_ERR:
			AddEvent2List("mes����ʧ�ܣ�");
			break;
		case WPARAM_SN:
			sText.Format(_T("SN��:%s mes�����жϽ��:%d"), m_chkSN, mesInfo.Messtatus);
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
	BYTE *pRdbuf = new BYTE[COM_MAX_RDSZIZE];//����buf

	//if(p->com_open_flag == FALSE){
	//	AfxMessageBox("���ڴ�ʧ�ܣ������Լ������ԣ� ");
	//	goto out;
	//}
	comInfo->portCom = p->general_option_instance.g_ttycom_port;
	comInfo->baudCom = p->general_option_instance.g_ttycom_baud;

	if(FALSE == comInfo->InitComm(comInfo->portCom, DEIGHT, SONE, PNONE, comInfo->baudCom)) {
		SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_UART_ERR, 0);
	}else{
		sText.Format("�򿪶˿�%d�ɹ�",comInfo->portCom);
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
		pto_GenerateRaw(&send_pkt, CMD_SET_AUDIO_START, 1234, cmd_dat, 19);//�ٴ�һλsn��
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
					if(0 == cmdPkt.u_dat16[0] || 1 == cmdPkt.u_dat16[0]){ //1��ʾ0.03.08�Ժ�İ汾
						if(0 == cmdPkt.u_dat16[0]) {      // �ɹ�
							p->iBatVal = cmdPkt.u_dat16[1];
							memcpy(p->firversion,&cmdPkt.u_dat[4],7);
							p->firversion[7] = '\0';
							memcpy(p->souversion,&cmdPkt.u_dat[11],7);
							p->souversion[7] = '\0';	
						}else if(1 == cmdPkt.u_dat16[0]){//0.03.08�Ժ�İ汾
							p->iBatVal = cmdPkt.u_dat[2];    //����
							p->rebootnum = cmdPkt.u_dat[3];  //��������
							memcpy(&p->voltage, &cmdPkt.u_dat[4], 2);  //��ѹ
							memcpy(&p->current, &cmdPkt.u_dat[6], 2);  //����
							memcpy(p->firversion, &cmdPkt.u_dat[8], 7);   //�̼��汾��
							p->firversion[7] = '\0';
							memcpy(p->souversion, &cmdPkt.u_dat[15], 7);   //��Դ�汾��
							p->souversion[7] = '\0';
						}
						if(p->general_option_instance.g_check_mes){
							if(p->general_option_instance.g_check_mes_type == 1){
								if(mesptr->power_SendChecks((LPCSTR)p->m_chkSN) == 0){            //�����ϱ��ӿ�    ��¼�ڶ���MES�ϴ�
									p->Mesvalue = mesptr->Messtatus;
									memcpy(p->Mesresring,mesptr->line,strlen(mesptr->line));
									SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_STRING, 0); 
									SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_SN, 0);
								}else
									SEND_USER_MSG(p->m_hWnd, WM_USERMSG_MASSPRODUCT, WPARAM_MES_ERR, 0);
								if(p->Mesvalue != 1){
									AfxMessageBox("mes�������ݴ��󣬲����Լ�������-1�� ");
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

// software_batpower ��Ϣ�������

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	INT list_num;
	INT count = list.GetCount();
	dlg_pram_init();
	m_bChkAllItem = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
	if(m_bChkAllItem){
		if(count < 1) {
			AfxMessageBox("�б�Ϊ��");
			return;
		}
		loop_times = 0;
		process_end = FALSE;
		endisable_input(process_end);
		open_process_thread(0);
	}else{
		list_num = list.GetCurSel();
		if(list_num < 0) {
			AfxMessageBox("��ѡ����Ҫ����SN��");
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
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString dir_path;
	if(last_sel_soft.Find("����") >= 0){
		dir_path = LOG_PATH_POWER_fz;
	}else if (last_sel_soft.Find("��ţ") >= 0){
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
	//	sText.Format("�򿪶˿�%d�ɹ�",m_ctlCom.portCom);
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString sText;
	UpdateData(TRUE);
	if(process_end){//��ʾ���ڳ���û���ܣ���������б�
		m_Esetsn.GetWindowTextA(sText);
		if(sText.GetLength() == 18){//v2 sn eg:A01A0A180801000001
			if( sText.Find("-") < 0 ){
				if(CheckSNv2(sText, general_option_instance.g_sn_prompt, 
								general_option_instance.g_sn_start, 
								general_option_instance.g_sn_count) != TRUE) {
					AfxMessageBox("��ǰ��ƷSN��(v2)����ȷ");
				}else{
					if(CompareSameName(sText) == FALSE){
						list.AddString(sText);
						m_Esetsn.SetWindowTextA("");
						sText.Format("%d", list.GetCount());
						GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
						list.SetCurSel( list.GetCount() - 1);
					}else {
						AfxMessageBox("�Ѿ�������ͬ��SN��");
						m_Esetsn.SetWindowTextA("");
					}
				}
			}
		}
		if(sText.GetLength() == 19){
			if(CheckSN(sText, general_option_instance.g_sn_prompt, 
								general_option_instance.g_sn_start, 
								general_option_instance.g_sn_count) != TRUE) {
				AfxMessageBox("��ǰ��ƷSN��(v1)����ȷ");
			}else{
					if(CompareSameName(sText) == FALSE){
						list.AddString(sText);
						m_Esetsn.SetWindowTextA("");
						sText.Format("%d", list.GetCount());
						GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
						list.SetCurSel( list.GetCount() - 1);
					}else {
						AfxMessageBox("�Ѿ�������ͬ��SN��");
						m_Esetsn.SetWindowTextA("");
					}
			}
		}
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void software_batpower::OnBnClickedButton16()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sText;
	list.ResetContent();
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
}


void software_batpower::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sText;
	int list_num = list.GetCurSel();
	if(list_num < 0) {
		AfxMessageBox("��ѡ����Ҫ����SN��");
		return;
	}
	list.DeleteString(list_num);
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
}


void software_batpower::OnBnClickedButton15()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int count = list.GetCount();
	if(count < 1) {
		AfxMessageBox("�б�Ϊ��");
		return;
	}

	char fliter[]="�б��ļ�(*.txt;)\0*.txt;\0"
		"�����ļ�(*.*)\0*.*\0\0";

	CFileDialog fdlg(FALSE);
	fdlg.m_ofn.lpstrFilter=fliter;
	if(IDOK!=fdlg.DoModal())
		return;

	CString path=fdlg.GetPathName();
	CString file=fdlg.GetFileName();
	path = path+".txt";

	if(TRUE == PathFileExists(path)) {
		if(AfxMessageBox("�ļ��Ѿ����ڣ�ȷ�����ǣ�", MB_YESNO) != IDYES)
			return;
	}

	CFile hFile;

	if(hFile.Open(path, CFile::modeCreate | CFile::modeReadWrite) != TRUE) {
		AfxMessageBox("���ļ�ʧ��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(list.GetCount() > 0) {
		if(AfxMessageBox("�����ļ������ԭ�����б�ȷ�����룿", MB_YESNO) != IDYES)
			return;
	}
	
	char fliter[]="�б��ļ�(*.txt;)\0*.txt;\0"
		"�����ļ�(*.*)\0*.*\0\0";
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrFilter=fliter;
	if(IDOK!=fdlg.DoModal())
		return;

	CString path=fdlg.GetPathName();

	TRACE("��ʼ�����ļ�����\n");

	CStdioFile hFile;
	if(hFile.Open(path, CFile::modeRead) != TRUE) {
		AfxMessageBox("���ļ�ʧ��");
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
				AfxMessageBox("�ļ�������ͬ��SN�ţ��Զ�����");
			}
		}

	}
	hFile.Close();
	sText.Format("%d", list.GetCount());
	GetDlgItem(IDC_STATIC_ALL_NUM)->SetWindowText(sText);
}


BOOL software_batpower::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}
