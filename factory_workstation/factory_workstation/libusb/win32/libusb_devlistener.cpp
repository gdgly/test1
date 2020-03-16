#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <setupapi.h>
#include <dbt.h>
#include <winioctl.h>
#include <strsafe.h>
#include <time.h>
#include <initguid.h>
#include <string>
#include "libusb_devlistener.h"
#include "libusb.h"
#include "../../libusb_legacy_protocol.h"
#pragma warning(disable:4200)

#define MAX_LOADSTRING 100

DEFINE_GUID(GUID_DEVINTERFACE_USB1_DEVICE, 0xA5DCBF10L,   0x6530,   0x11D2,   0x90,   0x1F,   0x00,   0xC0,   0x4F,   0xB9,   0x51,   0xED);
#define   GUID_CLASS_USB_DEVICE                       GUID_DEVINTERFACE_USB_DEVICE1

libusbDevListener* libusb_ls_pointer = NULL;

HWND hWnd = NULL;
HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING] = TEXT ("DevCaps1") ;
HDEVNOTIFY hDevNotify;

BOOL InitNotification();

ATOM				DevListenerRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
int                 start_win32api_dev_listener(void);

struct s_vpid {
	int vendorId;
	int productId;
};

libusbDevListener::libusbDevListener()//(QObject *parent) : QObject(parent)
{
	count = 0;
	libusb_ls_pointer = this;

	memset(solts,0,sizeof(solts));
	start_win32api_dev_listener();
	start_thread();
}

libusbDevListener::~libusbDevListener()
{
}

int libusbDevListener::get_solt_number(char *dbcc_name,bool add_flag)
{
	for(int i=0;i<count;i++)
		if(!strcmp(&dbcc_name[25],&solts[i].path[25]))
			return i;

	if(!add_flag)
		return -1;
	//strcpy(solts[count].path,dbcc_name);
	strcpy_s(solts[count].path,dbcc_name);
	return count++;
}

struct jz_gpio_func_def {
	uint32_t port;
	uint32_t func;
	uint32_t pins;
};
struct gpio_param_info {
	struct jz_gpio_func_def uart_gpio[1];
	uint32_t nr_gpio_func;
	struct jz_gpio_func_def gpio[12];
};
struct global_info {
	uint32_t extal;
	uint32_t cpufreq;
	uint32_t ddrfreq;
	uint32_t ddr_div;
	uint32_t uart_base;
	uint32_t baud_rate;
};
struct ParameterInfo{
	uint32_t magic;
	uint32_t size;
	uint32_t data[0];
};

volatile msg_info msg_send;
int libusbDevListener::WorkerThreadProc(void *param)
{
	libusbDevListener *p = (libusbDevListener*)param;
	//::PostMessage(p->hwnd, 0x400+33, 0, NULL);
	char *dbcc_name = p->dbcc_name;
	if(p->start_burn)
		p->add_device(dbcc_name);
	p->start_burn = false;
	return 0;
}
void libusbDevListener::add_device(char *dbcc_name)
{
	char buf[32];
	static unsigned int device_id = 0;
	CCurlCtrl *mesptr = &mesInfo;
	msg_info *msg_p;

	TRACE("%s",dbcc_name);
	libusb_legacy_protocol libusb_legacy_protocol(dbcc_name);
	msg_p = new msg_info;
	msg_p->msg_type = MSG_BY_T_BTN_START;
	::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
	if(general_option_instance.g_check_mes  == 1){
		Sleep(5);
		if(general_option_instance.g_check_mes_type == 1){
			mesptr->clear_flag();
			mesptr->g_mes_ip = general_option_instance.g_mes_ip;
			mesptr->g_mes_port = general_option_instance.g_mes_port;
			if(mesptr->SendBurns((LPCSTR)sn_num) == 0){//上传MES流程检查接口   烧写模式第一个mes上传
			}else{
				AfxMessageBox("mes连不上服务器! ");
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_EMES_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				return;
			}
			if(mesptr->Messtatus == 1){
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_SMES_SEND;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
			}else{
				AfxMessageBox("机器已过站! ");
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_EMES_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				return;
			}
		}
	}
	Sleep(5);
	if(libusb_legacy_protocol.get_cpu_info(&buf, 8)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_GETCPU;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		return;
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 10;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);		
	}

	struct ParameterInfo *infoparam;
	struct ParameterInfo *gpioparam;
	struct ParameterInfo *ddrparam;

	infoparam = (ParameterInfo*)malloc(sizeof(ParameterInfo)+sizeof(global_info));
	memset(infoparam, 0, sizeof(ParameterInfo)+sizeof(global_info));
	global_info* pgb = (global_info*)infoparam->data;
	pgb->extal = 24000000;
	pgb->cpufreq = 600000000;
	pgb->ddrfreq = 200000000;
	pgb->uart_base = 2;
	pgb->baud_rate = 115200;
	infoparam->size = sizeof(global_info) + sizeof(gpio_param_info);//info 和 gpio的size写到一起了
	infoparam->magic = ('B' << 24) | ('D' << 16) | ('I' << 8) | ('F' << 0);

	gpioparam = (ParameterInfo*)malloc(sizeof(ParameterInfo)+sizeof(gpio_param_info));
	memset(gpioparam, 0, sizeof(ParameterInfo)+sizeof(gpio_param_info));
	gpio_param_info *gp = (gpio_param_info*)gpioparam->data;
	gp->uart_gpio[0].port = 0x2;
	gp->uart_gpio[0].func = 0x11;
	gp->uart_gpio[0].pins = 0x80000000;
	gp->gpio[0].port = 0x0;
	gp->gpio[0].func = 0x11;
	gp->gpio[0].pins = 0xfc000000;
	gp->nr_gpio_func = 1;
	gpioparam->size = sizeof(gpio_param_info);//未使用
	gpioparam->magic = ('G' <<24) | ('P' <<16) |('I' <<8) |('O' << 0);//未使用

	ddrparam = (ParameterInfo*)malloc(sizeof(ParameterInfo)+sizeof(ddr_registers));
	memset(ddrparam, 0, sizeof(ParameterInfo)+sizeof(ddr_registers));
	ddr_registers *ddr_registers_param = NULL;
	ddr_registers_param = (ddr_registers*)ddrparam->data;
	DDRBaseParam DDRBaseParam_ins;
	DDRBaseParam_ins.ddr_fill_param();
	DDRBaseParam_ins.ddr_generate_register();
	DDRBaseParam_ins.ddr_convert_param(ddr_registers_param);
	ddrparam->size = sizeof(struct ddr_registers);
	ddrparam->magic = ('D' << 24) | ('D' << 16) | ('R' << 8) | 0;

	int g_size = 2*sizeof(ParameterInfo) + sizeof(global_info) + sizeof(gpio_param_info) + sizeof(ddr_registers);
	char* g_buf = (char*)malloc(g_size);
	memcpy(g_buf, infoparam, 
		sizeof(ParameterInfo) + sizeof(global_info));
	memcpy(g_buf + sizeof(ParameterInfo) + sizeof(global_info), gpioparam->data, 
		sizeof(gpio_param_info));
	memcpy(g_buf + sizeof(ParameterInfo) + sizeof(global_info) + sizeof(gpio_param_info), ddrparam, 
		sizeof(ParameterInfo) + sizeof(ddr_registers));
	if( libusb_legacy_protocol.send_stage1_global_info(
		(unsigned char*)g_buf, g_size, 0xf4001000, FALSE )){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_GLOB_INFO;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		free(g_buf);
		free(infoparam);
		free(gpioparam);
		free(ddrparam);
		return;
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 20;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);		
	}
	free(g_buf);
	free(infoparam);
	free(gpioparam);
	free(ddrparam);

	CFile m_hLogFile;
	CString sFname;

	char* spl_g_buf = NULL;
	sFname.Format("firmware/burn/spl_lpddr.bin");
	if(m_hLogFile.Open(sFname, CFile::modeRead) == TRUE) {
		m_hLogFile.SeekToBegin();
		g_size = (int)m_hLogFile.GetLength();
		spl_g_buf = (char*)malloc(g_size);
		m_hLogFile.Read(spl_g_buf,g_size);
		m_hLogFile.Close();
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_OPEN_SPL;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);			
		return;
	}
	if(libusb_legacy_protocol.run_stage1_firmware((unsigned char*)spl_g_buf,
		g_size, 0xf4001800, 0, FALSE, FALSE)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_RUN_SPL;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
		free(spl_g_buf);
		return;
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 40;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);			
	}
	free(spl_g_buf);

	Sleep(1100);
	memset(buf,0,8);
	if(libusb_legacy_protocol.get_cpu_info(&buf, 8)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_GETCPU2;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		return;
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 50;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);		
	}

	sFname.Format("firmware/burn/uboot.bin");
	if(m_hLogFile.Open(sFname, CFile::modeRead) == TRUE) {
		m_hLogFile.SeekToBegin();
		g_size = (int)m_hLogFile.GetLength();
		g_buf = (char*)malloc(g_size);
		m_hLogFile.Read(g_buf,g_size);
		m_hLogFile.Close();
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_OPEN_UBOOT;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);		
		return;
	}
	if(libusb_legacy_protocol.run_stage2_firmware((unsigned char*)g_buf,
		g_size, 0x80100000, FALSE, FALSE)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_RUN_UBOOT;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		free(g_buf);
		return;	
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 60;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);				
	}
	free(g_buf);


struct policy_param_info{
	uint32_t use_nand_mgr;
	uint32_t use_nand_mtd;
	uint32_t use_mmc;
	uint32_t use_sfc_nor;
	uint32_t use_sfc_nand;
	uint32_t offsets[32];
};
	ParameterInfo *policyparam;
	policyparam = (ParameterInfo*)malloc(sizeof(ParameterInfo)+sizeof(policy_param_info) + 8);
	memset(policyparam, 0, sizeof(ParameterInfo)+sizeof(policy_param_info));
	policyparam->magic = ('P' << 24) | ('O' << 16) | ('L' << 8) |('I' << 0);
	policyparam->size = sizeof(ParameterInfo)+sizeof(policy_param_info);
	policy_param_info policyinfo;
	memset(&policyinfo, 0, sizeof(policy_param_info));
	policyinfo.use_sfc_nand = 1;
	memcpy(policyparam->data, &policyinfo, sizeof(policy_param_info));

	volatile bool stop = FALSE;
	if(libusb_legacy_protocol.send_cmd_config(1,1,(unsigned char*)policyparam,policyparam->size+8,&stop)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_SEND_CMD;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		free(policyparam);
		return;			
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 75;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);			
	}

	uint32_t flash_chipid = 0;
	if(libusb_legacy_protocol.get_flash_info(&flash_chipid)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_GET_FLASH_INFO;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		return;			
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 80;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);			
	}

struct spi_param_info{
	uint32_t download_params;
	uint32_t quad_mode;
	uint32_t spi_erase_block_size;
	uint32_t spi_erase;
};
struct jz_spinand_partition {
	char name[32];         /* identifier string */
	uint32_t size;          /* partition size */
	uint32_t offset;        /* offset within the master MTD space */
	uint32_t mask_flags;       /* master MTD flags to mask out for this partition */
	uint32_t manager_mode;         /* manager_mode mtd or ubi */
};
struct jz_sfc_nand_burner_param {
	unsigned int magic_num;
	int partition_num;
	struct jz_spinand_partition partition[0];
};
#define NAND_MAGIC						0x646e616e
enum partition_manager_mode{
	MTD_MODE = 0,
	UBI_MANAGER,
	MTD_D_MODE,
};
	int bufsize = 0;
	int partcount=8;
	jz_spinand_partition partition_ins[8];
	memcpy(partition_ins, &burn_by_option_instance.spinand_partition_ins, sizeof(jz_spinand_partition)*8);
	//jz_spinand_partition partition_ins[8]={
	//	{"uboot",		0x100000,	0x0,		0, MTD_MODE},
	//	{"logo",		0x100000,	0x100000,	0, MTD_MODE},
	//	{"kernel",		0x500000,	0x200000,	0, MTD_MODE},
	//	{"recovery",	0xf00000,	0x700000,	0, MTD_MODE},
	//	{"rootfs",		0x3200000,	0x1600000,	0, UBI_MANAGER},
	//	{"sn",			0x200000,	0x4800000,	0, MTD_MODE},
	//	{"data",		0x800000,	0x4a00000,	0, UBI_MANAGER},
	//	{"usrfs",		0x2e00000,	0x5200000,	0, UBI_MANAGER},//0xae00000 for 256m flash
	//};
	int paramsize = sizeof(jz_sfc_nand_burner_param) + sizeof(jz_spinand_partition)*partcount;

	bufsize = sizeof(ParameterInfo) + sizeof(spi_param_info) + paramsize;
	//ParameterInfo * sfcparam = (ParameterInfo*)realloc(sfcparam, bufsize);
	ParameterInfo * sfcparam = (ParameterInfo*)malloc(bufsize);
	memset(sfcparam, 0, bufsize);
	spi_param_info *sfcinfo = (spi_param_info*)sfcparam->data;
	sfcinfo->download_params = 1;
	sfcinfo->quad_mode = 1;
	sfcinfo->spi_erase_block_size = 65536;
	sfcinfo->spi_erase = 1;

	//bufsize += paramsize;
	//sfcparam = (ParameterInfo*)realloc(sfcparam, bufsize);
	char* pc=(char*)(sfcparam->data) + sizeof(spi_param_info);
	memset(pc, 0, paramsize);

	struct jz_sfc_nand_burner_param *burner_param = (jz_sfc_nand_burner_param *)pc;
	burner_param->magic_num = NAND_MAGIC;
	burner_param->partition_num = partcount;
	jz_spinand_partition *partition = burner_param->partition;
	memcpy(partition, partition_ins, sizeof(jz_spinand_partition) * partcount);

	sfcparam->size = bufsize - sizeof(ParameterInfo);
	sfcparam->magic = ('S' << 24) | ('F' << 16) | ('C' << 8) | 0;

struct debug_param_info{
	uint32_t efuse_gpio;
	uint32_t log_enabled;
	uint32_t transfer_data_chk;
	uint32_t write_back_chk;
	uint32_t transfer_size;
	uint32_t stage2_timeout;
};
	struct ParameterInfo *dbgparam;
	dbgparam = (struct ParameterInfo *)malloc(sizeof(ParameterInfo)+sizeof(debug_param_info));
	memset(dbgparam, 0, sizeof(ParameterInfo)+sizeof(debug_param_info));
	debug_param_info* dbg_info = (debug_param_info*)dbgparam->data;
	dbg_info->efuse_gpio = -1;
	dbg_info->log_enabled = 1;
	dbg_info->transfer_data_chk = 1;
	dbg_info->write_back_chk = 1;
	dbgparam->size = sizeof(debug_param_info);
	dbgparam->magic = ('D' << 24) | ('B' << 16) | ('G' << 8) | 0;

	g_buf = (char*)malloc(dbgparam->size+8 + policyparam->size+8 + sfcparam->size+8);
	memcpy(g_buf, dbgparam, dbgparam->size+8);
	memcpy(g_buf+dbgparam->size+8, policyparam, policyparam->size+8);
	memcpy(g_buf+dbgparam->size+8+policyparam->size+8, sfcparam, sfcparam->size+8);

	if(libusb_legacy_protocol.send_cmd_config(1,1,(unsigned char*)g_buf,
		dbgparam->size+8 + policyparam->size+8 + sfcparam->size+8,&stop)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_SEND_CMD2;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		free(sfcparam);
		free(dbgparam);
		free(policyparam);
		free(g_buf);
		return;		
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 85;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);			
	}
	free(sfcparam);
	free(dbgparam);
	free(policyparam);
	free(g_buf);

	if(libusb_legacy_protocol.stage2_init(50,0,&stop)){
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_STRING;
		msg_p->data1 = MSG_T_STRING_BURN_INIT;
		msg_p->data2 = MSG_T_STRING_ERR;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
		return;	
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_PROGRESS;
		msg_p->data1 = MSG_T_PROGRESS_INIT;
		msg_p->data2 = 100;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);			
	}
	Sleep(20);

	int transfer_size = 65536;
	g_buf = (char*)malloc(transfer_size);
	uint64_t partition_off;
	int times,i;
	for(times=0; times<8; times++){
		if( times==0 ){//将uboot与recovery烧写顺序对换一下，防止只烧uboot后系统不能烧写
			i=3;
		}else if( times==3 ){//将uboot与recovery烧写顺序对换一下，防止只烧uboot后系统不能烧写
			i=0;
		}else{
			i=times;
		}
		switch (i){
			case 0:
				sFname.Format("\\u-boot-with-spl.bin");
				//msg_send.data1 = MSG_T_PROGRESS_PART1;
				break;
			case 1:
				sFname.Format("\\logo");
				//msg_send.data1 = MSG_T_PROGRESS_PART2;
				break;
			case 2:
				sFname.Format("\\uImage");
				//msg_send.data1 = MSG_T_PROGRESS_PART3;
				break;
			case 3:
				sFname.Format("\\recovery");
				//msg_send.data1 = MSG_T_PROGRESS_PART4;
				break;
			case 4:
				sFname.Format("\\rootfs.ubifs");
				//msg_send.data1 = MSG_T_PROGRESS_PART5;
				break;
			case 5:
				//sFname.Format("\\sn");
				//msg_send.data1 = MSG_T_PROGRESS_PART6;
				break;
			case 6:
				sFname.Format("\\config.ubifs");
				//msg_send.data1 = MSG_T_PROGRESS_PART7;
				break;
			case 7:
				sFname.Format("\\usrfs.ubifs");
				//msg_send.data1 = MSG_T_PROGRESS_PART8;
				break;
			default:
				break;
		}
		sFname = burn_by_option_instance.g_wrFirm_folder + sFname;
		partition_off = burn_by_option_instance.spinand_partition_ins[i].offset;
		
		if(m_hLogFile.Open(sFname, CFile::modeRead) == TRUE && i!=5) {
			m_hLogFile.SeekToBegin();
			g_size = (int)m_hLogFile.GetLength();
		}else{
			if(i==5){//sn号不读文件
				g_size = sizeof(sn_flash);
			}else{
				continue;
			}
		}
		libusb_legacy_protocol.stage2_send_file_length(g_size);
		int ret;
		for(uint64_t j=0,ar=0; j<g_size; j+=ar){
			int retry = 5;
			if(i==5){
extern unsigned int local_crc32(unsigned int crc,unsigned char *buffer, unsigned int size);
				//memset(sn_num, 0, 20);
				//memcpy(sn_num, "B02A2D185102008005", 18);
				sn_flash sn_stru;
				memcpy(sn_stru.svn_num, sn_num, 20);
				sn_stru.svn_crc32 = local_crc32(0xffffffff,sn_num,20);
				sn_stru.mask = 0x5a5a5a5a;
				ar = sizeof(sn_flash);
				memcpy(g_buf, &sn_stru, sizeof(sn_flash));
			}else{
				ar = m_hLogFile.Read(g_buf,transfer_size);
			}
			do{
				ret = libusb_legacy_protocol.stage2_write((unsigned char*)g_buf, (uint32_t)ar, partition_off, (uint32_t)j, (6 << 16) | (1 & 0xffff),
											&stop, 50, 0, 1);
				if(ret) {
					printf("(%d)stage2_write error(%d), we will retry in 500 ms\n",libusb_legacy_protocol.port_num,ret);
					Sleep(500);
				}
			}while(retry-- && ret);
			if(ret == -EINVAL) {
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_STRING;
				msg_p->data1 = MSG_T_STRING_CRC_ERR;
				msg_p->data2 = MSG_T_STRING_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				m_hLogFile.Close();
				goto policy_faild;
			} else if(ret == -EIO) {
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_STRING;
				msg_p->data1 = MSG_T_STRING_EIO_ERR;
				msg_p->data2 = MSG_T_STRING_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
				m_hLogFile.Close();
				goto policy_faild;
			} else if(ret == -EPERM) {
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_STRING;
				msg_p->data1 = MSG_T_STRING_PERM_ERR;
				msg_p->data2 = MSG_T_STRING_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				m_hLogFile.Close();
				goto policy_faild;
			} else if(ret != 0) {
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_STRING;
				msg_p->data1 = MSG_T_STRING_DISCONNECT;
				msg_p->data2 = MSG_T_STRING_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
				m_hLogFile.Close();
				goto policy_faild;
			}
			msg_p = new msg_info;
			msg_p->msg_type = MSG_BY_T_PROGRESS;
			msg_p->data1 = i + MSG_T_PROGRESS_PART1;
			uint64_t pro_size = (j+ar)*100/g_size;
			msg_p->data2 = (int)pro_size;
			::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);		
		}
		if(i!=5)
			m_hLogFile.Close();
		Sleep(20);
	}
	Sleep(20);
	if(general_option_instance.g_check_mes  == 1){
		if(general_option_instance.g_check_mes_type == 1){
			if(mesptr->SendBurne((LPCSTR)sn_num) == 0){            //数据上报接口    烧录第二次MES上传
			}else{
				AfxMessageBox("mes连不上服务器! ");
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_EMES_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);		
			}
			if(mesptr->Messtatus == 3){
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_ALL_SUSS;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
			}else{
				AfxMessageBox("机器已过站! ");
				msg_p = new msg_info;
				msg_p->msg_type = MSG_BY_T_EMES_ERR;
				::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);
				goto policy_faild;
			}
		}else if(general_option_instance.g_check_mes_type == 2){
			struct ch_mes_data ch_mes_data; 
			ch_mes_data.data_flag = 1;
			mesptr->changh_mes_int(sn_num, ch_mes_data);
		}
	}else{
		msg_p = new msg_info;
		msg_p->msg_type = MSG_BY_T_ALL_SUSS;
		::PostMessage(hwnd, WM_USERMSG_MASSPRODUCT, 0, (LPARAM)msg_p);	
	}

policy_faild:
	free(g_buf);
}

void libusbDevListener::del_device(char *dbcc_name)
{

}

int libusbDevListener::stop_thread()
{
	UnregisterDeviceNotification(hDevNotify);
	return 0;
}

int libusbDevListener::start_thread()
{
	//if(this->obj == NULL)
	//	return -1;
	return InitNotification();
}

int start_win32api_dev_listener(void)
{
	//MSG msg;
	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	if (!DevListenerRegisterClass(hInstance)) {
		int nError;
		nError = GetLastError();
		printf("RegisterClass failed %d!\n", nError);
		return FALSE;
	}

	if (!InitInstance (hInstance, SW_HIDE)) {
		return FALSE;
	}

	return TRUE;
}

ATOM DevListenerRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS    wndclass;

	wndclass.style      = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc= WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance  = hInstance;
	wndclass.hIcon      = LoadIcon (NULL, IDI_APPLICATION);
	wndclass.hCursor    = LoadCursor (NULL, IDC_ARROW);
	wndclass.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wndclass.lpszMenuName   = NULL;
	wndclass.lpszClassName  = szWindowClass;

	return RegisterClass (&wndclass);
}

BOOL InitNotification()
{
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;

	ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
	NotificationFilter.dbcc_size = sizeof(NotificationFilter);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	NotificationFilter.dbcc_reserved = 0;
	NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB1_DEVICE;

	hDevNotify = RegisterDeviceNotification(hWnd, &NotificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
	if( !hDevNotify ) {
		int nError;
		nError = GetLastError();
		printf("Register device notification failed %d!\n", nError);
		return FALSE;
	}

	return TRUE;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	if(hWnd == NULL) {
		hWnd = CreateWindow(szWindowClass, szTitle, WS_EX_TRANSPARENT,
							CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	}

	if (!hWnd) {
		int nError;
		nError = GetLastError();
		printf("Create window failed %d!\n", nError);
		return FALSE;
	}

	return TRUE;
}

char* CStrToStr1(char *pcstr,const wchar_t *pwstr, size_t len)
{
	int nlength=wcslen(pwstr);
	int nbytes = WideCharToMultiByte( 0, 0, pwstr, nlength, NULL, 0, NULL, NULL);
	if(nbytes>(int)len)
		nbytes=len;
	WideCharToMultiByte( 0, 0, pwstr, nlength, pcstr, nbytes, NULL, NULL);

	return pcstr;
}

void UpdateDevice(PDEV_BROADCAST_DEVICEINTERFACE pDevInf, WPARAM wParam)
{
	bool isSupportDevice = false;

	if (!libusb_ls_pointer) {
		printf("libusb_ls_pointer is NULL!\n");
		return;
	}
	//buf_send[0] = 1;
	//buf_send[2] = 2;
	//buf_send[3] = 3;
	//::SendMessage(libusb_ls_pointer->hwnd, 0x400+33, 0, (LPARAM)buf_send);

	CString usb_device_name;
	int vid, pid;
	usb_device_name.Format("%s",pDevInf->dbcc_name);
	CString vendorid = usb_device_name.Mid(12,4);
	vendorid = "0x" + vendorid;
	CString productid = usb_device_name.Mid(21,4);
	productid = "0x" + productid;
	StrToIntEx(vendorid, STIF_SUPPORT_HEX, (int*)&vid);
	StrToIntEx(productid, STIF_SUPPORT_HEX, (int*)&pid);
	if(vid == 0xA108 && pid == 0x1000){//判断是否是x1000的usb烧写设备
		isSupportDevice = true;
	}
	
	if(!isSupportDevice) {
		if(DBT_DEVICEARRIVAL == wParam) {
			TRACE("can't find any USBCloner device!");
			TRACE("dbcc_name:%s",pDevInf->dbcc_name);
		}
		return;
	}

	if ( DBT_DEVICEARRIVAL == wParam ) {
		DWORD idThread;
		HANDLE        hThread;
		memset(libusb_ls_pointer->dbcc_name, 0, sizeof(libusb_ls_pointer->dbcc_name));
		memcpy(libusb_ls_pointer->dbcc_name, pDevInf->dbcc_name, strlen(pDevInf->dbcc_name));
		hThread   = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)libusbDevListener::WorkerThreadProc, libusb_ls_pointer, 0, &idThread);
		//libusb_ls_pointer->add_device(pDevInf->dbcc_name);
	} else {
		libusb_ls_pointer->del_device(pDevInf->dbcc_name);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(message == WM_DEVICECHANGE) {
		switch (wParam)
		{
		case DBT_DEVICEARRIVAL:
		case DBT_DEVICEREMOVECOMPLETE:
			//TRACE("lalalalala");
			UpdateDevice((PDEV_BROADCAST_DEVICEINTERFACE)lParam, wParam);
		}
		//TRACE("lalalalala %d\n", wParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
