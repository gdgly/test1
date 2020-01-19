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

	memcpy(cmdPkt->magic0,"check ",6);

	if(cmdPkt->u_dat != extdat)    // 如果是同一地址，就不需要复制了
		memcpy(cmdPkt->u_dat, extdat, extlen);


	return extlen + 6;
}

int pto_GenerateResp(void *obuf, NetPtoPtr rcmdPkt, short result)
{
	NetPtoPtr cmdPkt = (NetPtoPtr)obuf;
	
/*	memcpy(cmdPkt, rcmdPkt, NPTO_HEAD_SIZE);
	cmdPkt->command  |= CMD_RESPONSE_FLAG;	
	cmdPkt->u_result  = result;
	cmdPkt->cmdlen    = NPTO_HEAD_SIZE + 2;
	cmdPkt->checksum  = pto_checksum(&cmdPkt->command, cmdPkt->cmdlen-3);
	
	return cmdPkt->cmdlen;*/
	return 0;
}


// 从当前缓冲区中查找一条完整的命令
// @src,ilen   查找缓冲区地址及大小
// @cmdPkt     完整命令存取地址
// @olen       缓冲区使用大小，注意它可能会大于当前命令的长度
// ret:        执行结果
int pto_Parse(void *src, int ilen, NetPtoPtr cmdPkt, int *olen)
{
	int i, cmdlen;
	unsigned char *ptr = (unsigned char*)src;

	// 查找协议头
	for(i = 0; i < ilen; i++) {
		if('c' == ptr[i] && 'h' == ptr[i+1] && 'e' == ptr[i+2] && 'c' == ptr[i+3] && 'k' == ptr[i+4] && ' ' == ptr[i+5] )
			break;
	}
	if(i >= ilen) {
		// 没有找到协议头
		*olen = ilen;
		return NPTO_PARSE_RESULT_NOMAGIC;   // 将所有的数据都设置为使用
	}

	cmdlen = ilen - i;
	if(cmdlen < 8) {       // too small data
		*olen = i;
		return NPTO_PARSE_RESULT_TOSMALL;
	}
	*olen = ilen;
	return NPTO_PARSE_RESULT_SUCC;
}

