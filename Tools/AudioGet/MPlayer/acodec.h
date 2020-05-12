/*
 * acodec.h
 *
 *  Created on: 2017年11月29日
 *      Author: DannyWang
 */

#ifndef _ACODEC_H_H
#define _ACODEC_H_H

#ifdef   __cplusplus
extern   "C"   {
#endif

// 初始化解压缩库
int acodec_init(void);

// 当前不能同时编码解码，在切换编码到解码或解码到编码时可以调用本函数
void acodec_reset(void);

// all param is BYTE
// return 0: Success
int acodec_decoder(unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

int acodec_encoder(unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

#ifdef   __cplusplus
}
#endif

#endif

