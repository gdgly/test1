/*
 * acodec.h
 *
 *  Created on: 2017��11��29��
 *      Author: DannyWang
 */

#ifndef _ACODEC_H_H
#define _ACODEC_H_H

#ifdef   __cplusplus
extern   "C"   {
#endif
#pragma comment(lib,"acodec.lib")
// ��ʼ����ѹ����
int acodec_init(void);

// ��ǰ����ͬʱ������룬���л����뵽�������뵽����ʱ���Ե��ñ�����
void acodec_reset(void);

// all param is BYTE
// return 0: Success
int acodec_decoder(unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

int acodec_encoder(unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

#ifdef   __cplusplus
}
#endif

#endif

