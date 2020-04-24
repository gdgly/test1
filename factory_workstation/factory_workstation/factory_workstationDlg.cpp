
// factory_workstationDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "factory_workstation.h"
#include "factory_workstationDlg.h"
#include "afxdialogex.h"
#include "afxwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_version;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_version, m_version);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cfactory_workstationDlg 对话框



Cfactory_workstationDlg::Cfactory_workstationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cfactory_workstationDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cfactory_workstationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, platform_ctrl_tab);
}

BEGIN_MESSAGE_MAP(Cfactory_workstationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &Cfactory_workstationDlg::OnTcnSelchangeTab1)
	ON_COMMAND(ID_option, &Cfactory_workstationDlg::Onoption)
	ON_COMMAND(ID_burn_check, &Cfactory_workstationDlg::Onburncheck)
	ON_COMMAND(ID_bluetooth, &Cfactory_workstationDlg::Onbluetooth)
	ON_COMMAND(ID_bat_power, &Cfactory_workstationDlg::Onbatpower)
	ON_COMMAND(ID_by_burn, &Cfactory_workstationDlg::Onbyburn)
	ON_COMMAND(ID_by_check, &Cfactory_workstationDlg::Onbycheck)
	ON_COMMAND(ID_by_total_machine, &Cfactory_workstationDlg::Onbytotalmachine)
	ON_COMMAND(ID_by_air, &Cfactory_workstationDlg::Onbyair)
	ON_COMMAND(ID_charge_current, &Cfactory_workstationDlg::Onchargecurrent)
	ON_COMMAND(ID_before_press_current, &Cfactory_workstationDlg::Onbeforepresscurrent)
	ON_COMMAND(ID_by_power, &Cfactory_workstationDlg::Onbypower)
	ON_COMMAND(ID_sz_burn, &Cfactory_workstationDlg::Onszburn)
	ON_COMMAND(ID_use, &Cfactory_workstationDlg::Onuse)
	ON_COMMAND(ID_sz_check, &Cfactory_workstationDlg::Onszcheck)
	ON_COMMAND(ID_sz_btaddr, &Cfactory_workstationDlg::Onszbtaddr)
	ON_COMMAND(ID_sz_checkbox, &Cfactory_workstationDlg::Onszcheckbox)
	ON_COMMAND(ID_sz_burnsn, &Cfactory_workstationDlg::Onszburnsn)
END_MESSAGE_MAP()


int Cfactory_workstationDlg::LoadCheckConf()
{
	CIni ini;
	CString sText, sFName;
	GetModuleFileName(NULL, sText.GetBuffer(MAX_PATH), MAX_PATH);
	sText.ReleaseBuffer();
	sText = sText.Left(sText.ReverseFind('\\'));
	sFName = sText;
	sFName += "\\";
	sFName += DEVICE_CHECK_CONF;
	//if(GetFileAttributes(sFName) == INVALID_FILE_ATTRIBUTES) {
	//	sText = sText.Left(sText.ReverseFind('\\') + 1);
	//	sFName = sText;
	//	sFName += DEVICE_CHECK_CONF;
	//}
	ini.SetPathName(sFName);
	glob_param_instance.sFName = sFName;

	glob_param_instance.general_option_instance.g_check_mes=	ini.GetUInt("GENERAL", "g_check_mes", 0x11, 10);
	glob_param_instance.general_option_instance.g_check_mes_type=	ini.GetUInt("GENERAL", "g_check_mes_type", 0x11, 10);
	//if(glob_param_instance.general_option_instance.g_check_mes_type != 1&&
	//	glob_param_instance.general_option_instance.g_check_mes_type != 2){
	//	glob_param_instance.general_option_instance.g_check_mes_type = 1;
	//}
	glob_param_instance.general_option_instance.g_mes_ip =		ini.GetString("GENERAL", "g_mes_ip");
	glob_param_instance.general_option_instance.g_mes_port =	ini.GetUInt("GENERAL", "g_mes_port", 0x11, 10);
	glob_param_instance.general_option_instance.g_sn_prompt =	ini.GetString("GENERAL", "g_sn_prompt");
	glob_param_instance.general_option_instance.g_sn_start =	ini.GetUInt("GENERAL", "g_sn_start", 0x11, 10);
	glob_param_instance.general_option_instance.g_sn_count =	ini.GetUInt("GENERAL", "g_sn_count", 0x11, 10);
	glob_param_instance.general_option_instance.g_ttycom_port = ini.GetUInt("GENERAL", "g_ttycom_port", 0x11, 10);
	glob_param_instance.general_option_instance.g_ttycom_baud = ini.GetUInt("GENERAL", "g_ttycom_baud", 0x11, 10);
	glob_param_instance.general_option_instance.g_quit_config_dilog=	ini.GetUInt("GENERAL", "g_quit_config_dilog", 0x11, 10);
	glob_param_instance.general_option_instance.g_if_open_nouse=	ini.GetUInt("GENERAL", "g_if_open_nouse", 0x11, 10);
	glob_param_instance.general_option_instance.g_station_name =				ini.GetString("GENERAL", "g_station_name");

	glob_param_instance.burncheck_option_instance.g_sJLinkPath=			ini.GetString("BURNCHECK", "g_sJLinkPath");
	glob_param_instance.burncheck_option_instance.g_wrFirm =			ini.GetString("BURNCHECK", "g_wrFirm");
	glob_param_instance.burncheck_option_instance.g_wrFirm_folder =		ini.GetString("BURNCHECK", "g_wrFirm_folder");
	glob_param_instance.burncheck_option_instance.g_burn_current_test =		ini.GetUInt("BURNCHECK", "g_burn_current_test", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_current_test =		ini.GetUInt("BURNCHECK", "g_current_test", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_addCurrent =		ini.GetUInt("BURNCHECK", "g_addCurrent", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_addlarge =			ini.GetUInt("BURNCHECK", "g_addlarge", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_chargCurrent =		ini.GetUInt("BURNCHECK", "g_chargCurrent", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_sleep_current_test =			ini.GetUInt("BURNCHECK", "g_sleep_current_test", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_sleep_current_delay =			ini.GetUInt("BURNCHECK", "g_sleep_current_delay", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_sleepCurrent =		ini.GetUInt("BURNCHECK", "g_sleepCurrent", 0x11, 10);

	glob_param_instance.burncheck_option_instance.g_currentcom_port =	ini.GetUInt("BURNCHECK", "g_currentcom_port", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_currentcom_baud =	ini.GetUInt("BURNCHECK", "g_currentcom_baud", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_out_str =			ini.GetString("BURNCHECK", "g_out_str");

	glob_param_instance.burncheck_option_instance.g_if_check_version_str =	ini.GetUInt("BURNCHECK", "g_if_check_version_str", 0x11, 10);
	glob_param_instance.burncheck_option_instance.g_out_sn_str =			ini.GetString("BURNCHECK", "g_out_sn_str");
	glob_param_instance.burncheck_option_instance.g_out_languages_str =		ini.GetString("BURNCHECK", "g_out_languages_str");

	glob_param_instance.bluetooth_option_instance.g_db_value_fz=			ini.GetUInt("BLUETOOTH", "g_db_value_fz", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_sub_value_fz=			ini.GetUInt("BLUETOOTH", "g_sub_value_fz", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_cali_max_value_fz=		ini.GetUInt("BLUETOOTH", "g_cali_max_value_fz", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_cali_min_value_fz=		ini.GetUInt("BLUETOOTH", "g_cali_min_value_fz", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_sub_value_air=			ini.GetUInt("BLUETOOTH", "g_sub_value_air", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_db_value_jn=			ini.GetUInt("BLUETOOTH", "g_db_value_jn", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_check_cycle_jn=			ini.GetUInt("BLUETOOTH", "g_check_cycle_jn", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_cycle_max_jn=			ini.GetUInt("BLUETOOTH", "g_cycle_max_jn", 0x11, 10);
	glob_param_instance.bluetooth_option_instance.g_cycle_min_jn=			ini.GetUInt("BLUETOOTH", "g_cycle_min_jn", 0x11, 10);
	
	glob_param_instance.batpower_option_instance.g_power_percent =			ini.GetUInt("BATPOWER", "g_power_percent", 0x11, 10);
	glob_param_instance.batpower_option_instance.g_reboot_num =				ini.GetUInt("BATPOWER", "g_reboot_num", 0x11, 10);
	glob_param_instance.batpower_option_instance.g_check_current =			ini.GetUInt("BATPOWER", "g_check_current", 0x11, 10);
	glob_param_instance.batpower_option_instance.g_current_value =			ini.GetUInt("BATPOWER", "g_current_value", 0x11, 10);
	glob_param_instance.batpower_option_instance.g_twotc_lasttime =			ini.GetUInt("BATPOWER", "g_twotc_lasttime", 0x11, 10);
	glob_param_instance.batpower_option_instance.g_twotc_consume_power =	ini.GetUInt("BATPOWER", "g_twotc_consume_power", 0x11, 10);
	glob_param_instance.batpower_option_instance.g_com_timeout_times =		ini.GetUInt("BATPOWER", "g_com_timeout_times", 0x11, 10);
	
	glob_param_instance.charge_option_instance.g_chargCurrent =		ini.GetUInt("CHARGECHECK", "g_chargCurrent", 0x11, 10);
	glob_param_instance.charge_option_instance.g_currentcom_port =	ini.GetUInt("CHARGECHECK", "g_currentcom_port", 0x11, 10);
	glob_param_instance.charge_option_instance.g_currentcom_baud =	ini.GetUInt("CHARGECHECK", "g_currentcom_baud", 0x11, 10);

	int i;
	CString part_name;
	glob_param_instance.burn_by_option_instance.g_wrFirm_folder =			ini.GetString("BY_BURN", "g_wrFirm_folder");
	for(i=0; i<8; i++){
		part_name.Format("BY_BURN_PART%d",i);
		memset(&glob_param_instance.burn_by_option_instance.spinand_partition_ins[i] ,0 ,sizeof(spinand_partition));
		glob_param_instance.burn_by_option_instance.spinand_partition_ins[i].manager_mode =		ini.GetUInt(part_name, "manager_mode", 0x11, 10);
		glob_param_instance.burn_by_option_instance.spinand_partition_ins[i].offset =			ini.GetUInt(part_name, "offset", 0x11, 16);
		glob_param_instance.burn_by_option_instance.spinand_partition_ins[i].size =				ini.GetUInt(part_name, "size", 0x11, 16);	
		sText =	ini.GetString(part_name, "name");
		memcpy(glob_param_instance.burn_by_option_instance.spinand_partition_ins[i].name, sText, (sText.GetLength()>=31) ? 31:sText.GetLength());
	}

	glob_param_instance.check_by_option_instance.g_current_test =			ini.GetUInt("BY_CHECK", "g_current_test", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_sleep_current_test =		ini.GetUInt("BY_CHECK", "g_sleep_current_test", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_backlight_current_test =	ini.GetUInt("BY_CHECK", "g_backlight_current_test", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_normal_current_max =		ini.GetUInt("BY_CHECK", "g_normal_current_max", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_normal_current_min =		ini.GetUInt("BY_CHECK", "g_normal_current_min", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_chargCurrent =			ini.GetUInt("BY_CHECK", "g_chargCurrent", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_backlight_Current =			ini.GetUInt("BY_CHECK", "g_backlight_Current", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_sleepCurrent =		ini.GetUInt("BY_CHECK", "g_sleepCurrent", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_currentcom_port =		ini.GetUInt("BY_CHECK", "g_currentcom_port", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_currentcom_baud =		ini.GetUInt("BY_CHECK", "g_currentcom_baud", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_usb_timeout =			ini.GetUInt("BY_CHECK", "g_usb_timeout", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_record_time =			ini.GetUInt("BY_CHECK", "g_record_time", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_recode_passdb =			ini.GetUInt("BY_CHECK", "g_recode_passdb", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_startcheck_times =			ini.GetUInt("BY_CHECK", "g_startcheck_times", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_sleep_waittime =			ini.GetUInt("BY_CHECK", "g_sleep_waittime", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_wifi_timeout =			ini.GetUInt("BY_CHECK", "g_wifi_timeout", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_wifi_speed =				ini.GetUInt("BY_CHECK", "g_wifi_speed", 0x11, 10);
	glob_param_instance.check_by_option_instance.g_wifi_ip=					ini.GetString("BY_CHECK", "g_wifi_ip");
	glob_param_instance.check_by_option_instance.g_wifi_ssid=				ini.GetString("BY_CHECK", "g_wifi_ssid");
	glob_param_instance.check_by_option_instance.g_wifi_passwd=				ini.GetString("BY_CHECK", "g_wifi_passwd");
	glob_param_instance.check_by_option_instance.g_bluetooth_board_mac=			ini.GetString("BY_CHECK", "g_bluetooth_board_mac");


	glob_param_instance.bp_current_by_option_instance.g_currentcom_port =			ini.GetUInt("BY_BP_CURRENT", "g_currentcom_port", 0x11, 10);
	glob_param_instance.bp_current_by_option_instance.g_currentcom_baud =			ini.GetUInt("BY_BP_CURRENT", "g_currentcom_baud", 0x11, 10);
	glob_param_instance.bp_current_by_option_instance.g_sleep_current_max =			ini.GetUInt("BY_BP_CURRENT", "g_sleep_current_max", 0x11, 10);
	glob_param_instance.bp_current_by_option_instance.g_sleep_current_min =			ini.GetUInt("BY_BP_CURRENT", "g_sleep_current_min", 0x11, 10);

	glob_param_instance.total_machine_by_option_instance.g_connect_type =			ini.GetUInt("BY_TOTAL_MACHINE", "g_connect_type", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_usb_timeout =			ini.GetUInt("BY_TOTAL_MACHINE", "g_usb_timeout", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_cali_min_value =			ini.GetUInt("BY_TOTAL_MACHINE", "g_cali_min_value", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_cali_max_value =			ini.GetUInt("BY_TOTAL_MACHINE", "g_cali_max_value", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_ts_err_times =			ini.GetUInt("BY_TOTAL_MACHINE", "g_ts_err_times", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_ts_pix_off =				ini.GetUInt("BY_TOTAL_MACHINE", "g_ts_pix_off", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_record_time_sec =		ini.GetUInt("BY_TOTAL_MACHINE", "g_record_time_sec", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_record_passdb =			ini.GetUInt("BY_TOTAL_MACHINE", "g_record_passdb", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_record_subdb =			ini.GetUInt("BY_TOTAL_MACHINE", "g_record_subdb", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_play_time_sec =			ini.GetUInt("BY_TOTAL_MACHINE", "g_play_time_sec", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_play_passdb =			ini.GetUInt("BY_TOTAL_MACHINE", "g_play_passdb", 0x11, 10);

	glob_param_instance.total_machine_by_option_instance.g_wifi_timeout =			ini.GetUInt("BY_TOTAL_MACHINE", "g_wifi_timeout", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_wifi_speed =				ini.GetUInt("BY_TOTAL_MACHINE", "g_wifi_speed", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_wifi_ip=					ini.GetString("BY_TOTAL_MACHINE", "g_wifi_ip");
	glob_param_instance.total_machine_by_option_instance.g_wifi_ssid=				ini.GetString("BY_TOTAL_MACHINE", "g_wifi_ssid");
	glob_param_instance.total_machine_by_option_instance.g_wifi_passwd=				ini.GetString("BY_TOTAL_MACHINE", "g_wifi_passwd");
	glob_param_instance.total_machine_by_option_instance.g_wifihot_timeout =			ini.GetUInt("BY_TOTAL_MACHINE", "g_wifihot_timeout", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_wifihot_ip=					ini.GetString("BY_TOTAL_MACHINE", "g_wifihot_ip");
	//glob_param_instance.total_machine_by_option_instance.g_wifihot_ssid=				ini.GetString("BY_TOTAL_MACHINE", "g_wifihot_ssid");
	glob_param_instance.total_machine_by_option_instance.g_wifihot_passwd=				ini.GetString("BY_TOTAL_MACHINE", "g_wifihot_passwd");
	glob_param_instance.total_machine_by_option_instance.g_wifihot_adb_timeout =		ini.GetUInt("BY_TOTAL_MACHINE", "g_wifihot_adb_timeout", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_local_net_name=				ini.GetString("BY_TOTAL_MACHINE", "g_local_net_name");
	glob_param_instance.total_machine_by_option_instance.g_local_ip=					ini.GetString("BY_TOTAL_MACHINE", "g_local_ip");
	glob_param_instance.total_machine_by_option_instance.g_pass_bat_power =				ini.GetUInt("BY_TOTAL_MACHINE", "g_pass_bat_power", 0x11, 10);
	glob_param_instance.total_machine_by_option_instance.g_bluetooth_board_mac=			ini.GetString("BY_TOTAL_MACHINE", "g_bluetooth_board_mac");
	
	glob_param_instance.air_by_option_instance.g_connect_type =						ini.GetUInt("BY_AIR", "g_connect_type", 0x11, 10);
	glob_param_instance.air_by_option_instance.g_usb_timeout =						ini.GetUInt("BY_AIR", "g_usb_timeout", 0x11, 10);
	glob_param_instance.air_by_option_instance.g_record_time_sec =					ini.GetUInt("BY_AIR", "g_record_time_sec", 0x11, 10);
	glob_param_instance.air_by_option_instance.g_record_subdb =						ini.GetUInt("BY_AIR", "g_record_subdb", 0x11, 10);
	glob_param_instance.air_by_option_instance.g_wifihot_adb_timeout =				ini.GetUInt("BY_AIR", "g_wifihot_adb_timeout", 0x11, 10);
	glob_param_instance.air_by_option_instance.g_wifihot_timeout =					ini.GetUInt("BY_AIR", "g_wifihot_timeout", 0x11, 10);
	glob_param_instance.air_by_option_instance.g_wifihot_passwd =					ini.GetString("BY_AIR", "g_wifihot_passwd");
	glob_param_instance.air_by_option_instance.g_wifihot_ip =						ini.GetString("BY_AIR", "g_wifihot_ip");
	glob_param_instance.air_by_option_instance.g_local_net_name =					ini.GetString("BY_AIR", "g_local_net_name");
	glob_param_instance.air_by_option_instance.g_local_ip =							ini.GetString("BY_AIR", "g_local_ip");

	glob_param_instance.batpower_by_option_instance.g_power_percent =				ini.GetUInt("BY_BATPOWER", "g_power_percent", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_currentcom_port =				ini.GetUInt("BY_BATPOWER", "g_currentcom_port", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_currentcom_baud =				ini.GetUInt("BY_BATPOWER", "g_currentcom_baud", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_current_test =				ini.GetUInt("BY_BATPOWER", "g_current_test", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_normal_current_max =				ini.GetUInt("BY_BATPOWER", "g_normal_current_max", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_normal_current_min =				ini.GetUInt("BY_BATPOWER", "g_normal_current_min", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_chargCurrent =				ini.GetUInt("BY_BATPOWER", "g_chargCurrent", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_if_setup_reset =					ini.GetUInt("BY_BATPOWER", "g_if_setup_reset", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_startcheck_times =					ini.GetUInt("BY_BATPOWER", "g_startcheck_times", 0x11, 10);
	glob_param_instance.batpower_by_option_instance.g_usb_timeout =					ini.GetUInt("BY_BATPOWER", "g_usb_timeout", 0x11, 10);

	glob_param_instance.tools_option_instance.g_by_get_left_filename =				ini.GetString("TOOL_BY_GETFILE", "g_by_get_left_filename");
	glob_param_instance.tools_option_instance.g_by_get_right_filename =				ini.GetString("TOOL_BY_GETFILE", "g_by_get_right_filename");
	glob_param_instance.tools_option_instance.g_by_savfilename =					ini.GetString("TOOL_BY_GETFILE", "g_by_savfilename");

	glob_param_instance.burn_sz_option_instance.g_wrFirm_folder =			ini.GetString("SZ_BURN", "g_wrFirm_folder");
	glob_param_instance.burn_sz_option_instance.g_if_burn_qcc =				ini.GetUInt("SZ_BURN", "g_if_burn_qcc", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_if_erase_flash =			ini.GetUInt("SZ_BURN", "g_if_erase_flash", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_if_burn_param =			ini.GetUInt("SZ_BURN", "g_if_burn_param", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_if_test_after_burn =		ini.GetUInt("SZ_BURN", "g_if_test_after_burn", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_if_burn_apollo =			ini.GetUInt("SZ_BURN", "g_if_burn_apollo", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_if_burn_box =				ini.GetUInt("SZ_BURN", "g_if_burn_box", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_sJLinkPath =				ini.GetString("SZ_BURN", "g_sJLinkPath");
	glob_param_instance.burn_sz_option_instance.g_hw_version =				ini.GetString("SZ_BURN", "g_hw_version");
	glob_param_instance.burn_sz_option_instance.g_usbdbg_num =				ini.GetString("SZ_BURN", "g_usbdbg_num");
	glob_param_instance.burn_sz_option_instance.g_if_read_freq =				ini.GetUInt("SZ_BURN", "g_if_read_freq", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_readfreq_waittime =				ini.GetUInt("SZ_BURN", "g_readfreq_waittime", 0x11, 10);
	glob_param_instance.burn_sz_option_instance.g_if_write_freq =				ini.GetUInt("SZ_BURN", "g_if_write_freq", 0x11, 10);

	glob_param_instance.check_sz_option_instance.g_if_check_light =				ini.GetUInt("SZ_CHECK", "g_if_check_light", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_if_check_tap =				ini.GetUInt("SZ_CHECK", "g_if_check_tap", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_if_check_plc =				ini.GetUInt("SZ_CHECK", "g_if_check_plc", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_if_check_apollo =			ini.GetUInt("SZ_CHECK", "g_if_check_apollo", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_check_timeout_s =			ini.GetUInt("SZ_CHECK", "g_check_timeout_s", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_check_light_addps_value =	ini.GetUInt("SZ_CHECK", "g_check_light_addps_value", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_record_time_sec =			ini.GetUInt("SZ_CHECK", "g_record_time_sec", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_record_passdb =				ini.GetUInt("SZ_CHECK", "g_record_passdb", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_record_subdb =				ini.GetUInt("SZ_CHECK", "g_record_subdb", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_play_time_sec =				ini.GetUInt("SZ_CHECK", "g_play_time_sec", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_play_passdb =				ini.GetUInt("SZ_CHECK", "g_play_passdb", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_init =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_init", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_light =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_light", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_tap =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_tap", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_plc =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_plc", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_apollo =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_apollo", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_record =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_record", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_play =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_play", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_if_check_bonemic =				ini.GetUInt("SZ_CHECK", "g_if_check_bonemic", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_glob_if_check_reboot =				ini.GetUInt("SZ_CHECK", "g_glob_if_check_reboot", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_wait_reboot_time_sec =				ini.GetUInt("SZ_CHECK", "g_wait_reboot_time_sec", 0x11, 10);
	glob_param_instance.check_sz_option_instance.g_wait_key_time_sec =				ini.GetUInt("SZ_CHECK", "g_wait_key_time_sec", 0x11, 10);

	glob_param_instance.btaddr_sz_option_instance.g_hw_version =				ini.GetString("SZ_BTADDR", "g_hw_version");
	glob_param_instance.btaddr_sz_option_instance.g_bt_name =					ini.GetString("SZ_BTADDR", "g_bt_name");
	glob_param_instance.btaddr_sz_option_instance.rl_flag =						ini.GetUInt("SZ_BTADDR", "rl_flag", 0x11, 10);
	//glob_param_instance.btaddr_sz_option_instance.g_station_name =				ini.GetString("SZ_BTADDR", "g_station_name");
	glob_param_instance.btaddr_sz_option_instance.g_if_locallic =				ini.GetUInt("SZ_BTADDR", "g_if_locallic", 0x11, 10);

	glob_param_instance.checkbox_sz_option_instance.g_if_check_wdt =				ini.GetUInt("SZ_CHECKBOX", "g_if_check_wdt", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_max20340 =			ini.GetUInt("SZ_CHECKBOX", "g_if_check_max20340", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_max77813 =			ini.GetUInt("SZ_CHECKBOX", "g_if_check_max77813", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_plc_commu =			ini.GetUInt("SZ_CHECKBOX", "g_if_check_plc_commu", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_hal =				ini.GetUInt("SZ_CHECKBOX", "g_if_check_hal", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_key =				ini.GetUInt("SZ_CHECKBOX", "g_if_check_key", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_usb =				ini.GetUInt("SZ_CHECKBOX", "g_if_check_usb", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_hal_after_plc =		ini.GetUInt("SZ_CHECKBOX", "g_if_check_hal_after_plc", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_cal_vol =			ini.GetUInt("SZ_CHECKBOX", "g_if_check_cal_vol", 0x11, 10);
	glob_param_instance.checkbox_sz_option_instance.g_if_check_plc_commu_right =	ini.GetUInt("SZ_CHECKBOX", "g_if_check_plc_commu_right", 0x11, 10);

	glob_param_instance.burnsn_sz_option_instance.g_burn_sn_len =	ini.GetUInt("SZ_BURNSN", "g_burn_sn_len", 0x11, 10);
	glob_param_instance.burnsn_sz_option_instance.g_l_sn_prompt =					ini.GetString("SZ_BURNSN", "g_l_sn_prompt");
	glob_param_instance.burnsn_sz_option_instance.g_r_sn_prompt =					ini.GetString("SZ_BURNSN", "g_r_sn_prompt");
	glob_param_instance.burnsn_sz_option_instance.g_c_sn_prompt =					ini.GetString("SZ_BURNSN", "g_c_sn_prompt");
	glob_param_instance.burnsn_sz_option_instance.g_if_repair_all =					ini.GetUInt("SZ_BURNSN", "g_if_repair_all", 0x11, 10);
	
	glob_param_instance.last_sel_soft = ini.GetString("GLOBLE", "last_sel_soft");
	if(glob_param_instance.last_sel_soft == ""){//ini中没找到last_sel_soft配置说明是第一次打开软件
		return 1;
	}
	return 0;
}

void Cfactory_workstationDlg::first_time_disp_tab()
{
	platform_ctrl_tab.InsertItem(0, _T("方舟"));
	platform_ctrl_tab.InsertItem(1, _T("金牛"));
	platform_ctrl_tab.InsertItem(2, _T("白羊"));
	platform_ctrl_tab.InsertItem(3, _T("双子"));
	platform_ctrl_tab.InsertItem(4, _T("小工具"));

	page_fz_dlg_int.Create(IDD_DIALOG_fz, &platform_ctrl_tab);
	page_jn_dlg_int.Create(IDD_DIALOG_jn, &platform_ctrl_tab);
	page_by_dlg_int.Create(IDD_DIALOG_by, &platform_ctrl_tab);
	page_sz_dlg_int.Create(IDD_DIALOG_sz, &platform_ctrl_tab);
	tools_dlg_int.Create(IDD_DIALOG_tools, &platform_ctrl_tab);
	CRect rc;
	platform_ctrl_tab.GetClientRect(rc);
	rc.top += 24;
	rc.bottom -= 4;
	rc.left += 4;
	rc.right -= 4;
	page_fz_dlg_int.MoveWindow(&rc);
	page_jn_dlg_int.MoveWindow(&rc);
	page_by_dlg_int.MoveWindow(&rc);
	page_sz_dlg_int.MoveWindow(&rc);
	tools_dlg_int.MoveWindow(&rc);
	page_fz_dlg_int.ShowWindow(SW_SHOW);
	page_jn_dlg_int.ShowWindow(SW_HIDE);
	page_by_dlg_int.ShowWindow(SW_HIDE);
	page_sz_dlg_int.ShowWindow(SW_HIDE);
	tools_dlg_int.ShowWindow(SW_HIDE);

	pDialog[0] = &page_fz_dlg_int;
	pDialog[1] = &page_jn_dlg_int;
	pDialog[2] = &page_by_dlg_int;
	pDialog[3] = &page_sz_dlg_int;
	pDialog[4] = &tools_dlg_int;
	pDialog_num = 5;
}

void Cfactory_workstationDlg::notfirst_time_disp_tab()
{
	platform_ctrl_tab.InsertItem(0, _T("默认"));
	platform_ctrl_tab.InsertItem(1, _T("方舟"));
	platform_ctrl_tab.InsertItem(2, _T("金牛"));
	platform_ctrl_tab.InsertItem(3, _T("白羊"));
	platform_ctrl_tab.InsertItem(4, _T("双子"));
	platform_ctrl_tab.InsertItem(5, _T("小工具"));

	default_dlg_int.Create(IDD_DIALOG_default, &platform_ctrl_tab);
	page_fz_dlg_int.Create(IDD_DIALOG_fz, &platform_ctrl_tab);
	page_jn_dlg_int.Create(IDD_DIALOG_jn, &platform_ctrl_tab);
	page_by_dlg_int.Create(IDD_DIALOG_by, &platform_ctrl_tab);
	page_sz_dlg_int.Create(IDD_DIALOG_sz, &platform_ctrl_tab);
	tools_dlg_int.Create(IDD_DIALOG_tools, &platform_ctrl_tab);
	CRect rc;
	platform_ctrl_tab.GetClientRect(rc);
	rc.top += 24;
	rc.bottom -= 4;
	rc.left += 4;
	rc.right -= 4;
	default_dlg_int.MoveWindow(&rc);
	page_fz_dlg_int.MoveWindow(&rc);
	page_jn_dlg_int.MoveWindow(&rc);
	page_by_dlg_int.MoveWindow(&rc);
	page_sz_dlg_int.MoveWindow(&rc);
	tools_dlg_int.MoveWindow(&rc);
	default_dlg_int.ShowWindow(SW_SHOW);
	page_fz_dlg_int.ShowWindow(SW_HIDE);
	page_jn_dlg_int.ShowWindow(SW_HIDE);
	page_by_dlg_int.ShowWindow(SW_HIDE);
	page_sz_dlg_int.ShowWindow(SW_HIDE);
	tools_dlg_int.ShowWindow(SW_HIDE);

	pDialog[0] = &default_dlg_int;
	pDialog[1] = &page_fz_dlg_int;
	pDialog[2] = &page_jn_dlg_int;
	pDialog[3] = &page_by_dlg_int;
	pDialog[4] = &page_sz_dlg_int;
	pDialog[5] = &tools_dlg_int;
	pDialog_num = 6;
}

void Cfactory_workstationDlg::tabdlg_set_param()
{
	parent_dlg_p = this;
	default_dlg_int.set_param((void*)parent_dlg_p);
	page_fz_dlg_int.set_param((void*)parent_dlg_p);
	page_jn_dlg_int.set_param((void*)parent_dlg_p);
	page_by_dlg_int.set_param((void*)parent_dlg_p);
	page_sz_dlg_int.set_param((void*)parent_dlg_p);
	tools_dlg_int.set_param((void*)parent_dlg_p);
}

// Cfactory_workstationDlg 消息处理程序
BOOL Cfactory_workstationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。
	TRACE("####################################\r\n");
	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CString soft_name;
	soft_name += "星空物联生产测试软件";
	soft_name += SOFT_VERSION;
	SetWindowText(soft_name);

	if( LoadCheckConf() ){
		tabdlg_set_param();
		first_time_disp_tab();
	}else{
		tabdlg_set_param();
		notfirst_time_disp_tab();
	}
	
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cfactory_workstationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cfactory_workstationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cfactory_workstationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cfactory_workstationDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	int i;
	int m_CurSelTab = platform_ctrl_tab.GetCurSel();
	for(i=0; i<pDialog_num; i++){
		if(i == m_CurSelTab){
			pDialog[i]->ShowWindow(SW_SHOW);
		}else{
			pDialog[i]->ShowWindow(SW_HIDE);
		}
	}
	page_by_dlg_int.refresh_view();
	page_fz_dlg_int.refresh_view();
	page_jn_dlg_int.refresh_view();
	page_sz_dlg_int.refresh_view();
	*pResult = 0;
}

void Cfactory_workstationDlg::Onoption()
{
	// TODO: 在此添加命令处理程序代码
	general_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.general_option_instance) );
}


void Cfactory_workstationDlg::Onburncheck()
{
	// TODO: 在此添加命令处理程序代码
	burncheck_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.burncheck_option_instance) );
}


void Cfactory_workstationDlg::Onbluetooth()
{
	// TODO: 在此添加命令处理程序代码
	bluetooth_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.bluetooth_option_instance) );
}


void Cfactory_workstationDlg::Onbatpower()
{
	// TODO: 在此添加命令处理程序代码
	batpower_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.batpower_option_instance) );
}

void Cfactory_workstationDlg::Onbyburn()
{
	// TODO: 在此添加命令处理程序代码
	burn_by_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.burn_by_option_instance) );
}

void Cfactory_workstationDlg::Onbycheck()
{
	// TODO: 在此添加命令处理程序代码
	check_by_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.check_by_option_instance) );
}

void Cfactory_workstationDlg::Onbytotalmachine()
{
	// TODO: 在此添加命令处理程序代码
	total_machine_by_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.total_machine_by_option_instance) );
}

void Cfactory_workstationDlg::Onbyair()
{
	// TODO: 在此添加命令处理程序代码
	air_by_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.air_by_option_instance) );
}

void Cfactory_workstationDlg::Onchargecurrent()
{
	// TODO: 在此添加命令处理程序代码
	charge_check_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.charge_option_instance) );
}

void Cfactory_workstationDlg::Onbeforepresscurrent()
{
	// TODO: 在此添加命令处理程序代码
	before_press_current_by_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.bp_current_by_option_instance) );
}

void Cfactory_workstationDlg::Onbypower()
{
	// TODO: 在此添加命令处理程序代码
	batpower_by_option_dlg  Dlg;
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.batpower_by_option_instance) );
}

void Cfactory_workstationDlg::Onszburn()
{
	// TODO: 在此添加命令处理程序代码
	burn_sz_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.burn_sz_option_instance) );
}

void Cfactory_workstationDlg::Onszcheck()
{
	// TODO: 在此添加命令处理程序代码
	check_sz_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.check_sz_option_instance) );
}

void Cfactory_workstationDlg::Onszbtaddr()
{
	// TODO: 在此添加命令处理程序代码
	btaddr_sz_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.btaddr_sz_option_instance) );
}

void Cfactory_workstationDlg::Onszcheckbox()
{
	// TODO: 在此添加命令处理程序代码
	check_sz_box_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.checkbox_sz_option_instance) );
}

#include "burn_sn_sz_option_dlg.h"
void Cfactory_workstationDlg::Onszburnsn()
{
	// TODO: 在此添加命令处理程序代码
	burn_sn_sz_option_dlg  Dlg; 
	Dlg.get_param( &glob_param_instance );
	Dlg.DoModal();
	Dlg.output_param( &(glob_param_instance.burnsn_sz_option_instance) );
}

#include "menu_use_option_dlg.h"
void Cfactory_workstationDlg::Onuse()
{
	// TODO: 在此添加命令处理程序代码
	menu_use_option_dlg  Dlg; 
	Dlg.DoModal();
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sText;
	sText.Format("factory_workstation，%s 版", SOFT_VERSION);
	m_version.SetWindowTextA(sText);
	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL Cfactory_workstationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN) 
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

