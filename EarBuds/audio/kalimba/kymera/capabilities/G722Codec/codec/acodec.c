/*
 * acodec.c
 *
 *  Created on: 2017年11月3日
 *      Author: DannyWang
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"

#include "acodec.h"

#define MAX_SAMPLES_PER_FRAME (DCT_LENGTH)
#define BYTES_PER_SAMPLE      (2)


#ifdef CONFIG_ICO_DECODE_TEST       /* TEST */
unsigned char audin[] = {
        0x8e, 0x0c, 0xA1, 0x70, 0x35, 0xa9, 0x97, 0x92, 0x0b, 0x9e, 0x50, 0x11, 0xbc, 0xe9, 0x21, 0x6d,
        0x74, 0xb6, 0x7f, 0x09, 0x74, 0x14, 0x0e, 0x52, 0xba, 0xd2, 0xf9, 0x82, 0xf6, 0x54, 0x80, 0x99,
        0xe0, 0x54, 0x91, 0xee, 0xfd, 0x27, 0xe9, 0x01, 0x06, 0x7c, 0x00, 0x05, 0x26, 0x60, 0x33, 0x97,
        0xc4, 0x69, 0x76, 0x81, 0xbd, 0x58, 0xfb, 0xc0, 0x02, 0xdc, 0x3a, 0x36, 0x92, 0x12, 0xb1, 0xa8,
        0x7e, 0xc6, 0xcf, 0xa5, 0x5c, 0x5b, 0x92, 0x86, 0x1c, 0xde, 0xdc, 0x4b, 0x7e, 0xa0, 0x82, 0x1f,
};
/*
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 ff ff ff ff 00 00 fe ff
ff ff 00 00 fe ff 00 00 fd ff fc ff fd ff fd ff
fd ff ff ff 00 00 00 00 fd ff fd ff ff ff fd ff
fd ff 00 00 ff ff fc ff fd ff fd ff fc ff fc ff
ff ff 00 00 00 00 fe ff fd ff fd ff ff ff fd ff
fd ff fd ff fc ff fd ff fd ff fd ff ff ff ff ff
00 00 00 00 fd ff fc ff fd ff fd ff ff ff ff ff
ff ff fe ff fa ff fc ff 00 00 00 00 ff ff fd ff
00 00 00 00 fd ff 00 00 00 00 fd ff fc ff fc ff
fd ff fc ff fc ff ff ff 00 00 00 00 fe ff fc ff
00 00 02 00 fc ff fc ff 00 00 ff ff fd ff 00 00
ff ff fc ff fd ff fc ff fa ff fe ff fc ff fb ff
ff ff ff ff 00 00 02 00 ff ff 00 00 02 00 fe ff
fa ff 00 00 fe ff fa ff fd ff fd ff 00 00 00 00
fb ff fa ff fd ff 00 00 fe ff fd ff fc ff fd ff
00 00 fe ff ff ff 00 00 ff ff fb ff ff ff fd ff
fc ff fb ff f9 ff 04 00 04 00 06 00 02 00 fb ff
01 00 fe ff 00 00 01 00 fc ff ff ff 00 00 ff ff
f9 ff f9 ff fd ff 03 00 02 00 fa ff fd ff f9 ff
fb ff 00 00 f9 ff fc ff ff ff f8 ff fc ff fc ff
fe ff 05 00 ff ff f9 ff f8 ff fd ff 01 00 fe ff
fd ff fe ff 02 00 fe ff 00 00 fd ff fc ff 0b 00
06 00 06 00 00 00 f5 ff fd ff fc ff ff ff fc ff
fd ff 01 00 fe ff ff ff ff ff ff ff fc ff fc ff
ff ff 00 00 ff ff f9 ff f7 ff fe ff 03 00 ff ff
fd ff f9 ff f9 ff 02 00 fa ff f9 ff 05 00 02 00
ff ff 03 00 04 00 00 00 00 00 fb ff f7 ff fd ff
fb ff f7 ff fd ff fc ff ff ff 02 00 fc ff fc ff
00 00 fe ff f9 ff 05 00 0a 00 fc ff fb ff 00 00
00 00 00 00 fc ff f9 ff fa ff fa ff f7 ff fc ff
01 00 05 00 fe ff fe ff 04 00 fc ff fe ff 05 00
03 00 fa ff f4 ff fb ff 01 00 fe ff 00 00 fd ff
fd ff fc ff fc ff 02 00 07 00 00 00 fe ff fd ff
fd ff fb ff f7 ff fd ff fb ff f8 ff fe ff f9 ff
f8 ff ff ff 00 00 04 00 04 00 f8 ff f0 ff f8 ff
fd ff fa ff fd ff 04 00 03 00 f9 ff fd ff fb ff
f8 ff ff ff 00 00 fd ff fa ff fa ff fb ff f4 ff
f8 ff fd ff fc ff 04 00 08 00 00 00 02 00 03 00
fb ff fe ff 02 00 ff ff fe ff 07 00 06 00 fc ff
*/

int acodec_decoder(unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize, unsigned char d_flag);

void acodec_show(unsigned char *ptr, int size)
{
        int i;

        printf("DATA ");
        for(i = 0; i < size; i++) {
                if(i % 16 == 0)
                        printf("\r\n");

                printf("%02x ", ptr[i]);
        }

        printf("\r\n");
}
#endif

#define SUPPORT_NO_OLD_MLT_COEFS   /* 代码中如果不出错，是不会用到old_mlt_coefs */

typedef struct tagACODECHANDLER {
#ifdef SUPPORT_STRUCT_SHAREMEM
    Word16      share_mem1[DCT_LENGTH];
    Word16      share_mem2[DCT_LENGTH];
    Word16      share_mem3[DCT_LENGTH];
#endif

    Word16      enc_old_frame[MAX_SAMPLES_PER_FRAME];
#ifndef SUPPORT_NO_OLD_MLT_COEFS
    Word16      old_mlt_coefs[MAX_SAMPLES_PER_FRAME];
#endif
    Word16      mlt_coefs[MAX_SAMPLES_PER_FRAME];
    Word16      dec_old_mag_shift;
    Rand_Obj    dec_randobj;

#ifdef SUPPORT_ZERO_DATA_DETECT
    UWord16     dec_zero_cnt;   /*检测到两次0数据，将所有数据设置为0 */
#endif
}AcodecHandler, *AcodecHPtr;


#ifdef SUPPORT_ZERO_DATA_DETECT
/* 特征数据 40 BYTE */
unsigned int g722_zero_dat[] = {
        0xbd308e0aU, 0x00FF833F,  0x00000000,  0x00000000,  0xFF000000U,
        0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU, 0xFFFFFFFFU};

static int acodec_is_zero_data(AcodecHPtr h, unsigned int *src)
{
        int i;

        for(i = 0; i < 10; i++) {
                if(src[i] != g722_zero_dat[i]) {
                        h->dec_zero_cnt = 0;
                        return -1;
                }
        }

        h->dec_zero_cnt += 1;
        return 0;
}
#else
#define acodec_is_zero_data(...)
#endif

int acodec_memsize(void)
{
    return (sizeof(AcodecHandler));
}

// 初始化解压缩库
void *acodec_init(void *memaddr, int memlen)
{
    if(memlen < sizeof(AcodecHandler))
        return NULL;

    acodec_reset(memaddr);
    return memaddr;
}

void acodec_reset(void *handle)
{
    AcodecHPtr h = (AcodecHPtr)handle;

    memset(h, 0, sizeof(AcodecHandler));

    h->dec_old_mag_shift = 0;
    h->dec_randobj.seed0 = 1;
    h->dec_randobj.seed1 = 1;
    h->dec_randobj.seed2 = 1;
    h->dec_randobj.seed3 = 1;

#ifdef SUPPORT_ZERO_DATA_DETECT
    h->dec_zero_cnt      = 0;
#endif

#ifdef SUPPORT_STRUCT_SHAREMEM
    share_mem1    = h->share_mem1;
    share_mem2    = h->share_mem2;
    share_mem3    = h->share_mem3;
#endif
}

static void acodec_swap_bytes(unsigned short *buf, unsigned count)
{
        unsigned short *end = buf + count;
        while (buf != end) {
                *buf = (unsigned short)((*buf << 8) | (*buf >> 8));
                ++buf;
        }
}

// all param is BYTE
// return 0: Success
int acodec_decoder(void *handle, int dec8K, unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize)
{
       Word16 mag_shift;
       Bit_Obj bitobj;
       AcodecHPtr h = (AcodecHPtr)handle;

       bitobj.code_word_ptr = (Word16*)indata;
       bitobj.current_word =  *bitobj.code_word_ptr;
       bitobj.code_bit_count = 0;
       bitobj.number_of_bits_left = MAX_SAMPLES_PER_FRAME;
       
       acodec_is_zero_data(h, (unsigned int*)indata);

       acodec_swap_bytes((unsigned short*)indata, REGION_SIZE);


       /* Process the input frame to get mlt coefs */
       decoder(&bitobj,
               &h->dec_randobj,
               NUMBER_OF_REGIONS,// codec_data->number_of_regions,
               h->mlt_coefs,
               &mag_shift,
               &h->dec_old_mag_shift,
  #ifdef SUPPORT_NO_OLD_MLT_COEFS
               (Word16*)outdata,
  #else
               h->old_mlt_coefs,
  #endif
               0/*frame_error_flag*/);

       /* Convert the mlt_coefs to PCM samples */
       rmlt_coefs_to_samples(h->mlt_coefs,
               h->enc_old_frame, //codec_data->dec_old_frame,
               (Word16*)outdata,
               MAX_SAMPLES_PER_FRAME, //codec_data->samples_per_frame,
               mag_shift);

#ifdef SUPPORT_ZERO_DATA_DETECT
       if(h->dec_zero_cnt >= 2) {
               memset(outdata, 0, MAX_SAMPLES_PER_FRAME*BYTES_PER_SAMPLE);
       }
#endif

#ifdef SUPPORT_DECODER_8Kto16K
       if(0 == dec8K) {
               *outsize =  (MAX_SAMPLES_PER_FRAME*BYTES_PER_SAMPLE);
               return 0;
       }

       /* resample 8K to 16K HZ */
#if 0
       {
	   Word16 *src_ptr, *new_ptr, i;
	   *outsize =  (MAX_SAMPLES_PER_FRAME*BYTES_PER_SAMPLE) * 2;

	   src_ptr = (Word16*)(outdata +  *outsize/2 - 2);
	   new_ptr = (Word16*)(outdata +  *outsize - 4);
	   for(i = 0; i < MAX_SAMPLES_PER_FRAME; i++) {
	       new_ptr[0] = src_ptr[0];
	       new_ptr[1] = (src_ptr[0] + src_ptr[1]) / 2;

	       src_ptr -= 1;
	       new_ptr -= 2;
	   }

	   // 最后一个值重新计算
	   new_ptr = (Word16*)(outdata +  *outsize - 2 - 4);
	   new_ptr[2] = new_ptr[0] + (new_ptr[1] - new_ptr[0]);
       }
#else
       {
           Word16 *src_ptr, *new_ptr, i;
           *outsize =  (MAX_SAMPLES_PER_FRAME*BYTES_PER_SAMPLE) * 2;

           src_ptr = (Word16*)(outdata +  *outsize/2 - 2);
           new_ptr = (Word16*)(outdata +  *outsize - 4);
           for(i = 0; i < MAX_SAMPLES_PER_FRAME; i++) {
               new_ptr[0] = new_ptr[1] = src_ptr[0];
               src_ptr -= 1;
               new_ptr -= 2;
           }
       }
#endif
       *outsize =  (MAX_SAMPLES_PER_FRAME*BYTES_PER_SAMPLE) * 2;
#else
       *outsize =  (MAX_SAMPLES_PER_FRAME*BYTES_PER_SAMPLE);
#endif

	return 0;
}

int acodec_encoder(void *handle, unsigned char *indata, unsigned short insize, unsigned char *outdata, unsigned short *outsize)
{
        Word16 mag_shift;
        AcodecHPtr h = (AcodecHPtr)handle;

	/* Convert input samples to rmlt coefs */
        mag_shift = samples_to_rmlt_coefs((Word16*)indata, h->enc_old_frame,
                h->mlt_coefs, MAX_SAMPLES_PER_FRAME);

	/* Encode the mlt coefs. Note that encoder output stream is
	 * 16 bit array, so we need to take care about endianness.
	 */
        (void)mag_shift;
        encoder(MAX_SAMPLES_PER_FRAME, NUMBER_OF_REGIONS,// codec_data->number_of_regions
                h->mlt_coefs, mag_shift, (Word16*)outdata);

	/* Encoder output are in native host byte order, while ITU says
	 * it must be in network byte order (MSB first).
	 */
//      好象QCC不需要在这儿交换
//	acodec_swap_bytes((unsigned short*)outdata, REGION_SIZE);

	*outsize = (REGION_SIZE*BYTES_PER_SAMPLE);
	return 0;
}

