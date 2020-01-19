#include "stdafx.h"

#ifndef FILE_PROTOCOL_H_H
#define FILE_PROTOCOL_H_H

#define NPTO_MAGIC0                       (0xAA)
#define NPTO_MAGIC1                       (0x55)
#define NPTO_HEAD_SIZE                    (8)        // Э��ͷ����
#define PTO_MAX_SIZE                      (512-NPTO_HEAD_SIZE)

typedef struct tagNETPROTOCOL {
	unsigned char  magic0[6];           // �㶨Ϊcheck 
  union {
    unsigned char  _dat[PTO_MAX_SIZE];
  }u;
}NetProto, *NetPtoPtr;
#define u_dat             u._dat     // �ڲ����ñ���
#define u_dat16           u._dat16   // �ڲ����ñ���
#define u_dat32           u._dat32   // �ڲ����ñ���
#define u_result          u._dat16[0]

enum {      // Parser buff status
	NPTO_PARSE_RESULT_SUCC = 0,
	
	NPTO_PARSE_RESULT_NOMAGIC=-99,   // û�ҵ�Э��ͷ
	NPTO_PARSE_RESULT_TOSMALL,       // ����û�н���ȫ
	NPTO_PARSE_RESULT_ERRCRC=-97,    // ����У�����
	NPTO_PARSE_RESULT_UNKNOWN,
};

enum {CMD_NONE=0,
	CMD_SET_AUDIO_START=0x01,        // ���͸��忨 [1 ��ʼ�� 0 ֹͣ]
	CMD_RESP_SET_AUDIO=0x81,

	CMD_BLE_AUD_DAT=0x02,            // �忨���ͻ�������Ƶ����
	CMD_BLE_AUD_DAT2=0x06,            // �忨���ͻ�������Ƶ���� ͨ��2

	CMD_RESP_GET_TFA9896_VALUE=0x83,      // 9896У��ֵ
	
	CMD_RESP_MAC_SN=0x85,            // �����忨SNֵ


	CMD_RESPONSE_FLAG=0x80,
};

// ���ر����������������
int pto_GenerateRaw(void *obuf, int cmd, unsigned int cmdseq,
	void *extdat, int extlen);
int pto_GenerateResp(void *obuf, NetPtoPtr rcmdPkt, short result);

int pto_Parse(void *src, int ilen, NetPtoPtr cmdPkt, int *olen);

#endif  // FILE_PROTOCOL_H_H
