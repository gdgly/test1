#include "stdafx.h"
#include "protocol.h"




static int pto_checksum(void *src, int ilen)
{
	int i;
	unsigned char crc = 0, *ptr = (unsigned char*)src;
	
	for(i = 0 ; i < ilen; i++)
		crc += ptr[i];

	return crc;
}

// 返回本条命令的升级长度
int pto_GenerateRaw(void *obuf, int cmd, unsigned int cmdseq,
	void *extdat, int extlen)
{
	NetPtoPtr cmdPkt = (NetPtoPtr)obuf;

	cmdPkt->magic0    = NPTO_MAGIC0;
	cmdPkt->magic1    = NPTO_MAGIC1;
	cmdPkt->command   = cmd;
	cmdPkt->cmdseq    = cmdseq;

	if(cmdPkt->u_dat != extdat)    // 如果是同一地址，就不需要复制了
		memcpy(cmdPkt->u_dat, extdat, extlen);

	cmdPkt->cmdlen    = NPTO_HEAD_SIZE + extlen;
	cmdPkt->checksum  = pto_checksum(&cmdPkt->command, cmdPkt->cmdlen-3);

	return cmdPkt->cmdlen;
}

int pto_GenerateResp(void *obuf, NetPtoPtr rcmdPkt, short result)
{
	NetPtoPtr cmdPkt = (NetPtoPtr)obuf;
	
	memcpy(cmdPkt, rcmdPkt, NPTO_HEAD_SIZE);
	cmdPkt->command  |= CMD_RESPONSE_FLAG;	
	cmdPkt->u_result  = result;
	cmdPkt->cmdlen    = NPTO_HEAD_SIZE + 2;
	cmdPkt->checksum  = pto_checksum(&cmdPkt->command, cmdPkt->cmdlen-3);
	
	return cmdPkt->cmdlen;
}


// 从当前缓冲区中查找一条完整的命令
// @src,ilen   查找缓冲区地址及大小
// @cmdPkt     完整命令存取地址
// @olen       缓冲区使用大小，注意它可能会大于当前命令的长度
// ret:        执行结果
int pto_Parse(void *src, int ilen, NetPtoPtr cmdPkt, int *olen)
{
	int i, cmdlen;
	unsigned char *ptr = (unsigned char*)src, checksum;

	// 查找协议头
	for(i = 0; i < ilen; i++) {
		if(NPTO_MAGIC0 == ptr[i] && NPTO_MAGIC1 == ptr[i+1])
			break;
	}
	if(i >= ilen) {
		// 没有找到协议头
		*olen = ilen;
		return NPTO_PARSE_RESULT_NOMAGIC;   // 将所有的数据都设置为使用
	}

	cmdlen = ilen - i;
	if(cmdlen < NPTO_HEAD_SIZE) {       // too small data
		*olen = i;
		return NPTO_PARSE_RESULT_TOSMALL;
	}

	memcpy(cmdPkt, &ptr[i], NPTO_HEAD_SIZE);
	if(cmdPkt->cmdlen > PTO_MAX_SIZE || cmdPkt->cmdlen < NPTO_HEAD_SIZE) {  // Error data
		*olen = i + 2;
		TRACE("ERR cmdlen=%d\n", cmdPkt->cmdlen);
		return NPTO_PARSE_RESULT_UNKNOWN;
	}

	if(cmdPkt->cmdlen > cmdlen) {
		*olen = i;
		return NPTO_PARSE_RESULT_TOSMALL;
	}

	memcpy(cmdPkt->u_dat, &ptr[i+NPTO_HEAD_SIZE], cmdPkt->cmdlen-NPTO_HEAD_SIZE);
	checksum = pto_checksum(&cmdPkt->command, cmdPkt->cmdlen-3);
	if(checksum != cmdPkt->checksum) {
		*olen = i + 2;
		TRACE("get:0x%x calc=0x%x\n", cmdPkt->checksum, checksum);
		return NPTO_PARSE_RESULT_ERRCRC;
	}

	*olen = i + cmdPkt->cmdlen;
	return NPTO_PARSE_RESULT_SUCC;
}

