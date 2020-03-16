#include "stdafx.h"

#ifndef FILE_PROTOCOL_H_H
#define FILE_PROTOCOL_H_H

#define NPTO_MAGIC0                       (0xAA)
#define NPTO_MAGIC1                       (0x55)
#define NPTO_HEAD_SIZE                    (8)        // 协议头长度
#define PTO_MAX_SIZE                      (512-NPTO_HEAD_SIZE)

typedef struct tagNETPROTOCOL {
	unsigned char  magic0;           // 恒定为0xAA
	unsigned char  magic1;           // 恒定为0x55
	unsigned char  checksum;         // 将本字段设置为0时,所有INT之和取BYTE。
	unsigned char  command;          // 命令字，命令字的最高位为1，表示反馈命令
	unsigned short cmdlen;           // 本条命令的长度
	unsigned short cmdseq;           // 命令序号，发送端标志，返回命令保持不变
	
  union {
    unsigned char  _dat[PTO_MAX_SIZE];
	unsigned short _dat16[PTO_MAX_SIZE/2];
	unsigned int   _dat32[PTO_MAX_SIZE/4];
  }u;
}NetProto, *NetPtoPtr;
#define u_dat             u._dat     // 内部引用别名
#define u_dat16           u._dat16   // 内部引用别名
#define u_dat32           u._dat32   // 内部引用别名
#define u_result          u._dat16[0]

enum {      // Parser buff status
	NPTO_PARSE_RESULT_SUCC = 0,
	
	NPTO_PARSE_RESULT_NOMAGIC=-99,   // 没找到协议头
	NPTO_PARSE_RESULT_TOSMALL,       // 数据没有接收全
	NPTO_PARSE_RESULT_ERRCRC=-97,    // 命令校验出错
	NPTO_PARSE_RESULT_UNKNOWN,
};

enum {CMD_NONE=0,
	CMD_SET_AUDIO_START=0x01,        // 发送给板卡 [1 开始， 0 停止]
	CMD_RESP_SET_AUDIO=0x81,

	CMD_BLE_AUD_DAT=0x02,            // 板卡发送回来的音频数据
	CMD_BLE_AUD_DAT2=0x06,            // 板卡发送回来的音频数据 通道2

	CMD_GET_BOARDINFO=0x08,          // 返回板卡信息

	CMD_RESP_GET_TFA9896_VALUE=0x83,      // 9896校正值
	
	CMD_RESP_MAC_SN=0x85,            // 反馈板卡SN值


	CMD_RESPONSE_FLAG=0x80,
};

// 返回本条命令的升级长度
int pto_GenerateRaw(void *obuf, int cmd, unsigned int cmdseq,
	void *extdat, int extlen);
int pto_GenerateResp(void *obuf, NetPtoPtr rcmdPkt, short result);

int pto_Parse(void *src, int ilen, NetPtoPtr cmdPkt, int *olen);

#endif  // FILE_PROTOCOL_H_H
