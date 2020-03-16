#include "stdafx.h"
#include "libusb_legacy_protocol.h"
//#include "global_info.h"
#include "errno.h"
#include <time.h>


//#define CPU_INFO_LENGTH 8
#define FLASH_INFO_LENGTH 3

#define STAGE_ADDR_MSB(addr) ((addr) >> 16)
#define STAGE_ADDR_LSB(addr) ((addr) & 0xffff)

#define INGENIC_OUT_ENDPOINT    (1 | USB_ENDPOINT_OUT)
#define INGENIC_IN_ENDPOINT     (1 | USB_ENDPOINT_IN)

/*bootrom stage request*/
#define VR_GET_CPU_INFO         0x00
#define VR_SET_DATA_ADDR        0x01
#define VR_SET_DATA_LEN         0x02
#define VR_FLUSH_CACHE          0x03
#define VR_PROG_STAGE1          0x04
#define VR_PROG_STAGE2          0x05
/*firmware stage request*/
#define VR_GET_ACK					0x10
#define VR_INIT							0x11
#define VR_WRITE						0x12
#define VR_READ						0x13
#define VR_UPDATE_CFG			0x14
#define VR_SYNC_TIME            0x15
#define VR_REBOOT					0x16
#define VR_POWER_OFF           0x17
#define VR_GET_CRC					0x19

#define VR_SEC_INIT										0x20
#define VR_SEC_BURN_RKCK						0x21
#define VR_SEC_SED_NKU							0x22
#define VR_SEC_GET_CK_LEN						0x23
#define VR_SEC_BURN_SECBOOT_EN         0x24
#define VR_SEC_SEDEN								0x25

#define VR_GET_FLASH_INFO						0x26

struct uboot_rtc_time {
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

union cmd {
	struct update_arg {
		uint32_t length;
	}update;

	struct write_arg {
		uint64_t partation;
		uint32_t ops;
		uint32_t offset;
		uint32_t length;
		uint32_t crc;
	}write;

	struct read_arg {
		uint64_t partation;
		uint32_t ops;
		uint32_t offset;
		uint32_t length;
	}read;

	struct info_arg{
		uint32_t medium;
	}info;

	struct sec_arg{
		uint32_t security_en;
	}security;

	struct uboot_rtc_time rtc;
};

unsigned int crc_table[] = {
	/* CRC polynomial 0xedb88320 */
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

unsigned int local_crc32(unsigned int crc,unsigned char *buffer, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++) {
		crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
	}
	return crc ;
}
//
//libusb_legacy_protocol::libusb_legacy_protocol(struct usb_device *dev,QObject *parent) :
//	QObject(parent)
//{
//	handle = usb_open(dev);
//	if(!handle)
//		return;
//	factor = 1;
//	usb_claim_interface(handle,0);
//	transfer_data_check = false;
//	write_back_check = false;
//}

libusb_legacy_protocol::libusb_legacy_protocol(char *dbcc_name)//(char *dbcc_name,QObject *parent) :
	//QObject(parent)
{
//#ifdef _WIN32
	handle = usb_open_by_dbcc_name(dbcc_name);
	if(!handle)
		return;
	factor = 1;
	usb_claim_interface(handle,0);
//#endif
}

libusb_legacy_protocol::~libusb_legacy_protocol()
{
	if(handle) {
		usb_release_interface(handle,0);
		usb_close(handle);
	}
}

int libusb_legacy_protocol::send_stage1_global_info(unsigned char *data, int len, unsigned int offset, bool flag)
{
	int ret = 0;
	ret = stage1_set_data_address(offset);
	if(ret)
		return ret;

	if(flag)
	{
		ret = stage1_set_data_length(len);
		if(ret)
			return ret;
	}

	ret = stage1_send_data(data,len);
	if(ret != 0) {
		//qDebug()<<"send_stage1_global_info error, ret = "<<ret;
		TRACE("send_stage1_global_info error, ret = %d",ret);
		return ret;
	}
	return 0;
}

int libusb_legacy_protocol::run_stage1_firmware(unsigned char *data, int len, unsigned int offset, int d2i_len, bool sec_en, bool flag)
{
	TRACE("run_stage1_firmware data=%08x len=%08x offset=%08x \n",data,len,offset);
	int ret;
	if(sec_en)
	{
		ret = stage1_set_data_address(offset-1536);
		TRACE("****************i catch1*******************\n");
	}
	else
		ret = stage1_set_data_address(offset);
	if(ret)
		return ret;

	if(flag)
	{
		ret = stage1_set_data_length(len);
		if(ret)
			return ret;
	}

	ret = stage1_send_data(data,len);
	if(ret != 0) {
		//qDebug()<<"run_stage1_firmware stage1_send_data error, ret = "<<ret;
		TRACE("run_stage1_firmware stage1_send_data error, ret = %d",ret);
		return ret;
	}

	if(d2i_len > 0) {
		if(sec_en)
		{
			ret = stage1_set_data_length(d2i_len+512);
			TRACE("****************i catch2*******************\n");
		}
		else
			ret = stage1_set_data_length(d2i_len);
		if(ret != 0) {
			//qDebug()<<"run_stage1_firmware stage1_set_data_length error, ret = "<<ret;
			TRACE("run_stage1_firmware stage1_set_data_length error, ret = %d",ret);
			return ret;
		}
	}

	return run_program(offset,true);
}

int libusb_legacy_protocol::run_stage2_firmware(unsigned char *data, int len, unsigned int offset, bool sec_en, bool flag)
{
	TRACE("run_stage2_firmware data=%08x len=%08x offset=%08x\n",data,len,offset);
	int ret = 0;
	if (sec_en)
		ret = stage1_set_data_address(offset-2048);
	else
		ret = stage1_set_data_address(offset);
	if(ret)
		return ret;

	if(flag)
	{
		ret = stage1_set_data_length(len);
		if(ret)
			return ret;
	}

	ret = stage1_send_data(data,len);
	if(ret != 0) {
		//qDebug()<<"run_stage2_firmware send data error, ret = "<<ret;
		TRACE("run_stage2_firmware send data error, ret = %d",ret);
		return ret;
	}

	ret = stage1_flush_stage2_cache();
	if(ret != 0) {
		//qDebug()<<"run_stage2_firmware flush cache error, ret = "<<ret;
		TRACE("run_stage2_firmware flush cache error, ret = %d",ret);
		return ret;
	}

	return run_program(offset,false);
}

int libusb_legacy_protocol::stage1_flush_stage2_cache()
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_FLUSH_CACHE,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)stage1_set_data_address error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::stage1_send_data(unsigned char *data,int length)
{
	int status = usb_bulk_write(
				handle,
				/* endpoint       */ INGENIC_OUT_ENDPOINT,
				/* *data          */ (char *)data,
				/* length         */ length,
				/* timeout        */ 2000 * factor);

	if(status != length) {
		TRACE("(%d)stage1_send_data error: (%d)%s\n",this->port_num,status,usb_strerror());
	}

	return length - status;
}

int libusb_legacy_protocol::stage1_set_data_address(unsigned int address)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SET_DATA_ADDR,
				/* wValue        */ STAGE_ADDR_MSB(address),
				/* wIndex        */ STAGE_ADDR_LSB(address),
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)stage1_set_data_address error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::stage2_send_file_length( int lenght)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SET_DATA_LEN,
				/* wValue        */ STAGE_ADDR_MSB(lenght),
				/* wIndex        */ STAGE_ADDR_LSB(lenght),
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)stage1_set_data_address error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::stage1_set_data_length(int length)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SET_DATA_LEN,
				/* wValue        */ STAGE_ADDR_MSB(length),
				/* wIndex        */ STAGE_ADDR_LSB(length),
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)stage1_set_data_length error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::run_program(unsigned int address,bool is_stage1)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ is_stage1?VR_PROG_STAGE1:VR_PROG_STAGE2,
				/* wValue        */ STAGE_ADDR_MSB(address),
				/* wIndex        */ STAGE_ADDR_LSB(address),
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)run_program error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::stage2_send_cmd_config(unsigned char *data, uint32_t length,volatile bool *stop)
{
	union cmd cmd;
	cmd.update.length = length;

	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_UPDATE_CFG,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)&cmd,
				/* wLength       */ sizeof(union cmd),
				5000);

	if (status != sizeof(union cmd)) {
		TRACE("(%d)send_cmd_config error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	Sleep(100);

	status = usb_bulk_write(
				handle,
				/* endpoint       */ INGENIC_OUT_ENDPOINT,
				/* *data          */ (char *)data,
				/* length         */ length,
				/* timeout        */ 5000 * factor);

	if(status != length) {
		TRACE("(%d)stage2_write usb_bulk_write error! (%d)%s\n",this->port_num,status,usb_strerror());
		return length - status;
	}

	return get_ack(50,10,stop);
}

int libusb_legacy_protocol::send_cmd_config(bool trans_check, bool write_check,unsigned char *data, uint32_t length,volatile bool *stop)
{
	int retry = 5;
	transfer_data_check = trans_check;
	write_back_check = write_check;

	while(--retry) {
		if(!stage2_send_cmd_config(data,length,stop))
			return 0;
		Sleep(100);
	}
	return -EIO;
}

int libusb_legacy_protocol::send_cmd_sync_time()
{
	union cmd cmd;

	time_t timep;
	tm	time_tm;
	struct tm *p;

	time(&timep); /*获得time_t结构的时间，UTC时间*/
	//p = gmtime(&timep);
	gmtime_s(&time_tm,&timep);
	p = &time_tm;

	cmd.rtc.tm_year     = p->tm_year;
	cmd.rtc.tm_mon      =  p->tm_mon;
	cmd.rtc.tm_mday     = p->tm_mday;
	cmd.rtc.tm_hour     = p->tm_hour ;
	cmd.rtc.tm_min      = p->tm_min;
	cmd.rtc.tm_sec      = p->tm_sec;
	cmd.rtc.tm_isdst    = -1;
	cmd.rtc.tm_wday     = p->tm_wday;
	cmd.rtc.tm_yday     = p->tm_yday;

	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SYNC_TIME,
				/* wValue        */ 1,
				/* wIndex        */ 0,
				/* Data          */ (char *)&cmd,
				/* wLength       */ sizeof(union cmd),
				5000);

	if (status != sizeof(union cmd)) {
		TRACE("(%d)send_cmd_sync_time error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::send_cmd_reboot()
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_REBOOT,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)send_cmd_reboot error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::send_cmd_poweroff()
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_POWER_OFF,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);

	if (status != 0) {
		TRACE("(%d)send_cmd_poweroff error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return 0;
}

int libusb_legacy_protocol::get_cpu_info(char (*buf)[32], int len)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_IN|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_GET_CPU_INFO,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)*buf,
				/* wLength       */ len,
				2000);

	if (status != len) {
		TRACE("(%d)get_cpu_info error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}
	return 0;
}

bool libusb_legacy_protocol::is_valid()
{
	return (handle != NULL);
}

int libusb_legacy_protocol::stage2_get_cpu_info(char (*buf)[32], int len)
{
	int retry = 5;
	while(--retry) {
		if(!get_cpu_info(buf, len))
			return 0;
		//thread->msleep(100);
	}
	return -EIO;
}

int libusb_legacy_protocol::stage2_init(int timeout, int delay_time, volatile bool *stop)
{
	int status,retry=5;

	do {
		status = usb_control_msg(
					handle,
					/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
					/* bRequest      */ VR_INIT,
					/* wValue        */ 0,
					/* wIndex        */ 0,
					/* Data          */ NULL,
					/* wLength       */ 0,
					2000);
	} while((status != 0) && --retry && !*stop);

	if(status != 0) {
		TRACE("(%d)stage2_init send cmd error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	return get_ack(timeout, delay_time, stop);
}


int libusb_legacy_protocol::stage2_write(unsigned char *data, uint32_t data_len, uint64_t partition,
										 uint32_t offset, uint32_t ops, volatile bool *stop, int timeout, int delay_time, bool is_all_erase)
{
	int status;
	union cmd cmd;

	if (data_len == 0)
		return data_len;

	cmd.write.partation = partition;
	cmd.write.length = data_len;
	cmd.write.offset = offset;
	if(transfer_data_check || write_back_check)
		cmd.write.crc = local_crc32(0xffffffff,data,data_len);
	cmd.write.ops = ops;
	status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_WRITE,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)&cmd,
				/* wLength       */ sizeof(union cmd),
				200000);

	if(status != sizeof(union cmd)) {
		TRACE("(%d)stage2_write usb_control_msg error! (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}
	status = usb_bulk_write(
				handle,
				/* endpoint       */ INGENIC_OUT_ENDPOINT,
				/* *data          */ (char *)data,
				/* length         */ data_len,
				/* timeout        */ 20000 * factor);

	if(status != data_len) {
		TRACE("(%d)stage2_write usb_bulk_write error! (%d)%s\n",this->port_num,status,usb_strerror());
		return data_len - status;
	}

	if(is_all_erase || offset != 0)
		timeout = 50;
	return get_ack(timeout, delay_time, stop);
}

int libusb_legacy_protocol::stage2_read(unsigned char *data,uint32_t data_len,uint64_t partition,
										uint32_t offset,uint32_t ops,volatile bool *stop)
{
	int status,ret;
	union cmd cmd;
	unsigned crc, crc_ck;

	if (data_len == 0)
		return data_len;

	cmd.read.partation = partition;
	cmd.read.length = data_len;
	cmd.read.offset = offset;
	cmd.read.ops = ops;
	status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_READ,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)&cmd,
				/* wLength       */ sizeof(union cmd),
				2000);

	if(status != sizeof(union cmd)) {
		//qCritical("(%d)stage2_read usb_control_msg error! (%d)%s\n",this->port_num,status,usb_strerror());
		TRACE("(%d)stage2_read usb_control_msg error! (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

    if (transfer_data_check)
    {
        ret = get_ack_and_crc(&crc_ck,10,stop);
        if (ret) {
            //qCritical("(%d)stage2_read usb read get crc error! %d\n",this->port_num,ret);
			TRACE("(%d)stage2_read usb read get crc error! %d\n",this->port_num,ret);
            return ret;
        }
    }

	status = usb_bulk_read(
				handle,
				/* endpoint       */ INGENIC_IN_ENDPOINT,
				/* *data          */ (char *)data,
				/* length         */ data_len,
				/* timeout        */ 2000 * factor);
	if(status <= 0) {
		//qCritical("(%d)stage2_read usb read error! (%d)%s\n",this->port_num,status,usb_strerror());
		TRACE("(%d)stage2_read usb read error! (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}


	if (transfer_data_check) {
		crc = local_crc32(0xffffffff, data, status);
		if (crc != crc_ck) {
			//qCritical("(%d)stage2_read usb read crc error!\n",this->port_num);
			TRACE("(%d)stage2_read usb read crc error!\n",this->port_num);
			return -EIO;
		}
	}


    ret = get_ack(10,10,stop);
    if (ret) {
        //qCritical("(%d)stage2_read usb read get ack error! %d\n",this->port_num,ret);
		TRACE("(%d)stage2_read usb read get ack error! %d\n",this->port_num,ret);
        return ret;
    }

    return ret;
}

int libusb_legacy_protocol::get_ack_and_crc(unsigned *crc,int timeout, volatile bool *stop)
{
	time_t start_t,stop_t;
	int status;
	int buf[2];

	while(timeout && !*stop) {
		buf[0] = -ETIMEDOUT;

		time(&start_t);
		status = usb_control_msg(
					handle,
					/* bmRequestType */ USB_ENDPOINT_IN|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
					/* bRequest      */ VR_GET_CRC,
					/* wValue        */ 0,
					/* wIndex        */ 0,
					/* Data          */ (char*)&buf,
					/* wLength       */ 2*sizeof(int),
					/* Individual computer longest 10s timeout */		timeout * 1000);
		time(&stop_t);

		if(buf[0] != -ETIMEDOUT){
			*crc = buf[1];
			return buf[0];
		}

		timeout = timeout - (int)(stop_t - start_t);
		if(timeout < 0)
			return -ETIMEDOUT;
	}

	return status;
}

int libusb_legacy_protocol::get_ack(int timeout, int delay_time, volatile bool *stop)
{
	time_t start_t,stop_t;
	int status,ack;

	if(timeout < delay_time)
		timeout = delay_time;

	while(timeout && !*stop) {
		ack = -ETIMEDOUT;

		time(&start_t);
		status = usb_control_msg(
					handle,
					/* bmRequestType */ USB_ENDPOINT_IN|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
					/* bRequest      */ VR_GET_ACK,
					/* wValue        */ 0,
					/* wIndex        */ 0,
					/* Data          */ (char*)&ack,
					/* wLength       */ sizeof(int),
					/* Individual computer longest 10s timeout */  timeout * 1000);

		time(&stop_t);

		if(ack != -ETIMEDOUT || (stop_t - start_t) == 0)
			return ack;

		if(timeout < (stop_t - start_t) )
			return -ETIMEDOUT;

		if ((stop_t - start_t) <  (timeout/6))
		{
			//qDebug("waitting for %ds after call the get_ack",timeout/6 - (stop_t - start_t));
			TRACE("waitting for %ds after call the get_ack",timeout/6 - (stop_t - start_t));
			Sleep((DWORD)(timeout/6 - (stop_t - start_t)));
			timeout -= timeout/6;
		}
		else
		{
			timeout -= (int)(stop_t - start_t);
		}
	}

	return status;
}

int libusb_legacy_protocol::stage2_get_flash_info(uint32_t medium, uint32_t *data)
{
	int retry = 5;

	union cmd cmd;
	cmd.info.medium = medium;

	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_UPDATE_CFG,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)&cmd,
				/* wLength       */ sizeof(union cmd),
				5000);

	if (status != sizeof(union cmd))
	{
		TRACE("(%d)send_cmd_config error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	while(--retry) {
		if(!get_flash_info(data))
			return 0;
		//thread->msleep(100);
	}
	return -EIO;
}



int libusb_legacy_protocol::get_flash_info(uint32_t *data)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_IN|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_GET_FLASH_INFO,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)data,
				/* wLength       */ FLASH_INFO_LENGTH,
				2000);

	//qDebug("get flash info: %x", *data);
	TRACE("get flash info: %x", *data);
	if (status != FLASH_INFO_LENGTH) {
		TRACE("(%d)get_flash_info error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}
	return 0;
}


//void libusb_legacy_protocol::set_owner(QThread *thread)
//{
//	this->thread = thread;
//}

int libusb_legacy_protocol::send_cmd_bootrom(unsigned int address,unsigned int length,char *data)
{

	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SET_DATA_LEN,
				/* wValue        */ STAGE_ADDR_MSB(length),
				/* wIndex        */ STAGE_ADDR_LSB(length),
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);                              //send length
	if (status != 0) {
		TRACE("(%d)stage1_set_data_address error: (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}
	status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SET_DATA_ADDR,
				/* wValue        */ STAGE_ADDR_MSB(address),
				/* wIndex        */ STAGE_ADDR_LSB(address),
				/* Data          */ NULL,
				/* wLength       */ 0,
				2000);                                  //send address
	if (status != 0) {
		TRACE("(%d)stage1_set_data_address error: (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}

	if (status != 0) {
		TRACE("(%d)stage1_set_data_length error: (%d)%s\n",this->port_num,status,usb_strerror());
		return status;
	}

	status = usb_bulk_read(
				handle,
				/* endpoint       */ INGENIC_IN_ENDPOINT,
				/* *data          */ (char *)data,
				/* length         */ length,
				/* timeout        */ 5000 * factor);
	if(status != length) {
		TRACE("(%d)stage2_write usb_bulk_write error! (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}
	return 0;
}

int libusb_legacy_protocol::send_security_start(volatile bool *stop)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SEC_INIT,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ 0,
				/* wLength       */ 0,
				2000);
	if (status != 0) {
		TRACE("(%d)send_cmd_config error: (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}
	return get_ack(50,10,stop);
}

int libusb_legacy_protocol::send_security_en(char *tmp, volatile bool *stop)
{
	union cmd cmd;
	cmd.security.security_en=*tmp;
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SEC_SEDEN,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ (char *)&cmd,
				/* wLength       */ sizeof(cmd),
				2000);

	if (status != sizeof(cmd)) {
		//qDebug("(%d)send_sec_en error: (%d)%s\n",this->port_num,status,usb_strerror());
		TRACE("(%d)send_sec_en error: (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}
	return get_ack(50,10,stop);
}

int libusb_legacy_protocol::send_security_rckey(volatile bool *stop)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SEC_BURN_RKCK,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ 0,
				/* wLength       */ 0,
				2000);
	if (status != 0) {
		TRACE("(%d)send_cmd_config error: (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}
	return get_ack(50,10,stop);
}


int libusb_legacy_protocol::send_security_burn_en(volatile bool *stop)
{
	int status = usb_control_msg(
				handle,
				/* bmRequestType */ USB_ENDPOINT_OUT|USB_TYPE_VENDOR|USB_RECIP_DEVICE,
				/* bRequest      */ VR_SEC_BURN_SECBOOT_EN,
				/* wValue        */ 0,
				/* wIndex        */ 0,
				/* Data          */ 0,
				/* wLength       */ 0,
				2000);
	if (status != 0) {
		TRACE("(%d)send_cmd_config error: (%d)%s\n",this->port_num,status,usb_strerror());
		return -1;
	}
	return get_ack(50,10,stop);
}

