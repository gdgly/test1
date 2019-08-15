/*
 * acodec.h
 *
 *  Created on: 2017年11月29日
 *      Author: DannyWang
 */

#ifndef _ACODEC_H_H
#define _ACODEC_H_H


// 初始化解压缩库
int acodec_init(void);

void acodec_reset(void);

// all param is BYTE
// return 0: Success
int acodec_decoder(int dec8K, unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

int acodec_encoder(unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

#endif

