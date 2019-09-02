/*
 * acodec.h
 *
 *  Created on: 2017年11月29日
 *      Author: DannyWang
 */

#ifndef _ACODEC_H_H
#define _ACODEC_H_H

int acodec_memsize(void);

// 初始化解压缩库
void *acodec_init(void *memaddr, int memlen);

void acodec_reset(void *handle);

// all param is BYTE
// return 0: Success
int acodec_decoder(void *handle, int dec8K, unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

int acodec_encoder(void *handle, unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize);

#endif

