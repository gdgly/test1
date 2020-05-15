#pragma once
#include "afxcmn.h"
#include "Ini.h"
#include "mes_zhuoyi_dll.h"

#define	SOFT_VERSION	" V0.74.8"
//#define	SOFT_VERSION	" VT.74.8"
#define DEVICE_CHECK_CONF "config.ini"

#define CHILD_DLG_NAME_FZ_BURN "·½ÖÛ ÉÕÐ´"
#define CHILD_DLG_NAME_FZ_CHECK "·½ÖÛ °å²â"
#define CHILD_DLG_NAME_FZ_BLUETOOTH "·½ÖÛ À¶ÑÀÒôÆµ"
#define CHILD_DLG_NAME_FZ_AIR "·½ÖÛ ÆøÃÜÐÔ"
#define CHILD_DLG_NAME_FZ_POWER "·½ÖÛ µçÁ¿"
#define CHILD_DLG_NAME_FZ_CHARGE "·½ÖÛ ³äµç"

#define CHILD_DLG_NAME_JN_BURN "½ðÅ£ ÉÕÐ´"
#define CHILD_DLG_NAME_JN_CHECK "½ðÅ£ °å²â"
#define CHILD_DLG_NAME_JN_BLUETOOTH "½ðÅ£ À¶ÑÀÒôÆµ"
#define CHILD_DLG_NAME_JN_POWER "½ðÅ£ µçÁ¿"
#define CHILD_DLG_NAME_JN_CHARGE "½ðÅ£ ³äµç"

#define CHILD_DLG_NAME_BY_BURN "°×Ñò ÉÕÐ´"
#define CHILD_DLG_NAME_BY_CHECK "°×Ñò °å²â"
#define CHILD_DLG_NAME_BY_CHECK_WIFIBT "°×Ñò °å²â(wifibt)"
#define CHILD_DLG_NAME_BY_FULL_MACHINE "°×Ñò Õû»ú²âÊÔ"
#define CHILD_DLG_NAME_BY_AIR "°×Ñò ÆøÃÜÐÔ"
#define CHILD_DLG_NAME_BY_BP_CURRENT "°×Ñò Ñ¹ºÏµçÁ÷¼ì²â"
#define CHILD_DLG_NAME_BY_BATPOWER "°×Ñò µç³ØµçÁ¿"

#define CHILD_DLG_NAME_SZ_BURN "Ë«×Ó ÉÕÐ´"
#define CHILD_DLG_NAME_SZ_CHECK "Ë«×Ó °å²â"
#define CHILD_DLG_NAME_SZ_BTADDR "Ë«×Ó ÉÕÐ´À¶ÑÀµØÖ·"
#define CHILD_DLG_NAME_SZ_CHECKBOX "Ë«×Ó ºÐ×Ó²âÊÔ(´®¿Ú)"
#define CHILD_DLG_NAME_SZ_BURNSN "Ë«×Ó snÉÕÐ´(´®¿Ú)"

#define LOG_PATH_BURN_fz "log\\burn_fz\\"
#define LOG_PATH_CHECK_fz "log\\check_fz\\"
#define LOG_PATH_BLUETOOTH_fz "log\\bluetooth_fz\\"
#define LOG_PATH_AIR_fz "log\\air_fz\\"
#define LOG_PATH_POWER_fz "log\\power_fz\\"
#define LOG_PATH_CHARGE_fz "log\\charge_fz\\"

#define LOG_PATH_BURN_jn "log\\burn_jn\\"
#define LOG_PATH_CHECK_jn "log\\check_jn\\"
#define LOG_PATH_BLUETOOTH_jn "log\\bluetooth_jn\\"
#define LOG_PATH_POWER_jn "log\\power_jn\\"
#define LOG_PATH_CHARGE_jn "log\\charge_jn\\"

#define LOG_PATH_BURN_by "log\\burn_by\\"
#define LOG_PATH_CHECK_by "log\\check_by\\"
#define LOG_PATH_CHECK_by_wifibt "log\\check_by_wifibt\\"
#define LOG_PATH_BP_CURRENT_by "log\\bp_current_by\\"
#define LOG_PATH_TOTAL_by "log\\total_by\\"
#define LOG_PATH_AIR_by "log\\air_by\\"
#define LOG_PATH_BATPOWER_by "log\\power_by\\"

#define LOG_PATH_BURN_sz "log\\burn_sz\\"
#define LOG_PATH_CHECK_sz "log\\check_sz\\"
#define LOG_PATH_BTADDR_sz "log\\btaddr_sz\\"
#define LOG_PATH_CHECKBOX_sz "log\\checkbox_sz\\"
#define LOG_PATH_BURNSN_sz "log\\burnsn_sz\\"

#define WM_USERMSG_MASSPRODUCT         (WM_USER+33)

#define SANNUO_MES_LOG_PATH "C:\\log\\"
struct general_option_str{
	UINT g_quit_config_dilog;

	UINT g_check_mes;
	UINT g_check_mes_type;//1º£ÅÉ,2³¤ºç,3Á¢Ñ¶,4ÈýÅµ,5×¿Òí
	CString g_mes_ip;
	UINT	g_mes_port;
	CString	g_station_name;

	CString g_sn_prompt;
	UINT	g_sn_start;
	UINT	g_sn_count;

	UINT g_ttycom_port;
	UINT g_ttycom_baud;

	UINT g_if_open_nouse;
};

struct burncheck_option_str{
	CString g_sJLinkPath;
	CString	g_wrFirm;
	CString	g_wrFirm_folder;

	UINT g_current_test;
	UINT g_burn_current_test;
	UINT g_addCurrent;
	UINT g_addlarge;
	UINT g_chargCurrent;
	UINT g_sleep_current_test;
	UINT g_sleep_current_delay;
	UINT g_sleepCurrent;

	UINT g_currentcom_port;
	UINT g_currentcom_baud;

	CString	g_out_str;

	UINT g_if_check_version_str;
	CString g_out_sn_str;
	CString g_out_languages_str;
};

struct bluetooth_option_str{
	UINT g_db_value_fz;
	UINT g_sub_value_fz;
	UINT g_cali_min_value_fz;
	UINT g_cali_max_value_fz;

	UINT g_sub_value_air;

	UINT g_db_value_jn;
	UINT g_check_cycle_jn;
	UINT g_cycle_min_jn;
	UINT g_cycle_max_jn;
};

struct batpower_option_str{
	UINT g_power_percent;
	UINT g_reboot_num;
	UINT g_check_current;
	UINT g_current_value;
	UINT g_twotc_lasttime;
	UINT g_twotc_consume_power;
	UINT g_com_timeout_times;
};

struct charge_option_str{
	UINT g_chargCurrent;

	UINT g_currentcom_port;
	UINT g_currentcom_baud;
};

struct spinand_partition {
	char name[32];         /* identifier string */
	UINT size;          /* partition size */
	UINT offset;        /* offset within the master MTD space */
	UINT mask_flags;       /* master MTD flags to mask out for this partition */
	UINT manager_mode;         /* manager_mode mtd or ubi */
};

struct burn_by_option_str{
	CString	g_wrFirm_folder;
	spinand_partition spinand_partition_ins[8];
};

struct check_by_option_str{
	UINT g_current_test;
	UINT g_sleep_current_test;
	UINT g_backlight_current_test;
	UINT g_normal_current_max;
	UINT g_normal_current_min;
	UINT g_chargCurrent;
	UINT g_sleepCurrent;
	UINT g_backlight_Current;
	UINT g_startcheck_times;
	UINT g_sleep_waittime;

	UINT g_currentcom_port;
	UINT g_currentcom_baud;

	UINT g_usb_timeout;
	UINT g_record_time;//(1-4s)
	UINT g_recode_passdb;

	UINT g_wifi_timeout;
	UINT g_wifi_speed;
	CString g_wifi_ip;
	CString g_wifi_ssid;
	CString g_wifi_passwd;
	CString g_bluetooth_board_mac;
};

struct bp_current_by_option_str{
	UINT g_currentcom_port;
	UINT g_currentcom_baud;

	UINT g_sleep_current_max;
	UINT g_sleep_current_min;
};

struct total_machine_by_option_str{
	UINT g_connect_type;//1usb,2wifi
	UINT g_usb_timeout;
	UINT g_cali_min_value;
	UINT g_cali_max_value;
	UINT g_ts_err_times;
	UINT g_ts_pix_off;
	UINT g_record_time_sec;
	UINT g_record_passdb;
	UINT g_record_subdb;
	UINT g_play_time_sec;
	UINT g_play_passdb;
	UINT g_wifi_timeout;
	UINT g_wifi_speed;
	CString g_wifi_ip;
	CString g_wifi_ssid;
	CString g_wifi_passwd;
	UINT g_wifihot_adb_timeout;
	UINT g_wifihot_timeout;
	CString g_wifihot_ip;
	CString g_wifihot_ssid;
	CString g_wifihot_passwd;
	CString g_local_net_name;
	CString g_local_ip;
	UINT g_pass_bat_power;
	CString g_bluetooth_board_mac;
};

struct air_by_option_str{
	UINT g_connect_type;//1usb,2wifi
	UINT g_usb_timeout;
	UINT g_record_time_sec;
	UINT g_record_subdb;
	UINT g_wifihot_adb_timeout;
	UINT g_wifihot_timeout;
	CString g_wifihot_ip;
	CString g_wifihot_passwd;
	CString g_local_net_name;
	CString g_local_ip;
};

struct batpower_by_option_str{
	UINT g_power_percent;

	UINT g_currentcom_port;
	UINT g_currentcom_baud;

	UINT g_current_test;
	UINT g_normal_current_max;
	UINT g_normal_current_min;
	UINT g_chargCurrent;
	UINT g_startcheck_times;

	UINT g_if_setup_reset;
	UINT g_usb_timeout;
};

struct tools_option_str{
	CString g_by_get_left_filename;
	CString g_by_get_right_filename;
	CString g_by_savfilename;
};

struct burn_sz_option_str{
	CString	g_wrFirm_folder;
	UINT g_if_burn_qcc;
	UINT g_if_erase_flash;
	UINT g_if_burn_param;
	UINT g_if_test_after_burn;
	UINT g_if_burn_apollo;
	UINT g_if_burn_box;
	UINT g_if_read_freq;
	UINT g_readfreq_waittime;
	UINT g_if_write_freq;
	CString g_sJLinkPath;
	CString	g_hw_version;
	CString	g_usbdbg_num;
};

struct check_sz_option_str{
	UINT g_glob_if_check_init;
	UINT g_glob_if_check_light;
	UINT g_glob_if_check_tap;
	UINT g_glob_if_check_plc;
	UINT g_glob_if_check_apollo;
	UINT g_glob_if_check_record;
	UINT g_glob_if_check_play;
	UINT g_glob_if_check_reboot;

	//init ²âÊÔ²ÎÊý
	UINT g_if_check_light;
	UINT g_if_check_tap;
	UINT g_if_check_plc;
	UINT g_if_check_apollo;
	UINT g_if_check_bonemic;

	UINT g_check_timeout_s;
	UINT g_check_light_addps_value;

	UINT g_record_time_sec;
	UINT g_record_passdb;
	UINT g_record_subdb;
	UINT g_play_time_sec;
	UINT g_play_passdb;

	UINT g_wait_reboot_time_sec;
	UINT g_wait_key_time_sec;
};

struct btaddr_sz_option_str{
	CString	g_hw_version;
	CString	g_bt_name;
	UINT rl_flag;
	//CString	g_station_name;
	UINT g_if_locallic;
};

struct checkbox_sz_option_str{
	UINT g_if_check_wdt;
	UINT g_if_check_max20340;
	UINT g_if_check_max77813;
	UINT g_if_check_plc_commu;
	UINT g_if_check_plc_commu_right;
	UINT g_if_check_hal;
	UINT g_if_check_hal_after_plc;
	UINT g_if_check_key;
	UINT g_if_check_usb;
	UINT g_if_check_cal_vol;
};

struct burnsn_sz_option_str{
	UINT g_burn_sn_len;
	CString g_l_sn_prompt;
	CString g_r_sn_prompt;
	CString g_c_sn_prompt;
	UINT g_if_repair_all;
};

struct glob_param{
	CString sFName;//ini file path
	CString last_sel_soft;
	struct general_option_str general_option_instance;
	struct burncheck_option_str burncheck_option_instance;
	struct bluetooth_option_str bluetooth_option_instance;
	struct batpower_option_str batpower_option_instance;
	struct charge_option_str charge_option_instance;
	struct burn_by_option_str burn_by_option_instance;
	struct check_by_option_str check_by_option_instance;
	struct bp_current_by_option_str bp_current_by_option_instance;
	struct total_machine_by_option_str total_machine_by_option_instance;
	struct air_by_option_str air_by_option_instance;
	struct batpower_by_option_str batpower_by_option_instance;
	struct tools_option_str tools_option_instance;
	struct burn_sz_option_str burn_sz_option_instance;
	struct check_sz_option_str check_sz_option_instance;
	struct btaddr_sz_option_str btaddr_sz_option_instance;
	struct checkbox_sz_option_str checkbox_sz_option_instance;
	struct burnsn_sz_option_str burnsn_sz_option_instance;
};
