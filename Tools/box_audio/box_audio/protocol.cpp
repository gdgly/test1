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

// ���ر����������������
int pto_GenerateRaw(void *obuf, int cmd, unsigned int cmdseq,
	void *extdat, int extlen)
{
	NetPtoPtr cmdPkt = (NetPtoPtr)obuf;

	memcpy(cmdPkt->magic0,"check ",6);

	if(cmdPkt->u_dat != extdat)    // �����ͬһ��ַ���Ͳ���Ҫ������
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


// �ӵ�ǰ�������в���һ������������
// @src,ilen   ���һ�������ַ����С
// @cmdPkt     ���������ȡ��ַ
// @olen       ������ʹ�ô�С��ע�������ܻ���ڵ�ǰ����ĳ���
// ret:        ִ�н��
int pto_Parse(void *src, int ilen, NetPtoPtr cmdPkt, int *olen)
{
	int i, cmdlen;
	unsigned char *ptr = (unsigned char*)src;

	// ����Э��ͷ
	for(i = 0; i < ilen; i++) {
		if('c' == ptr[i] && 'h' == ptr[i+1] && 'e' == ptr[i+2] && 'c' == ptr[i+3] && 'k' == ptr[i+4] && ' ' == ptr[i+5] )
			break;
	}
	if(i >= ilen) {
		// û���ҵ�Э��ͷ
		*olen = ilen;
		return NPTO_PARSE_RESULT_NOMAGIC;   // �����е����ݶ�����Ϊʹ��
	}

	cmdlen = ilen - i;
	if(cmdlen < 8) {       // too small data
		*olen = i;
		return NPTO_PARSE_RESULT_TOSMALL;
	}
	*olen = ilen;
	return NPTO_PARSE_RESULT_SUCC;
}

