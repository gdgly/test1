#ifndef LIBUSB_DEVLISTENER_H
#define LIBUSB_DEVLISTENER_H

#include "libusb.h"
#include "../../ddr_base_param.h"
#include "../../glob_param.h"
#include "../../CurlCtrl.h"
//#include "libusb_legacy_protocol.h"
struct msg_info{
	int msg_type;
	int data1;//分区
	int data2;//百分比
};
enum {
	MSG_BY_T_STRING=0, 
	MSG_BY_T_PROGRESS, 
	MSG_BY_T_BTN_START, 
	MSG_BY_T_SMES_SEND,
	MSG_BY_T_EMES_SEND, 
	MSG_BY_T_EMES_ERR, 
	MSG_BY_T_ALL_SUSS, 
};
enum {
	MSG_T_STRING_GETCPU=0,
	MSG_T_STRING_GLOB_INFO,
	MSG_T_STRING_OPEN_SPL,
	MSG_T_STRING_RUN_SPL,
	MSG_T_STRING_GETCPU2,
	MSG_T_STRING_OPEN_UBOOT,
	MSG_T_STRING_RUN_UBOOT,
	MSG_T_STRING_SEND_CMD,
	MSG_T_STRING_GET_FLASH_INFO,
	MSG_T_STRING_SEND_CMD2,
	MSG_T_STRING_BURN_INIT,
	MSG_T_STRING_CRC_ERR,
	MSG_T_STRING_EIO_ERR,
	MSG_T_STRING_PERM_ERR,
	MSG_T_STRING_DISCONNECT,
};

enum {
	MSG_T_STRING_SUCCESS=0,
	MSG_T_STRING_ERR,
};
enum {
	MSG_T_PROGRESS_INIT=0,
	MSG_T_PROGRESS_PART1,
	MSG_T_PROGRESS_PART2,
	MSG_T_PROGRESS_PART3,
	MSG_T_PROGRESS_PART4,
	MSG_T_PROGRESS_PART5,
	MSG_T_PROGRESS_PART6,
	MSG_T_PROGRESS_PART7,
	MSG_T_PROGRESS_PART8,
};
struct sn_flash {
    uint32_t mask;
    char svn_num[20] ;       /* Image Header Magic Number    */
    uint32_t svn_crc32;
    void *reverse;
};
struct solt {
	char path[128];
	//libusb_legacy_protocol *protocol;
};


class libusbDevListener //: public QObject,public IDeviceListener
{
	//Q_OBJECT
public:
	int start_thread();
	int stop_thread();
	//int registerNotify(IEventManager &obj);
	void add_device(char *dbcc_name);
	void del_device(char *dbcc_name);

	CCurlCtrl     mesInfo;
	struct general_option_str general_option_instance;
	struct burn_by_option_str burn_by_option_instance;
	volatile int start_burn;
	unsigned char sn_num[20];
	char dbcc_name[500];
	volatile HWND hwnd;
	static int WorkerThreadProc(void *param);
public:
	explicit libusbDevListener(/*QObject *parent = 0*/);
	virtual ~libusbDevListener();
	enum {
		MAX_SOLTS=32
	};

private:
	//QMutex mutex;
	int count;
	int get_solt_number(char *dbcc_name,bool add_flag);
	struct solt solts[MAX_SOLTS];
};

#endif // LIBUSB_DEVLISTENER_H
