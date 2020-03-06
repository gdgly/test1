#ifndef RWFILE_H
#define RWFILE_H

#include "av_headset.h"
#include <pmalloc.h>

/* 升级文件 */
#define FILE_NAME "/rwfs/firmware.bin"
/* 标记升级文件是否下载成功 */
#define FILE_NAME_OK "/rwfs/firmware_ok.bin"


FILE_INDEX FileOpen(char * file_name);
FILE_INDEX FindFileIndex(char * file_name);
int FileWrite(FILE_INDEX findex, uint8 *buffer, int length);
int FileClose(void);
int FileCancel(char *fname);            // 删除文件


uint8 FileWriteOk(uint8 value);
uint8 FileReadOk(void);
#endif //RWFILE_H

