#ifndef LIBUSB_LEGACY_PROTOCOL_H
#define LIBUSB_LEGACY_PROTOCOL_H
#include "stdint.h"
#include "libusb/win32/libusb.h"

#if 0
enum _WRITE_OPS {
	ST_MEMORY = 0x0<<16|0x0,
	ST_NAND_RAW = 0x1<<16|0x0,
	ST_NAND_IMAGE,
	ST_NAND_OOB,
	ST_MMC = 0x2<<16|0x0
};

enum _BURNER_REQUEST
{
	VR_GET_CPU_INFO = 0,
	VR_SET_DATA_ADDRESS,
	VR_SET_DATA_LENGTH,
	VR_FLUSH_CACHES,
	VR_PROGRAM_START1,
	VR_PROGRAM_START2,
	VR_GET_ACK = 0x10,
	VR_CONTROL,
	VR_WRITE,
	VR_READ
};
#endif

class libusb_legacy_protocol //: public QObject,public IProtocol
{
//	Q_OBJECT
public:
	explicit libusb_legacy_protocol(char *dbcc_name);//(char *dbcc_name,QObject *parent = 0);
	//explicit libusb_legacy_protocol(struct usb_device *dev,QObject *parent = 0);
	~libusb_legacy_protocol();
public:
	unsigned int port_num;
	//void set_owner(QThread *thread);
	bool is_valid();
	int send_stage1_global_info(unsigned char *data,int len,unsigned int offset, bool flag);
	int run_stage1_firmware(unsigned char *data, int len, unsigned int offset, int d2i_len, bool sec_en,bool flag);
	int run_stage2_firmware(unsigned char *data, int len, unsigned int offset, bool sec_en, bool flag);
	int get_cpu_info(char (*buf)[32], int len);
	int stage2_init(int timeout, int delay_time, volatile bool *stop);
	int stage2_write(unsigned char *data,uint32_t data_len,uint64_t partition,
					 uint32_t offset,uint32_t ops,
					 volatile bool *stop,int timeout,int delay_time,bool is_all_erase);
	int stage2_read(unsigned char *data,uint32_t data_len,
					uint64_t partition,uint32_t offset,uint32_t ops,volatile bool *stop);
	int send_cmd_reboot();
	int send_cmd_sync_time();
	int send_cmd_poweroff();
	int stage2_get_cpu_info(char (*buf)[32], int len);
	void enable_transfer_data_check(bool enable);
	void enable_write_back_check(bool enable);
	int send_cmd_config(bool trans_check, bool write_check,unsigned char *data, uint32_t length,volatile bool *stop);
	int stage2_send_file_length(int length);
	int stage2_get_flash_info(uint32_t medium, uint32_t *data);
	int get_flash_info(uint32_t *chipid);
	int send_cmd_bootrom(unsigned int address,unsigned int length,char *data);
	int send_security_start(volatile bool *stop);
	int send_security_en(char *tmp, volatile bool *stop);
	int send_security_rckey(volatile bool *stop);
	int send_security_burn_en(volatile bool *stop);
//signals:
//
//public slots:

private:
	//QThread *thread;
	usb_dev_handle *handle;
private:
	int factor;
	int stage1_flush_stage2_cache();
	int stage1_set_data_address(unsigned int address);
	int stage1_set_data_length(int length);
	int stage1_send_data(unsigned char *data,int length);
	int run_program(unsigned int address,bool is_stage1);
	int get_ack(int timeout,int delay_time,volatile bool *stop);
	int get_ack_and_crc(unsigned *crc,int timeout,volatile bool *stop);
	int stage2_send_cmd_config(unsigned char *data,uint32_t length,volatile bool *stop);
	bool transfer_data_check;
	bool write_back_check;
};

#endif // LIBUSB_LEGACY_PROTOCOL_H
